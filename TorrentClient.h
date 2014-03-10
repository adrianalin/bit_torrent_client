/*
 * TorrentClient.h
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#ifndef TORRENTCLIENT_H_
#define TORRENTCLIENT_H_

#include <string>

class TorrentClientPrivate;

class TorrentClient {
private:
    TorrentClientPrivate* d;
    std::string torrentFile;
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
};

#endif /* TORRENTCLIENT_H_ */
