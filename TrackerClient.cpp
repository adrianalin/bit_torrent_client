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
	happyhttp::Connection conn(torrentDownloader->metaInfo().announceUrl(), 80);

	// Percent encode the hash
	std::string infoHash = torrentDownloader->infoHash();
	std::string encodedSum = urlencode(infoHash);

	bool seeding = (torrentDownloader->state() == TorrentClient::Seeding);
}

void TrackerClient::httpRequestDone(const happyhttp::Response* r, void* userData) {
	std::cout<<"COMPLETE m_BytesRead="<<r->m_BytesRead<<endl;
}
