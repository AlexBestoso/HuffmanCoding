# Huffman Coding - c++ : BLOCKED

Development Notes : 
	packByte faulure may be because of this line of code: 

		int chunk = (packingTarget >> (chunkIdx*binaryMax)) & 0xff;
	
	this line does nothing to allign packign target bits.
