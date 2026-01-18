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
	int testRuns = 1;
	testRandom256Colors();
	qa.logCurrent = "[*]-~ Test Name : Random 256\n";
	qa.logCurrent += " |_-~ Description: Ensure that compression on a file containing all 256 possible bytes doesn't fail.\n";
	qa.logCurrent += " |_-~ Test will run "+std::to_string(testRuns)+" times.\n";
	qa.lprint(qa.logCurrent, qa.logCurrent.length());
	
	HuffmanCoding hc;
	std::string testMessage="";
	size_t msgSize=0;
	printf("\r |");
	testMessage = testRandom256GenMessage(&msgSize);

	if(!hc.createTreeLetters((char*)testMessage.c_str(), msgSize)){
		qa.logCurrent = "\n[E]-~ Test Failed.\n";
	//	qa.logCurrent += "|_-~ Iteration "+std::to_string(i)+" of "+std::to_string(testRuns)+"\n";
		qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
		qa.lprint(qa.logCurrent, qa.logCurrent.length());
		return false;
	}
		
	if(!hc.createFrequency((char*)testMessage.c_str(), msgSize)){
		qa.logCurrent = "\n[E]-~ Test Failed.\n";
		qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
		qa.lprint(qa.logCurrent, qa.logCurrent.length());
		return false;
	}

	 
	if(!hc.sortFreqencies()){
		qa.logCurrent = "\n[E]-~ Test Failed.\n";
		qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
		qa.lprint(qa.logCurrent, qa.logCurrent.length());
		return false;
	}

	if(!hc.plantTree()){
		qa.logCurrent = "\n[E]-~ Test Failed.\n";
		qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
		qa.lprint(qa.logCurrent, qa.logCurrent.length());
		return false;
	}

	if(!hc.generateCodeTable()){
		qa.logCurrent = "\n[E]-~ Test Failed.\n";
		qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
		qa.lprint(qa.logCurrent, qa.logCurrent.length());
		return false;
	}

	if(!hc.encode((char*)testMessage.c_str(), msgSize)){
		qa.logCurrent = "\n[E]-~ Test Failed.\n";
		qa.logCurrent += "|_-~ Error Message : "+hc.getErrorMessage()+"\n";
		qa.lprint(qa.logCurrent, qa.logCurrent.length());
		return false;
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
