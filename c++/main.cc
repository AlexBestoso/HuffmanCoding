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
	size_t endTestSize = 0;
	std::srand(time(0));

	printf("[*] Generating random byte frequencies...\n");
	for(int i=0; i<256; i++){
		counts[i] = rand() % (60-1+1)+1;
		ogMsgSize += counts[i];
	}
	endTestSize = ogMsgSize;
	printf("[*] Original Msg (size : %ld) : \n\033[0;32m", ogMsgSize);
	if(dbg != "debug") printf("\tRun %s debug to view generated message.", argv[0]);
	char *ogMsg = new char[ogMsgSize];
	for(int i=0, midx=0; i<256 && midx < ogMsgSize; i++){
		for(int j=0; j<counts[i] && midx < ogMsgSize; j++){
			ogMsg[midx] = i;
			if(dbg == "debug")
				printf("0x%x ", ogMsg[midx] & 0xff);
			midx++;
		}
	}
	printf("\033[0m\n");
		

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
	if(dbg == "debug"){
		printf("\033[0;34m");
		hc.printTreeLetters();
                printf("\n\033[0;32m");
                hc.printFrequencies();
                printf("\n\033[0;33m");
                hc.printCodeTable();

	}
	printf("successful!\n");
	
	size_t compressedData_s = hc.out_s;
	char *compressedData = new char[compressedData_s];
	printf("[*] Generated Output (size : %ld):\n", hc.out_s);
	if(dbg != "debug") printf("\t\033[0;32mRun %s debug to view generated message.\033[0m", argv[0]);
	for(int i=0; i<hc.out_s; i++){
		compressedData[i] = hc.out[i];
		
		if(dbg == "debug"){
			if((i%16) == 0) printf("\n");
			printf("%x ", hc.out[i]&0xff);
		}
	}
	printf("\n");

	printf("[*] Attempting decompression...");
	if(!hc.decompress(compressedData, compressedData_s)){
		printf("Failed %s\n", hc.getErrorMessage().c_str());
		printf("\033[5mDATA DUMP\033[0;31m\n");
                hc.printTreeLetters();
                printf("\n\033[0;32m");
                hc.printFrequencies();
                printf("\n\033[0;33m");
                hc.printCodeTable();
                printf("\n\033[1;34m");
                hc.printTree();
                delete[] ogMsg;
		delete[] compressedData;
                printf("\033[0m");
                exit(EXIT_FAILURE);
	}
	printf("success!\n");

	printf("[*] Generated Output (size : %ld):\n", hc.out_s);
	if(dbg != "debug") printf("\t\033[0;32mRun %s debug to view generated message.\033[0m", argv[0]);
	for(int i=0; i<hc.out_s; i++){
		compressedData[i] = hc.out[i];
		
		if(dbg == "debug"){
			if((i%16) == 0) printf("\n");
			printf("%x ", hc.out[i]&0xff);
		}
	}
	printf("\n");

	if(endTestSize != hc.out_s){
		printf("\n\033[5mDATA DUMP\033[0;31m\n");
                hc.printTreeLetters();
                printf("\n\033[0;32m");
                hc.printFrequencies();
                printf("\n\033[0;33m");
                hc.printCodeTable();
                printf("\n\033[1;34m");
                hc.printTree();
		printf("[FAILED] Invalid message. Decompressed message is not the same size as the original! og:%ld != decomp:%ld\n", ogMsgSize, hc.out_s);
		delete[] ogMsg;
        	delete[] compressedData;
		exit(EXIT_FAILURE);
	}
	
	for(int i=0; i<endTestSize; i++){
		if(ogMsg[i] != hc.out[i]){
			printf("[FAILED] Invalid message on index %d. Original message is not the same as the decompressed message.\n", i);
			delete[] ogMsg;
        		delete[] compressedData;
			exit(EXIT_FAILURE);
		}
	}

	delete[] ogMsg;
	delete[] compressedData;

	printf("[SUCCESS] Message passed compression AND decompression.");
	exit(EXIT_SUCCESS);
}
