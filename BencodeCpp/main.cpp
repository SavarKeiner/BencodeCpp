
#include <fstream>
#include<iostream>
#include "bencode.h"

void printvalues(BNode node);

int main() {
	Bencode *abc = new Bencode();
	std::ifstream is("C:\\Users\\Slav\\Desktop\\903646CC99DD59B48BB539EC42D80D7B980FD938.torrent", std::ifstream::binary);

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
	_CrtDumpMemoryLeaks();
	return 0;
}

void printvalues(BNode node) {
	std::cout << "key: " << node.key << " value: " << node.value << " type " << node.type << std::endl;

	for (int i = 0; i < node.nodeList.size(); i++) {
		//std::cout << "key: " << node.nodeList.at(i).key << " value: " << node.nodeList.at(i).value << " type " << node.nodeList.at(i).type << std::endl;

		if (!node.nodeList.empty())
			printvalues(node.nodeList.at(i));
	}
}