/*
 * TrackerClient.cpp
 *
 *  Created on: 11.03.2014
 *      Author: adrianp
 */

#include "TrackerClient.h"
#include "happyhttp.h"

#include <ws2tcpip.h>

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

void httpRequestOnBegin(const happyhttp::Response* r, void* userdata) {
	printf( "BEGIN (%d %s)\n", r->getstatus(), r->getreason() );
}

void httpRequestOnData(const happyhttp::Response* r, void* userdata, const unsigned char* data, int n) {
	fwrite(data, 1, n, stdout);
}

void httpRequestOnDone(const happyhttp::Response* r, void* userData) {
	std::cout<<"finished request"<<std::endl;
}

void TrackerClient::fetchPeerList() {
#ifdef WIN32
	WSAData wsaData;
	int code = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if( code != 0 )
	{
		fprintf(stderr, "shite. %d\n",code);
		return ;
	}
#endif //WIN32
	// Percent encode the hash
	std::string infoHash = torrentDownloader->infoHash();
	std::string encodedSum = urlencode(infoHash);

	bool seeding = (torrentDownloader->state() == TorrentClient::Seeding);

	try
	{
		const char* headers[5];
		happyhttp::Connection conn(torrentDownloader->metaInfo().announceUrl().c_str(), 80);
		conn.setcallbacks(httpRequestOnBegin, httpRequestOnData, httpRequestOnDone, 0);

		headers[0]="info_hash";
		headers[1]=encodedSum.c_str();

		//		conn.putheader("info_hash", encodedSum.c_str());
		//		conn.putheader("peer_id", ConnectionManager::instance()->clientId());
		conn.putheader("port", static_cast<std::ostringstream*>( &(std::ostringstream() << 6881) )->str().c_str());
		conn.putheader("compact", "1");
		conn.putheader("uploaded", static_cast<std::ostringstream*>( &(std::ostringstream() << torrentDownloader->uploadedBytes()) )->str().c_str());

		if (!firstSeeding) {
			conn.putheader("downloaded", "0");
			conn.putheader("left", "0");
		} else {
			conn.putheader("downloaded", static_cast<std::ostringstream*>( &(std::ostringstream() << torrentDownloader->downloadedBytes()) )->str().c_str());
			int left;
			if(metaInfo.totalSize() - torrentDownloader->downloadedBytes() > 0)
				left = metaInfo.totalSize() - torrentDownloader->downloadedBytes();
			else
				left=0;
			conn.putheader("left", static_cast<std::ostringstream*>( &(std::ostringstream() << (seeding ? 0 : left)) )->str().c_str());
		}

		if (seeding && firstSeeding) {
			conn.putheader("event", "completed");
			firstSeeding = false;
		} else if (firstTrackerRequest) {
			firstTrackerRequest = false;
			conn.putheader("event", "started");
		} else if(lastTrackerRequest) {
			conn.putheader("event", "stopped");
		}

		if (!trackerId.length()==0)
			conn.putheader("trackerid", trackerId.c_str());

		conn.request( "GET", "/happyhttp/test.php", 0, 0,0 );
		while( conn.outstanding() )
			conn.pump();
	}

	catch( happyhttp::Wobbly& e )
	{
		fprintf(stderr, "Exception:\n%s\n", e.what() );
	}

#ifdef WIN32
	WSACleanup();
#endif // WIN32
}
