#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <ctime>

#include "HuffmanCoding.class.h"

int main(int argc, char *argv[]){
	std::string dbg = argc > 1 ? argv[1] : "";
	printf("[*] Running Huffman Coding 256 Byte Test\n");
	HuffmanCoding hc;
	int counts[256];
	size_t ogMsgSize = 0;
	std::srand(time(0));

	printf("[*] Generating random byte frequencies...");
	for(int i=0; i<256; i++){
		counts[i] = rand() % (60-1+1)+1;
		ogMsgSize += counts[i];
	}
	printf("%ld bytes.\n[*] Original Msg : \n\033[0;32m", ogMsgSize);
	if(dbg != "debug") printf("\tRun %s debug to view generated message.", argv[0]);
	char *ogMsg = new char[ogMsgSize];
	for(int i=0, midx=0; i<256 && midx < ogMsgSize; i++){
		for(int j=0; j<counts[i] && midx < ogMsgSize; j++){
			ogMsg[midx] = i;
			if(dbg == "debug")
				printf("0x%x ", ogMsg[midx] & 0xff);
			midx++;
		}
	}printf("\033[0m\n");

	printf("[*] Attempting Compression...");
	if(!hc.compress(ogMsg, ogMsgSize)){
		printf("Failed. %s\n", hc.getErrorMessage().c_str());
		printf("\033[5mDATA DUMP\033[0;31m\n");
		hc.printTreeLetters();
		printf("\n\033[0;32m");
		hc.printFrequencies();
		printf("\n\033[0;33m");
		hc.printCodeTable();
		printf("\n\033[1;34m");
		hc.printTree();
		delete[] ogMsg;
		printf("\033[0m");
		exit(EXIT_FAILURE);
	}
	printf("successful!\n");
	//hc.printTreeOrigins();
	
	delete[] ogMsg;
	exit(EXIT_SUCCESS);
}
