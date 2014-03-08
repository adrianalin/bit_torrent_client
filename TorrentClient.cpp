/*
 * TorrentClient.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include <stdio.h>
#include "TorrentClient.h"

TorrentClient::TorrentClient() {
	// TODO Auto-generated constructor stub

}

TorrentClient::~TorrentClient() {
	// TODO Auto-generated destructor stub
}

bool TorrentClient::setTorrent(char* fileName)
{
	FILE* pFile;
	pFile = fopen(fileName, "r");
	if(pFile==NULL){
		perror("Error  opening file");
	}
	else
	{

	}

	return true;
}
