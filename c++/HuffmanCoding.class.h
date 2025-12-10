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
		bool new_isTopNode(/*int targetVal, */int targetIndex, /*int targetLayerIndex, int *layerSizes, size_t layerSizes_s,*/ int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex){
			int topLayerIndex=0;
			int bottomLayerIndex=0;
			int layerStartDescriptor = 0;
                        size_t topSize = 0;//layerSizes[targetLayerIndex];
                        size_t bottomSize = -1; // layerSizes[targetLayerIndex-1];
			bool processing = false;

			// Determine the size and location of top and bottom layers.
			for(int i=nodeCache_s-1, test=-1; i>=0; i--){
				topSize++;
				if(i==nodeCache_s-1)
					test = nodeCache[i];
				if(processing){
					if(test > nodeCache[i]){
						topSize--;
						layerStartDescriptor = i+1;
						break;
					}
				}else{
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

		//	printf("Measured top size %ld at layer index %d\n", topSize, topLayerIndex);
		//	printf("Measured bottom size %ld at layer index %d\n", bottomSize, bottomLayerIndex);
		//	printf("Measured start descriptor : %d\n", layerStartDescriptor);

			int topIterEnd = layerStartDescriptor+topSize;
                        int *top = new int[topSize];
			
			//printf("top : ");
			for(int i=layerStartDescriptor, idx=0; i<topIterEnd && idx<topSize; i++, idx++){
				top[idx] = nodeCache[i];
			//	printf("%d ", top[idx]);
			}//printf("\n");


                        int *bottom = NULL;
			if(bottomSize == -1){
				// we requested a value from the bottom layer. determine if it's a 1 or a 0,
				// then set the zero and one index values respectively.
				if(targetIndex == layerStartDescriptor && (topSize%2) == 1){
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
		//	printf("bottom : ");
			for(int i=topIterEnd, idx=0; i<bottomIterEnd; i++, idx++){
                               	bottom[idx] = nodeCache[i];
		//		printf("%d ", bottom[idx]);
                       	}//printf("\n");
			
			int bottomStart = topIterEnd;
			int targetVal = nodeCache[targetIndex];
			processing = false;
			for(int i=layerStartDescriptor; i<topIterEnd; i++){
				int grabbed = nodeCache[i];
				for(int j=bottomStart; j<bottomIterEnd; j++){
					int zero = nodeCache[j];
					if(!(j+1<bottomIterEnd)){
						printf("Unhandled error. - bottom check out of bounds\n");
						break;
					}
					int one = nodeCache[j+1];
					int sum = zero + one;
					if(sum == grabbed){ // top node / leaf.
						// node is valid; but is it related to our target node?
						bottomStart = j + 2;
						if(i == targetIndex){
							zeroIndex[0] = j;
							oneIndex[0] = j+1;
						//	printf("Grabbed top node.\n");
							return true;
						}
						processing = false;
						break;
					}
					
					if(!(i+1<topIterEnd)){
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
						//	printf("Grabbed bottom node.\n");
							zeroIndex[0] = j;
							oneIndex[0] = i+1;
							return false;
						}
						processing = false;
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


		bool isTopNode(int targetVal, int targetIndex, int targetLayerIndex, int *layerSizes, size_t layerSizes_s, int *nodeCache, size_t nodeCache_s){
			if(targetLayerIndex < 1 || targetVal == -1) // haven't made any subnodes yet.
                                return true;
			int leadupOffset = nodeCache_s-1;
			for(int i=0; i<targetLayerIndex-1; i++){
				leadupOffset-= layerSizes[i];
			}

                        size_t topSize = layerSizes[targetLayerIndex];
                        size_t bottomSize = layerSizes[targetLayerIndex-1];
                        int *top = new int[topSize];
                        int *bottom = new int[bottomSize];
			

                        for(int i=leadupOffset, j=bottomSize-1; i>leadupOffset-bottomSize && j>=0; i--, j--){
                                bottom[j] = nodeCache[i];
                        }
                        leadupOffset -= bottomSize;
                        bool updated = false;
                        for(int i=leadupOffset, j=topSize-1; i>leadupOffset-topSize && j>=0; i--, j--){
                                top[j] = nodeCache[i];
                                if(i == targetIndex && !updated){
                                        targetIndex = j;
                                        updated = true;
                                }
                        }

			int bottomStart=0;
                        for(int i=0; i<topSize; i++){
                                if(i==0 && top[i] == targetVal && targetIndex == 0) break;
                                for(int j=bottomStart; j<bottomSize; j++){
                                        if(j+1 >= bottomSize) break;
                                        if(i+1 >= topSize) break;
                                        if(top[i] == (bottom[j] + bottom[j+1]) && top[i] == targetVal){
						delete[] bottom;
                        			delete[] top;
						return true;
                                        }else if(top[i] == (bottom[j] + bottom[j+1]) && top[i] != targetVal){
                                                bottomStart=j+2;
                                                break;
                                        }else if(top[i] == (bottom[j] + top[i+1]) && top[i+1] == targetVal){
						delete[] bottom;
                                                delete[] top;
                                                return false;
                                        }else if(top[i] == (bottom[j] + top[i+1]) && top[i+1] != targetVal){
                                                bottomStart = j+1;
                                                break;
                                        }else if((top[i] != (bottom[j] + bottom[j+1])) && (top[i] != (bottom[j] + top[i+1]))){
						continue;
					}else{
						this->setError(4543, "isTopNode(…) - An unknown error condition was hit.");
						return false;
					}
                                }
                        }

                        delete[] bottom;
                        delete[] top;
			return true;
		}

		bool buildTree(int *tree, size_t tree_s){
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

			// generate the first layer size.
			size_t layerCount=1;
			int *layerSizes = new int[layerCount];
			layerSizes[0] = this->frequencies_s;
			
			// generate the entire node cache
			size_t nodeCacheSize = tree_s;
			int *nodeCache = new int[nodeCacheSize];
			
			// determine the starting point of the second layer.
			int nodeIndex = nodeCacheSize - 1 - layerSizes[0];
			
			// Fill empty cache values with -1
			for(int i=0; i<nodeCacheSize-this->frequencies_s; i++) nodeCache[i] = -1;
				
			// Fill layer 1 with frequency values.
			for(int i=nodeCacheSize-layerSizes[0], j=0; i<nodeCacheSize && j<this->frequencies_s; i++, j++)
				nodeCache[i]=this->frequencies[j];
			
			// Begin main loop, starting at the index of the next layer.
			for(int i=nodeIndex; i>=0; i--){
				size_t newLayerSize = 0; 
				
				// from the start of the next uncalculated layer, 
				// start looping from the beginning of the current layer
				int value = -1;
				int past = -1;
				int z=0, o=0;
				for(int j=i+layerSizes[layerCount-1]; j>i; j--){
					if(nodeIndex < 0) break;
					if(value == -1){
						if(isTopNode(nodeCache[j], j, layerCount-1, layerSizes, layerCount, nodeCache, nodeCacheSize))
						//if(this->new_isTopNode(j, nodeCache, nodeCacheSize, &z, &o))
							value = nodeCache[j];
						continue;
					}
					if(past == -1){
						if(!isTopNode(nodeCache[j], j, layerCount-1, layerSizes, layerCount, nodeCache, nodeCacheSize)){
						//if(!this->new_isTopNode(j, nodeCache, nodeCacheSize, &z, &o)){
							if(this->failed()){
								delete[] nodeCache;
								return false;
							}
							continue;
						}
						past = value + nodeCache[j];
						nodeCache[nodeIndex] = past;
						nodeIndex--;
						newLayerSize++;
						value = -1;
						continue;
					}
					if(value == nodeCache[j]){
						if(!isTopNode(nodeCache[j], j, layerCount-1, layerSizes, layerCount, nodeCache, nodeCacheSize)){
						//if(!this->new_isTopNode(j, nodeCache, nodeCacheSize, &z, &o)){
							if(this->failed()){
								delete[] nodeCache;
								return false;
							}
							continue;
						}
						past = value + nodeCache[j];
                                                nodeCache[nodeIndex] = past;
                                                nodeIndex--;
                                                newLayerSize++;
                                                value = -1;
                                                continue;
					}
					if(j == i){
                                                past = value + past;
                                                nodeCache[nodeIndex] = past;
                                                nodeIndex--;
                                                newLayerSize++;
                                                value = -1;
						continue;
					}
					if(nodeCache[j] < past){
						if(!isTopNode(nodeCache[j], j, layerCount-1, layerSizes, layerCount, nodeCache, nodeCacheSize)){
						//if(!this->new_isTopNode(j, nodeCache, nodeCacheSize, &z, &o)){
							if(this->failed()){
								delete[] nodeCache;
								return false;
							}
							continue;
						}
						past = value + nodeCache[j];
						nodeCache[nodeIndex] = past;
                                                nodeIndex--;
                                                newLayerSize++;
                                                value = -1;
						continue;
					}
					if(nodeCache[j] > past){
						past = value + past;
                                                nodeCache[nodeIndex] = past;
                                                nodeIndex--;
                                                newLayerSize++;
                                                value = -1;
						j++;
                                                continue;
					}
					if(nodeCache[j] == past && value < past){
                                                past = value + past;
                                                nodeCache[nodeIndex] = past;
                                                nodeIndex--;
                                                newLayerSize++;
                                                value = -1;
                                                j++;
						continue;
					}
				}
				if(past == -1) break;
				if(nodeIndex < 0) break;
				if(value != -1){
					nodeCache[nodeIndex] = value + past;
					nodeIndex--;
					newLayerSize++;
				}

				// Adjust for the next layer.
				int *transfer = new int[layerCount];
                                for(int i=0; i<layerCount; i++)
                                        transfer[i] = layerSizes[i];
                                delete[] layerSizes;
                                layerCount++;
                                layerSizes = new int[layerCount];
                                for(int i=0; i<layerCount-1; i++)
                                        layerSizes[i] = transfer[i];
                                layerSizes[layerCount-1] = newLayerSize;
                                delete[] transfer;
				i=nodeIndex+1;
			}

			for(int i=0; i<tree_s; i++){
				tree[i] = nodeCache[i];
			}
			delete[] nodeCache;
			int max=0;
			for(int i=0; i<this->frequencies_s; i++){
                                max += this->frequencies[i];
                        }
			if(max != tree[0]){
				this->setError(37707, "buildTree(...) - tree[0] isn't equal to the expected maximum frequency sum.");
				return false;
			}
			return true;
		}

		size_t countTreeLayers(int *tree, size_t tree_s){
			if(tree == NULL){
				this->setError(0x123, "countTreeLayers() - tree is null.");
				return 0;
			}
			if(tree_s <= 0){
				this->setError(0x124, "countTreeLayers() - tree_s <= 0. Treating like null.");
				return 0;
			}
			size_t ret =0;
			int tracer = tree[0];
			for(int i=1; i<tree_s; i++){
				if(tree[i] > tracer || i == tree_s-1) ret++;
				tracer = tree[i];
			}
			printf("Tree Layers : %ld\n", ret);
			return ret;
		}

		bool calcLayerSizes(int *layerSizes, size_t layerCount, int *tree, size_t tree_s){
			if(tree == NULL){
				this->setError(445, "calcLayerSizes(...) - tree is null.");
				return false;
			}
			if(tree_s <= 0){
				this->setError(446, "calcLayerSizes(...) - tree_s <= 0. Treating like null.");
				return false;
			}
			if(layerSizes == NULL){
				this->setError(447, "calcLayerSizes(...) - layerSizes is null.");
				return false;
			}
			if(layerCount <= 0){
				this->setError(448, "calcLayerSizes(...) - layerCount <= 0. Treating like null.");
				return false;
			}
			for(int i=layerCount-1; i>=0; i--){
				int size=0;
				int idx=0;
				int tracer = tree[0];
				for(int j=1; j<tree_s; j++){
					if(tree[j] <= tracer && i == idx){
						size++;
					}else if((tree[j] > tracer || j == tree_s-1) && i == idx){
						size++;
						layerSizes[i] = size;
						printf("Size : %d\n", size);
						size = -1;
						break;
					}else if(tree[j] > tracer || j == tree_s-1){
						idx++;
					}
                                	tracer = tree[j];
                        	}
				if(size != -1){
					size++;
					layerSizes[i] = size;
					printf("Size : %d\n", size);
				}
			}
			return true;
		}

		bool isolateLayers(int *tree, size_t tree_s, int **isolatedLayers, int layerCount, int *layerSizes){
			if(tree == NULL){
                                this->setError(445, "isolateLayers(...) - tree is null.");
                                return false;
                        }
                        if(tree_s <= 0){
                                this->setError(446, "isolateLayers(...) - tree_s <= 0. Treating like null.");
                                return false;
                        }
			if(isolatedLayers == NULL){
				this->setError(777, "isolateLayers() - isolatedLayers is null.");
				return false;
			}
			if(layerSizes == NULL){
				this->setError(778, "isloateLayers() - layerSizes is null.");
				return false;
			}
			if(layerCount <= 0){
				this->setError(779, "isolateLayers() - layerCount <= 0. Treating like null.");
				return false;
			}
			int start=0;
			for(int i=0; i<layerCount; i++){
				int tracer = tree[0];
				int idx=0;
				int subIdx=0;
                                for(int j=1; j<tree_s && subIdx<layerSizes[i]; j++){ 
					if(isolatedLayers[i] == NULL){
						this->setError(780, "isolateLayers() - isolatedLayers[i] is null, improperly allocated.");
						return false;
					}
                                        if(tree[j] <= tracer && i == idx){
						if(subIdx == 0){
                                                	isolatedLayers[i][subIdx] = tracer;
							subIdx++;
							if(!(subIdx<layerSizes[i])) break;
						}
                                                isolatedLayers[i][subIdx] = tree[j];
						subIdx++;
                                        }else if(tree[j] > tracer && i == idx){
                                                break;
                                        }else if(tree[j] > tracer || j == tree_s-1){
                                                idx++;
                                        }
                                        tracer = tree[j];
                                }
			}
			return true;
		}
		
		bool buildCodingTable(int *tree, size_t tree_s){
			if(this->treeLetters == NULL){
				this->setError(1234, "buildCodingTable() - treeLetters is null.");
				return false;
			}
                	if(this->treeLetters_s <= 0){
				this->setError(1235, "buildCodingTable() - treeLetters_s <= 0. Treating like null");
				return false;
			}
			if(this->frequencies == NULL){
                                this->setError(1234, "buildCodingTable() - frequencies is null.");
                                return false;
                        }
                        if(this->frequencies_s <= 0){
                                this->setError(1235, "buildCodingTable() - frequencies_s <= 0. Treating like null");
                                return false;
                        }
			if(tree == NULL){
                                this->setError(1234, "buildCodingTable() - tree is null.");
                                return false;
                        }
                        if(tree_s <= 0){
                                this->setError(1235, "buildCodingTable() - tree_s <= 0. Treating like null");
                                return false;
                        }
			if(this->frequencies_s != this->treeLetters_s){
				this->setError(12333, "buildCodingTable() - frequencies_s != treeLetters_s. Invalid coding arrays.");
				return false;
			}
			
			this->destroyCodingTable();
			this->codeTable_s = this->frequencies_s*2;
			this->codeTable = new int[this->codeTable_s];
			for(int i=0; i<this->codeTable_s; i++) this->codeTable[i] = -1;

			size_t treeLayerCount = this->countTreeLayers(tree, tree_s);
			int *treeLayerSizes = new int[treeLayerCount];
			if(!this->calcLayerSizes(treeLayerSizes, treeLayerCount, tree, tree_s)){
				this->setError(1236, "buildCodingTable() - failed to count layer sizes.");
				return false;
			}
			int **isolatedLayers = new int*[treeLayerCount];
			for(int i=0; i<treeLayerCount; i++)
				isolatedLayers[i] = new int[treeLayerSizes[i]];
			
			if(!this->isolateLayers(tree, tree_s, isolatedLayers, treeLayerCount, treeLayerSizes)){
				this->setError(1237, "buildCodingTable() - failed to isolate layers.");
				delete[] treeLayerSizes;
                        	for(int i=0; i<treeLayerCount; i++)
                        	        delete[] isolatedLayers[i];
                        	delete[] isolatedLayers;
				return false;
			}

			printf("[DBG] Isolated Layers : \n");
			for(int i=0; i<treeLayerCount; i++){
				printf("\t%d) ", i);
				for(int j=0; j<treeLayerSizes[i]; j++){
					printf("%d ", isolatedLayers[i][j]);
				}printf("\n");
			}printf("\n");
			
			// build all the table values as you go along.
			// we're basically gonna go through the exact same process for making the tree;
			// but instead of building the tree, we're gonna make each char's code.
			// So we'll likely be processing two values at a time, and as we go up in the layers, 
			// we'll be handling more chars per single iteration. It's relative to the nodes...
			for(int i=treeLayerCount-1; i>=0; i--){
				if(isolatedLayers[i] == NULL){
					delete[] treeLayerSizes;
					for(int i=0; i<treeLayerCount; i++){
						if(isolatedLayers[i] != NULL)
			                                delete[] isolatedLayers[i];
					}
					this->setError(454545, "buildCodingTable() - isolatedLayers[i] is null.\n");
					return false;
				}
				int *layer = isolatedLayers[i];
				size_t targetLayer_s = treeLayerSizes[i];
				int tracer=targetLayer_s-1 < 0 ? -1 : layer[targetLayer_s-1];
				if(tracer == -1){
					this->setError(4444, "buildCodingTable() - targetLayer_s induces an underflow.");
					return false;
				}
				printf("Procesing layer %d, contains %ld bytes\n\t", i, targetLayer_s);
				for(int j=targetLayer_s-2; j>=0; j--){
					if(i==treeLayerCount-1){
						// First layer follows simple logic.
						if(j==0 && (targetLayer_s % 2) == 1){
							this->codeTable[j] = 1; // starting bit count is 1.
							this->codeTable[targetLayer_s] = 0;
							printf("%d ", layer[j]);
							continue;
						}
						this->codeTable[j] = 1; // starting bit count is 1.
						this->codeTable[j+targetLayer_s] = 0; // this is the left branch.

						this->codeTable[j+1] = 1; // starting bit count is 1.
						this->codeTable[j+1+targetLayer_s] = 1<<7; // this is the right branch.
						printf("%d ", layer[j]);
						printf("%d ", layer[j+1]);
						j--;
						if(j==0) j++;

						
						continue;
					}

					// Processing layers that may have top nodes...
					// I have to determine which values are associated with each pairing of nodes.
					// and then push the relavent bits to their locations in the table before going to the 
					// next pair
					
					if(j == targetLayer_s-2) 
						printf("%d ", layer[j+1]);
					printf("%d ", layer[j]);
				}
				printf("\n");
			}

			printf("-----CODE TABLE----\n| BIT COUNT | ENCODED VAL | ORIGINAL VAL |\n");
			for(int i=0; i<this->frequencies_s; i++){
				printf("|    %d    |    0x%x    |       %c    |\n", this->codeTable[i], this->codeTable[this->frequencies_s+i], this->treeLetters[i]);
			}
			printf("---------------\n");
			
			delete[] treeLayerSizes;
			for(int i=0; i<treeLayerCount; i++){
				if(isolatedLayers[i] != NULL)
					delete[] isolatedLayers[i];
			}
			delete[] isolatedLayers;
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
			if(!this->buildTree(tree, treeSize)){
				this->setError(0x500, "encode(char *data, size_t dataSize) - failed to build tree.");
				return false;
			}

			printf("[DBG] Generated Tree : ");
			for(int i=0; i<treeSize; i++)
				printf("[%d]%d ", i, tree[i]);
			printf("\n");

			if(!this->buildCodingTable(tree, treeSize)){
				this->setError(0x501, "encode(char *data, size_t dataSize) - failed to build coding table.");
				return false;
			}

			/// development zone
			int dbg_layerindex = 0;
			int dbg_targetIndex = 3;
			int zeroIndex=-1, oneIndex=-1;
			for(int i=0; i<treeSize; i++){
			dbg_targetIndex = i;
			if(new_isTopNode(dbg_targetIndex, tree, treeSize, &zeroIndex, &oneIndex)){
				printf("[%d]%d is a top node. zero Index [%d]%d | one Index [%d]%d\n", dbg_targetIndex, tree[dbg_targetIndex], zeroIndex, tree[zeroIndex], oneIndex, tree[oneIndex]);
			}else{
				printf("[%d]%d is a bottom node. zero Index [%d]%d | one Index [%d]%d\n", dbg_targetIndex, tree[dbg_targetIndex], zeroIndex, tree[zeroIndex], oneIndex, tree[oneIndex]);
			}
			}
			/// end development zone
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
			this->codeTable = NULL;
			this->codeTable_s = 0;
			this->treeLetters = NULL;
			this->treeLetters_s = 0;
			this->frequencies = NULL;
			this->frequencies_s = 0;
			this->tablesSorted = false;
			this->clearError();
		}
		~HuffmanCoding(){
			this->destroyCodingTable();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
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
