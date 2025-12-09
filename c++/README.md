# Huffman Coding - c++

Development Notes : 
We appear to have a working and properly created tree, though there's a lot of QA to go through.

For now we're gonna get back to building the encoding table, as this newer implementation produces a tree that reflects the pen and paper result.

0) 131 31 15 
1) 100 53 33 19 16 9 6 4 
2) 47 23 20 14 10 9 8 8 5 4 2 2 2 
3) 24 12 11 11 9 8 6 5 5 5 4 4 4 4 4 3 2 2 2 1 1 1 1 1 1 

0)                                             131 31 15 
1)                                 100 53 33 19 16 9 6 4 
2)                        47 23 20 14 10 9 8 8 5 4 2 2 2 
3) 24 12 11 11 9 8 6 5 5 5 4 4 4 4 4 3 2 2 2 1 1 1 1 1 1 

' '(24) 't'(12) 'a'(11) 'n'(11) 's'(9) 'e'(8) 'i'(6) 'y'(5) 'o'(5) 'r'(5) 'h'(4) 'f'(4) 'u'(4) 'd'(4) 'g'(4) 'w'(3) 'I'(2) 'l'(2) 'c'(2) 'W'(1) 'j'(1) ','(1) 'm'(1) 'v'(1) '.'(1)
