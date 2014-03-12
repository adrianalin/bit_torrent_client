/*
 * TorrentClient.h
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#ifndef TORRENTCLIENT_H_
#define TORRENTCLIENT_H_

#include <string>
#include "MetaInfo.h"

class TorrentClientPrivate;
class TorrentPeer;
class MetaInfo;

class TorrentPeer {
public:
	std::string address;
	int port;
	std::string id;
	bool interesting;
	bool seed;
	unsigned int lastVisited;
	unsigned int connectStart;
	unsigned int connectTime;
	std::string pieces;
	int numCompletedPieces;

	inline bool operator==(const TorrentPeer &other)
    		{
		return port == other.port
				&& address == other.address
				&& id == other.id;
    		}
};

class TorrentClient {
private:
	TorrentClientPrivate* d;
	std::string torrentFile;
	void fullVerificationDone();
public:
	enum State {
		Idle,
		Paused,
		Stopping,
		Preparing,
		Searching,
		Connecting,
		WarmingUp,
		Downloading,
		Endgame,
		Seeding
	};
	enum Error {
		UnknownError,
		TorrentParseError,
		InvalidTrackerError,
		FileError,
		ServerError
	};

	TorrentClient();
	virtual ~TorrentClient();

	bool setTorrent(char* fileName);
	bool setTorrent();
	MetaInfo metaInfo() const;

	void setMaxConnections(int connections);
	int maxConnections() const;

	void setDestinationFolder(const std::string &directory);
	std::string destinationFolder() const;

	void setDumpedState(const std::string &dumpedState);
	std::string dumpedState() const;

	// Progress and stats for download feedback.
	long long progress() const;
	void setDownloadedBytes(long long bytes);
	long long downloadedBytes() const;
	void setUploadedBytes(long long bytes);
	long long uploadedBytes() const;
	int connectedPeerCount() const;
	int seedCount() const;

	// Accessors for the tracker
	std::string peerId() const;
	std::string infoHash() const;
	int serverPort() const;

	// State and error.
	TorrentClient::State state() const;
	std::string stateString() const;
	TorrentClient::Error error() const;
	std::string errorString() const;
};

#endif /* TORRENTCLIENT_H_ */
