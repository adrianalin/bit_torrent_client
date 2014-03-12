/*
 * TorrentClient.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <map>
#include "TorrentClient.h"
#include "MetaInfo.h"
#include "TrackerClient.h"
#include "sha1.h"

// These constants could also be configurable by the user.
static const int ServerMinPort = 6881;
static const int ServerMaxPort = /* 6889 */ 7000;
static const int BlockSize = 16384;
static const int MaxBlocksInProgress = 5;
static const int MaxBlocksInMultiMode = 2;
static const int MaxConnectionPerPeer = 1;
static const int RateControlWindowLength = 10;
static const int RateControlTimerDelay = 1000;
static const int MinimumTimeBeforeRevisit = 30;
static const int MaxUploads = 4;
static const int UploadScheduleInterval = 10000;

class TorrentPiece {
public:
	int index;
	int length;
	std::string completedBlocks;
	std::string requestedBlocks;
	bool inProgress;
};

class TorrentClientPrivate {
public:
	TorrentClientPrivate(TorrentClient *qq);

	TorrentClient::Error error;
	TorrentClient::State state;
	std::string errorString;
	std::string stateString;

	void setError(TorrentClient::Error errorCode);
	void setState(TorrentClient::State state);
	TorrentClientPrivate();
	virtual ~TorrentClientPrivate();

	// Where to save data
	std::string destinationFolder;
	MetaInfo metaInfo;

	// Announce tracker and file manager
	std::string peerId;
	std::string infoHash;
	TrackerClient trackerClient;
	//	FileManager fileManager;	toDo!!

	// Connections
	//	QList<PeerWireClient *> connections; toDo
	//	QList<TorrentPeer *> peers; toDo!!
	bool schedulerCalled;
	void callScheduler();
	bool connectingToClients;
	void callPeerConnector();
	int uploadScheduleTimer;

	// Pieces
	//	QMap<int, PeerWireClient *> readIds;
	//	QMultiMap<PeerWireClient *, TorrentPiece *> payloads;
	std::map<int, TorrentPiece *> pendingPieces;
	std::string completedPieces;
	std::string incompletePieces;
	int pieceCount;

	// Progress
	int lastProgressValue;
	long long downloadedBytes;
	long long uploadedBytes;
	int downloadRate[RateControlWindowLength];
	int uploadRate[RateControlWindowLength];
	int transferRateTimer;

	TorrentClient *q;
};

inline TorrentClientPrivate::TorrentClientPrivate(TorrentClient* qq): trackerClient(qq),q(qq)
{
	error = TorrentClient::UnknownError;
	state = TorrentClient::Idle;
	errorString = "Unknown error";
	stateString = "Idle";
	schedulerCalled = false;
	connectingToClients = false;
	uploadScheduleTimer = 0;
	lastProgressValue = -1;
	pieceCount = 0;
	downloadedBytes = 0;
	uploadedBytes = 0;
	memset(downloadRate, 0, sizeof(downloadRate));
	memset(uploadRate, 0, sizeof(uploadRate));
	transferRateTimer = 0;
}

TorrentClientPrivate::~TorrentClientPrivate() {
	// TODO Auto-generated destructor stub
}

void TorrentClientPrivate::setState(TorrentClient::State state){
	this->state = state;
	switch (state) {
	case TorrentClient::Idle:
		stateString = "Idle";
		break;
	case TorrentClient::Paused:
		stateString = "Paused";
		break;
	case TorrentClient::Stopping:
		stateString = "Stopping";
		break;
	case TorrentClient::Preparing:
		stateString = "Preparing";
		break;
	case TorrentClient::Searching:
		stateString = "Searching";
		break;
	case TorrentClient::Connecting:
		stateString = "Connecting";
		break;
	case TorrentClient::WarmingUp:
		stateString = "Warming up";
		break;
	case TorrentClient::Downloading:
		stateString = "Downloading";
		break;
	case TorrentClient::Endgame:
		stateString = "Finishing";
		break;
	case TorrentClient::Seeding:
		stateString = "Seeding";
		break;
	}
	std::cout<<"state = "<<stateString<<std::endl;
}

