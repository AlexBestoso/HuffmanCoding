#include <stdio.h>
#include <unistd.h>
#include <string>

#include "HuffmanCoding.class.h"

int main(void){
	printf("Huffman Coding Tests\n");
	HuffmanCoding hc;

	std::string msg = "What I always found interesting was the fact that I can just write for days and days, seemingly never running out of things to say.";
	printf("Compressing the message (%ld) : \n\t%s\n", msg.length(), msg.c_str());

	if(!hc.compress((char *)msg.c_str(), msg.length())){
		printf("Compression failed : %s\n", hc.getErrorMessage().c_str());
		exit(EXIT_FAILURE);
	}

	size_t compressedSize = hc.out_s;
	char *compressedMsg = new char[compressedSize];
	printf("Compressed Data : ");
	for(int i=0; i<hc.out_s; i++){
		compressedMsg[i] = hc.out[i];
		printf("%c", hc.out[i]);
	}
	printf("\n");

	if(!hc.decompress(compressedMsg, compressedSize)){
		printf("Decompression failed : %s\n", hc.getErrorMessage().c_str());
		exit(EXIT_FAILURE);
	}
	
	printf("Decompressed Data : ");
	for(int i=0; i<hc.out_s; i++){
		printf("%c", hc.out[i]);
	}
	printf("\n");
	delete[] compressedMsg;
	exit(EXIT_SUCCESS);
}
