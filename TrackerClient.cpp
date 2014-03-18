/*
 * TrackerClient.cpp
 *
 *  Created on: 11.03.2014
 *      Author: adrianp
 */

#include "TrackerClient.h"
#include "ConnectionManager.h"

#ifdef WIN32
#include <ws2tcpip.h>
#endif

#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdio.h>
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
	int i=0;
	for(i=0; i<max; i++)
	{
		escaped.append("%");
		escaped.append(char2hex(c[i]));

		//		if ( (48 <= c[i] && c[i] <= 57) ||//0-9
		//				(65 <= c[i] && c[i] <= 90) ||//abc...xyz
		//				(97 <= c[i] && c[i] <= 122) || //ABC...XYZ
		//				(c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'')
		//		)
		//		{
		//			escaped.append( &c[i], 1);
		//		}
		//		else
		//		{
		//			escaped.append("%");
		//			escaped.append( char2hex(c[i]) );//converts char 255 to string "ff"
		//		}
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
	// Percent encode the hash
	std::string infoHash = torrentDownloader->infoHash();
	std::string encodedSum = urlencode(infoHash);

	bool seeding = (torrentDownloader->state() == TorrentClient::Seeding);

	char* portNumber;
	int len=metaInfo.announceUrl().length()-strlen("/announce")-strlen("http://");
	std::string hostAndPort=metaInfo.announceUrl().substr(strlen("http://"), len).c_str();
	portNumber=(char*)hostAndPort.substr(hostAndPort.find(":")+1, hostAndPort.length()-hostAndPort.find(":")).c_str();
	std::string host=hostAndPort.substr(0, hostAndPort.find_first_of(":"));

	char uploadedBytes[100];
	char downloadedBytes[100];
	char leftBytes[100];
	ConnectionManager cm(atoi(portNumber), host);
	std::string requestString, responseString;

	requestString.append("GET /announce?info_hash=");
	requestString.append(encodedSum+"&");
	requestString.append("peer_id=");
	requestString.append("ABCDEFGHIJKLMNOPQRST&");
	requestString.append("port=");
	sprintf(portNumber, "%d", 6881);
	requestString.append(portNumber);
	requestString.append("&");
	requestString.append("compact=1&");
	requestString.append("uploaded=");
	sprintf(uploadedBytes, "%d", torrentDownloader->uploadedBytes());
	requestString.append(uploadedBytes);
	requestString.append("&");

	if (!firstSeeding) {
		requestString.append("downloaded=");
		sprintf(downloadedBytes, "%d", 0);
		requestString.append(downloadedBytes);
		requestString.append("&");
		requestString.append("left=");
		sprintf(leftBytes, "%d", 0);
		requestString.append(leftBytes);
		requestString.append("&");
	} else {
		requestString.append("downloaded=");
		sprintf(downloadedBytes, "%lld", torrentDownloader->downloadedBytes());
		requestString.append(downloadedBytes);
		requestString.append("&");

		int left;
		if(metaInfo.totalSize() - torrentDownloader->downloadedBytes() > 0)
			left = metaInfo.totalSize() - torrentDownloader->downloadedBytes();
		else
			left=0;
		requestString.append("left=");
		sprintf(leftBytes, "%d", (seeding ? 0 : left));
		requestString.append(leftBytes);
		requestString.append("&");
	}

	if (seeding && firstSeeding) {
		requestString.append("event=completed");
		firstSeeding = false;
	} else if (firstTrackerRequest) {
		requestString.append("event=started");
		firstTrackerRequest = false;
	} else if(lastTrackerRequest) {
		requestString.append("event=stopped");
	}

	if (!trackerId.length()==0)
	{
		requestString.append("trackerid=");
		requestString.append(trackerId);
	}
	requestString.append(" HTTP/1.1\r\n");
	requestString.append("\r\n");
	std::cout<<"\n\n--HTTP request string--\n"<<requestString;

	responseString=cm.HTTPRequest(requestString);
}
