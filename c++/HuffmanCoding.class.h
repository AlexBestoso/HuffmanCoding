#define HUFFMAN_DEBUGGING 1
class HuffmanCoding{
	private:
		char *treeLetters;
		size_t treeLetters_s;
	
		int *frequencies;
		size_t frequencies_s;
		int frequencyMax;

		int *codeTable;
		size_t codeTable_s;

		int *treeData;
		int *treeDataTypes;
		size_t treeData_s;
		
		bool tablesSorted;

		int error;
		std::string error_msg;
		
		void destroyTreeData(void){
			if(this->treeData != NULL)
				delete[] this->treeData;
			if(this->treeDataTypes != NULL)
				delete[] this->treeDataTypes;
			this->treeData = NULL;
			this->treeDataTypes = NULL;
			this->treeData_s = 0;
		}
		void resizeTreeData(size_t size){
                        if(size == 0){
                                this->destroyTreeData();
                        }else if(this->treeData == NULL){
                                this->treeData = new int[size];
				this->treeDataTypes = new int[size];
                                this->treeData_s = size;
                        }else{
                                size_t oldSize = this->treeData_s;
                                int *transfer = new int[oldSize];
				int *transferTwo = new int[oldSize];
                                for(int i=0; i<oldSize; i++){
                                        transfer[i] = this->treeData[i];
					transferTwo[i] = this->treeDataTypes[i];
				}
                                this->destroyTreeData();
                                this->treeData = new int[size];
				this->treeDataTypes = new int[size];
                                this->treeData_s = size;
                                for(int i=0; i<this->treeData_s && i<oldSize; i++){
                                        this->treeData[i] = transfer[i];
					this->treeDataTypes[i] = transferTwo[i];
                                }
				
                                delete[] transfer;
				delete[] transferTwo;
                        }
                }
		bool validateTreeData(void){
			if(this->treeData_s <= 0){
                                this->setError(600, "validateTreeData(void) - treeData_s <= 0. Treating as null.");
                                return false;
                        }
                        if(this->treeData == NULL){
                                this->setError(601, "validateTreeData(void) - treeData is null.");
                                return false;
                        }
			return true;
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
		bool validateFrequencies(void){
			if(this->frequencies_s <= 0){
                                this->setError(3402, "validateFrequencies(void) - frequencies_s <= 0. Treating as null.");
                                return false;
                        }
                        if(this->frequencies == NULL){
                                this->setError(3403, "validateFrequencies(void) - frequencies is null.");
                                return false;
                        }
			return true;
		}

		void destroyOut(void){
			if(this->out != NULL)
				delete[] this->out;
			this->out = NULL;
			this->out_s = 0;
		}
		
		bool createTreeLetters(char *data, size_t dataSize){
			if(data == NULL){
				this->setError(200, "createTreeLetters(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(201, "createTreeLetters(char *data, size_t dataSize) - dataSize is <= 0, treating data as null");
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
				this->setError(300, "createFrequency(char *data, size_t dataSize) - treeLetters is null.");
				return false;
			}
			if(this->treeLetters_s <= 0){
				this->setError(301, "createFrequency(char *data, size_t dataSize) - treeLetters_s is <= 0, treating treeLetters as null.");
				return false;
			}
			if(data == NULL){
				this->setError(302, "createFrequency(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(303, "createFrequency(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
				return false;
			}
			this->destroyFrequencies();
			this->frequencyMax=0;
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
			for(int i=0; i<this->frequencies_s; i++)
				this->frequencyMax += this->frequencies[i];
			return true;
		}

		bool sortFreqencies(void){
			if(!this->validateFrequencies()){
				this->setError(400, "sortFreqencies(void) - failed to validate frequencies.");
				return false;
			}
			if(this->treeLetters_s <= 0){
				this->setError(401, "sortFreqencies(void) - treeLetters_s is 0, treating as null.");
				return false;
			}
			if(this->treeLetters_s != this->frequencies_s){
				this->setError(402, "sortFreqencies(void) - Table Corruption, treeLetters_s != frequencies_s.");
				return false;
			}
			if(this->treeLetters == NULL){
				this->setError(403, "sortFreqencies(void) - treeLetters is null.");
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

		// TODO: Fix for when target == 0.
		bool findRelativeLayers(int target, int *nodeCache, size_t nodeCache_s, size_t *out_topSize, size_t *out_bottomSize, int *out_segmentStart){
			if(nodeCache == NULL){
				this->setError(520, "findRelativeLayers(int target, int *nodeCache, size_t nodeCache_s, int *out_topSize, int *out_bottomSize, int *out_segmentStart) - nodeCache is null.");
				return false;
			}
			if(nodeCache_s <= 0){
				this->setError(521, "findRelativeLayers(int target, int *nodeCache, size_t nodeCache_s, int *out_topSize, int *out_bottomSize, int *out_segmentStart) - nodeCache_s <= 0. treating as null.");
				return false;
			}
			if(!this->validateFrequencies()){
				this->setError(522, "findRelativeLayers(int target, int *nodeCache, size_t nodeCache_s, int *out_topSize, int *out_bottomSize, int *out_segmentStart) - failed to validate frequencie.");
				return false;
			}
			if(target >= nodeCache_s){
				this->setError(523, "findRelativeLayers(int target, int *nodeCache, size_t nodeCache_s, int *out_topSize, int *out_bottomSize, int *out_segmentStart) - target >= nodeCache_s, treating as out of bounds.");
				return false;
			}

			out_topSize[0] = 0;
			out_bottomSize[0] = 0;
			out_segmentStart[0] = -1;
			int layerStart = 0;
			int layerSize=0;
			bool found=false;
			for(int i=0, tracer=-1; i<nodeCache_s; i++){
				if(i == target)
					found = true;
				if(tracer == -1){
					tracer = nodeCache[i];
					layerSize=1;
					layerStart = i;
					continue;
				}


				if(tracer >= nodeCache[i]){
					layerSize++;
					tracer = nodeCache[i];
					continue;
				}
				
				if(found) break;
				layerStart = i;
				layerSize=1;
				tracer = nodeCache[i];
			}

			out_segmentStart[0] = layerStart;
			out_topSize[0] = layerSize;

			layerStart = layerStart+layerSize;
			found=false;
			for(int i=layerStart, tracer=-1; i<nodeCache_s; i++){
				if(tracer == -1){
                                        tracer = nodeCache[i];
                                        layerSize=1;
                                        layerStart = i;
                                        continue;
                                }

                                if(tracer >= nodeCache[i]){
                                        layerSize++;
                                        tracer = nodeCache[i];
                                        continue;
                                }

				break;
			}
			out_bottomSize[0] = layerSize;

			return true;
		}

		bool processBaseNodeValues(int cacheTarget, int cacheLayerStart, size_t cacheLayerSize, size_t cacheSize, int *out_zero, int *out_one){
			if(!this->validateFrequencies()){
				this->setError(1999, "processBaseNodeValues(int targetIndex, int layerStart, size_t layerSize, int *out_zero, int *out_one) - failed to validate frequencies.");
				return false;
			}
			if(cacheTarget < cacheSize-this->frequencies_s){
				printf("DEBUG : %d < %d\n", cacheTarget, cacheLayerStart);
				this->setError(2000, "processBaseNodeValues(int targetIndex, int layerStart, size_t layerSize, int *out_zero, int *out_one) - targetIndex not in base layer.");
				return false;
			} 
			if(cacheLayerSize != this->frequencies_s){
				this->setError(2001, "processBaseNodeValues(int cacheTarget, int cacheLayerStart, size_t layerSize, size_t cacheSize, int *out_zero, int *out_one) - layerSize misaligned with frequencies_s.");
				return false;
			}			

			int cacheToRootConvert = cacheSize - (cacheSize-cacheLayerSize);
			int convertedTarget = cacheTarget - cacheToRootConvert;

			bool oddLayer = (cacheLayerSize%2) == 1;
			bool oddTarget = (convertedTarget%2) == 1;

			if(cacheTarget == cacheLayerStart && oddLayer){
				out_zero[0] = cacheTarget;
				out_one[0] = -1;
			}else if(oddLayer){
				out_zero[0] = oddTarget ? cacheTarget : -1;
				out_one[0] = oddTarget ? -1 : cacheTarget;
			}else{
				out_zero[0] = oddTarget ? -1 : cacheTarget;
				out_one[0] = oddTarget ? cacheTarget : -1;
			}
			return true;
		}
		// TODO: Go through the entire layer to determine if the target is a top or bottom node.
		// Using the starting index and size of the layer, which target should be part of, 
		// find the start of the previous layer, and determine if the target value is a bottom or top node.
		bool nodeValueIsValid(int target, int start, int size, int *nodeCache, size_t nodeCache_s){
			// ensure target is within start and size bounds.
			if(!(target <= start) && !(target > start-size)){
				this->setError(4456, "nodeValueIsValid() - target is out of row bounds.");
				return false;
			}
			// ensure node cache is valid.
			if(nodeCache == NULL || nodeCache_s <= 0){
				this->setError(45444, "nodeValueIsValid() - nodeCache is invalid.");
				return false;
			}
			// check if there's a layer beneath. No layer, return true, all top nodes.
			if(target >= (nodeCache_s - this->frequencies_s)){
				return true;
			}
			// calculate lower layer start and size.
			if(start < 0 || start >= nodeCache_s){
				this->setError(46778, "nodeValueIsValid() - start out of bounds.");
				return false;
			}
			int lowerLayerSize=0;
			for(int i=start, tracer = nodeCache[start]; i<nodeCache_s; i++){
				if(nodeCache[i] > tracer){
					break;
				}
				lowerLayerSize++;
				tracer = nodeCache[i];
			}	
			int lowerLayerStart=start+lowerLayerSize;
			// generate sums until target
			bool found=false;
			for(int i=lowerLayerStart, j=start, tracer=-1,sum=-1; (i>=0 && i>start) && (j >= 0 && j>(start-size)); i--){
				if(tracer==-1){
					tracer = nodeCache[i];
					continue;
				}
				if(sum == -1){
					if(found){
						return true;
					}
					sum = tracer + nodeCache[i];
					if(j == sum) found = true;
					j--;
					tracer = -1;
					continue;
				}
			}
			// once target found, if end of buffer, return true, its a top node.
			// once target found, store variables, calcuate next sum.
			// if the next sum is created using the prior sum, return false, it's a bottom node.
			// else return true, it's a top node.	
			return found;
		}
		bool isTopNode(int targetIndex, int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex){
			if(nodeCache == NULL){
				this->setError(500, "isTopNode(int targetIndex, int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex) - nodeCache is null.");
				return false;
			}
			if(nodeCache_s <= 0){
				this->setError(501, "isTopNode(int targetIndex, int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex) - nodeCache_s <= 0. Treating as null.");
				return false;
			}
			if(targetIndex < 0 || targetIndex >= nodeCache_s){
				this->setError(502, "isTopNode(int targetIndex, int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex) - target index out of bounds.");
				return false;
			}

			zeroIndex[0] = -1;
			oneIndex[0] = -1;
			
			// Identify relative layer offsets and sizes.
			int layerStart = 0;
                        size_t topSize = 0;
                        size_t bottomSize = -1;
			// Returns bottomSize -1 if the target index is in the base layer/the leaf layer.
			if(!this->findRelativeLayers(targetIndex, nodeCache, nodeCache_s, &topSize, &bottomSize, &layerStart)){
				this->setError(502, "isTopNode(int targetIndex, int *nodeCache, size_t nodeCache_s, int *zeroIndex, int *oneIndex) - failed to find relative layers.");	
				return false;
			}
			if(bottomSize == -1){
				return this->processBaseNodeValues(targetIndex, layerStart+topSize-1, topSize, nodeCache_s, zeroIndex, oneIndex);
			}
		
			
			// This gets processed backwords (root to leaf)
			printf("targetIndex : %d\n", targetIndex);
			printf("\trelative values: top, bottom, start : %ld, %ld, %d\n", topSize, bottomSize, layerStart);
			
			int bottomStart = layerStart+topSize+bottomSize-1;
			int bottomEnd = layerStart+topSize;
			int topStart = layerStart+topSize-1;
			bool checking=false;
			for(int i=bottomStart, j=topStart, tracer=-1, tracerIdx=0, sum=-1; i>=bottomEnd && j>=layerStart; i--){
				if(!this->nodeValueIsValid(i, bottomStart, bottomSize, nodeCache, nodeCache_s)){
					printf("\tSkipping I[%d]! It's a bottom node\n", i);
					continue;
				}else{
					printf("\tI[%d] is top node.\n", i);
				}
				if(tracer == -1){
					tracer = nodeCache[i];
					tracerIdx=i;
					continue;
				}
				if(sum == -1){
					if(j == targetIndex){ // check value too.
						zeroIndex[0] = i;
						oneIndex[0] = tracerIdx;
						checking = true;
					}
					sum = tracer + nodeCache[i];
					tracer = -1;
					j--;
					continue;
				}
				if(checking){
					printf("\tzero : [%d]%d | one [%d]%d | expected sum [%d]%d calculated sum %d\n", zeroIndex[0], nodeCache[zeroIndex[0]], oneIndex[0], nodeCache[oneIndex[0]], targetIndex, nodeCache[targetIndex], nodeCache[zeroIndex[0]]+nodeCache[oneIndex[0]]);
					if(nodeCache[targetIndex] != nodeCache[zeroIndex[0]]+nodeCache[oneIndex[0]]){
						this->setError(33432, "isTopNode() - [A] expected value is not the same as calculated value.");
						return false;
					}
					if(sum < nodeCache[i]){
						return false; 
					}else{
						return true;
					}
				}else{
					if(sum == nodeCache[i]){
						sum = nodeCache[i] + tracer;
						if(j == targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = tracerIdx;
							checking = true;
						}
						j--;
						tracer = -1;
						continue;
					}
					if(sum < nodeCache[i]){
						sum = tracer + sum;
						if(j == targetIndex){
							zeroIndex[0] = tracerIdx;
							oneIndex[0] = j+1;
							checking = true;
						}
						j--;
						tracer = nodeCache[i];
						tracerIdx = i;
						continue;
					}
				
					if(sum > nodeCache[i]){
						sum = tracer + nodeCache[i];
						if(j == targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = tracerIdx;
							checking = true;
						}
						j--;
						tracer = -1;
						continue;
					}
				}
			}

			if(zeroIndex[0] == -1 && oneIndex[0] == -1){
				this->setError(33335, "isTopNode() - failed to identify node source values.");
				return false;
			}
			printf("\tzero : [%d]%d | one [%d]%d | expected sum [%d]%d calculated sum %d\n", zeroIndex[0], nodeCache[zeroIndex[0]], oneIndex[0], nodeCache[oneIndex[0]], targetIndex, nodeCache[targetIndex], nodeCache[zeroIndex[0]]+nodeCache[oneIndex[0]]);
			if(nodeCache[targetIndex] != nodeCache[zeroIndex[0]]+nodeCache[oneIndex[0]]){
				this->setError(33455, "isTopNode() - [B] expected value is not the same as calculated value.");
				return false;
			}

			return true;
		}

		bool seedLayers(void){
			if(!this->validateFrequencies()){
				this->setError(4301, "seedLayers(void) - failed to validate frequenncies.");
				return false;
			}
			if(this->treeData != NULL){
				this->setError(13224, "tree data already seeded.");
				return false;
			}
			this->resizeTreeData(this->frequencies_s);
			for(int i=0; i<this->treeData_s; i++){
				this->treeData[i] = this->frequencies[i];
				this->treeDataTypes[i] = 1; // base layer all top nodes.
			}

			size_t workBuffer_s = this->treeData_s;
			int *workBuffer = new int[workBuffer_s];
			int *workTypeBuffer = new int[workBuffer_s];
			size_t workBuffer_fill=0;

			for(int i=treeData_s-1, tracer=-1, sum=-1; workBuffer_fill < workBuffer_s && i>=0; i--){
				// Handle the final odd case without regard to tracer.
				if(i == 0 && (this->frequencies_s%2) == 1){
					if(sum == -1){
						this->setError(343434, "seedLayers(void) - sum is invalid for final value.");
						return false;
					}
					workBuffer[workBuffer_fill] = sum+treeData[i];
					workTypeBuffer[workBuffer_fill] = 1;
					workTypeBuffer[workBuffer_fill-1] = 0;
					workBuffer_fill++;
					break;
				}

				// get focual point from past :o
				if(tracer == -1){
					tracer = treeData[i];
					continue;
				}

				// Create an inital sum
				if(sum == -1){
					sum = tracer + treeData[i];
					workBuffer[workBuffer_fill] = sum;
					workTypeBuffer[workBuffer_fill] = 0;// assume current is a bottom, there was no previous.
					workBuffer_fill++;
					tracer = -1;
					continue;
				}

				if(tracer == this->treeData[i] || sum >= this->treeData[i]){
					sum = this->treeData[i] + tracer;
					workBuffer[workBuffer_fill] = sum;
					workTypeBuffer[workBuffer_fill] = 1; // assum current is bottom buffer
					workTypeBuffer[workBuffer_fill-1] = 1; // we know that the previous was a top buffer
					tracer = -1;
					workBuffer_fill++;
					continue;
					
				}

				// if sum is less than future, use the sum, and move future to tracer,
				if(sum < this->treeData[i]){
					sum = tracer+sum;
					workBuffer[workBuffer_fill] = sum;
					workTypeBuffer[workBuffer_fill] = 1; // assum current is bottom buffer
					workTypeBuffer[workBuffer_fill-1] = 0; // we know that the previous was a bottom buffer
					workBuffer_fill++;
					tracer = this->treeData[i];
					continue;
				}
			}
			workTypeBuffer[workBuffer_fill-1] = 1; // last layer value always a top node.

			// push original data to end of array
			size_t originalSize = this->treeData_s;
			this->resizeTreeData(originalSize+workBuffer_fill);
			for(int i=this->treeData_s-1, track=0;  track<originalSize && i>=0; i--, track++){
				if((i-workBuffer_fill) < 0){break;}
				this->treeData[i] = this->treeData[i-workBuffer_fill];
				this->treeDataTypes[i] = this->treeDataTypes[i-workBuffer_fill];
			}

			// populate front of array with new data
			for(int i=0; i<workBuffer_fill && (workBuffer_fill-1-i) >= 0; i++){
				this->treeData[i] = workBuffer[workBuffer_fill-1-i];
				this->treeDataTypes[i] = workTypeBuffer[workBuffer_fill-1-i];
			}
			
			delete[] workBuffer;
			delete[] workTypeBuffer;
			return true;

		}

		[[deprecated("getTopLayerStart() is depricated.")]]
		int getTopLayerStart(void){
			if(!this->validateTreeData()){
				this->setError(1212, "getTopLayerStart() - invalid treeData.");
				return -1;
			}
			for(int i=0, track=-1; i<this->treeData_s; i++){
				if(track == -1)
					track = this->treeData[i];
				if(treeData[i] > track)
					return i-1;
				track = this->treeData[i];
			}
			this->setError(1313, "getTopLayerStart(void) - no top layer found.");
			return -1;
		}

		bool growLayer(void){
			if(!this->validateFrequencies()){
				this->setError(601, "growLayer(void) - invalid frequencies.");
				return false;
			}
			if(this->treeData == NULL){
				if(!this->seedLayers()){
					this->setError(602, "growLoayer(void) - failed to seed layers.");
					return false;
				}
				return true;
			}
			
			size_t workBuffer_fill=0;
			size_t workBuffer_s = this->frequencies_s;
			int *workBuffer = new int[workBuffer_s];
			int *workTypeBuffer = new int[workBuffer_s];
			
			size_t layerCount = this->countLayers();
			int *layerIndecies = new int[layerCount];
			this->getLayerIndecies(layerIndecies, layerCount);
			printf("DBG grow Layer, count : %ld\n", layerCount);
			int topLayerStart = layerIndecies[0]-1;//this->getTopLayerStart();
			size_t topLayerSize = topLayerStart+1;
			

			// TODO: Investigate if this modular equation is throwing things off.
			printf("\tTop layer Start : %d\n", topLayerStart);
			printf("\tTop layer Size : %ld\n", topLayerSize);
			bool oddMode = (this->treeData_s % 2) == 1 ? true : false; 
			for(int i=topLayerStart, tracer=-1, sum=-1; i>=0; i--){
				if(oddMode){
					if(i==0){
						sum = this->treeData[i] + sum;
                                	        printf("%d - I ODD\n", sum);
                                	        workBuffer[workBuffer_fill] = sum;
                                	        workTypeBuffer[workBuffer_fill] = 1;
                                	        workTypeBuffer[workBuffer_fill-1] = 0;
                                	        workBuffer_fill++;
                                	        tracer = this->treeData[i];
						oddMode = ~oddMode;
                                	        continue;

					}
					if(tracer == -1){
						tracer = this->treeData[i];
						continue;
					}
					if(sum == -1){
                                	        sum = tracer + this->treeData[i];
                                	        printf("%d - S\n", sum);
                                	        workBuffer[workBuffer_fill] = sum;
                                	        workTypeBuffer[workBuffer_fill] = 0;
                                	        workBuffer_fill++;
                               		        tracer = -1;
                                	        continue;
                                	}

                                	if(tracer == this->treeData[i] || sum >= treeData[i]){
                                	        sum = this->treeData[i] + tracer;
                                	        printf("%d - A\n", sum);
                                	        workBuffer[workBuffer_fill] = sum;
                                	        workTypeBuffer[workBuffer_fill] = 0;
                                	        workTypeBuffer[workBuffer_fill-1] = 1;
                                	        workBuffer_fill++;
                                	        tracer = -1;
                                	        continue;
                                	}
                                	if(sum < this->treeData[i]){
                                	        sum = tracer + sum;
                                	        printf("%d - B\n", sum);
                                	        workBuffer[workBuffer_fill] = sum;
                                	        workTypeBuffer[workBuffer_fill] = 0;
                                	        workTypeBuffer[workBuffer_fill-1] = 0;
                                	        workBuffer_fill++;
                                	        tracer = treeData[i];
						oddMode = ~oddMode;
                                	        continue;
                                	}
					continue;
				}

				// Even mode
				if(tracer == -1){
					tracer = this->treeData[i];
					continue;
				}
				if(i==0){
					sum = this->treeData[i] + tracer;
                                        printf("%d - I EVEN\n", sum);
                                        workBuffer[workBuffer_fill] = sum;
                                        workTypeBuffer[workBuffer_fill] = 1;
                                        workTypeBuffer[workBuffer_fill-1] = 1;
                                        workBuffer_fill++;
                                        tracer = -1;
					continue;
				}
				if(sum == -1){
                                        sum = tracer + this->treeData[i];
                                        printf("%d - S\n", sum);
                                        workBuffer[workBuffer_fill] = sum;
                                        workTypeBuffer[workBuffer_fill] = 0;
                                        workBuffer_fill++;
                                        tracer = -1;
                                        continue;
                                }

                                if(tracer == this->treeData[i] || sum >= treeData[i]){
                                        sum = this->treeData[i] + tracer;
                                        printf("%d - A\n", sum);
                                        workBuffer[workBuffer_fill] = sum;
                                        workTypeBuffer[workBuffer_fill] = 0;
                                        workTypeBuffer[workBuffer_fill-1] = 1;
                                        workBuffer_fill++;
                                        tracer = -1;
                                        continue;
                                }
                                if(sum < this->treeData[i]){
                                        sum = tracer + sum;
                                        printf("%d - B\n", sum);
                                        workBuffer[workBuffer_fill] = sum;
                                        workTypeBuffer[workBuffer_fill] = 0;
                                        workTypeBuffer[workBuffer_fill-1] = 0;
                                        workBuffer_fill++;
                                        tracer = treeData[i];
					oddMode = ~oddMode;
                                        continue;
                                }
			}
			workTypeBuffer[workBuffer_fill-1] = 1;

			printf("Work buffer : ");
			for(int i=0; i<workBuffer_fill; i++)
				printf("%d:%s ", workBuffer[i], workTypeBuffer[i] == 0 ? "B" : "T");
			printf("\n");
						
			// push original data to end of array
			size_t originalSize = this->treeData_s;
			this->resizeTreeData(originalSize+workBuffer_fill);
			for(int i=this->treeData_s-1, track=0;  track<originalSize && i>=0; i--, track++){
				if((i-workBuffer_fill) < 0){break;}
				this->treeData[i] = this->treeData[i-workBuffer_fill];
				this->treeDataTypes[i] = this->treeDataTypes[i-workBuffer_fill];
			}

			// populate front of array with new data
			for(int i=0; i<workBuffer_fill && (workBuffer_fill-1-i) >= 0; i++){
				this->treeData[i] = workBuffer[workBuffer_fill-1-i];
				this->treeDataTypes[i] = workTypeBuffer[workBuffer_fill-1-i];
			}


			delete[] workBuffer;
			delete[] workTypeBuffer;
			return true;
		}
	
		bool plantTree(void){
			if(!this->validateFrequencies()){
				this->setError(700, "plantTree(void) - invalid frequencies.");
				return false;
			}
			this->destroyTreeData();
			this->destroyCodingTable();
			this->codeTable_s = this->frequencies_s*2;
			this->codeTable = new int[this->codeTable_s];
			for(int i=0; i<this->codeTable_s; i++){
				this->codeTable[i] = 0;
			}
			
			// seed the tree, and begin coding table.
			int startSize = this->treeData_s;
			int dbg=0;
			while(1){
				if(this->treeData != NULL){
					if(this->treeData[0] >= this->frequencyMax) break;
				}
				printf("debug Size : %ld\n", this->treeData_s);
				if(!this->growLayer()){
					this->setError(702, "plantTree(void) - Failed to grow tree layer.");
					return false;
				}
				this->printTree();
			}
			if(this->treeData[0] != this->frequencyMax){
				this->setError(3333, "plantTree(void) - Failed to grow tree, tree is corrupt.");
				return false;
			}

			int zero=-1, one=-1;
			int baseLayerEnd = this->treeData_s - this->frequencies_s;
			int continueationIndex = baseLayerEnd-1;

			// Complete code table generation.
			for(int i=continueationIndex; i>=0; i--){
				bool nodeType = this->isTopNode(i, this->treeData, this->treeData_s, &zero, &one);
				if(this->failed()){
					this->setError(703, "plantTree(void) - Failed to identify top node.");
					return false;
				}
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
					if(this->failed()){
						this->setError(704, "plantTree(void) - Failed to identify top node.");
						return false;
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
					if(this->failed()){
						this->setError(705, "plantTree(void) - failed to identify top node.");
						return false;
					}
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
			
			return true;
		}

		std::string getCodeBinary(int idx){
			if(this->codeTable == NULL){
				this->setError(800, "getCodeBinary(int idx) - codeTable is null.");
				return "";
			}
			if(idx < 0 || idx >= this->codeTable_s){
				this->setError(801, "getCodeBinary(int idx) - idx is out of bounds.");
				return "";
			}
			if(this->frequencies_s+idx < 0 || this->frequencies_s+idx >= this->codeTable_s){
				this->setError(802, "getCodeBinary(int idx) - frequencies_s+idx is out of bounds.");
				return "";
			}
			std::string ret = "";
			int codeSize = this->codeTable[idx];
			int code = this->codeTable[this->frequencies_s+idx];
			for(int i=0; i<codeSize; i++){
				int bit = (code >> (7-i)) & 1;
				ret += std::to_string(bit);
			}
			return ret;
		}

		int codeToTableIndex(std::string code){
			for(int i=0; i<this->frequencies_s; i++){
				std::string comp = this->getCodeBinary(i);
				if(this->failed()){
					this->setError(900, "codeToTableIndex(std::string code) - failed to get binary code.");
					return -1;
				}
				if(comp == code){
					return i;
				}
			}
			return -1;
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
			if(this->frequencies == NULL){
				this->setError(1000, "packHeader(void) - frequencies is null.");
				return false;
			}
			if(this->frequencies_s <= 0){
				this->setError(1001, "packHeader(void) - frequencies_s <= 0. Treating as null.");
				return false;
			}
			if(this->treeLetters == NULL){
                                this->setError(1002, "packHeader(void) - treeLetters is null.");
                                return false;
                        }
                        if(this->treeLetters_s <= 0){
                                this->setError(1003, "packHeader(void) - treeLetters_s <= 0. Treating as null.");
                                return false;
                        }
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
				if(!(i<this->out_s)){
					this->setError(1004, "packHeader(void) - i out of bounds.");
					return false;
				}
				this->out[i] = freq_b; i++;
				if(!(i<this->out_s)){
					this->setError(1005, "packHeader(void) - i out of bounds.");
					return false;
				}
				this->out[i] = freq_c; i++;
				if(!(i<this->out_s)){
					this->setError(1006, "packHeader(void) - i out of bounds.");
					return false;
				}
				this->out[i] = freq_d; i++;
				if(!(i<this->out_s)){
					this->setError(1007, "packHeader(void) - i out of bounds.");
					return false;
				}
				this->out[i] = letter;
				j++;
			}	
			return true;
		}
		
		bool unpackHeader(char *data, size_t dataSize){
			if(data == NULL){
				this->setError(1100, "unpackHeader(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(1101, "unpackHeader(char *data, size_t dataSize) - dataSize <= 0, treating as null.");
				return false;
			}

			this->destroyTreeLetters();
			this->destroyFrequencies();
			char letterCount = data[0];
			this->frequencies_s = (size_t)letterCount;
			this->treeLetters_s = this->frequencies_s;
			this->frequencies = new int[this->frequencies_s];
			this->treeLetters = new char[this->treeLetters_s];
			
			int headerSize = 1 + (this->frequencies_s*sizeof(int)) + this->treeLetters_s;
			for(int i=1, j=0; i<headerSize && i<dataSize && j<this->frequencies_s && j<this->treeLetters_s; i++){
				int freq=0;
				char letter=0x00;
				freq += data[i] << (8*3); i++;
				if(!(i<dataSize)){
					this->setError(1102, "unpackHeader(char *data, size_t dataSize) - i out of bounds.");
					return false;
				}
				freq += data[i] << (8*2); i++;
				if(!(i<dataSize)){
					this->setError(1103, "unpackHeader(char *data, size_t dataSize) - i out of bounds.");
					return false;
				}
				freq += data[i] << (8*1); i++;
				if(!(i<dataSize)){
					this->setError(1104, "unpackHeader(char *data, size_t dataSize) - i out of bounds.");
					return false;
				}
				freq += data[i]; i++;
				if(!(i<dataSize)){
					this->setError(1105, "unpackHeader(char *data, size_t dataSize) - i out of bounds.");
					return false;
				}
				letter = data[i];
				
				this->frequencies[j] = freq;
				this->treeLetters[j] = letter;
				j++;
			}
			return true;
		}

		bool encode(char *data, size_t dataSize){
			if(!this->plantTree()){
				this->setError(0x1200, "encode() - failed to plant tree.");
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

			if(!this->packHeader()){
				this->setError(1201, "encode() - failed to pack header.");
				return false;
			}

			int bitLoop=0;
			this->out[headerSize] = outRemainder;
			for(int i=0, o=headerSize+1; i<dataSize && o<this->out_s; i++){
				int codeIndex = this->charToTableIndex(data[i]);
				std::string binary = getCodeBinary(codeIndex);
				for(int j=0; j<binary.length() && o<this->out_s; j++){
					int bit = binary[j] == '0' ? 0 : 1;
					this->out[o] += bit << (7-bitLoop);
					bitLoop++;
					if((bitLoop%8) == 0){
						bitLoop=0;
						o++;
					}
				}
			}

			return true;
		}

		bool decode(char *data, size_t dataSize){
			size_t headerSize = (this->frequencies_s*sizeof(int)) + this->treeLetters_s + 1;
			int paddingCount = (int)data[headerSize];
			int dataStart = headerSize+1;
			int bitCount = ((dataSize - headerSize-1) * 8) - paddingCount;
			std::string grab = "";
			std::string obuff="";
			size_t obuff_s =0;
			destroyOut();
			for(int i=dataStart; i<dataSize && bitCount > 0; i++){
				char val = data[i];
				for(int j=0; j<8 && bitCount>0; j++){
					grab += std::to_string((val >> (7-j)) & 1);
					bitCount--;
					int decoded = this->codeToTableIndex(grab);
					if(decoded != -1){
						obuff += this->treeLetters[decoded];
						obuff_s++;
						grab = "";
					}
				}
			}
			this->out_s = obuff_s;
			this->out = new char[this->out_s];
			for(int i=0; i<obuff_s; i++)
				this->out[i] = obuff[i];
			return true;
		}

		void clearError(void){
			this->error = -1;
			this->error_msg = "";
		}

		void setError(int c, std::string m){
			this->error = c;
			this->error_msg += "["+std::to_string(c)+"] " + m+"\n";
		}

		int countLayers(void){
			int ret = 1;
			if(!this->validateTreeData()){
				this->setError(111, "countLayers() - invalid tree data.");
				return -1;
			}
			
			for(int i=0, track=this->treeData[0]; i<this->treeData_s; i++){
				if(track < this->treeData[i])
					ret++;
				track = this->treeData[i];
			}
			return ret;
		}
		
		bool getLayerIndecies(int *out, size_t out_s){
			if(!this->validateTreeData()){
				this->setError(4444, "getLayerIndecies() - invalid tree data.");
				return false;
			}
			for(int i=0, j=0, track=this->treeData[0]; j<out_s && i<this->treeData_s; i++){
				if(track < this->treeData[i]){
					out[j] = i;
					j++;
				}
				track = this->treeData[i];
			}
			//out[out_s-1] = this->treeData_s-1;
			return true;
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
			this->treeDataTypes = NULL;
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

		void printTreeLetters(void){
			printf("Tree Letters : ");
			if(this->treeLetters == NULL || this->treeLetters_s <=0){
				printf("NULL\n");
				return;
			}
			for(int i=0; i<treeLetters_s; i++){
				printf("[%d]%c ", i, this->treeLetters[i]);
			}printf("\n");
		}

		void printFrequencies(void){
			printf("Frequencies (mex:%d): ", this->frequencyMax);
			if(this->frequencies == NULL || this->frequencies_s <= 0){
				printf("NULL\n");
				return;
			}
			for(int i=0; i<this->frequencies_s; i++){
				printf("[%d]%d ", i, this->frequencies[i]);
			}printf("\n");
		}

		void printCodeTable(void){
			printf("Code Table : \n\tindex | bit count |  code | char\n");
			if(this->frequencies == NULL || this->frequencies_s <= 0 || this->codeTable == NULL || this->codeTable_s <= 0 || this->treeLetters == NULL || this->treeLetters_s <=0){
                                printf("NULL\n");
                                return;
                        }
			for(int i=0; i<this->frequencies_s; i++){
				printf("\t%d    | %d        | %s     | %c\n", i, this->codeTable[i], this->getCodeBinary(i).c_str(), this->treeLetters[i]);
			}printf("\n");
		}

		void printTree(void){
			if(this->treeData == NULL || this->treeData_s <= 0){
				printf("NULL\n");
				return;
			}
			
			size_t layerCount = this->countLayers();
			int *layerIndecies = new int[layerCount];
			if(!getLayerIndecies(layerIndecies, layerCount)){
				printf("Failed to print tree.\n");
				return;
			}
			
			printf("\nTree Layers Count : %ld\n", layerCount);	
			printf("Tree Layer 0:");
			for(int i=0, j=0, pretty=0; i<this->treeData_s && j<layerCount; i++){
				if(i==layerIndecies[j]){
					printf("\n\n");
					j++;
					pretty = 0;
					printf("Tree Layer %d:\n", j);
				}else if((pretty%7) == 0){
					printf("\n");
				}
				
				printf("[%d|%s]%d\t", i, this->treeDataTypes[i] == 0 ? "\033[0;31mbottom\033[0m" : "\033[0;32mtop\033[0m", this->treeData[i]);
				pretty++;
			}
		}

		bool compress(char *data, size_t dataSize){
			this->clearError();
			this->destroyCodingTable();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
			this->tablesSorted = false;
			if(data == NULL){
				this->setError(0, "compress(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(1, "compress(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
				return false;
			}

			if(!this->createTreeLetters(data, dataSize)){
				this->setError(2, "compress(char *data, size_t dataSize) - Failed to create tree letters.");
				return false;
			}

			if(!this->createFrequency(data, dataSize)){
				this->setError(3, "compress(char *data, size_t dataSize) - Failed to create frequency table");
				return false;
			}

			if(!this->sortFreqencies()){
				this->setError(4, "compress(char *data, size_t dataSize) - sortFreqencies failed.");
				return false;
			}

			if(!this->encode(data, dataSize)){
				this->setError(5, "compress(char *data, size_t dataSize) - Failed to encode data.");
				return false;
			}
			

			return true;
		}

		bool decompress(char *data, size_t dataSize){
			this->clearError();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
			if(data == NULL){
                                this->setError(100, "decompress(char *data, size_t dataSize) - data is null.");
                                return false;
                        }
                        if(dataSize <= 0){
                                this->setError(101, "decompress(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
                                return false;
                        }
			if(!this->unpackHeader(data, dataSize)){
				this->setError(102, "decompress(char *data, size_t dataSize) - faiiled to unpack header.");
				return false;
			}

			#if HUFFMAN_DEBUGGING == 1
                        int freqMax = 0;
                        printf("[DBG] Imported Frequencies : \n\t");
                        for(int i=0; i<this->frequencies_s; i++){
                                printf("'%c'(%d) ", this->treeLetters[i], this->frequencies[i]);
                                freqMax += this->frequencies[i];
                        }
                        printf("\n");
                        printf("[DBG] Expected Max : %d\n", freqMax);
                        #endif

			if(!this->plantTree()){
                                this->setError(103, "decompress(char *data, size_t dataSize) - failed to plant tree.");
                                return false;
                        }
                        printf("[DBG] Tree Planted!\n");
			printf("Tree : ");
                        for(int i=0; i<this->treeData_s; i++)
                                printf("[%d]%d ", i, this->treeData[i]);
                        printf("\n");
                        printf("-----CODE TABLE----\nIDX | BIT COUNT | ENCODED VAL | ORIGINAL VAL |\n");
                        for(int i=0; i<this->frequencies_s; i++){
                                printf("%d  |    %d    |    %s    |       %c    |\n", i, this->codeTable[i], this->getCodeBinary(i).c_str(), this->treeLetters[i]);
                        }
                        printf("---------------\n");
		
			if(!this->decode(data, dataSize)){
				this->setError(104, "decompress(char *data, size_t dataSize) - failed to decode the data.");
				return false;
			}
			return true;
		}
};
