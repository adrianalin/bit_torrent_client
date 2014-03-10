/*
 * MetaInfo.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include "MetaInfo.h"

using namespace std;

MetaInfo::MetaInfo() {
	// TODO Auto-generated constructor stub
	clear();
}

MetaInfo::~MetaInfo() {
	// TODO Auto-generated destructor stub
}

void MetaInfo::clear() {
//	metaInfoAnnounce.clear();
//	metaInfoComment.clear();
//	metaInfoCreatedBy.clear();
//	metaInfoName.clear();
//	metaInfoPieceLength=0;
//	metaInfoSha1Sums.clear();
//	metaInfoAnnounceList.clear();
//	metaInfoMultiFiles.clear();
//	metaInfoHTTPseeds.clear();
}

bool MetaInfo::parse(torrent_data* content) {
	clear();

	bencode_t ben, ben2;
	BencodeParser parser;
	parser.bencode_init(&ben, content->data, content->size);
	const char* buf;
	long int number;
	int len;

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
			std::string str(buf, len);
			metaInfoAnnounce = str;
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
		} else if(key == "info") {
			std::cout<<key<<std::endl;
			if(parser.bencode_is_dict(&ben2)){
				metaInfoFileForm = SingleFileForm;
				bencode_t ben3;
				while(parser.bencode_dict_has_next(&ben2)){
					if(!parser.bencode_dict_get_next(&ben2, &ben3, &buf, &len)){
						std::cout<<"error getting dictionary element\n";
						return false;
					}

					std::string key(buf, len);
					if(key == "files"){
						metaInfoFileForm = MultiFileForm;
					} else if(key=="length"){
						parser.bencode_int_value(&ben3, &number);
						metaInfoSingleFile.length = number;
						std::cout<<key<<"="<<number<<std::endl;
					} else if(key=="name"){
						parser.bencode_string_value(&ben3, &buf, &len);
						metaInfoSingleFile.name=string(buf, len);
						std::cout<<key<<"="<<metaInfoSingleFile.name<<std::endl;
					} else if(key=="piece length"){
						parser.bencode_int_value(&ben3, &number);
						metaInfoSingleFile.pieceLength=number;
						std::cout<<key<<"="<<metaInfoSingleFile.pieceLength<<std::endl;
					} else if(key=="md5sum"){
						parser.bencode_string_value(&ben3, &buf, &len);
						metaInfoSingleFile.md5sum = string(buf, len).c_str();
						std::cout<<key<<"="<<metaInfoSingleFile.md5sum<<std::endl;
					} else if(key=="pieces"){
						std::cout<<key<<std::endl;
						parser.bencode_string_value(&ben3, &buf, &len);
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
		}
	}
	return true;
}
