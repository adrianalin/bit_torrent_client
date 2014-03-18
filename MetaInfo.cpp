/*
 * MetaInfo.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include "MetaInfo.h"
#include "BencodeParser.h"

using namespace std;

int numberOfDigits(int num){
	int nd=0;
	while(num!=0){
		nd=nd+1;
		num=num/10;
	}
	return nd;
}

MetaInfo::MetaInfo() {
	// TODO Auto-generated constructor stub
	clear();
}

MetaInfo::~MetaInfo() {
	// TODO Auto-generated destructor stub
}

std::string MetaInfo::infoValue() const {
	return infoData;
}

MetaInfo::FileForm MetaInfo::fileForm() const {
	return metaInfoFileForm;
}

std::string MetaInfo::announceUrl() const {
	return metaInfoAnnounce;
}

std::list<std::string> MetaInfo::announceList() const {
	return metaInfoAnnounceList;
}

int MetaInfo::creationDate() const {
	return metaInfoCreationDate;
}

std::string MetaInfo::comment() const {
	return metaInfoComment;
}

std::string MetaInfo::createdBy() const {
	return metaInfoCreatedBy;
}

MetaInfoSingleFile MetaInfo::singleFile() const {
	return metaInfoSingleFile;
}

std::string MetaInfo::name() const {
	return metaInfoName;
}

int MetaInfo::pieceLength() const {
	return metaInfoPieceLength;
}

std::list<std::string> MetaInfo::sha1Sums() const {
	return metaInfoSha1Sums;
}

int MetaInfo::totalSize() const {
	if (fileForm() == SingleFileForm)
		return singleFile().length;

//	qint64 size = 0;
//	foreach (MetaInfoMultiFile file, multiFiles())
//	size += file.length;
//	return size;
}

std::list<MetaInfoMultiFile> MetaInfo::multiFiles() const {
	return metaInfoMultiFiles;
}

void MetaInfo::clear() {
	metaInfoAnnounce.clear();
	metaInfoComment.clear();
	metaInfoCreatedBy.clear();
	metaInfoName.clear();
	metaInfoPieceLength=0;
	metaInfoSha1Sums.clear();
	metaInfoAnnounceList.clear();
	metaInfoMultiFiles.clear();
	metaInfoHTTPseeds.clear();
}

bool MetaInfo::parse(string& content) {
	clear();

	bencode_t ben, ben2;
	BencodeParser parser;
	parser.bencode_init(&ben, content.c_str(), (int)content.length());
	const char* buf;
	long int number;
	int len;

	totalSizeSingleFile = content.length();

	if(!parser.bencode_is_dict(&ben)){
		std::cout<<"not a dictionary";
		return false;
	}

	while(parser.bencode_dict_has_next(&ben))
	{
		if(parser.bencode_dict_get_next(&ben, &ben2, &buf, &len) == 0)
		{
			std::cout<<"breaking the loop\n";
			break;
		}
		std::string key(buf, len);

		if(key == "announce"){
			parser.bencode_string_value(&ben2, &buf, &len);
			metaInfoAnnounce= string(buf, len);
			std::cout<<key<<"="<<metaInfoAnnounce<<std::endl;
		} else if(key == "comment"){
			parser.bencode_string_value(&ben2, &buf, &len);
			metaInfoComment = string(buf, len);
			std::cout<<key<<"="<<metaInfoComment<<std::endl;
		} else if(key == "creation date"){
			parser.bencode_int_value(&ben2, &number);
			std::cout<<key<<"="<<number<<std::endl;
			//			metaInfoCreationDate
		} else if(key == "httpseeds"){
			cout<<key<<std::endl;
			if(parser.bencode_is_list(&ben2)){
				bencode_t ben3;
				while(parser.bencode_list_has_next(&ben2)){
					if(!parser.bencode_list_get_next(&ben2, &ben3))
					{
						std::cout<<"error getting list element\n";
						return false;
					}
					std::string val;
					parser.bencode_string_value(&ben3, &buf, &len);
					val = string(buf, len);
					std::cout<<val<<std::endl;
					metaInfoHTTPseeds.push_back(val);
				}
			}
		} else if(key == "info") { //get info section
			std::cout<<key<<std::endl;
			int infoSectionLength=strlen("d");
			infoData = string(ben2.str, ben2.len);
			if(parser.bencode_is_dict(&ben2)){
				metaInfoFileForm = SingleFileForm;
				bencode_t ben3;
				while(parser.bencode_dict_has_next(&ben2)){
					if(!parser.bencode_dict_get_next(&ben2, &ben3, &buf, &len)){
						std::cout<<"error getting dictionary element\n";
						return false;
					}

					std::string key(buf, len);
					infoSectionLength = infoSectionLength+numberOfDigits(len)+len+1;
					if(key == "files"){
						metaInfoFileForm = MultiFileForm;
					} else if(key=="length"){
						parser.bencode_int_value(&ben3, &number);
						infoSectionLength = infoSectionLength + numberOfDigits(number) + 2;
						metaInfoSingleFile.length = number;
						std::cout<<key<<"="<<number<<std::endl;
					} else if(key=="name"){
						parser.bencode_string_value(&ben3, &buf, &len);
						infoSectionLength = infoSectionLength+len+numberOfDigits(len)+1;
						metaInfoSingleFile.name=string(buf, len);
						std::cout<<key<<"="<<metaInfoSingleFile.name<<std::endl;
					} else if(key=="piece length"){
						parser.bencode_int_value(&ben3, &number);
						infoSectionLength = infoSectionLength + numberOfDigits(number)+2;
						metaInfoSingleFile.pieceLength=number;
						std::cout<<key<<"="<<metaInfoSingleFile.pieceLength<<std::endl;
					} else if(key=="md5sum"){
						parser.bencode_string_value(&ben3, &buf, &len);
						infoSectionLength = infoSectionLength+len+numberOfDigits(len)+1;
						metaInfoSingleFile.md5sum = string(buf, len).c_str();
						std::cout<<key<<"="<<metaInfoSingleFile.md5sum<<std::endl;
					} else if(key=="pieces"){
						std::cout<<key<<std::endl;
						parser.bencode_string_value(&ben3, &buf, &len);
						infoSectionLength = infoSectionLength+len+numberOfDigits(len)+1;
						const char* sha1Data=string(buf, len).c_str();
						for(int i=0; i<len; i+=20){
							char* sha1Sum= (char*)malloc(20);
							memcpy(sha1Sum, sha1Data, 20);
							std::cout<<sha1Sum<<std::endl;
							metaInfoSingleFile.sha1Sums.push_back(sha1Sum);
							sha1Data=sha1Data+20;
						}
					}
				}
			}
			infoSectionLength = infoSectionLength+1;
			infoData = string(infoData.c_str(), infoSectionLength);
			std::cout<<"infoSectionLength = "<<infoSectionLength<<std::endl;
		}
	}
	return true;
}
