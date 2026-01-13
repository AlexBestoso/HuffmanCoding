# Huffman Coding - c++ : BLOCKED

Development Notes : 
	- update the body variable to be an int* instead of a char*
	- Use frequency and code table to calculate body size.
	- use unpacked encoded body to decode the compressed data.
	- unpackHeader() should no longer conduct decoding. That should be done in a dedicated function.
