# Huffman Coding - c++

Development Notes : 

Problem, After grow layer detects a bottom node, we need to flip the context to account for odd layer elements.
And if we hit another bottom layer, it need to switch back to even layer element handling.

If it's odd, then the last value will always be a top node, and the second to last will always be a bottom.
if it's even, it SHOULD be both top nodes.
