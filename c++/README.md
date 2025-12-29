# Huffman Coding - c++

Development Notes : 

move in {
a = 7+2‎ = 9
7 - a ‎ = -2
X=n>>(-2)&  7
}
bi=7
as=bi+2
sa=7-as
isa=as-7
x=d >>  sa & 7

equaation A : x = d >> (7-(7+2)) & 7
if ovflo
i++
errcheck
equation B : x+=d >> (8-isa) & 7


bi++


Today we're focused on the unpack header, and are unpacking the container size.

there is a condtion where we get a decompression failure.

We need to modify the compression header structure.
Solution should include: 
	- create a struct that contains all the required header variables.
	- upate back and unpack functions to use this new struct.

The problem appears to be a flaw in the packing algorithm that induces a misalignment of bits.
