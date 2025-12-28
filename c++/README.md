# Huffman Coding - c++

Development Notes : 

Today we're focused on the unpack header, and are unpacking the container size.

there is a condtion where we get a decompression failure.

We need to modify the compression header structure.
Solution should include: 
	- create a struct that contains all the required header variables.
	- upate back and unpack functions to use this new struct.

The problem appears to be a flaw in the packing algorithm that induces a misalignment of bits.
