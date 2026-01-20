#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include <time.h>
#include <fcntl.h>

#include "HuffmanCoding.class.h"

#include "./qa.class.h"
#include "./qaFunctions/qaFunctions.h"

int testId=0;
const int testsAvailable=1;
int testsRandom256Rounds=200;

#define TEST_RANDOM256 0
void testRandom256Colors(void){
	qa.color_reset();
	qa.color_whiteBg();
	qa.color_blackFg();
}

std::string testRandom256GenMessage(size_t *sizeOut){
	std::string ret = "";
	int byteBox[256];
	sizeOut[0] = 0;
	for(int i=0; i<256; i++){
		byteBox[i] = (rand() % 256) + 1;
		for(int j=0; j<byteBox[i]; j++){
			ret += (char)i;
			sizeOut[0]++;
		}
	}
	return ret;
}

bool testRandom256(int argc, char *argv[]){
	if(argTag == "-t0rounds="){
		testsRandom256Rounds = std::stoi(argValue);
	}
	testRandom256Colors();
	qa.logCurrent = "[*]-~ Test Name : Random 256\n";
	qa.logCurrent += " |_-~ Description: Ensure that compression on a file containing all 256 possible bytes doesn't fail.\n";
	qa.logCurrent += " |_-~ Test will run "+std::to_string(testsRandom256Rounds)+" times.\n";
	qa.logCurrent += "\r |";
	qa.lprint(qa.logCurrent, qa.logCurrent.length());

	struct timeElement elementA = qa.newElement("compress_createTreeLetters");
	struct timeElement elementB = qa.newElement("compress_createFrequency");
	struct timeElement elementC = qa.newElement("compress_sortFreqencies");
	struct timeElement elementD = qa.newElement("compress_plantTree");
	struct timeElement elementE = qa.newElement("compress_generateCodeTable");
	struct timeElement elementF = qa.newElement("compress_encode");
	int lastMsgSize = 0;
	int lastCompressionSize = 0;
	int lastDifference = 0;
	int differenceSum = 0;
	double averageDifference = 0;

	
		HuffmanCoding hc;
		std::string testMessage="";
	for(int i=0; i<testsRandom256Rounds; i++){
		std::string compMsg="";
		size_t compMsgSize=0;
		size_t msgSize=0;

		// Generate Message.
		testMessage = testRandom256GenMessage(&msgSize);
		lastMsgSize = msgSize;

		// Compress Message.
		printf("\r /");
		fflush(stdout);
		qa.startElementTimer(&elementA);
		if(!hc.createTreeLetters((char*)testMessage.c_str(), msgSize)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, createTreeLetters: "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementA);
		
		printf("\r —");
		fflush(stdout);
		qa.startElementTimer(&elementB);
		if(!hc.createFrequency((char*)testMessage.c_str(), msgSize)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, createFrequency: "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementB);

	 	printf("\r \\");
		fflush(stdout);
		qa.startElementTimer(&elementC);
		if(!hc.sortFreqencies()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, sortFreqencies: "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementC);

		printf("\r |");
		fflush(stdout);
		qa.startElementTimer(&elementD);
		if(!hc.plantTree()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, plantTree: "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementD);

		printf("\r /");
		fflush(stdout);
		qa.startElementTimer(&elementE);
		if(!hc.generateCodeTable()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, generateCodeTable: "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementE);

		printf("\r *");
		fflush(stdout);
		qa.startElementTimer(&elementF);
		if(!hc.encode((char*)testMessage.c_str(), msgSize)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, encode: "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementF);
		printf("\r \\");

		// Calculate compressed file size difference.
		lastCompressionSize = hc.out_s;
		lastDifference = lastMsgSize - lastCompressionSize;
		differenceSum += lastDifference;
		averageDifference = differenceSum / (i+1);
		fflush(stdout);
		printf("out size : %ld\n", hc.out_s);

		// Transfer data:
		for(int a=0; a<hc.out_s; a++){
			compMsg += hc.out[a];
		}
		compMsgSize = hc.out_s;

		// Decompress Message
		HuffmanCoding hcD;
		int bodyStart = 0;
		int bodyPadding = 0;
		int headerPadding = 0;
		if(!hcD.unpackHeader((char *)compMsg.c_str(), compMsgSize, &bodyStart, &bodyPadding, &headerPadding)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, unpackHeader: "+hcD.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}

		if(!hcD.plantTree()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, plantTree: "+hcD.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}

		if(!hcD.generateCodeTable()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, generateCodeTable: "+hcD.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}

		if(!hcD.unpackBody((char *)compMsg.c_str(), compMsgSize, bodyStart, headerPadding, bodyPadding)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message, unpackBody: "+hcD.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}

		// Validate decompression 
	}
	
	return true;
}
int main(int argc, char *argv[]){
	srand(time(NULL));
	qa.freshLog();
	welcome(argc, argv);
	for(int i=0;i<testsAvailable; i++){
		switch(testId){
			case TEST_RANDOM256:
				if(!testRandom256(argc, argv)){
					
				}
			break;
		}
	}
	qa.quit(EXIT_SUCCESS);
}
