#define HUFFMAN_DEBUGGING 1
class HuffmanCoding{
	private:
		char *treeLetters;
		size_t treeLetters_s;
	
		int *frequencies;
		size_t frequencies_s;

		int *codeTable;
		size_t codeTable_s;
		
		bool tablesSorted;

		int error;
		std::string error_msg;


		void destroyTreeLetters(void){
			if(this->treeLetters != NULL)
				delete[] this->treeLetters;
			this->treeLetters = NULL;
			this->treeLetters_s = 0;
		}
		void resizeTreeLetters(size_t size){
			if(size == 0){
				this->destroyTreeLetters();
			}else if(this->treeLetters == NULL){
				this->treeLetters = new char[size];
				this->treeLetters_s = size;
			}else{
				size_t oldSize = this->treeLetters_s;
				char *transfer = new char[oldSize];
				for(int i=0; i<oldSize; i++)
					transfer[i] = this->treeLetters[i];
				this->destroyTreeLetters();
				this->treeLetters = new char[size];
				this->treeLetters_s = size;
				for(int i=0; i<this->treeLetters_s && i<oldSize; i++){
					this->treeLetters[i] = transfer[i];
				}
				delete[] transfer;
			}
		}
		void destroyFrequencies(void){
			if(this->frequencies != NULL)
				delete[] this->frequencies;
			this->frequencies = NULL;
			this->frequencies_s = 0;
		}
		void resizeFrequencies(size_t size){
                        if(size == 0){
                                this->destroyFrequencies();
                        }else if(this->frequencies == NULL){
                                this->frequencies = new int[size];
                                this->frequencies_s = size;
                        }else{
                                size_t oldSize = this->frequencies_s;
                                int *transfer = new int[oldSize];
                                for(int i=0; i<oldSize; i++)
                                        transfer[i] = this->frequencies[i];
                                this->destroyFrequencies();
                                this->frequencies = new int[size];
                                this->frequencies_s = size;
                                for(int i=0; i<this->frequencies_s && i<oldSize; i++){
                                        this->frequencies[i] = transfer[i];
                                }
                                delete[] transfer;
                        }
                }
		void destroyOut(void){
			if(this->out != NULL)
				delete[] this->out;
			this->out = NULL;
			this->out_s = 0;
		}
		
