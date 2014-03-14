/*
 * TrackerClient.cpp
 *
 *  Created on: 11.03.2014
 *      Author: adrianp
 */

#include "TrackerClient.h"
#include "happyhttp.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

std::string char2hex( char dec )
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
	if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=97-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

std::string urlencode(const std::string &c)
{
	std::string escaped="";
	int max = c.length();
	for(int i=0; i<max; i++)
	{
		if ( (48 <= c[i] && c[i] <= 57) ||//0-9
				(65 <= c[i] && c[i] <= 90) ||//abc...xyz
				(97 <= c[i] && c[i] <= 122) || //ABC...XYZ
				(c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
		)
		{
			escaped.append( &c[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
		}
	}
	return escaped;
}

TrackerClient::TrackerClient(TorrentClient *downloader):torrentDownloader(downloader) {
	// TODO Auto-generated constructor stub
	downloadedBytes = 0;
	length = 0;
	uploadedBytes = 0;
	requestInterval = 5 * 60;
	requestIntervalTimer = -1;
	firstTrackerRequest = true;
	lastTrackerRequest = false;
	firstSeeding = true;
}

TrackerClient::~TrackerClient() {
	// TODO Auto-generated destructor stub
}

void TrackerClient::start(const MetaInfo& info) {
	metaInfo = info;
	//	QTimer::singleShot(0, this, SLOT(fetchPeerList()));

	if (metaInfo.fileForm() == MetaInfo::SingleFileForm) {
		length = metaInfo.singleFile().length;
	}
	else
	{
		std::list<MetaInfoMultiFile> files = metaInfo.multiFiles();
		for (std::list<MetaInfoMultiFile>::iterator it=files.begin(); it!=files.end(); it++)
			length += (*it).length;
	}
	fetchPeerList();
}

void TrackerClient::stop() {
}

void TrackerClient::startSeeding() {
}

void TrackerClient::fetchPeerList() {
	//	QUrl url(metaInfo.announceUrl());

	// Base the query on announce url to include a passkey (if any)
	//	QUrlQuery query(url);

	// Percent encode the hash
	std::string infoHash = torrentDownloader->infoHash();
	const char* infhash=torrentDownloader->infoHash().c_str();
	std::string encodedSum;
	const char* enc;
	const char* result;
	encodedSum = urlencode(infoHash);
	enc = encodedSum.c_str();
	result=encodedSum.c_str();

	bool seeding = (torrentDownloader->state() == TorrentClient::Seeding);

	//	query.addQueryItem("info_hash", encodedSum);
	//	query.addQueryItem("peer_id", ConnectionManager::instance()->clientId());
	//	query.addQueryItem("port", QByteArray::number(TorrentServer::instance()->serverPort()));
	//	query.addQueryItem("compact", "1");
	//	query.addQueryItem("uploaded", QByteArray::number(torrentDownloader->uploadedBytes()));
	//
	//	if (!firstSeeding) {
	//		query.addQueryItem("downloaded", "0");
	//		query.addQueryItem("left", "0");
	//	} else {
	//		query.addQueryItem("downloaded",
	//				QByteArray::number(torrentDownloader->downloadedBytes()));
	//		int left = qMax<int>(0, metaInfo.totalSize() - torrentDownloader->downloadedBytes());
	//		query.addQueryItem("left", QByteArray::number(seeding ? 0 : left));
	//	}
	//
	//	if (seeding && firstSeeding) {
	//		query.addQueryItem("event", "completed");
	//		firstSeeding = false;
	//	} else if (firstTrackerRequest) {
	//		firstTrackerRequest = false;
	//		query.addQueryItem("event", "started");
	//	} else if(lastTrackerRequest) {
	//		query.addQueryItem("event", "stopped");
	//	}
	//
	//	if (!trackerId.isEmpty())
	//		query.addQueryItem("trackerid", trackerId);
	//
	//	url.setQuery(query);
	//
	//	QNetworkRequest req(url);
	//	if (!url.userName().isEmpty()) {
	//		uname = url.userName();
	//		pwd = url.password();
	//		connect(&http, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
	//				this, SLOT(provideAuthentication(QNetworkReply*,QAuthenticator*)));
	//	}
	//	http.get(req);
}
