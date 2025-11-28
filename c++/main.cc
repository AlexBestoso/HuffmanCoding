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
	printf("NOT YET FINISHED\n");
	
	exit(EXIT_SUCCESS);
}
