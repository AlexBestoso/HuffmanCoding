# Huffman Coding - c++

Development Notes : 

it appears that the flaw is in entry container...
*] Round i:132/256, hi:315 < dataSize:8101


There seems to be some potential high missalignments.
Need to go through the unpack functions and better track the byte targeted for unpacking, and it's iterator, hi.
