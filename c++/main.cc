#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include <time.h>

#include "HuffmanCoding.class.h"

clock_t timeStart, timeEnd;
double cpuTime;
struct element{
	int count;
	double sumation;
	double quotant;
};

struct element initElement(struct element e){
	e.count = 0;
	e.sumation = 0.0;
	e.quotant = 0.0;
	return e;
}
void printElement(const char *elementName, struct element e){
	printf("breakdown of element '%s'\n", elementName);
	printf("\tcount: %d\n\tsumation: %lf\n\tquotant: %lf\n", e.count, e.sumation, e.quotant);
}

struct element compressElement;
struct element decompressElement;

void startTimer(void){
	timeStart = clock();
}

void endTimer(void){
	timeEnd = clock() - timeStart;
	cpuTime = ((double)timeEnd) / CLOCKS_PER_SEC;
}

void printTimer(const char *operationDescription){
	printf("%s took %lf to execute.\n", operationDescription, cpuTime);
}

int main(int argc, char *argv[]){
	compressElement = initElement(compressElement);
	decompressElement = initElement(decompressElement);
	int rounds=1;
	if(argc >= 2){
		rounds = std::stoi(argv[1]);
	}
	for(int q=0; q < rounds; q++){
	system("clear");
	printf("===Round %d of %d\n", q, rounds);
	printf("[*] Generating random data to compress.\n");
	HuffmanCoding hc;
	int counts[256];
	size_t ogMsgSize = 0;
	size_t endTestSize = 0;
	std::srand(time(0));

	for(int i=0; i<256; i++){
		counts[i] = rand() % (60-1+1)+1;
		ogMsgSize += counts[i];
	}
	endTestSize = ogMsgSize;
	char *ogMsg = new char[ogMsgSize];
	for(int i=0, midx=0; i<256 && midx < ogMsgSize; i++){
		for(int j=0; j<counts[i] && midx < ogMsgSize; j++){
			ogMsg[midx] = i;
			midx++;
		}
	}
	
	printf("[*] Compressing %ld bytes of data at location %p\n", ogMsgSize, ogMsg);
	printf("[^] Starting timer...\n");
	startTimer();
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
		printf("\033[0m");
		exit(EXIT_FAILURE);
	}
	endTimer();
	compressElement.count++;
	compressElement.sumation += cpuTime;
	compressElement.quotant = compressElement.sumation / compressElement.count;
	printTimer("[*] Compression time: ");
	printf("\n\n");
	
	size_t compressedData_s = hc.out_s;
	char *compressedData = new char[compressedData_s];
	for(int i=0; i<hc.out_s; i++){
		compressedData[i] = hc.out[i];
	}

	printf("[*] Decompressing %ld bytes of data at location %p\n", compressedData_s, compressedData);
	printf("[^] Starting timer...\n");
	startTimer();
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
                printf("\033[0m");
                exit(EXIT_FAILURE);
	}
	endTimer();
	decompressElement.count++;
	decompressElement.sumation += cpuTime;
	decompressElement.quotant = decompressElement.sumation / decompressElement.count;
	printTimer("[*] Decompression time: ");
	printf("\n\n");


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
		exit(EXIT_FAILURE);
	}
	
	int goods=0, bads=0;
	for(int i=0; i<endTestSize; i++){
		if(ogMsg[i] != hc.out[i]){
			bads = i;
			break;
		}else{
			goods++;
		}
	}

	if(goods != endTestSize){
		printf("[TEST RESULTS] %d good matches, Failure on iteration %d.\n", goods, bads);
		printf("[FAILURE] Compression was not successful.\n");
		exit(EXIT_FAILURE);
	}else{
		printf("[SUCCESS] Message passed compression AND decompression.\n");
	}
	}
	printf("----------RESULTS----------\n");
	printElement("Compress Element", compressElement);
	printf("Average Compression time : %lf seconds.\n\n", compressElement.quotant);
	printElement("Decompress Element", decompressElement);
	printf("Average Decompression time : %lf seconds.\n", decompressElement.quotant);
	exit(EXIT_SUCCESS);
}
