# Huffman Coding - c++

Security review of the `encode()` function is next, which will complete the review of the compression progress,
leaving only the decompress code left to review.
---- devnotes above ----

To use vim as an IDE, run `vim -u ./huff.vimrc`, to open all the files with line folds.

documentation pending.
## Execution Statistics
```
===Round 99999 of 100000
[*] Generating random data to compress.
[*] Compressing 7824 bytes of data at location 0x556a547c7b30
[^] Starting timer...
[*] Compression time:  took 0.011011 to execute.


[*] Decompressing 8627 bytes of data at location 0x556a547ce070
[^] Starting timer...
[*] Decompression time:  took 0.010690 to execute.


[SUCCESS] Message passed compression AND decompression.
----------RESULTS----------
breakdown of element 'Compress Element'
	count: 100000
	sumation: 1093.595881
	quotant: 0.010936
Average Compression time : 0.010936 seconds.

breakdown of element 'Decompress Element'
	count: 100000
	sumation: 874.194227
	quotant: 0.008742
Average Decompression time : 0.008742 seconds.
```
