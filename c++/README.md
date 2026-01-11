# Huffman Coding - c++ : BLOCKED

Development Notes : 
<<<<<<< HEAD
	pack header, Why are you oh so annoying.

		lol, use the sherrection to calculate the first chunk to process.
		set the relbitIdx so that the loop can figure out where to start.
		figure out the loop,  
=======
	packByte faulure may be because of this line of code: 

		int chunk = (packingTarget >> (chunkIdx*binaryMax)) & 0xff;
	
	this line does nothing to allign packign target bits.
>>>>>>> 3f4f68a720004b1205b0b28886ab8e77ec79035c
