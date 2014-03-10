/*
 * TorrentClient.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include <stdio.h>
#include <string.h>
#include <fstream>
#include "TorrentClient.h"
#include "MetaInfo.h"

class TorrentClientPrivate {
public:
    TorrentClient::Error error;
    TorrentClient::State state;
    std::string errorString;
    std::string stateString;

    void setError(TorrentClient::Error errorCode);
    TorrentClientPrivate();
    virtual ~TorrentClientPrivate();

    // Where to save data
    std::string destinationFolder;
    MetaInfo metaInfo;
};

TorrentClientPrivate::TorrentClientPrivate() {
    // TODO Auto-generated constructor stub

}

TorrentClientPrivate::~TorrentClientPrivate() {
    // TODO Auto-generated destructor stub
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
    d = new TorrentClientPrivate;
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

    return true;
}
