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
bool testRandom256(void){
	int testRuns = 200;
	testRandom256Colors();
	qa.logCurrent = "[*]-~ Test Name : Random 256\n";
	qa.logCurrent += " |_-~ Description: Ensure that compression on a file containing all 256 possible bytes doesn't fail.\n";
	qa.logCurrent += " |_-~ Test will run "+std::to_string(testRuns)+" times.\n";
	qa.logCurrent += "\r |";
	qa.lprint(qa.logCurrent, qa.logCurrent.length());

	struct timeElement elementA = qa.newElement("compress_createTreeLetters");
	struct timeElement elementB = qa.newElement("compress_createFrequency");
	struct timeElement elementC = qa.newElement("compress_sortFreqencies");
	struct timeElement elementD = qa.newElement("compress_plantTree");
	struct timeElement elementE = qa.newElement("compress_generateCodeTable");
	struct timeElement elementF = qa.newElement("compress_encode");
	
	for(int i=0; i<testRuns; i++){
		HuffmanCoding hc;
		std::string testMessage="";
		size_t msgSize=0;
		testMessage = testRandom256GenMessage(&msgSize);

		printf("\r /");
		fflush(stdout);
		qa.startElementTimer(&elementA);
		if(!hc.createTreeLetters((char*)testMessage.c_str(), msgSize)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementA);
		
		printf("\r —");
		fflush(stdout);
		qa.startElementTimer(&elementB);
		if(!hc.createFrequency((char*)testMessage.c_str(), msgSize)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementB);

	 	printf("\r \\");
		fflush(stdout);
		qa.startElementTimer(&elementC);
		if(!hc.sortFreqencies()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementC);

		printf("\r |");
		fflush(stdout);
		qa.startElementTimer(&elementD);
		if(!hc.plantTree()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementD);

		printf("\r /");
		fflush(stdout);
		qa.startElementTimer(&elementE);
		if(!hc.generateCodeTable()){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.stopElementTimer(&elementE);

		printf("\r *");
		fflush(stdout);
		qa.startElementTimer(&elementF);
		if(!hc.encode((char*)testMessage.c_str(), msgSize)){
			qa.logCurrent = "\n[E]-~ Test Failed.\n";
			qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
			qa.lprint(qa.logCurrent, qa.logCurrent.length());
			return false;
		}
		qa.startElementTimer(&elementF);
		printf("\r \\");
		fflush(stdout);
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
				if(!testRandom256()){
					
				}
			break;
		}
	}
	qa.quit(EXIT_SUCCESS);
}
