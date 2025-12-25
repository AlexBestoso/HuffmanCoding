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
			int treeDataLayerCount;
			int *treeLayerIndecies;
			int *treeLayerSizes;

			int *workQueue;
			size_t workQueue_s;

			size_t workBuffer_fill;
                        size_t workBuffer_s;
                        int *workBuffer;
                        int *workTypeBuffer;
			
			bool tablesSorted;

			int error;
			std::string error_msg;

			void destroyWorkBuffer(void){
				if(workBuffer != NULL){
					delete[] workBuffer;
				}
				if(workTypeBuffer != NULL){
					delete[] workTypeBuffer;
				}
				workBuffer = NULL;
				workTypeBuffer = NULL;
				workBuffer_s=0;
				workBuffer_fill=0;
			}
			
			void destroyTreeLayers(void){
				if(this->treeLayerIndecies != NULL)
					delete[] this->treeLayerIndecies;
				if(this->treeLayerSizes != NULL)
					delete[] this->treeLayerSizes;
				this->treeLayerIndecies = NULL;
				this->treeLayerSizes = NULL;
				this->treeDataLayerCount = 0;
			}
			void resizeTreeLayers(size_t size){
				if(size == 0){
					this->destroyTreeLayers();
				}else if(this->treeLayerIndecies == NULL || this->treeLayerSizes == NULL){
					this->destroyTreeLayers();
					this->treeLayerIndecies = new int[size];
					this->treeLayerSizes = new int[size];
					this->treeDataLayerCount = size;
				}else{
					size_t oldSize = this->treeDataLayerCount;
					int *transfer = new int[oldSize];
					int *transferTwo = new int[oldSize];
					for(int i=0; i<oldSize; i++){
						transfer[i] = this->treeLayerIndecies[i];
						transferTwo[i] = this->treeLayerSizes[i];
					}
					this->destroyTreeLayers();
					this->treeLayerIndecies = new int[size];
					this->treeLayerSizes = new int[size];
					this->treeDataLayerCount = size;
					for(int i=0; i<this->treeDataLayerCount && i<oldSize; i++){
						this->treeLayerIndecies[i] = transfer[i];
						this->treeLayerSizes[i] = transferTwo[i];
					}

					delete[] transfer;
					delete[] transferTwo;
				}
			}
			void destroyWorkQueue(void){
				if(this->workQueue != NULL){
					delete[] this->workQueue;
				}
				this->workQueue = NULL;
				this->workQueue_s = 0;
			}
			void resizeWorkQueue(size_t size){
				if(size == 0){
					this->destroyWorkQueue();
				}else if(this->workQueue == NULL){
					this->workQueue = new int[size];
					this->workQueue_s = size;
				}else{
					size_t oldSize = this->workQueue_s;
					int *transfer = new int[oldSize];
					for(int i=0; i<oldSize; i++){
						transfer[i] = this->treeData[i];
					}
					this->destroyWorkQueue();
					this->workQueue = new int[size];
					this->workQueue_s = size;
					for(int i=0; i<this->workQueue_s && i<oldSize; i++){
						this->treeData[i] = transfer[i];
					}
					
					delete[] transfer;
				}
			}
			bool validateWorkQueue(void){
				if(this->workQueue == NULL){
					this->setError(5645, "validateWorkQueue() - workQueue is null.");
					return false;
				}
				if(this->workQueue_s <= 0){
					this->setError(456456, "validateWorkQueue() - workQueue_s <= 0, treating as null.");
					return false;
				}
				return true;
			}
			int pushWorkQueue(int val, int knownFill){
				if(!this->validateFrequencies()){
					this->setError(234, "pushWorkQueue() - invalid frequencies.");
					return -1;
				}
				if(!this->validateWorkQueue()){
					this->clearError();
					this->resizeWorkQueue(this->frequencies_s);
					for(int i=0; i<this->workQueue_s; i++)
						this->workQueue[i] = -1;
				}else if(this->workQueue[this->workQueue_s-1] != -1){
					this->resizeWorkQueue(this->workQueue_s+1);
					this->workQueue[this->workQueue_s-1] = -1;
				}

				int start = knownFill >= 0 && knownFill <workQueue_s ? knownFill : 0;
				int fillSize = start;
				bool failure=true;
				for(int i=start; i<this->workQueue_s; i++){
					if(this->workQueue[i] == -1){
						failure=false;
						this->workQueue[i] = val;
						fillSize++;
						break;
					}
					fillSize++;
				}
				if(failure){
					this->setError(54355, "pushWorkQueue() - failed to push value.");
					return -1;
				}
				return fillSize;
			}
			int popWorkQueue(void){
				if(!this->validateWorkQueue()){
					this->setError(5345, "popWorkQueue() - invalid work queue.");
					return -1;
				}
				int ret = this->workQueue[0];
				this->workQueue[0] = -1;
				for(int i=1; i<this->workQueue_s; i++){
					this->workQueue[i-1] = this->workQueue[i];
					if(this->workQueue[i] == -1) break;
					this->workQueue[i] = -1;
					
				}
				return ret;
			}

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
				this->resizeTreeLayers(1);
				this->treeLayerSizes[0] = this->frequencies_s; 
				this->workBuffer_fill=0;
				this->workBuffer_s = this->frequencies_s;
				this->workBuffer = new int[workBuffer_s];
				this->workTypeBuffer = new int[workBuffer_s];
				for(int i=0; i<this->workBuffer_s; i++){
					this->workBuffer[i] = 0;
					this->workTypeBuffer[i] = 0;
				}
				return true;

			}

			bool calculateLayerIndecies(void){
				// implement layer validation
				int dataRemainder = this->treeData_s-1;
				for(int i=0; i<this->treeDataLayerCount; i++){
					this->treeLayerIndecies[i] = dataRemainder;
					dataRemainder -= this->treeLayerSizes[i];
				}
				return true;
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
				
				
				this->calculateLayerIndecies();
				size_t layerCount = this->treeDataLayerCount;
				int topLayerStart = this->treeLayerIndecies[layerCount-1];
				size_t topLayerSize = this->treeLayerSizes[layerCount-1];
				if(topLayerSize <=0){
					this->setError(4545, "growLayer() - invalid top size");
					return false;
				}else if(topLayerSize == 1){
					return false;
				}else if(topLayerSize == 2 && this->treeDataTypes[0] == 1 && this->treeDataTypes[1] == 0){
					return false;
				}else if(this->treeDataTypes[0] == 0){
					this->setError(543, "growLayer() - invalid seed data.");
					return false;
				}


				this->workBuffer_fill=0;

				for(int i=topLayerStart, tracer=-1, sum=-1; i>=0; i--){
					if(this->frequencyMax <= this->treeData[i]) break;
					if(this->treeDataTypes[i] == 0){ // it's a bottom node, don't use it.
						continue;
					}
					if(tracer == -1){
						if(i == 0){
							if(sum == -1){
								this->setError(777, "growLayer() - this error should never happen, Ha!");
								return false;
							}
							sum = this->treeData[i] + sum;
							workBuffer[workBuffer_fill] = sum;
							workTypeBuffer[workBuffer_fill] = 1;
							workTypeBuffer[workBuffer_fill-1] = 0;
							workBuffer_fill++;
							break;
						}
						tracer = this->treeData[i];
						continue;
					}
					if(sum == -1){
						sum = this->treeData[i] + tracer;
						workBuffer[workBuffer_fill] = sum;
						workTypeBuffer[workBuffer_fill] = 1;
						workTypeBuffer[workBuffer_fill-1] = 1;
						workBuffer_fill++;
						tracer = -1;
						continue;
					}
					if(this->treeData[i] == sum){
						if(i==0){
							sum = tracer + sum;
							workBuffer[workBuffer_fill] = sum;
                                                	workTypeBuffer[workBuffer_fill] = 0;
                                                	workTypeBuffer[workBuffer_fill-1] = 0;
                                                	workBuffer_fill++;

							sum = this->treeData[i] + sum;
							workBuffer[workBuffer_fill] = sum;
                                                        workTypeBuffer[workBuffer_fill] = 1;
                                                        workBuffer_fill++;
							break;
						}
						sum = tracer + sum;
						workBuffer[workBuffer_fill] = sum;
                                                workTypeBuffer[workBuffer_fill] = 1;
                                                workTypeBuffer[workBuffer_fill-1] = 0;
                                                workBuffer_fill++;
						tracer = this->treeData[i];
						continue;
					}
					if(this->treeData[i] < sum){
						sum = this->treeData[i] + tracer;
						workBuffer[workBuffer_fill] = sum;
                                                workTypeBuffer[workBuffer_fill] = 1;
                                                workTypeBuffer[workBuffer_fill-1] = 1;
                                                workBuffer_fill++;
						tracer = -1;
					}
					if(this->treeData[i] > sum){
						if(i==0){
							sum = tracer + sum;
                                                        workBuffer[workBuffer_fill] = sum;
                                                        workTypeBuffer[workBuffer_fill] = 0;
                                                        workTypeBuffer[workBuffer_fill-1] = 0;
                                                        workBuffer_fill++;

                                                        sum = this->treeData[i] + sum;
                                                        workBuffer[workBuffer_fill] = sum;
                                                        workTypeBuffer[workBuffer_fill] = 1;
                                                        workBuffer_fill++;
							break;
						}
						sum = tracer + sum;
                                                workBuffer[workBuffer_fill] = sum;
                                                workTypeBuffer[workBuffer_fill] = 0;
                                                workTypeBuffer[workBuffer_fill-1] = 0;
                                                workBuffer_fill++;
						tracer = this->treeData[i];
					}
				}
				if(workBuffer_fill <= 0){
					printf("Nothing to fill.\n");
					return false;
				}
				workTypeBuffer[workBuffer_fill-1] = 1;

				this->resizeTreeLayers(this->treeDataLayerCount+1);
				this->treeLayerSizes[this->treeDataLayerCount-1] = workBuffer_fill;
							
				// push original data to end of array
				size_t originalSize = this->treeData_s;
				this->resizeTreeData(originalSize+workBuffer_fill);
				for(int i=this->treeData_s-1, track=0;  track<originalSize && i>=0; i--, track++){
					if((i-workBuffer_fill) < 0){break;}
					this->treeData[i] = this->treeData[i-workBuffer_fill];
					this->treeDataTypes[i] = this->treeDataTypes[i-workBuffer_fill];
				}

				// populate front of array with new data
				for(int i=0; i<workBuffer_fill && (workBuffer_fill-1-i) >= 0 && (workBuffer_fill-1-i) < workBuffer_s; i++){
					this->treeData[i] = workBuffer[workBuffer_fill-1-i];
					this->treeDataTypes[i] = workTypeBuffer[workBuffer_fill-1-i];
				}

				return true;
			}
		
			bool plantTree(void){
				if(!this->validateFrequencies()){
					this->setError(700, "plantTree(void) - invalid frequencies.");
					return false;
				}
				this->destroyTreeData();
				
				// seed the tree, and begin coding table.
				int startSize = this->treeData_s;
				this->treeDataLayerCount=0;
				// TODO: put an error time out. This shouldn't loop more than frequencies_s times.
				while(this->growLayer()){
					if(this->failed()){
						return false;
					}
				}
				if(this->failed()){
					return false;
				}
				this->calculateLayerIndecies();
				if(this->treeData[0] != this->frequencyMax){
					this->setError(3333, "plantTree(void) - Failed to grow tree, tree is corrupt.");
					return false;
				}

				return true;
			}


		bool generateCodeTable(void){
			if(!this->validateTreeData()){
				this->setError(44456, "generateCodeTable() - failed to validate tree data.");
				return false;
			}
			if(!this->validateFrequencies()){
				this->setError(665434, "generateCodeTable() - failed to validate frequencies.");
				return false;
			}
			// TODO: Validate tree letters

			// Calculate Layer Count, and top layer start index.
			size_t layerCount = this->treeDataLayerCount;

			// allocate the coding table.
			this->destroyCodingTable();
			this->codeTable_s = this->frequencies_s*2;
			this->codeTable = new int[this->codeTable_s];
			for(int i=0; i<this->codeTable_s; i++){
				this->codeTable[i] = 0;
			}
			off_t tableSize_o = 0;
			off_t tableCode_o = this->frequencies_s;
			int converter = this->treeData_s - this->frequencies_s;
			int zero=-1;
			int one=-1;
			int queueFill=0;
			// generate code table.
			for(int i=this->treeData_s-1; i>=0; i--){
				this->getSubIndecies(i, &zero, &one);
				if(this->failed() || (one == -1 && zero == -1)){
                                	this->setError(3434, "generateCodeTable() - failed to get sub indecies.");
                                	return false;
                                }
				if(zero != -1){
					queueFill = this->pushWorkQueue(zero, queueFill);
					while(queueFill > 0){
						int queueData = this->popWorkQueue();
						if(this->failed()){
							this->setError(7786, "generateCodeTable() - failed to pop work queue.");
							return false;
						}
						queueFill--;
						int o=-1, z=-1;
						this->getSubIndecies(queueData, &o, &z);
						if(this->failed() || (o==-1 && z==-1)){
							this->setError(4433, "generateCodeTable() - zero's subindecies failed to generate.");
							return false;
						}
						if(o!=-1 && o >= this->treeData_s-this->frequencies_s){
							o = o - converter;
							this->codeTable[o+tableSize_o]++;
							this->codeTable[o+tableCode_o] = this->codeTable[o+tableCode_o] >> 1;
						}else if(o >= 0 && o <this->treeData_s && o != queueData){
							queueFill = this->pushWorkQueue(o, queueFill);
						}
					
						if(z!=-1 && z >= this->treeData_s-this->frequencies_s){
							z = z - converter;
							this->codeTable[z+tableSize_o]++;
							this->codeTable[z+tableCode_o] = this->codeTable[z+tableCode_o] >> 1;
						}else if(z >= 0 && z <this->treeData_s && z != queueData){
							queueFill = this->pushWorkQueue(z, queueFill);
						}

					}
				}
				if(one != -1){
					queueFill = this->pushWorkQueue(one, queueFill);
					while(queueFill > 0){
						int queueData = this->popWorkQueue();
						if(this->failed()){
							this->setError(7386, "generateCodeTable() - failed to pop work queue.");
							return false;
						}
						queueFill--;
						int o=-1, z=-1;
						this->getSubIndecies(queueData, &o, &z);
						if(this->failed() || (o==-1 && z==-1)){
							this->setError(3433, "generateCodeTable() - zero's subindecies failed to generate.");
							return false;
						}
						if(o!=-1 && o >= this->treeData_s-this->frequencies_s){
							o = o - converter;
							this->codeTable[o+tableSize_o]++;
							this->codeTable[o+tableCode_o] = (1<<7)+this->codeTable[o+tableCode_o] >> 1;
						}else if(o >= 0 && o <this->treeData_s && o != queueData){
							queueFill = this->pushWorkQueue(o, queueFill);
						}
					
						if(z!=-1 && z >= this->treeData_s-this->frequencies_s){
							z = z - converter;
							this->codeTable[z+tableSize_o]++;
							this->codeTable[z+tableCode_o] = (1<<7)+this->codeTable[z+tableCode_o] >> 1;
						}else if(z >= 0 && z <this->treeData_s && z != queueData){
							queueFill = this->pushWorkQueue(z, queueFill);
						}

					}
				}

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
		bool getSubIndecies(int targetIndex, int *zeroIndex, int *oneIndex){
			if(!this->validateTreeData()){
				this->setError(44442, "getSubIndecies() - invalid tree data.");
				return false;
			}
			if(!this->validateFrequencies()){
				this->setError(3542, "getSubIndecies() - invalid frequencies.");
				return false;
			}
			if(targetIndex < 0 || targetIndex >= this->treeData_s){
				std::string message = "getSubIndecies() - targetIndex is out of bounds. target:"+std::to_string(targetIndex)+" | treeSize:"+std::to_string(this->treeData_s);
				this->setError(44452, message.c_str());
				return false;
			}
			// TODO: validate layers

			int targetLayer = -1;
			for(int i=0; i<this->treeDataLayerCount; i++){
				int start = this->treeLayerIndecies[i];
				int end = this->treeLayerIndecies[i]-this->treeLayerSizes[i]+1;
				if(targetIndex <= start && targetIndex >= end){
					targetLayer = i;
					break;
				}
			}
			if(targetLayer == -1){
				this->setError(5555, "getSubIndecies() - failed to get target layer.");
				return false;
			}
			if(targetLayer == 0){
				int pos = (this->frequencies_s%2) == 0 ? (1+targetIndex)%2 : (targetIndex == this->treeLayerIndecies[targetLayer]-this->treeLayerSizes[targetLayer]-1 ? 0 : (targetIndex)%2);
				if(pos == 0){
					zeroIndex[0] = targetIndex;
					oneIndex[0] = -1;
				}else{
					zeroIndex[0] = -1;
					oneIndex[0] = targetIndex;
				}
				return true;
			}

			int targetLayerStart = this->treeLayerIndecies[targetLayer];
			int targetLayerEnd = targetLayerStart-this->treeLayerSizes[targetLayer];
			int sourceLayer = targetLayer-1;
			int sourceLayerStart = this->treeLayerIndecies[sourceLayer];
			int sourceLayerEnd = sourceLayerStart - this->treeLayerSizes[sourceLayer];

			for(int i=sourceLayerStart, t=targetLayerStart, tracerIdx=0,tracer=-1, sum=-1; i>sourceLayerEnd && t>targetLayerEnd; i--){
				if(this->treeDataTypes[i] == 0){ // it's a bottom node, don't use it.
                                	continue;
                                }
				if(tracer == -1){
					if(i == sourceLayerEnd+1){
						if(sum == -1){
							this->setError(777, "getSubIndecies() - this error should never happen, Ha!");
							return false;
						}
						sum = this->treeData[i] + sum;
						if(t == targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = t+1;
							return true;
						}
						t--;
						break;
					}
					tracer = this->treeData[i];
					tracerIdx=i;
					continue;
				}
				if(sum == -1){
					sum = this->treeData[i] + tracer;
					if(t == targetIndex){
						zeroIndex[0] = i;
						oneIndex[0] = tracerIdx;
						return true;
					}
					t--;
					tracer = -1;
					tracerIdx=-1;
					continue;
				}
				if(this->treeData[i] == sum){
					if(i==sourceLayerEnd+1){
						sum = tracer + sum;
						if(t==targetIndex){
							zeroIndex[0] = tracerIdx;
							oneIndex[0] = t+1;
							return true;
						}
						t--;
						if(!(t>targetLayerEnd)){
							this->setError(453, "getSubIndecies() - tree misaligned.");
							return false;
						}
						sum = this->treeData[i] + sum;
						if(t == targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = t+1;
							return true;
						}
						break;
					}
					sum = tracer + sum;
					if(t==targetIndex){
						zeroIndex[0] = tracerIdx;
						oneIndex[0] = t+1;
						return true;
					}
					t--;
					tracer = this->treeData[i];
					tracerIdx=i;
					continue;
				}
				if(this->treeData[i] < sum){
					sum = this->treeData[i] + tracer;
					if(t==targetIndex){
						zeroIndex[0] = i;
						oneIndex[0] = tracerIdx;
						return true;
					}
					t--;
					tracer = -1;
					tracerIdx=-1;
					continue;
				}
				if(this->treeData[i] > sum){
					if(i==sourceLayerEnd+1){
						sum = tracer + sum;
						if(t == targetIndex){
							zeroIndex[0] = tracerIdx;
							oneIndex[0] = t+1;
							return true;
						}
						t--;
						if(!(t>targetLayerEnd)){
							this->setError(4533, "getSubIndecies() - miss a ligned tree.");
							return false;
						}
                                                sum = this->treeData[i] + sum;
						if(t == targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = t+1;
							return true;
						}
						t--;
						break;
					}
					sum = tracer + sum;
					if(t == targetIndex){
						zeroIndex[0] = tracerIdx;
						oneIndex[0] = t+1;
						return true;
					}
					t--;
					tracer = this->treeData[i];
					tracerIdx=i;
					continue;
				}
			}

			std::string msg = "getSubIndecies(target:"+std::to_string(targetIndex)+") - dbg : sourceLayer:"+std::to_string(sourceLayer)+" | targetLayer:"+std::to_string(targetLayer);
			this->setError(4444, msg.c_str());
			return false;
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
			this->workQueue_s = 0;
			this->workQueue = NULL;
			this->treeLayerIndecies = NULL;
                        this->treeLayerSizes = NULL;
                        this->treeDataLayerCount = 0;
			this->clearError();
		}
		~HuffmanCoding(){
			this->destroyCodingTable();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
			this->destroyTreeData();
			this->destroyWorkQueue();
			this->destroyTreeLayers();
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
			
			size_t layerCount = this->treeDataLayerCount;
			int *layerIndecies = this->treeLayerIndecies;
			
			printf("\nTree Layers Count : %ld\n", layerCount);
			printf("Expected 0 value : %d\n", this->frequencyMax);
			int t=0;
			int pretty=0;
			int count=0;
			for(int i=this->treeDataLayerCount-1; i>=0; i--){
				printf("Tree Layer %d, size %d, start %d, end %d", i, this->treeLayerSizes[i], this->treeLayerIndecies[i], this->treeLayerIndecies[i]-this->treeLayerSizes[i]);
				count=0;
				for(int j=0; j<this->treeLayerSizes[i]; j++){
					if((pretty%7) == 0){
						count+=7;
                                	        printf("\n%d >) ",count );
					}
					printf("[%d|%s]%d\t", t, this->treeDataTypes[t] == 0 ? "\033[0;31mbottom\033[0m" : "\033[0;32mtop\033[0m", this->treeData[t]);
					t++;
					pretty++;
				}
				pretty=0;
				printf("\n");
			}
		}

		bool debug(){
			printf("\nDebugging!\n");
			int zero=-1;
			int one=-1;
			for(int i=this->treeDataLayerCount-1; i>=0; i--){
				int layerStart = this->treeLayerIndecies[i];
				int layerEnd = layerStart-this->treeLayerSizes[i];
				printf("DEBUG LAYER %d, start:%d, end:%d\n", i, layerStart, layerEnd);
				for(int j=1+layerEnd; j<layerStart+1; j++){
					if(!this->getSubIndecies(j, &zero, &one)){
						this->setError(444, "debug() - failed to get sub indecies.");
						return false;
					}
					printf("%d) %d is of 0:%d[%d], 1:%d[%d] - %s", j, this->treeData[j], zero > -1 ? this->treeData[zero] : 0x00, zero, one > -1 ? this->treeData[one] : 0x00, one, this->treeDataTypes[j] == 1 ? "\033[0;42mtop\033[0m\n" : "\033[0;41mbottom\033[0m\n");
				}
			}
			printf("leaving function.\n");
			return false;
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

			if(!this->plantTree()){
                                this->setError(545, "compress() - failed to plant tree.");
                                return false;
                        }
			
		//	this->debug();
		//	return false;
			if(!this->generateCodeTable()){
				this->setError(5555, "compress() - failed to generate code table.");
				return false;
			}

			printf("[DBG} : ");
			this->printCodeTable();

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
