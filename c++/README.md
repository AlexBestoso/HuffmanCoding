# Huffman Coding - c++

So far, we have everything needed to make the tree; but We, As in Me, need to figure out the best way to actually create and store this tree in memory.

We determine the values of this tree from leaf to root…

so…

this following istance should be of size… 9

a(1) b(1) c(2) d(2) e(3)
  \   /	              /
    2	    \   /    /
    \	      4     /
      \	     /     /
	 6        /
	  \      /
	      9

This tree would look like the above…

Process is:
	add all even matches of frequency (1), move to the next frequency
	add remainder frequency with the first frequency of this round, add all even counts, move to the next frequency.
	before moving to the next frequency, you want to combine all lower freuencies into one freuqency sum.

	that's the secret.

	add all your sums, and before going to add the next layer of frequency sums, you must add all pairs.

	because there's only two freuencies of 1, we just add them, and then move directly to the next frequncy.
	because there's only two frequencies of 2, we just add them; but because there's an existing node (sum 2), before moving on to the final freq, we need to add our sum of 4 with our sum of 6.
	Now that the highest node (value 6) sits alone, without pair, we can iterate to the next frequency (3) and add THAT to our 6.

	it's all about adding remainder nodes.

That's the process; but now I gotta figure out the encoding.
