#pragma once

#include <string>
#include <vector>
#include "Bnode.h"

class Bencode {
/*	class InfoDirectory {
	public:
		std::string pieceLength;
		std::string pieces;
		std::string name;
		std::vector<BNode> fileList;

		bool isPrivate = false;
	};*/
public:
	BNode decodedNode;

	/*std::string announce;
	std::vector<std::string> announce_list;
	std::string creationDate;
	std::string comment;
	std::string createdBy;
	std::string encoding;*/
	
	void decode(std::string tf);
	int getIntFromTF(const std::string &tf,const int tfcurpos, int& bytesRead);
	int getNodeType(const std::string& tf,const int tfcurpos);
	BNode* createDictNode(const std::string &tf, int& tfcurpos, const bool hasLabel);
	BNode* createListNode(const std::string& tf, int& tfcurpos, const bool hasLabel);
	BNode* createStringNode(const std::string& tf, int& tfcurpos);
	BNode* createInegerNode(const std::string& tf, int& tfcurpos, const bool hasLabel);
};