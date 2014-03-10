#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>
#include "MetaInfo.h"
#include "TorrentClient.h"

int main()
{
	TorrentClient tc;
	tc.setTorrent((char*)"debian-7.4.0-i386-CD-1.iso.torrent", strlen("debian-7.4.0-i386-CD-1.iso.torrent"));
	return 0;
}
