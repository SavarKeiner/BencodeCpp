#include "bencode.h"
#include "Bnode.h"
#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>

void Bencode::decode(std::string tf) {
	int tfcurpos = 0;
	int nodeType = -1;

	nodeType = getNodeType(tf, tfcurpos);
	if (nodeType == 3) {
		this->decodedNode = *createDictNode(tf, tfcurpos, false);
	}
}

BNode* Bencode::createDictNode(const std::string& tf, int& tfcurpos, const bool hasLabel) {
	int nodeType = 0;
	BNode* node = new BNode();
	node->type = NodeType::dict;
	node->nodeList = *(new std::vector<BNode>());

	if (hasLabel == true) {
		int lenght = 0;
		int bytesRead = 0;
		lenght = getIntFromTF(tf, tfcurpos, bytesRead);
		tfcurpos = tfcurpos + bytesRead + 1;
		node->key = tf.substr(tfcurpos, lenght);
		tfcurpos += lenght;

		std::cout << "Creating Dict Node with label: current position: " << tfcurpos << " key: " << node->key << std::endl;
	}
	else {
		std::cout << "Creating Dict Node without label: current position: " << tfcurpos << node->key << std::endl;
	}

	tfcurpos += 1; //remove 'd'

	while (nodeType != NodeType::end) {
		nodeType = getNodeType(tf, tfcurpos);

		if (nodeType == NodeType::byteString) {//bytestring
			int bytesRead = 0;
			int lenght = 0;
			lenght = getIntFromTF(tf, tfcurpos, bytesRead);
			nodeType = getNodeType(tf, tfcurpos + bytesRead + 1 + lenght);

			if (nodeType == NodeType::byteString) { //example "3:abc3:def"
				node->nodeList.push_back(*createStringNode(tf, tfcurpos));
			}
			else if (nodeType == NodeType::list) { //examle "3:abcle" this means that the list has a label
				BNode* bns = createListNode(tf, tfcurpos, true);
				node->nodeList.push_back(*bns);
			}
			else if (nodeType == NodeType::dict) { //example "3:abcde" this means that the dictionary has a label
				BNode* bns = createDictNode(tf, tfcurpos, true);
				node->nodeList.push_back(*bns);
			}
			else if (nodeType == NodeType::integer) { // example "6:lengthi123456789e"
				BNode* bns = createInegerNode(tf, tfcurpos, true);
				node->nodeList.push_back(*bns);
			}

		} else 	if (nodeType == NodeType::list) { //list
			//example "le"
			BNode* bn = createListNode(tf, tfcurpos, false);
			node->nodeList.push_back(*bn);

		} else if (nodeType == NodeType::dict) { //dict
			// example "de"
			BNode* bn = createDictNode(tf, tfcurpos, true);
			node->nodeList.push_back(*bn);
		} else if (nodeType == NodeType::integer) { //integer
			//integer without label should not exist in dict
		}
		else if (nodeType == NodeType::end) { //end
			tfcurpos += 1;
		}
	}

	return node;
}

