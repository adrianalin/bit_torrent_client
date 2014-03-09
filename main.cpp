#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>
#include "MetaInfo.h"

int main()
{
	MetaInfo mi;
	torrent_data* torrentData;
	torrentData=readFile((char*)"debian-7.4.0-i386-CD-1.iso.torrent", strlen("debian-7.4.0-i386-CD-1.iso.torrent")); //"debian-7.4.0-i386-CD-1.iso.torrent"
	cout<<"torrent data = "<<torrentData->data<<std::endl;
	mi.parse(torrentData);
	return 0;
}
