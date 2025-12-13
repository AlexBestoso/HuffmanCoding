# Huffman Coding - c++

Development Notes : 
new_isTopNode() appears to work with all the leaf and node values in my test tree.
It has also been redesigned so that it can be used to build both the tree AND the code table at the same time.


From the below output we can see the symptoms of the error.
on this single unit's processing, we add a bit to the codeTable indecies 1 and 2 TWICE!

because we hit the end of the node, and the o variable is equal to the one variable... 
47 is made of index 24 (24) and index 12 (23)

24 is NOT made of 24 and 12, this is a bug. but we can fix this by not adding the offending 12 to the queue.
	if the o and z variables equal to the one and zero, then we know we've got an invalid o/z, depending on which value is >= baseLayerStart.

The logic is that the top left and right paths cannot share paths from it's lower nodes.
the node 47 creates this pattern: 

[index]frequency

     [11]47
 [24]24  [12]23

Because if the nature of nodes, on the 0 path, left, we cannot produce another [12]23, which is what this "processing zero bit" output shows 
happening.

our code creates this : 

     11
 24      12
24 12   25 26

Which is wrong. What we want is

     11
 24      12
        25 26

CodeGen top node [11]47 | zero : 24 | one : 12
	Processing zero bit...
		zero process | target:24 | z:24 | o:12
		B | Adding 0 bit to 0 only
		New Queue : 12 ~
		zero process | target:12 | z:25 | o:26
		A | Adding 0 bit to 1 and 2
		New Queue : ~
	Processing one bit...		One process | target:12 | z:25 | o:26
		A | Adding 1 bit to 1 and 2
		New Queue : ~

