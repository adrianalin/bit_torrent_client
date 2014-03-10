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

typedef struct{
	char* data;
	int size;
} torrent_data;

class TorrentClient {
private:
	TorrentClientPrivate* d;
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

	bool setTorrent(char* fileName, size_t num);
	bool setTorrent(torrent_data* torrent);
};

#endif /* TORRENTCLIENT_H_ */
