/*
 * TrackerClient.h
 *
 *  Created on: 11.03.2014
 *      Author: adrianp
 */

#ifndef TRACKERCLIENT_H_
#define TRACKERCLIENT_H_

#include "TorrentClient.h"

class TrackerClient {
public:
	TrackerClient(TorrentClient* downloader);
	~TrackerClient();

	void start(const MetaInfo &info);
	void stop();
	void startSeeding();

//	signals:
//	void connectionError(QNetworkReply::NetworkError error);
//
//	void failure(const QString &reason);
//	void warning(const QString &message);
//	void peerListUpdated(const QList<TorrentPeer> &peerList);
//
//	void uploadCountUpdated(qint64 newUploadCount);
//	void downloadCountUpdated(qint64 newDownloadCount);
//
//	void stopped();

protected:
//	void timerEvent(QTimerEvent *event);

//	private slots:
	void fetchPeerList();
	void httpRequestDone(const happyhttp::Response* r, void* userData);
//	void provideAuthentication(QNetworkReply *reply, QAuthenticator *auth);

private:
	TorrentClient *torrentDownloader;

	int requestInterval;
	int requestIntervalTimer;
//	QNetworkAccessManager http;
	MetaInfo metaInfo;
	std::string trackerId;
	std::list<TorrentPeer> peers;
	long long  uploadedBytes;
	long long downloadedBytes;
	long long length;
	std::string uname;
	std::string pwd;

	bool firstTrackerRequest;
	bool lastTrackerRequest;
	bool firstSeeding;
};

#endif /* TRACKERCLIENT_H_ */
