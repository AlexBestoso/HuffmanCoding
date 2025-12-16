#define HUFFMAN_DEBUGGING 1
class HuffmanCoding{
	private:
		char *treeLetters;
		size_t treeLetters_s;
	
		int *frequencies;
		size_t frequencies_s;

		int *codeTable;
		size_t codeTable_s;

		int *treeData;
		size_t treeData_s;
		
		bool tablesSorted;

		int error;
		std::string error_msg;
		
		void destroyTreeData(void){
			if(this->treeData != NULL)
				delete[] this->treeData;
			this->treeData = NULL;
			this->treeData_s = 0;
		}
			
		void destroyCodingTable(void){
			if(this->codeTable != NULL)
				delete[] this->codeTable;
			this->codeTable = NULL;
			this->codeTable_s = 0;
		}
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

		// returns true if target is NOT an inner node, and returns the indecies of the two values that created
		// the target value, organized relative to their encoding value (1 or 0)
		// TODO: Fix boolean. Index 9 wrongly marked as a top node.
		bool isTopNode(/*int targetVal, */int targetIndex, /*int targetLayerIndex, int *layerSizes, size_t layerSizes_s,*/ int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex){
			int topLayerIndex=0;
			int bottomLayerIndex=0;
			int layerStartDescriptor = 0;
                        size_t topSize = 0;//layerSizes[targetLayerIndex];
                        size_t bottomSize = -1; // layerSizes[targetLayerIndex-1];
			bool processing = false;

			// Determine the size and location of top and bottom layers.
			for(int i=nodeCache_s-1, test=-1; i>=0; i--){
				if(i==nodeCache_s-1)
					test = nodeCache[i];
				topSize++;
				if(processing){
					if(test == -1){
						printf("TEST BREAK A\n");
						break;
					}
					if(test > nodeCache[i]){
						topSize--;
						layerStartDescriptor = i+1;
						break;
					}
				}else{
					if(test == -1){
						printf("TEST BREAK B\n");
					}
					if(i==0){
						layerStartDescriptor = 0;
					}
					if(test > nodeCache[i]){
						bottomSize = topSize-1;
						topSize = 1;
						bottomLayerIndex = topLayerIndex;
						topLayerIndex++;
					}
				}
				if(i == targetIndex){
					processing = true;
				}
				test = nodeCache[i];
			}

			int topIterEnd = layerStartDescriptor+topSize;
                        int *top = new int[topSize];
			
			int topStart = layerStartDescriptor;
			for(int i=layerStartDescriptor, idx=0; i<topIterEnd && idx<topSize; i++, idx++){
				top[idx] = nodeCache[i];
			}


                        int *bottom = NULL;
			if(bottomSize == -1){
				// we requested a value from the bottom layer. determine if it's a 1 or a 0,
				// then set the zero and one index values respectively.
				if(targetIndex == layerStartDescriptor && (topSize%2) == 1 && targetIndex+this->frequencies_s == nodeCache_s){
					zeroIndex[0] = layerStartDescriptor;
					oneIndex[0] = targetIndex-(topSize/2);
					return true;
				}
				for(int i=topIterEnd-1, prev=-1; i>=layerStartDescriptor; i--){
					if(i == topIterEnd-1 && targetIndex == i){
						oneIndex[0] = i;
						zeroIndex[0] = i-1;
						return true;
					}else if(i == topIterEnd-1){
						prev = i;
						continue;
					}
					if(targetIndex == i || targetIndex == prev){
						oneIndex[0] = prev;
						zeroIndex[0] = i;
						return true;
					}
					i--;
					prev = i;
				}
				this->setError(12345, "isTopNode() - Failed to process provided layer 0 value.");
				oneIndex[0] = -1;
				zeroIndex[0] = -1;
				return false;
			}

			// A bottom layer exists, contine for more advanced processing.
			int bottomIterEnd = layerStartDescriptor+topSize+bottomSize;
			bottom = new int[bottomSize];
			for(int i=topIterEnd, idx=0; i<bottomIterEnd; i++, idx++){
                               	bottom[idx] = nodeCache[i];
                       	}
			
			int bottomStart = topIterEnd;
			int targetVal = nodeCache[targetIndex];
			processing = false;
			bool falseify = false;
			int finalize=-1;
			for(int i=layerStartDescriptor; i<topIterEnd; i++){
				int grabbed = nodeCache[i];
				for(int j=bottomStart; j>=bottomStart; j++){
					int zero = nodeCache[j];
					if(!(j+1<bottomIterEnd)){
						printf("Unhandled error. - bottom check out of bounds %d+1 < %d\n", j, bottomIterEnd);
						i=topIterEnd;
						break;
					}
					int one = nodeCache[j+1];
					int sum = zero + one;
					if(sum == grabbed){ // top node / leaf.
						// node is valid; but is it related to our target node?
						bottomStart = j + 2;
						if(i == targetIndex){
							if(finalize != -1){
								if((finalize == j) || (finalize == j+1)){
									return false;
								}else{
									return true;
								}
							}
							zeroIndex[0] = j;
							oneIndex[0] = j+1;
							return falseify == true ? false : true;
						}
						processing = false;
						break;
					}
					
					if(!(i+1<nodeCache_s)){
						processing = false;
						printf("Unhandled error - top check out of bounds.");
						break;
					}
					one = nodeCache[i+1];
					sum = zero+one;
					if(sum == grabbed){ // bottom node / interier node
						// node is valid; but is it related to our target node?
						bottomStart = j + 1;
						if(i == targetIndex){
							if(finalize != -1){
                                                                if((finalize == j) || (finalize == i+1)){
                                                                        return false;
                                                                }else{
                                                                        return true;
                                                                }
                                                        }
							zeroIndex[0] = j;
							oneIndex[0] = i+1;
							if(targetIndex == topStart){
								return true;
							}else{
								finalize = targetIndex;
								targetIndex--;
								bottomStart = topIterEnd;
								i = layerStartDescriptor-1;
								break;
							}
						}
						processing = false;
						if(i+1 == targetIndex){
							falseify = true;
						}
						break;
					}
				
					// we gotta maintain the I index; but shift the J starting index by 1.
					// if we don't get a match on this bottom row, then we have bad data.
					//printf("Nothing found yet, bump j start, keep i.\n");
						i--; // we only want to do this once.
					processing = true;
					bottomStart++;
					break;
				}
			}

                        delete[] bottom;
                        delete[] top;
			return true;
		}



	
		bool growLayer(void){
			if(this->treeData_s <= 0){
				return false;
			}
			if(this->treeData == NULL){
				return false;
			}
			if(this->treeData[this->treeData_s-1] == -1){
				if(this->frequencies_s <= 0){
					return false;
				}
				if(this->frequencies == NULL){
					return false;
				}
				// seeding layer
				int baseLayerStart = this->treeData_s-1;
				for(int i=baseLayerStart, j=this->frequencies_s-1; i>=this->treeData_s-this->frequencies_s && j>=0; i--, j--)
                                	this->treeData[i] = this->frequencies[j];
				int sumLayerStart = baseLayerStart-this->frequencies_s;
				for(int i=baseLayerStart, j=sumLayerStart, track=-1; i>sumLayerStart && j>=0; i--){
					if(track == -1){
						track = this->treeData[i];
						continue;
					}
					this->treeData[j] = this->treeData[i] + track;
					
					j--;
					i--;
					if(i==sumLayerStart+1 && (this->frequencies_s%2) == 1){
						treeData[j] = treeData[i] + treeData[j+1];
						break;
					}
					track = treeData[i];
				}
				
				return true;
			}

			int topLayerStart=-1;
			int topLayerEnd=-1;
			int bottomLayerStart=0;
			int bottomLayerEnd=0;
			for(int i=this->treeData_s-1; i>=0; i--){
				if(this->treeData[i] == -1){
					topLayerStart = i;
					break;
				}
			}
			if(topLayerStart == -1){
				// no more room to grow
				return false;
			}
			topLayerEnd = topLayerStart;
			bottomLayerEnd = topLayerStart+1;
			for(int i=bottomLayerEnd, track=-1; i<this->treeData_s; i++){
				if(i==topLayerStart+1)
					track = this->treeData[i];
				if(track<this->treeData[i]){
					bottomLayerStart = i-1;
					break;
				}
				track = this->treeData[i];
			}
			if(bottomLayerStart == -1)
				bottomLayerStart = this->treeData_s-1;

			for(int i=bottomLayerStart, sum=-1, next=-1, nextOffset=0; i>=bottomLayerEnd && topLayerEnd >= 0; i--){
				int z=0, o=0;
				if(!this->isTopNode(i, this->treeData, this->treeData_s, &z, &o))
					continue;
				if(next == -1){
					nextOffset=1;
					for(int j=i; i>=bottomLayerEnd; j++){
						next = i-nextOffset >= 0 && i-nextOffset >= bottomLayerEnd ? this->treeData[i-nextOffset] : -1;
						if(next == -1) break;
						int a=0, b=0;
						if(!this->isTopNode(i-nextOffset, this->treeData, this->treeData_s, &a, &b)){
							nextOffset++;
							next = -1;
						}else{
							break;
						}
					}
				}
				if(sum == -1 && this->treeData[topLayerEnd] == -1){
					sum = this->treeData[i] + next;
					this->treeData[topLayerEnd] = sum;
					topLayerEnd--;
					next = -1;
					i-=nextOffset;
					continue;
				}
				
				if(sum <= next || next == -1){
					this->treeData[topLayerEnd] = this->treeData[i] + sum;
					sum = this->treeData[topLayerEnd];
					topLayerEnd--;
					next = -1;
					continue;	
				}else{
					int a=0, b=0;
					if(!this->isTopNode(i, this->treeData, this->treeData_s, &a, &b)){
						next=-1;
						continue;
					}
					this->treeData[topLayerEnd] = this->treeData[i] + next;
					sum = this->treeData[topLayerEnd];
					topLayerEnd--;
					i-=nextOffset;
					next = -1;
					continue;
				}
			}
			return true;
		}
	
