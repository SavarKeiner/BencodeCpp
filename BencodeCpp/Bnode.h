#pragma once

#include <string>
#include <vector>

enum NodeType { list = 1, integer = 2, dict = 3, byteString = 4, end = 5, listString = 6 };

class BNode {
public:
	std::string key;
	std::string value;
	std::vector<BNode> nodeList;
	NodeType type;
};