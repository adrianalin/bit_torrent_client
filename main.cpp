#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>
#include "MetaInfo.h"
#include "TorrentClient.h"

int main()
{
	TorrentClient client;
    if(!client.setTorrent((char*)"debian-7.4.0-i386-CD-1.iso.torrent"))
    {
    	cout<<"file cannot be loaded!";
    	return 1;
    }
    client.setDestinationFolder(".");
    client.start();
	return 0;
}
