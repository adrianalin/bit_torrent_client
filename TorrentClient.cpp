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

bool TorrentClient::setTorrent(char* name, size_t num)
{
	char fileName[50];
	torrent_data* torrent;
	int numberOfBytesRead;

	torrent = (torrent_data*)malloc(sizeof(torrent_data));
	if(!torrent){
		std::cout<<"error allocate memor for torrent"<<std::endl;
		return NULL;
	}

	//check filename
	if(num>sizeof(fileName)){
		std::cout<<"filename "<<name<<" too long"<<std::endl;
		return NULL;
	}

	//read filename
	memset(fileName, 0, sizeof(fileName));
	memcpy(fileName, name, num);
	std::cout<<"filename = "<<fileName<<std::endl;

	//open file
	FILE *input_file = fopen(fileName, "rb");
	if(!input_file){
		std::cout<<"error opening file "<<fileName<<std::endl;
		return NULL;
	}

	//calculate file size
	fseek(input_file, 0, SEEK_END);
	torrent->size = ftell(input_file);
	rewind(input_file);
	//alloc memory for reading whole file
	torrent->data = (char*)malloc(torrent->size * (sizeof(char)));

	if(!torrent->data){
		std::cout<<"error allocating memory for file contents"<<std::endl;
		return NULL;
	}

	//read file contents
	numberOfBytesRead = fread(torrent->data, sizeof(char), torrent->size, input_file);
	fclose(input_file);

	//check read
	if(numberOfBytesRead != torrent->size){
		std::cout<<"error: numberOfBytesRead != input_file_size";
		return NULL;
	}

	std::cout<<"number of bytes read = "<<numberOfBytesRead<<std::endl;
	setTorrent(torrent);

	return true;
}

bool TorrentClient::setTorrent(torrent_data* torrent) {
	if(!d->metaInfo.parse(torrent))
}