void TorrentClientPrivate::setError(TorrentClient::Error errorCode)
{
	this->error = errorCode;
	switch (error) {
	case TorrentClient::UnknownError:
		errorString = "Unknown error";
		break;
	case TorrentClient::TorrentParseError:
		errorString = "Invalid torrent data";
		break;
	case TorrentClient::InvalidTrackerError:
		errorString = "Unable to connect to tracker";
		break;
	case TorrentClient::FileError:
		errorString = "File error";
		break;
	case TorrentClient::ServerError:
		errorString = "Unable to initialize server";
		break;
	}
	std::cout<<"error = "<<errorString<<std::endl;
}

/******************************************************************************************************************/

TorrentClient::TorrentClient() {
	// TODO Auto-generated constructor stub
	d = new TorrentClientPrivate(this);
}

TorrentClient::~TorrentClient() {
	// TODO Auto-generated destructor stub
	delete d;
}

bool TorrentClient::setTorrent(char* fileName)
{
	std::ifstream in(fileName, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		torrentFile=contents;
		setTorrent();
		return true;
	}

	return false;
}

bool TorrentClient::setTorrent() {
	if(!d->metaInfo.parse(torrentFile)){
		d->setError(TorrentParseError);
		return false;
	}
	unsigned char result[20];
	memset(result, 0, sizeof(result));
	sha1::calc((void*)d->metaInfo.infoValue().c_str(), d->metaInfo.infoValue().length(), result);
	d->infoHash = std::string(result, sizeof(result));

	return true;
}

MetaInfo TorrentClient::metaInfo() const {
	return d->metaInfo;
}

void TorrentClient::setDestinationFolder(const std::string& directory) {
	d->destinationFolder=".";
}

std::string TorrentClient::destinationFolder() const {
	return d->destinationFolder;
}

void TorrentClient::setDumpedState(const std::string& dumpedState) {
}

std::string TorrentClient::dumpedState() const {
}

long long TorrentClient::progress() const {
}

void TorrentClient::setDownloadedBytes(long long bytes) {
}

long long TorrentClient::downloadedBytes() const {
}

void TorrentClient::setUploadedBytes(long long bytes) {
}

long long TorrentClient::uploadedBytes() const {
}

int TorrentClient::connectedPeerCount() const {
}

int TorrentClient::seedCount() const {
}

std::string TorrentClient::peerId() const {
}

std::string TorrentClient::infoHash() const {
	d->infoHash;
}

int TorrentClient::serverPort() const {
}

TorrentClient::State TorrentClient::state() const {
}

std::string TorrentClient::stateString() const {
}

TorrentClient::Error TorrentClient::error() const {
}

std::string TorrentClient::errorString() const {
}

void TorrentClient::fullVerificationDone() {
//	// Update our list of completed and incomplete pieces.
//	d->completedPieces = d->fileManager.completedPieces();
//	d->incompletePieces.resize(d->completedPieces.size());
//	d->pieceCount = d->completedPieces.size();
//	for (int i = 0; i < d->fileManager.pieceCount(); ++i) {
//		if (!d->completedPieces.testBit(i))
//			d->incompletePieces.setBit(i);
//	}
//
//	updateProgress();
//
//	// If the checksums show that what the dumped state thought was
//	// partial was in fact complete, then we trust the checksums.
//	QMap<int, TorrentPiece *>::Iterator it = d->pendingPieces.begin();
//	while (it != d->pendingPieces.end()) {
//		if (d->completedPieces.testBit(it.key()))
//			it = d->pendingPieces.erase(it);
//		else
//			++it;
//	}
//
//	d->uploadScheduleTimer = startTimer(UploadScheduleInterval);
//
//	// Start the server
//	TorrentServer *server = TorrentServer::instance();
//	if (!server->isListening()) {
//		// Set up the peer wire server
//		for (int i = ServerMinPort; i <= ServerMaxPort; ++i) {
//			if (server->listen(QHostAddress::Any, i))
//				break;
//		}
//		if (!server->isListening()) {
//			d->setError(ServerError);
//			return;
//		}
//	}
//
//	d->setState(d->completedPieces.count(true) == d->pieceCount ? Seeding : Searching);

	// Start the tracker client
	d->trackerClient.start(d->metaInfo);
}
