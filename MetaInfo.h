/*
 * MetaInfo.h
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#ifndef METAINFO_H_
#define METAINFO_H_

#include <list>
#include <string>
#include <ctime>

struct MetaInfoSingleFile
{
    long long length;
    unsigned char md5sum;
    std::string name;
    int pieceLength;
    std::list<unsigned char> sha1Sums; //QList<QByteArray> sha1Sums;
};

struct MetaInfoMultiFile
{
    long long length;
    unsigned char md5sum;
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

	bool parse(unsigned char* data);

private:
	void clear();

    std::string errString;
    unsigned char content[10];
    unsigned char infoData[10];

    FileForm metaInfoFileForm;
    MetaInfoSingleFile metaInfoSingleFile;
    std::list<MetaInfoMultiFile> metaInfoMultiFiles;
    std::string metaInfoAnnounce;
    std::list<std::string> metaInfoAnnounceList;
    struct tm metaInfoCreationDate;
    std::string metaInfoComment;
    std::string metaInfoCreatedBy;
    std::string metaInfoName;
    int metaInfoPieceLength;
    std::list<unsigned char> metaInfoSha1Sums; //QList<QByteArray> metaInfoSha1Sums;
};

#endif /* METAINFO_H_ */
