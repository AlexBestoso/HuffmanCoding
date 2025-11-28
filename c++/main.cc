#include <stdio.h>
#include <unistd.h>
#include <string>

#include "HuffmanCoding.class.h"

int main(void){
	printf("Huffman Coding Tests\n");

	std::string msg = "What I always found interesting was the fact that I can just write for days and days, seemingly never running out of things to say.";
	printf("Encoding the message (%ld) : \n\t%s\n", msg.length(), msg.c_str());
	exit(EXIT_SUCCESS);
}
