# Huffman Coding - c++


-    /    10   \
-  / 6  \       |
-  |    |    /  4   \
-  |    |    |    /  2  \   
 q(3) e(3) c(2) b(1) a(1)
  0    1    2    3    4
Goal, using the above tree, derive frequency indecies.

left, add none to index, right add 1.
	

2 4 6 10


has something to do wiht the index of the nodes.
node index

Potential encoding method:

Layout the tree into a single dimentional signed int array, including both the nodes and the literals.
Take note of the indecies of each literal, and store them in a seperate int array.

Now from literal, starting at the index provided, traverse to the root to build an encoding table.
Table will be a multi dimensional array that matches the target byte, the total number of bits used in the encoded value, AND the actual encoded byte.
You will then be able to use that to create an encoded bit stream.

The decoding process will use knowledge of the root index, and the literal index array, to translate the encoded values back to the literal values.

The frquency table will be stored as follows:
	the very first byte will tell us how many literals to use.
	then for each literal we will store the the size of the frequency size(1 byte), followed by the literal(1 byte), followed by the size(1 to 255 bytes)
	creating a header that will be of the following structure : [literal count] ([freq size] [literal] [freq])
	
	this method will allow us to store data without having to store the full size of an int data type, typically 4 bytes. Potentially saving us 1 byte per literal in the header.
