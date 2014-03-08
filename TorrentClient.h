/*
 * TorrentClient.h
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include <string>


#ifndef TORRENTCLIENT_H_
#define TORRENTCLIENT_H_

class TorrentClient {
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

	bool setTorrent(char *fileName);
};

#endif /* TORRENTCLIENT_H_ */
