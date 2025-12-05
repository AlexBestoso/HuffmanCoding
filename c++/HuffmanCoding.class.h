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
		
		int countLikeFrequencies(int target){
			int ret = 0;
			for(int i=0; i<this->frequencies_s; i++){
				if(this->frequencies[i] == target)
					ret++;
			}
			return ret;
		}

		int subnodeValidate(int nextFreq, int layerIndex, int currentNodeIndex, int nextNodeIndex, int *nodeCache, size_t nodeCache_s, int *layers, size_t layers_s){
			if(layerIndex <= 0 || nextFreq == -1) // haven't made any subnodes yet.
				return nextFreq;
				
			return nextFreq;
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
			size_t layerCount=1;
			int *layerSizes = new int[layerCount];
			layerSizes[0] = this->frequencies_s;
			size_t nodeCacheSize = (this->frequencies_s - 1) + this->frequencies_s;
			int *nodeCache = new int[nodeCacheSize];
			int nodeIndex = nodeCacheSize - layerSizes[0] - 1;
			for(int i=0; i<nodeCacheSize-this->frequencies_s; i++) nodeCache[i] = -1;
			for(int i=nodeCacheSize-this->frequencies_s, j=0; i<nodeCacheSize;i++, j++)
				nodeCache[i]=this->frequencies[j];

			bool processing = true;
			int end = nodeIndex;
			int start = nodeCacheSize-1;
			int currentLayer = 0;
			while(processing){
				if(nodeIndex < 0) break;
				int newSize = 0;
				for(int i=start; i>end; i--){
					int current = nodeCache[i];
					int prevSum = nodeIndex+1 <= end && (nodeIndex+1 < nodeCacheSize) ? nodeCache[nodeIndex+1] : -1;
					int next = i-1 > end ? nodeCache[i-1] : -1;
					next = this->subnodeValidate(next, currentLayer, i, i-1, nodeCache, nodeCacheSize, layerSizes, layerCount);
					if(next == -1){
                                                if(prevSum == -1){
                                                        this->setError(0x37707, "buildTree() - unexpected condition.");
                                                        return false;
                                                }
                                                nodeCache[nodeIndex] = prevSum + current;
						printf("Node val (%d) : %d | Current : %d | next : %d | prevSum : %d\n", i, nodeCache[nodeIndex], current, next, prevSum);
						newSize++;
                                                nodeIndex--;
                                        }else if(prevSum == -1 || next == current || prevSum > next){
                                                nodeCache[nodeIndex] = current + next;
						printf("Node val (%d) : %d | Current : %d | next : %d | prevSum : %d\n", i, nodeCache[nodeIndex], current, next, prevSum);
						newSize++;
                                                i--;
                                                nodeIndex--;
                                        }else if(prevSum <= next){
                                                nodeCache[nodeIndex] = current + prevSum;
						printf("Node val (%d) : %d | Current : %d | next : %d | prevSum : %d\n", i, nodeCache[nodeIndex], current, next, prevSum);
						newSize++;
                                                nodeIndex--;
                                        }else{
                                                this->setError(0x7777, "buildTree() - spooky impossible error.");
                                                return false;
                                        }
				}
				
				// Push new layer size
                        	int *transfer = new int[layerCount];
				for(int i=0; i<layerCount; i++)
					transfer[i] = layerSizes[i];
				delete[] layerSizes;
				layerCount++;
				layerSizes = new int[layerCount];
				for(int i=0; i<layerCount-1; i++)
					layerSizes[i] = transfer[i];
				layerSizes[layerCount-1] = newSize;
				delete[] transfer;
				currentLayer++;

				// calculate new start and end size
				end = nodeIndex;
                        	start = end + newSize;
				newSize = 0;
				if(currentLayer > 1) break;

			}

			printf("[DEBUG] Node Cache : ");
			for(int i=0; i<nodeCacheSize; i++){
				printf("%d ", nodeCache[i]);
			}printf("\n");
			delete[] nodeCache;
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

/*
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
			}*/
			
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
