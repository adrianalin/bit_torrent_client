/*
 * TrackerClient.cpp
 *
 *  Created on: 11.03.2014
 *      Author: adrianp
 */

#include "TrackerClient.h"

TrackerClient::TrackerClient(TorrentClient *downloader):torrentDownloader(downloader) {
	// TODO Auto-generated constructor stub
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
	std::string encodedSum;
	for (int i = 0; i < infoHash.size(); ++i) {
		encodedSum += '%';
//		encodedSum += QByteArray::number(infoHash[i], 16).right(2).rightJustified(2, '0');
	}

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
