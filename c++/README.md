# Huffman Coding - c++

Development Notes : 

Okay, So I've found yet another algorithm.

This method requires me to modify what I got now so that the nodeCache contains the frequency values from the table.

By doing this, and storing the size of each "layer", the nodeCache can be read in reverse, using the layers array, to determine which values are of what layer.

We can then use these two arrays, and a layer index, to determin if a nodeCache value in the previous layer is under a node within the same layer, or not.

	     		Layers :    0 		1 			2				3
in our example we have nodeCache : -1 -1 -1 | 90 43 33 19 16 9 6 4 |  47 23 20 14 10 9 8 8 5 4 2 2 2 |
The pipes separate the layers.
layer 1 is improperly calculated because of layer 2.
layer 1s 43 value was calculated using layer 1 (33) and layer 2 (23); but this is wrong.
Because in layer 2, 23 isn't a top node. It's in the shadow of the value 47.
But we can't determine this with layer 2 alone.
We need to have available in our node cache our original frequencies so that we can, via function, determine if 23 was created using a value from the same layer that it resides. What I call a subnode.

Once done, we shoudld be able to turn layer 1's  90 and 43 into 100 and 53


          80
            33             6
	      19   16  9    4	
47 23                               20 14    10 9       8 8 5 4      2 2 2


47  23     20 14 10 9 8 8 5 4 2 2 2 
24  12 11     11 9 8 6 5 5 5 4 4 4 4 4 3 2 2 2 1 1 1 1 1 1


 
100 53 33 19 16 9 6 4 


top nodes are 100, 16, 9, and 6


     100    
   /     53
  /     /   33             6
 /     /   /  19  16  9   / 4 
47 23 20 14 10 9 8 8 5 4 2 2 2


              131       
     100              31
   /     53        /    15 
  /     /   33    /   /    6
 /     /   /  19  16  9   / 4 
47 23 20 14 10 9 8 8 5 4 2 2 2