BNode* Bencode::createListNode(const std::string& tf, int& tfcurpos, const bool hasLabel) {
	int nodeType = 0;
	BNode * bn = new BNode();
	bn->type = NodeType::list;
	bn->nodeList = *(new std::vector<BNode>());

	if (hasLabel == true) {
		int lenght = 0;
		int bytesRead = 0;
		lenght = getIntFromTF(tf, tfcurpos, bytesRead);
		tfcurpos = tfcurpos + bytesRead + 1;
		bn->key = tf.substr(tfcurpos, lenght);
		tfcurpos += lenght;

		std::cout << "Creating List Node with label: current position: " << tfcurpos << " key: " << bn->key << std::endl;
	}
	else {
		std::cout << "Creating List Node without label: current position: " << tfcurpos << std::endl;
	}

	tfcurpos += 1; //remove 'l'
	
	while (nodeType != NodeType::end) {
		nodeType = getNodeType(tf, tfcurpos);

		if (nodeType == NodeType::byteString) {// example 3:abc
			int lenght = 0;
			int bytesRead = 0;
			int nodeType = 0;
			lenght = getIntFromTF(tf, tfcurpos, bytesRead);
			nodeType = getNodeType(tf, tfcurpos + bytesRead + 1 + lenght);

			if (nodeType == NodeType::list) { //examle "3:abcle"
				BNode* bns = createListNode(tf, tfcurpos, true);
				bn->nodeList.push_back(*bns);
			}
			else if (nodeType == NodeType::dict) { //example "3:abcde"
				BNode* bns = createDictNode(tf, tfcurpos, true);
				bn->nodeList.push_back(*bns);
			}
			else { //example 3:abc
				BNode *bns = new BNode();
				int bytesRead1 = 0;
				int lenght1 = 0;

				lenght1 = getIntFromTF(tf, tfcurpos, bytesRead1);
				tfcurpos = tfcurpos + bytesRead1 + 1;
				bns->value = tf.substr(tfcurpos, lenght1);
				tfcurpos += lenght1;

				bn->nodeList.push_back(*bns);

				std::cout << "Created List Node with string as value: current position: " << tfcurpos << " value: " << bns->value << std::endl;
			}
		}
		else if (nodeType == NodeType::list) { //list example "le"
			BNode* bns = createListNode(tf, tfcurpos, false);
			bn->nodeList.push_back(*bns);
		}
		else if (nodeType == NodeType::dict) { //dict example "de"
			BNode* bns = createDictNode(tf, tfcurpos, false);
			bn->nodeList.push_back(*bns);
		}
		else if (nodeType == NodeType::integer) { //example i1234567890e
			BNode* bns = createInegerNode(tf, tfcurpos, false);
			bn->nodeList.push_back(*bns);
		}
		else if (nodeType == NodeType::end) { //end
			tfcurpos += 1; //remove 'e'
		}
	}
	return bn;
}

int Bencode::getNodeType(const std::string& tf,const int tfcurpos) {
	if (isdigit(tf[tfcurpos])) { //this is a string
		return NodeType::byteString;
	}
	else if (tf[tfcurpos] == 'l') { //this is a list
		return NodeType::list;
	}
	else if (tf[tfcurpos] == 'd') { //this is a dict
		return NodeType::dict;
	}
	else if (tf[tfcurpos] == 'i') { //this is a integer
		return NodeType::integer;
	}
	else if (tf[tfcurpos] == 'e') { //this is e
		return NodeType::end;
	}
	return -1;
}

BNode* Bencode::createStringNode(const std::string& tf, int& tfcurpos) {
	BNode *bn = new BNode();
	bn->type = NodeType::byteString;
	int bytesRead = 0;
	int lenght = 0;

	lenght = getIntFromTF(tf, tfcurpos, bytesRead);
	tfcurpos = tfcurpos + bytesRead + 1;
	bn->key = tf.substr(tfcurpos, lenght);
	tfcurpos += lenght;

	lenght = getIntFromTF(tf, tfcurpos, bytesRead);
	tfcurpos = tfcurpos + bytesRead + 1;
	bn->value = tf.substr(tfcurpos, lenght);
	tfcurpos += lenght;

	std::cout << "Created byteString Node: " << tfcurpos << " key: " << bn->key << " value: " << bn->value << std::endl;
	return bn;
}

BNode* Bencode::createInegerNode(const std::string& tf, int& tfcurpos, const bool hasLabel) {
	BNode *bn = new BNode();
	bn->type = NodeType::integer;
	int bytesRead = 0;
	int lenght = 0;

	if (hasLabel) {
		lenght = getIntFromTF(tf, tfcurpos, bytesRead);
		tfcurpos = tfcurpos + bytesRead + 1;
		bn->key = tf.substr(tfcurpos, lenght);
		tfcurpos += lenght;
	}

	tfcurpos++; //remove 'i'
	getIntFromTF(tf, tfcurpos, bytesRead);
	bn->value = tf.substr(tfcurpos, bytesRead);
	tfcurpos += bytesRead;
	tfcurpos++; //remove 'e'

	std::cout << "created Integer Node: key: " << bn->key << " value: " << bn->value << std::endl;
	return bn;
}

int Bencode::getIntFromTF(const std::string &tf,const int tfcurpos, int& bytesRead) {
	int i = 0;
	int returnVal = 0;
	std::string intAsString;

	for (i = 0; i < (tf.length() - tfcurpos); i++) {
		if (isdigit(tf[tfcurpos + i]) == false)
			break;
	}

	if (i == 0) {
		std::cout << "---ERROR: could not extract int: " << tf.substr(tfcurpos) << std::endl;
		bytesRead = 0;
		return 0;
	}

	intAsString = tf.substr(tfcurpos, i);
	returnVal = std::stoi(intAsString);
	bytesRead = intAsString.length();

	return returnVal;
}