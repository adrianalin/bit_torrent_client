#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>
#include "MetaInfo.h"

torrent_data* readFile(char* name, size_t num){
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
	fseek(input_file, 0, 2);
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
	return torrent;
}

int main()
{
	MetaInfo mi;
	torrent_data* torrentData;
	torrentData=readFile((char*)"debian-7.4.0-i386-CD-1.iso.torrent", strlen("debian-7.4.0-i386-CD-1.iso.torrent")); //"debian-7.4.0-i386-CD-1.iso.torrent"
	cout<<"torrent data = "<<torrentData->data<<std::endl;
	mi.parse(torrentData);
	return 0;
}