		bool plantTree(void){
			this->destroyTreeData();
			this->destroyCodingTable();
			this->treeData_s = this->frequencies_s+this->frequencies_s-1;
                        this->treeData = new int[this->treeData_s];
			this->codeTable_s = this->frequencies_s*2;
			this->codeTable = new int[this->codeTable_s];
			for(int i=0; i<this->treeData_s; i++)
				this->treeData[i] = -1;
			for(int i=0; i<this->codeTable_s; i++){
				this->codeTable[i] = 0;
			}
			
			// seed the tree, and begin coding table.
			while(this->treeData[0] == -1)
				this->growLayer();

			int zero=-1, one=-1;
			int baseLayerEnd = this->treeData_s - this->frequencies_s;
			int continueationIndex = baseLayerEnd-1;

			// Complete code table generation.
			for(int i=continueationIndex; i>=0; i--){
				bool nodeType = this->isTopNode(i, this->treeData, this->treeData_s, &zero, &one);
				// every final result under index zero, needs to have a 0 added.
				bool processing=true;
				size_t queueSize=this->treeData_s+1;
				int converter = this->treeData_s-this->frequencies_s;
				int *queue = new int[queueSize];
				int qIndex=0;
				for(int a=0; a<queueSize; a++)
					queue[a] = -1;
				queue[0] = zero;
				while(queue[0] != -1){
					int z=0, o=0;
					int target = queue[0];
					queue[0] = -1;
					if(target > baseLayerEnd){
						target = target-converter;
						this->codeTable[target]++;
                                                this->codeTable[target+this->frequencies_s] = (this->codeTable[target+this->frequencies_s] >> 1);
						for(int j=0; j<queueSize-1; j++){
							queue[j] = queue[j+1];
						}
						qIndex--;
						continue;
					}
					this->isTopNode(target, this->treeData, this->treeData_s, &z, &o);
					if(z >= baseLayerEnd && o > baseLayerEnd){
						// Shift queue, reduce size by 1.
						for(int j=0; j<queueSize-1; j++){
							queue[j] = queue[j+1];
						}
						qIndex--;

						// convert treeData index to code table index, 
						z = z - converter;
						o = o - converter;
						if(z < 0 || z >= this->codeTable_s || z+this->frequencies_s >= this->codeTable_s){
                                                        printf("INVALID INDEX !! CASE 0 A | z: %d and %ld vs %ld\n", z, z+this->frequencies_s, this->codeTable_s);
                                                        break;
                                                }
						if(o < 0 || o >= this->codeTable_s || o+this->frequencies_s >= this->codeTable_s){
                                                        printf("INVALID INDEX !! CASE 0 B\n");
                                                        break;
                                                }
						// add 0 to both z and o code table index.
						this->codeTable[z]++;
						this->codeTable[z+this->frequencies_s] = (this->codeTable[z+this->frequencies_s] >> 1);
						this->codeTable[o]++;
						this->codeTable[o+this->frequencies_s] = (this->codeTable[o+this->frequencies_s] >> 1);

					}else if(z >= baseLayerEnd && !(o > baseLayerEnd)){
						// add o to queue, shift towards 0
						if(qIndex+1 < queueSize && o != target && o != one){
							queue[qIndex+1] = o;
							for(int j=0; j<queueSize-1; j++){
                                                        	queue[j] = queue[j+1];
                                                        }
						}

						// convert treeData index to code table index, 
						z = z - converter;
						// add a 0 to index z in the code table.
						if(z < 0 || z >= this->codeTable_s || z+this->frequencies_s >= this->codeTable_s){
							printf("INVALID INDEX !! CASE 1\n");
							break;
						}
						this->codeTable[z]++;
					}else{
						// neither is at base layer, add both to queu
						if(qIndex+2 < queueSize){
							if(z != target && o != target){
								queue[qIndex+1] = z;
								queue[qIndex+2] = o;
								qIndex+=1;
							}else if(z == target){
								queue[qIndex+1] = o;
							}else{
								queue[qIndex+1] = z;
							}
							for(int j=0; j<queueSize-1; j++){
								queue[j] = queue[j+1];
							}
						}
					}
				}

				// every final result under index one, needs to have a 1 added.
				for(int a=0; a<queueSize; a++)
					queue[a] = -1;
				queue[0] = one;
				qIndex=0;
				while(queue[0] != -1){
					int z=0, o=0;
					int target = queue[0];
					this->isTopNode(target, this->treeData, this->treeData_s, &z, &o);
					queue[0] = -1;
					if(target > baseLayerEnd){
                                                target = target-converter;
                                                this->codeTable[target]++;
                                                this->codeTable[target+this->frequencies_s] = (1<<7) + (this->codeTable[target+this->frequencies_s] >> 1);
                                                for(int j=0; j<queueSize-1; j++){
                                                        queue[j] = queue[j+1];
                                                }
                                                qIndex--;
                                                continue;
                                        }
					if(z >= baseLayerEnd && o > baseLayerEnd){
						// Shift queue, reduce size by 1.
						for(int j=0; j<queueSize-1; j++){
							queue[j] = queue[j+1];
						}
						qIndex--;

						// convert treeData index to code table index, 
						z = z - converter;
						o = o - converter;
						if(z < 0 || z >= this->codeTable_s || z+this->frequencies_s >= this->codeTable_s){
                                                        printf("INVALID INDEX !! CASE 0 A | z: %d and %ld vs %ld\n", z, z+this->frequencies_s, this->codeTable_s);
                                                        break;
                                                }
						if(o < 0 || o >= this->codeTable_s || o+this->frequencies_s >= this->codeTable_s){
                                                        printf("INVALID INDEX !! CASE 0 B\n");
                                                        break;
                                                }
						// add 0 to both z and o code table index.
						this->codeTable[z]++;
						this->codeTable[z+this->frequencies_s] = (this->codeTable[z+this->frequencies_s] >> 1) + (1<<7);
						this->codeTable[o]++;
						this->codeTable[o+this->frequencies_s] = (this->codeTable[o+this->frequencies_s] >> 1) + (1<<7);

					}else if(z >= baseLayerEnd && !(o > baseLayerEnd)){
						// add o to queue, shift towards 0
						if(qIndex+1 < queueSize && o != target && one != o){
							queue[qIndex+1] = o;
							for(int j=0; j<queueSize-1; j++){
                                                        	queue[j] = queue[j+1];
                                                        }
						}

						// convert treeData index to code table index, 
						z = z - converter;
						// add a 0 to index z in the code table.
						if(z < 0 || z >= this->codeTable_s || z+this->frequencies_s >= this->codeTable_s){
							printf("INVALID INDEX !! CASE 1\n");
							break;
						}
						this->codeTable[z]++;
						this->codeTable[z+this->frequencies_s] = (this->codeTable[z+this->frequencies_s] >> 1) + (1<<7);
					}else{
						// neither is at base layer, add both to queu
						if(qIndex+2 < queueSize){
							if(z != target && o != target){
								queue[qIndex+1] = z;
								queue[qIndex+2] = o;
								qIndex+=1;
							}else if(z == target){
								queue[qIndex+1] = o;
							}else{
								queue[qIndex+1] = z;
							}
							for(int j=0; j<queueSize-1; j++){
								queue[j] = queue[j+1];
							}
						}
					}
				}


				if(queue != NULL)
					delete[] queue;
			}
			
			printf("Tree : ");
			for(int i=0; i<this->treeData_s; i++)
				printf("[%d]%d ", i, this->treeData[i]);
			printf("\n");
			printf("-----CODE TABLE----\nIDX | BIT COUNT | ENCODED VAL | ORIGINAL VAL |\n");
                        for(int i=0; i<this->frequencies_s; i++){
                                printf("%d  |    %d    |    %s    |       %c    |\n", i, this->codeTable[i], this->getCodeBinary(i).c_str(), this->treeLetters[i]);
                        }
                        printf("---------------\n");
			return true;
		}