		bool createTreeLetters(char *data, size_t dataSize){
			if(data == NULL){
				this->setError(0x900, "createTreeLetters(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(0x901, "createTreeLetters(char *data, size_t dataSize) - dataSize is <= 0, treating data as null");
				return false;
			}
			this->destroyTreeLetters();
			for(int i=0; i<dataSize; i++){
				char testChar = data[i];
				bool matched = false;
				for(int j=0; j<this->treeLetters_s; j++){
					if(testChar == this->treeLetters[j]){
						matched = true;
						break;
					}
				}
				if(!matched){
					this->resizeTreeLetters(this->treeLetters_s + 1);
					this->treeLetters[this->treeLetters_s-1] = testChar;
				}
			}
		
			return true;	
		}

		bool createFrequency(char *data, size_t dataSize){
			if(this->treeLetters == NULL){
				this->setError(0x910, "createFrequency(char *data, size_t dataSize) - treeLetters is null.");
				return false;
			}
			if(this->treeLetters_s <= 0){
				this->setError(0x911, "createFrequency(char *data, size_t dataSize) - treeLetters_s is <= 0, treating treeLetters as null.");
				return false;
			}
			if(data == NULL){
				this->setError(0x912, "createFrequency(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(0x913, "createFrequency(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
				return false;
			}
			this->destroyFrequencies();
			for(int i=0; i<this->treeLetters_s; i++){
				char targetChar = this->treeLetters[i];
				int freq = 0;
				for(int j=0; j<dataSize; j++){
					if(targetChar == data[j])
						freq++;
				}
				this->resizeFrequencies(this->frequencies_s + 1);
				this->frequencies[this->frequencies_s - 1] = freq;
			}
			return true;
		}

		bool sortFreqencies(void){
			if(this->treeLetters_s <= 0){
				this->setError(0x923, "sortFreqencies(void) - treeLetters_s is 0, treating as null.");
				return false;
			}
			if(this->frequencies_s <= 0){
				this->setError(0x924, "sortFreqencies(void) - frequencies_s <= 0, treating as null.");
				return false;
			}
			if(this->treeLetters_s != this->frequencies_s){
				this->setError(0x920, "sortFreqencies(void) - Table Corruption, treeLetters_s != frequencies_s.");
				return false;
			}
			if(this->treeLetters == NULL){
				this->setError(0x921, "sortFreqencies(void) - treeLetters is null.");
				return false;
			}
			if(this->frequencies == NULL){
				this->setError(0x922, "sortFreqencies(void) - frequencies is null.");
				return false;
			}

			size_t sortList_s = this->frequencies_s;
			int *sortList = new int[sortList_s];
			int *indexList = new int[sortList_s];
			int indexI = 0;
			for(int i=0; i<sortList_s; i++){
				sortList[i] = this->frequencies[i];
			}
			
			while(sortList_s > 0){
				// Identify smallest value.
				int biggest=sortList[0];
				for(int i=0; i<sortList_s; i++){
					if(sortList[i] > biggest)
						biggest = sortList[i];
				}
				
				// Gather indecies of smallest values
				int matchCount = 0;
				for(int i=0; i<this->frequencies_s && indexI<this->frequencies_s; i++){
					if(this->frequencies[i] == biggest){
						indexList[indexI] = i;
						indexI++;
						matchCount++;
					}
				}

				// strip sort list of the smallest value.
				size_t transfer_s = sortList_s-matchCount;
				int *transfer = new int[transfer_s];
				for(int i=0, t=0; i<sortList_s && t<sortList_s-matchCount; i++){
					if(sortList[i] != biggest){
						transfer[t] = sortList[i];
						t++;
					}
				}
				delete[] sortList;
				sortList = NULL;
				sortList_s = transfer_s;
				if(sortList_s > 0){
					sortList = new int[sortList_s];
					for(int i=0; i<sortList_s; i++){
						sortList[i] = transfer[i];
					}
				}
				delete[] transfer;
				this->tablesSorted = true;
			}
			
			// Use index list to re-organize frequency and tree letter arrays.
			if(sortList != NULL)
				delete[] sortList;
			sortList = new int[this->frequencies_s];
			char *sortList2 = new char[this->frequencies_s];
			for(int i=0; i<this->frequencies_s; i++){
				int targetIndex = indexList[i];
				if(targetIndex >= this->frequencies_s)
					break;
				sortList[i] = this->frequencies[targetIndex];
				sortList2[i] = this->treeLetters[targetIndex];
			}
			
			for(int i=0; i<this->frequencies_s; i++){
				this->frequencies[i] = sortList[i];
				this->treeLetters[i] = sortList2[i];
			}

			delete[] sortList2;
			delete[] sortList;
			delete[] indexList;
			return true;
		}


		/*
		 * using the starting index, find the next node in the tree.
		 * */
		int findNodeIndex(int startingIndex, size_t tree_s, int *literals, size_t literalsSize){
			// plus 1 because startingIndex should be a node.
			for(int i=startingIndex+1; i<tree_s; i++){
				bool good = false;
				for(int j=0; j<literalsSize; j++){
					if(i == literals[j]){
						good = false;
						break;
					}else{
						good = true;
					}
				}
				if(good) return i;
			}	
			return -1;
		}

		bool isLiteral(int targetIndex, int *literals, size_t literalsSize){
			if(literals == NULL){
				this->setError(0x37707, "isLiteral(int targetIndex, int *literals, size_t literalsSize) - literals is null");
				return false;
			}
			if(literalsSize <= 0){
                                this->setError(0x37707, "isLiteral(int targetIndex, int *literals, size_t literalsSize) - literalsSize <= 0, treating as null.");
                                return false;
                        }
			for(int i=0; i<literalsSize; i++){
				if(literals[i] == targetIndex) return true;
			}
			return false;
		}

		bool buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize){
			if(this->frequencies == NULL){
				this->setError(0x800, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - frequencies is null.");
				return false;
			}
			if(this->frequencies_s <= 0){
				this->setError(0x801, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - frequencies_s <= 0, treating as null.");
				return false;
			}
			if(this->treeLetters == NULL){
				this->setError(0x802, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - treeLetters is null.");
				return false;
			}
			if(this->treeLetters_s <= 0){
				this->setError(0x803, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - treeLetters_s <= 0, treating as null.");
				return false;
			}
			if(tree == NULL){
				this->setError(0x804, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - tree is null.");
				return false;
			}
			if(tree_s <= 0){
				this->setError(0x805, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - tree_s <= 0, treating as null.");
				return false;
			}
			if(literals == NULL){
				this->setError(0x806, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - literals is null.");
				return false;
			}
			if(literalsSize <= 0){
				this->setError(0x807, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - literalsSize <= 0, treating as null.");
				return false;
			}
			if(literalsSize != this->frequencies_s && this->frequencies_s != this->treeLetters_s){
				this->setError(0x808, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - literalSize not aligned with frequencies_s and treeLetters_s.");
				return false;
			}
			// init literals.
			for(int i=0; i<literalsSize; i++)
				literals[i] = -1;
			int tableIndex=this->frequencies_s-1; // we sorted from most to least frequent, start with the least frequent.
			// the number of like frequencies doesn't matter, all that matters is that this was sorted.
			int literalCtr = 0;
			int prevFreq=0;
			bool finalLeftover = (this->frequencies_s % 2) == 1 ? true : false;
			bool firstSum = true;
			for(int i=tree_s-1; i>=0; i--){
				if(tableIndex==0 && finalLeftover){
					int nodeIndexA = this->findNodeIndex(i, tree_s, literals, literalsSize);
                                        int nodeIndexB = this->findNodeIndex(nodeIndexA, tree_s, literals, literalsSize);
                                        if(nodeIndexA < 0){
						this->setError(0x809, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - NodeIndexA induces underflow.");
                                                return false;
                                        }
                                        if(nodeIndexA >= tree_s){
						this->setError(0x810, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexA induces overflow.");
                                                return false;
                                        }
                                        if(nodeIndexB < 0){
						this->setError(0x811, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexB induces underflow.");
                                                return false;
                                        }
                                        if(nodeIndexB >= tree_s){
						this->setError(0x812, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexB induces overflow.");
                                                return false;
                                        }
                                        tree[i] = tree[nodeIndexA] + tree[nodeIndexB];
                                        #if HUFFMAN_DEBUGGING == 1
                                                printf("[DBG] Node Index A : %d | Node Index B : %d\n", nodeIndexA, nodeIndexB);
                                                printf("[DBG] C tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
                                        #endif
					i--;
					if(i < 0){
						this->setError(0x813, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - loop index induces underflow.");
						return false;
					}
					if(i >= tree_s){
						this->setError(0x814, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - loop index induces overflow.");
						return false;
					}
					if(tableIndex < 0){
						this->setError(0x815, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - tableIndex induces underflow.");
						return false;
					}
					if(tableIndex >= this->treeLetters_s || tableIndex >= literalsSize || tableIndex >= this->frequencies_s){
						this->setError(0x816, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - tableeIndex induces overflow.");
						return false;
					}
					tree[i] = (int)this->treeLetters[tableIndex];
					#if HUFFMAN_DEBUGGING == 1
							printf("[DBG] D tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
					#endif
					literals[tableIndex] = i;
					int nodeIndex = this->findNodeIndex(i, tree_s, literals, literalsSize);
					if(nodeIndex < 0){
						this->setError(0x817, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndex induces underflow.");
						return false;
					}
					if(nodeIndex >= tree_s){
						this->setError(0x818, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndex induces overflow.");
						return false;
					}

					i--;
					if(i>=0){
						tree[i] = tree[nodeIndex] + this->frequencies[tableIndex];
						#if HUFFMAN_DEBUGGING == 1
							printf("[DBG] Node Index : %d\n", nodeIndex);
							printf("[DBG] E tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
						#endif
					}else{
						this->setError(0x819, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - loop index underflow caused final node failure.");
						return false;
					}
					break;
				}

				if(literalCtr >= 2){ // We have an unused node.
					literalCtr = 0;
					if(firstSum){
						// we need to create another node!
						firstSum = false;
						i++;
						continue;
					}
					int nodeIndexA = this->findNodeIndex(i, tree_s, literals, literalsSize);
					int nodeIndexB = this->findNodeIndex(nodeIndexA, tree_s, literals, literalsSize);
					if(nodeIndexA < 0){
						this->setError(0x820, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexA induces underflow.");
						return false;
					}
					if(nodeIndexA >= tree_s){
						this->setError(0x821, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexA induces overflow.");
						return false;
					}
					if(nodeIndexB < 0){
						this->setError(0x822, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexB inducees underflow.");
						return false;
					}
					if(nodeIndexB >= tree_s){
						this->setError(0x223, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - nodeIndexB induces overflow.");
						return false;
					}
					tree[i] = tree[nodeIndexA] + tree[nodeIndexB];
					#if HUFFMAN_DEBUGGING == 1
						printf("[DBG] Node Index A : %d | Node Index B : %d\n", nodeIndexA, nodeIndexB);
						printf("[DBG] C tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
					#endif
					continue;
					
				}
				if(literalCtr < 2){ // we need to create a node.
					if(tableIndex < 0){
						this->setError(0x224, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - tableIndex induces underflow.");
						return false;
					}
					if(tableIndex >= this->treeLetters_s || tableIndex >= literalsSize || tableIndex >= this->frequencies_s){
						this->setError(0x225, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - tableIndex induces underflow.");
						return false;
					}
					tree[i] = (int)this->treeLetters[tableIndex];
					#if HUFFMAN_DEBUGGING == 1
						printf("[DBG] A tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
					#endif
					literals[tableIndex] = i;
					if(literalCtr == 0){
						prevFreq = this->frequencies[tableIndex];
						tableIndex--;
						literalCtr++;
						continue;
					}

					i--;
					if(i>= 0 && i<tree_s){
						tree[i] = this->frequencies[tableIndex] + prevFreq;
						#if HUFFMAN_DEBUGGING == 1
							printf("[DBG] B tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
						#endif
						tableIndex--;
						literalCtr++;
					}else{
						this->setError(0x226, "buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize) - loop index out of bounds, caused faulure to calcualte tree.");
						return false;
					}
				}
			}
			return true;
		}

		int deriveBit(int *tree, size_t tree_s, int *literals, size_t literals_s, int targetIndex, int startingIndex){
			if(tree == NULL){
				this->setError(0x37707, "deriveBit(int *tree, size_t tree_s, int targetIndex, int startingIndex) - tree is null.");
				return -1;
			}
			if(tree_s <= 0){
				this->setError(0x37707, "deriveBit(int *tree, size_t tree_s, int targetIndex, int startingIndex) - tree_s <= 0, treating as null.");
				return -1;
			}
			if(literals == NULL){
				this->setError(0x37707, "deriveBit(...) - literals is null.");
				return -1;
			}
			if(literals_s <= 0){
				this->setError(0x3370, "deriveBit(...) - literals_s <= 0, treating as null.");
				return -1;
			}
			if(targetIndex < 0 || targetIndex >= tree_s){
				this->setError(0x37707, "deriveBit(int *tree, size_t tree_s, int targetIndex, int startingIndex) - targetIndex out of bounds.");
				return -1;
			}
			if(startingIndex < 0 || startingIndex >= tree_s){
				this->setError(0x37707, "deriveBit(int *tree, size_t tree_s, int targetIndex, int startingIndex) - startingIndex out of bounds.");
				return -1;
			}
			int ret = 0;
			int literalCount = 0;
			if(startingIndex == (tree_s-3)){
                                // at the tail end.
                                if(targetIndex == (startingIndex+1)) return 0;
                                if(targetIndex == (startingIndex+2)) return 1;
                                this->setError(0x37707, "deriveBit(...) - Failed to determine bit value (tail end)");
                                return -1;
                        }
			// if starting index is followed by two literals, the nodes are based off the two literals.
			// if the starting index is followed by a sum/illiteral, the nodes are based off of the next two illiterals.
			// if the starting index is followed by a single literal and then an illiteral, the node is based off the combination of the two.
			int matchCondition = -1;
			bool targetIsLiteral = this->isLiteral(targetIndex, literals, literals_s);
			bool matchA = this->isLiteral(startingIndex+1, literals, literals_s);
			bool matchB = this->isLiteral(startingIndex+2, literals, literals_s);
			bool matchC = this->isLiteral(startingIndex+3, literals, literals_s);
			if(targetIsLiteral){
				if(matchA && matchB){
					matchCondition = 0;
				}else if(matchA && !matchB){
					matchCondition = 1;
				}
			}else{
				if(matchA && matchB && !matchC){
                                        matchCondition = 2;
                                }else if(matchA && matchB){
                                        matchCondition = 0;
                                }else if(matchA && !matchB){
                                        matchCondition = 1;
                                }
			}
			#if HUFFMAN_DEBUGGING == 1
			printf("[DBG] Match Condition : %d \n", matchCondition);
			#endif
			
			int oneIndex;
			int zeroIndex;
			switch(matchCondition){
				case 0:
				case 1:
					zeroIndex = startingIndex + 1;
					oneIndex = startingIndex + 2;	
				break;
				case 2:
					zeroIndex = startingIndex;
					for(int i=startingIndex+2; i<tree_s; i++){
						if(!this->isLiteral(i, literals, literals_s)){
							oneIndex = i;
							break;
						}
					}
						
				break;
			}
			#if HUFFMAN_DEBUGGING == 1
			printf("[DBG] zero Index : %d | one index : %d\n", zeroIndex, oneIndex);
			#endif

			if(targetIndex == zeroIndex){
				return 0;
			}else if(targetIndex == oneIndex){
				return 1;
			}else{
				this->setError(0x37707, "deriveBit(...) - targetIndex is invalid.");
				return -1;
			}
			this->setError(0x37707, "deriveBit(...) - execution out of order, higly unusual.");
			return -1;
		}
		/*
		 * Code table structure:
		 * even byte contains bit count, odd byte contains encoded byte relative to bit count
		 * 0x3 0xff would mean that the first 3 bits, from msb -> lsb, are the encoded value.
		 * The index of each encode byte is relative to the index of the values in the code treeLetter array.
		 * because our bytes never exceed 0xff, we shouldn't need more than 8 bits of storage per encoded value.
		 * */
		bool buildCodeTable(int *tree, size_t tree_s, int *literals, size_t literals_s, size_t codeCount){
			this->codeTable_s = codeCount*2;
			this->codeTable = new int[this->codeTable_s];
			int tableIndex = 0;
			int bitCount=0;
			int encoded=0;
			for(int i=0; i<literals_s; i++){
				// identify encoded value and size.
				int targetIndex = literals[i];
				bitCount = 0;
				encoded = 0;
				bool firstRound = true;
				for(int j=targetIndex; j>=0 && j<tree_s; j--){
					bool itsALiteral = this->isLiteral(j, literals, literals_s);
					if(this->failed()){
						this->setError(0x400, "buildCodeTable(int *tree, size_t tree_s, int *literals, size_t literals_s, size_t codeCount) - literal detection failed.");
						return false;
					}
					if(itsALiteral){
						continue;
					}
					// found a sum! Now determine if it's a 0 or 1.
					#if HUFFMAN_DEBUGGING == 1
					printf("[DBG] target Index : %d | starting index : %d\n", targetIndex, j);
					#endif
					int bit = this->deriveBit(tree, tree_s, literals, literals_s, targetIndex, j);
					#if HUFFMAN_DEBUGGING == 1
					printf("[DBG] Derived Bit : %d\n", bit);
					#endif
					if(this->failed()){
						this->setError(0x401, "buildCodeTable(int *tree, size_t tree_s, int *literals, size_t literals_s, size_t codeCount) - deriveBit failed.");
						return false;
					}

					bit = bit << 7;
					bitCount++;
					encoded = (encoded >> 1) + bit;
					if(bitCount > 7) break;
					if(targetIndex == 1) break;
				
					// determine new target index.
					for(int k=j; k>=0; k--){
						if(!this->isLiteral(k, literals, literals_s) && !this->isLiteral(k+1, literals, literals_s)){
							targetIndex = k;
							j=targetIndex-1;
							break;
						}
					}
					
					#if HUFFMAN_DEBUGGING == 1
					printf("[DBG] new target : %d\n", targetIndex);
					#endif
					
				}

				#if HUFFMAN_DEBUGGING == 1
				printf("\t[DBG] Derived bit count : %d | encoded byte : 0x%x\n", bitCount, encoded);
				#endif
				
				// store encoded value and size into code table.
				if(tableIndex < this->codeTable_s){
					this->codeTable[tableIndex] = bitCount;
				}else{
					this->setError(0x402, "buildCodeTable(int *tree, size_t tree_s, int *literals, size_t literals_s, size_t codeCount) - codeTable Overflow A.");
					return false;
				}
				tableIndex++;
				if(tableIndex < this->codeTable_s){
                                        this->codeTable[tableIndex] = encoded;
                                }else{
                                        this->setError(0x402, "buildCodeTable(int *tree, size_t tree_s, int *literals, size_t literals_s, size_t codeCount) - codeTable Overflow B.");
                                        return false;
                                }
				tableIndex++;
				
			}
			return true;
		}

		bool packHeader(void){

			return true;
		}
		
		bool unpackHeader(void){

			return true;
		}

		bool encode(char *data, size_t dataSize){
			size_t treeSize = this->frequencies_s+this->frequencies_s-1;
			int *tree = new int[treeSize];
			size_t literalIndexListSize = this->frequencies_s;
			int *literalIndexList = new int[literalIndexListSize];
			if(!this->buildTree(tree, treeSize, literalIndexList, literalIndexListSize)){
				this->setError(0x500, "encode(char *data, size_t dataSize) - failed to build tree.");
				return false;
			}

			int validateNodeMax=0;
			for(int i=0; i<this->frequencies_s; i++)
				validateNodeMax += this->frequencies[i];
			if(validateNodeMax != tree[0]){
				this->setError(0x501, "encode(char *data, size_t dataSize) - node max validation failed.");
				return false;
			}
			#if HUFFMAN_DEBUGGING == 1
			printf("[DBG] Tree : ");
			for(int i=0; i<treeSize; i++){
				printf("%d ", tree[i]);
			}printf("\n");
			#endif

			if(!this->buildCodeTable(tree, treeSize, literalIndexList, literalIndexListSize, this->frequencies_s)){
				this->setError(0x502, "encode(char *data, size_t dataSize) - failed to create code table.");
				return false;
			}
			
			delete[] tree;
			return true;
		}

		bool deriveTreeLetters(void){return true;}
		bool deriveFrequency(void){return true;}

		void clearError(void){
			this->error = -1;
			this->error_msg = "";
		}

		void setError(int c, std::string m){
			this->error = c;
			this->error_msg += "["+std::to_string(c)+"] " + m+"\n";
		}

	public:
		char *out;
		size_t out_s;

		bool failed(void){
			return this->error != -1 ? true : false;
		}

		int getError(void){
			return this->error;
		}
		std::string getErrorMessage(void){
			return this->error_msg;
		}
	
		HuffmanCoding(){
			this->out = NULL;
			this->out_s = 0;
			this->treeLetters = NULL;
			this->treeLetters_s = 0;
			this->frequencies = NULL;
			this->frequencies_s = 0;
			this->tablesSorted = false;
			this->clearError();
		}
		~HuffmanCoding(){
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
		}

		bool compress(char *data, size_t dataSize){
			this->clearError();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
			this->tablesSorted = false;
			if(data == NULL){
				this->setError(0x000, "compress(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(0x001, "compress(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
				return false;
			}

			if(!this->createTreeLetters(data, dataSize)){
				this->setError(0x002, "compress(char *data, size_t dataSize) - Failed to create tree letters.");
				return false;
			}

			if(!this->createFrequency(data, dataSize)){
				this->setError(0x003, "compress(char *data, size_t dataSize) - Failed to create frequency table");
				return false;
			}

			#if HUFFMAN_DEBUGGING == 1
			printf("[DBG] Frequencies : \n\t");
			for(int i=0; i<this->frequencies_s; i++)
				printf("'%c'(%d) ", this->treeLetters[i], this->frequencies[i]);
			printf("\n");
			#endif

			if(!this->sortFreqencies()){
				this->setError(0x004, "compress(char *data, size_t dataSize) - sortFreqencies failed.");
				return false;
			}
			#if HUFFMAN_DEBUGGING == 1
			printf("[DBG] Sorted Frequencies : \n\t");
			for(int i=0; i<this->frequencies_s; i++)
				printf("'%c'(%d) ", this->treeLetters[i], this->frequencies[i]);
			printf("\n");
			#endif

			if(!this->encode(data, dataSize)){
				this->setError(0x004, "compress(char *data, size_t dataSize) - Failed to encode data.");
				return false;
			}
			

			return true;
		}

		bool decompress(char *data, size_t dataSize){
			this->clearError();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
			return true;
		}
};
