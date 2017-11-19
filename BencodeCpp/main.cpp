#include <fstream>
#include<iostream>
#include "bencode.h"

void printvalues(BNode node);

int main() {
	Bencode *abc = new Bencode();
	std::string path = "C:\\"; // path to .torrent file
	std::ifstream is(path, std::ifstream::binary);

	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);

		char * buffer = new char[length];
		is.read(buffer, length);

		std::string str(buffer, length);

		abc->decode(str);

		is.close();

		delete[] buffer;
		std::cout << "encoder end" << std::endl;
	}

	std::cout << "printing all nodes" << std::endl;
	for (int i = 0; i < abc->decodedNode.nodeList.size(); i++) {
		printvalues(abc->decodedNode.nodeList.at(i));
	}
	delete abc;
	return 0;
}

void printvalues(BNode node) {
	std::cout << "key: " << node.key << " value: " << node.value << " type " << node.type << std::endl;

	for (int i = 0; i < node.nodeList.size(); i++) {
		if (!node.nodeList.empty())
			printvalues(node.nodeList.at(i));
	}
}