		std::string getCodeBinary(int idx){
			std::string ret = "";
			int codeSize = this->codeTable[idx];
			int code = this->codeTable[this->frequencies_s+idx];
			for(int i=0; i<codeSize; i++){
				int bit = (code >> (7-i)) & 1;
				ret += std::to_string(bit);
			}
			return ret;
		}
		
		int charToTableIndex(char val){
			for(int i=0; i<this->treeLetters_s; i++){
				if(val == this->treeLetters[i])
					return i;
			}
			return -1;
		}

		/* Header Structure:
		 *  1 byte number of tree letters, N.
		 *  followed by N*(4 byte int, + 1 byte char)
		 * Int is stored big endian
		 * */
		bool packHeader(void){
			size_t headerSize = (this->frequencies_s*sizeof(int)) + this->treeLetters_s + 1;
			char byteOne_count = this->treeLetters_s;
			
			this->out[0] = byteOne_count;
			for(int i=1, j=0; i<headerSize && i<this->out_s && j<this->treeLetters_s && j<this->frequencies_s; i++){
				char freq_a=0,freq_b=0,freq_c=0,freq_d=0, letter=this->treeLetters[j];
				freq_a = (this->frequencies[j] >> 8*3) & 0xff;
				freq_b = (this->frequencies[j] >> 8*2) & 0xff;
				freq_c = (this->frequencies[j] >> 8*1) & 0xff;
				freq_d = this->frequencies[j] & 0xff;
				this->out[i] = freq_a; i++;
				if(!(i<this->out_s)) return false;
				this->out[i] = freq_b; i++;
				if(!(i<this->out_s)) return false;
				this->out[i] = freq_c; i++;
				if(!(i<this->out_s)) return false;
				this->out[i] = freq_d; i++;
				if(!(i<this->out_s)) return false;
				this->out[i] = letter;
				j++;
			}	
			return true;
		}
		
