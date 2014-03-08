/*
 * MetaInfo.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: adrian
 */

#include "MetaInfo.h"

MetaInfo::MetaInfo() {
	// TODO Auto-generated constructor stub
	clear();
}

MetaInfo::~MetaInfo() {
	// TODO Auto-generated destructor stub
}

void MetaInfo::clear() {
	errString.clear();
	metaInfoAnnounce.clear();
	metaInfoComment.clear();
	metaInfoCreatedBy.clear();
	metaInfoName.clear();
	metaInfoPieceLength=0;
	metaInfoSha1Sums.clear();
	metaInfoAnnounceList.clear();
	metaInfoMultiFiles.clear();
}

bool MetaInfo::parse(unsigned char* data) {

}
