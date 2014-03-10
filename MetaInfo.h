/*
 * MetaInfo.h
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#ifndef METAINFO_H_
#define METAINFO_H_

#include <list>
#include <string.h>
#include <ctime>
#include "BencodeParser.h"

struct MetaInfoSingleFile
{
    long long length;
    const char* md5sum;
    std::string name;
    int pieceLength;
    std::list<const char* > sha1Sums; //QList<QByteArray> sha1Sums;
};

struct MetaInfoMultiFile
{
    long long length;
    char md5sum;
    std::string path;
};

class MetaInfo {
public:
	MetaInfo();
	virtual ~MetaInfo();

	enum FileForm {
		SingleFileForm,
		MultiFileForm
	};

	bool parse(torrent_data* content);

private:
	void clear();

    FileForm metaInfoFileForm;
    MetaInfoSingleFile metaInfoSingleFile;
    std::list<MetaInfoMultiFile> metaInfoMultiFiles;
    std::list<std::string> metaInfoHTTPseeds;
    std::string metaInfoAnnounce;
    std::list<std::string> metaInfoAnnounceList;
    struct tm metaInfoCreationDate;
    std::string metaInfoComment;
    std::string metaInfoCreatedBy;
    std::string metaInfoName;
    int metaInfoPieceLength;
    std::list<char*> metaInfoSha1Sums; //QList<QByteArray> metaInfoSha1Sums;
};

#endif /* METAINFO_H_ */