		bool unpackHeader(void){

			return true;
		}

		bool encode(char *data, size_t dataSize){
			if(!this->plantTree()){
				this->setError(0x502, "encode() - failed to plant tree.");
				return false;
			}
			printf("[DBG] Tree Planted!\n");
			
			this->destroyOut();
			this->out_s = 0;
			int bodySize = 0;
			for(int i=0; i<this->frequencies_s; i++){
				int freq = this->frequencies[i];
				int bitCount = this->codeTable[i];
				bodySize += bitCount * freq;
			}


			size_t headerSize = (this->frequencies_s*sizeof(int)) + this->treeLetters_s + 1; // the one continas the treeLetters_s
			char outRemainder = (bodySize%8);
			bodySize = outRemainder == 0 ? bodySize/8 : (bodySize/8) + 1;

			this->out_s = headerSize + bodySize + 1; // the one contains the outRemainder
			this->out = new char[this->out_s];
			for(int i=0; i<this->out_s; i++) this->out[i] = 0x00;

			this->packHeader();

			int bitLoop=0;
			this->out[headerSize] = outRemainder;
			printf("[DBG] encoded binary string :\n");
			for(int i=0, o=headerSize+1; i<dataSize && o<this->out_s; i++){
				int codeIndex = this->charToTableIndex(data[i]);
				std::string binary = getCodeBinary(codeIndex);
				for(int j=0; j<binary.length() && o<this->out_s; j++){
					int bit = binary[j] == '0' ? 0 : 1;
					printf("%d", bit);
					this->out[o] += bit << (7-bitLoop);
					bitLoop++;
					if((bitLoop%8) == 0){
						bitLoop=0;
						o++;
					}
				}
			}printf("\n");

			printf("[DBG] Output buffer (size : %ld | Remainder : %d) : \n", this->out_s, outRemainder);
			for(int i=0; i<this->out_s;i++){
				printf("%c", this->out[i]);
			}printf("\n");
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
			this->codeTable = NULL;
			this->codeTable_s = 0;
			this->treeLetters = NULL;
			this->treeLetters_s = 0;
			this->frequencies = NULL;
			this->frequencies_s = 0;
			this->tablesSorted = false;
			this->treeData=NULL;
			this->treeData_s = 0;
			this->clearError();
		}
		~HuffmanCoding(){
			this->destroyCodingTable();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
			this->destroyTreeData();
		}

		bool compress(char *data, size_t dataSize){
			this->clearError();
			this->destroyCodingTable();
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

			if(!this->sortFreqencies()){
				this->setError(0x004, "compress(char *data, size_t dataSize) - sortFreqencies failed.");
				return false;
			}
			#if HUFFMAN_DEBUGGING == 1
			int freqMax = 0;
			printf("[DBG] Sorted Frequencies : \n\t");
			for(int i=0; i<this->frequencies_s; i++){
				printf("'%c'(%d) ", this->treeLetters[i], this->frequencies[i]);
				freqMax += this->frequencies[i];
			}
			printf("\n");
			printf("[DBG] Expected Max : %d\n", freqMax);
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
