#define HUFFMAN_DEBUGGING 1
class HuffmanCoding{
	private:
		char *treeLetters;
		size_t treeLetters_s;
	
		int *frequencies;
		size_t frequencies_s;
		
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

		void sortFreqencies(void){
			if(this->treeLetters_s != this->frequencies_s)
				return;
			if(this->treeLetters == NULL || this->frequencies == NULL)
				return;

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
		}

		size_t countLayers(void){
			if(!this->tablesSorted){
				
				return 0;
			}
			size_t ret = 0;
			for(int i=0, check=0; i<this->frequencies_s; i++){
				if(i == 0){
					check = this->frequencies[i];
					ret++;
					continue;
				}
				if(check != this->frequencies[i])
					ret++;
			}
		
			return ret;
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

		bool buildTree(int *tree, size_t tree_s, int *literals, size_t literalsSize){
			if(this->frequencies == NULL){
				return false;
			}
			if(this->frequencies_s <= 0){
				return false;
			}
			if(this->treeLetters == NULL){
				return false;
			}
			if(this->treeLetters_s <= 0){
				return false;
			}
			if(tree == NULL){
				return false;
			}
			if(tree_s <= 0){
				return false;
			}
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

                                                return false;
                                        }
                                        if(nodeIndexA >= tree_s){

                                                return false;
                                        }
                                        if(nodeIndexB < 0){

                                                return false;
                                        }
                                        if(nodeIndexB >= tree_s){

                                                return false;
                                        }
                                        tree[i] = tree[nodeIndexA] + tree[nodeIndexB];
                                        #if HUFFMAN_DEBUGGING == 1
                                                printf("[DBG] Node Index A : %d | Node Index B : %d\n", nodeIndexA, nodeIndexB);
                                                printf("[DBG] C tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
                                        #endif
					i--;
					tree[i] = (int)this->treeLetters[tableIndex];
					#if HUFFMAN_DEBUGGING == 1
							printf("[DBG] D tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
					#endif
					literals[tableIndex] = i;
					int nodeIndex = this->findNodeIndex(i, tree_s, literals, literalsSize);
					if(nodeIndex < 0){

						return false;
					}
					if(nodeIndex >= tree_s){

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

						return false;
					}
					if(nodeIndexA >= tree_s){

						return false;
					}
					if(nodeIndexB < 0){

						return false;
					}
					if(nodeIndexB >= tree_s){

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
					if(i>= 0){
						tree[i] = this->frequencies[tableIndex] + prevFreq;
						#if HUFFMAN_DEBUGGING == 1
							printf("[DBG] B tree cell %d : #%d : '%c'\n", i, tree[i], tree[i]);
						#endif
						tableIndex--;
						literalCtr++;
					}
				}
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
			this->buildTree(tree, treeSize, literalIndexList, literalIndexListSize);

			#if HUFFMAN_DEBUGGING == 1
			int dbg_nodeMax=0;
			for(int i=0; i<this->frequencies_s; i++)
				dbg_nodeMax += this->frequencies[i];
			printf("[DBG] Expected Max Node Value : %d\n", dbg_nodeMax);
			printf("[DBG] Tree : ");
			for(int i=0; i<treeSize; i++){
				printf("%d ", tree[i]);
			}printf("\n");
			#endif
			
			// calculate layers
		/*	size_t layerCount = this->countLayers();
			if(layerCount <= 0){
		
				return false;
			}
			int *layerSizes = new int[layerCount];
			if(!this->calculateLayerSizes(layerSizes, layerCount)){

				return false;
			}

			delete[] layerSizes;*/
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

			this->sortFreqencies();
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
