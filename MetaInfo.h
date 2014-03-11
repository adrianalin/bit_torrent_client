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

	bool parse(std::string &content);

	std::string infoValue() const;
	MetaInfo::FileForm fileForm() const;
	std::string announceUrl() const;
	std::list<std::string> announceList() const;
	int creationDate() const;
	std::string comment() const;
	std::string createdBy() const;

	// For single file form
	MetaInfoSingleFile singleFile() const;

	// For multifile form
	std::list<MetaInfoMultiFile> multiFiles() const;
	std::string name() const;
	int pieceLength() const;
	std::list<std::string> sha1Sums() const;

	// Total size
	int totalSize() const;

private:
	void clear();
	FileForm metaInfoFileForm;
	MetaInfoSingleFile metaInfoSingleFile;
	std::list<MetaInfoMultiFile> metaInfoMultiFiles;
	std::list<std::string> metaInfoHTTPseeds;
	std::string metaInfoAnnounce;
	std::list<std::string> metaInfoAnnounceList;
	int metaInfoCreationDate;
	std::string metaInfoComment;
	std::string metaInfoCreatedBy;
	std::string metaInfoName;
	std::string infoData;
	int metaInfoPieceLength;
	int totalSizeSingleFile;
	std::list<std::string> metaInfoSha1Sums; //QList<QByteArray> metaInfoSha1Sums;
};

#endif /* METAINFO_H_ */
