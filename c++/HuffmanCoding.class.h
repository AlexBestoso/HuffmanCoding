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

			char *header;
			size_t header_s;

			char *body;
			size_t body_s;
			
			bool tablesSorted;

			int error;
			std::string error_msg;

			void destroyHeader(void){
				if(this->header != NULL){
					delete[] this->header;
				}
				this->header_s = 0;
			}
			void destroyBody(void){
				if(this->body != NULL){
					delete[] this->body;
				}
				this->body_s = 0;
			}

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
							sum = tracer + this->treeData[i];
							workBuffer[workBuffer_fill] = sum;
                                                	workTypeBuffer[workBuffer_fill] = 1;
                                                	workTypeBuffer[workBuffer_fill-1] = 1;
                                                	workBuffer_fill++;
							break;
						}
						sum = tracer + this->treeData[i];
						workBuffer[workBuffer_fill] = sum;
                                                workTypeBuffer[workBuffer_fill] = 1;
                                                workTypeBuffer[workBuffer_fill-1] = 1;
                                                workBuffer_fill++;
						tracer = -1;
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
		
		bool isBaseIndex(int target){
			if(!this->validateTreeData()){
				this->setError(4444, "isBaseIndex() - invalid tree data.");
				return false;
			}
			// TODO: validate tree layers.
			if(target <= this->treeLayerIndecies[0] && target > (this->treeLayerIndecies[0]-this->treeLayerSizes[0])){
				return true;
			}
			return false;
		}

		bool addBitToCodeTable(int targetIndex, int bit){
			if(!this->validateFrequencies()){
				this->setError(45345, "addBitToCodeTable() - failed to validate frequencies.");
				return false;
			}
			// TODO validate code table.
			if(targetIndex < 0 || targetIndex >= this->codeTable_s){
				this->setError(45454, "addBitToCodeTable() - targetIndex is out of bounds.");
				return false;
			}
			this->codeTable[targetIndex]++;
			int codeIdx = targetIndex+this->frequencies_s;
			this->codeTable[codeIdx] = (this->codeTable[codeIdx] << 1) + (bit&0x01);
			return true;
		}

		bool codeTableSortByBitCount(void){
			if(!this->validateTreeData()){
                                this->setError(44456, "generateCodeTable() - failed to validate tree data.");
                                return false;
                        }
                        if(!this->validateFrequencies()){
                                this->setError(665434, "generateCodeTable() - failed to validate frequencies.");
                                return false;
                        }
			// TODO: validate code table
			this->destroyWorkBuffer();
			this->workBuffer_s = this->frequencies_s;
			this->workBuffer = new int[this->workBuffer_s];
			int targetIdxA = 0;
			int targetIdxB = 0;
			int swapA = 0;
			int swapB = 0;
			
			
			this->destroyWorkBuffer();
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
			// TODO: validate tree layers

			// Calculate Layer Count, and top layer start index.
			size_t layerCount = this->treeDataLayerCount;

			// allocate the coding table.
			this->destroyCodingTable();
			this->codeTable_s = this->frequencies_s*2;
			this->codeTable = new int[this->codeTable_s];
			for(int i=0; i<this->codeTable_s; i++){
				this->codeTable[i] = 0;
			}
			int converter = this->treeData_s - this->frequencies_s;
			int zero=-1;
			int one=-1;
			int queueFill=0;
			int start = this->treeLayerIndecies[1];
			int bitArray[2] = {0};
			for(int i=0; i<=start; i++){
				if(this->isBaseIndex(i)){
					this->setError(3234, "generateCodeTable() - huffman tree missaligned.");
					return false; // we start one layer away from the leafs
				}
				if(!this->getSubIndecies(i, &zero, &one) && this->failed()){
					this->setError(666, "generateCodeTable() - failed to get inital sub indecies.");
					return false;
				}
				bitArray[0] = zero;
				bitArray[1] = one;
				for(int bit=0; bit<2; bit++){
					if(this->isBaseIndex(bitArray[bit])){
						int newIndex = bitArray[bit] - converter; // convert to 0 to frequencie_s
						if(!this->addBitToCodeTable(newIndex, bit)){
							this->setError(45423, "generateCodeTable() - failed to add bit to code table.");
							return false;
						}
						continue;
					}
					queueFill = 0;
					queueFill = this->pushWorkQueue(bitArray[bit], queueFill);
					while(queueFill > 0){
						int target = this->popWorkQueue();
						queueFill--;

						if(!this->getSubIndecies(target, &zero, &one) && this->failed()){
							this->setError(3333, "generateCodeTable() - failed to get sub node to add bit to.");
							return false;
						}

						if(this->isBaseIndex(zero)){
							int newIndex = zero - converter; // convert to 0 to frequencie_s
							if(!this->addBitToCodeTable(newIndex, bit)){
								this->setError(445, "generateCodeTable() - failed to add bit to zero index.");
								return false;
							}
						}else{
							queueFill = this->pushWorkQueue(zero, queueFill);
						}

						if(this->isBaseIndex(one)){
							int newIndex = one - converter; // convert to 0 to frequencie_s
							if(!this->addBitToCodeTable(newIndex, bit)){
								this->setError(435, "generateCodeTable() - failed to add bit to one index.");
								return false;
							}
						}else{
							queueFill = this->pushWorkQueue(one, queueFill);
						}
					}
				}
			}
			this->codeTableSortByBitCount();
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
			for(int i=codeSize-1;i>=0; i--){
				int bit = 1 & (code>>i);
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

		/* Header Structure: we can use what we got to calculate padding in the body.
		 *  3 bits to store final bit index
		 *  9 bits to store freqiency count
		 *  entry(
			3 bits to store container size
			1 to 4 bytes containing frequency variable.
			1 byte for the char.
		    )
		 * Int is stored big endian
		 * */
		void dbg_pb(const char *msg, int val, int bits, int highlight){
			this->dbg_pb(msg, val, bits, highlight, 1);
		}

		void dbg_pb(const char *msg, int val, int bits, int highlight, int bitCount){
			printf("%s", msg); 
			for(int i=0; i<bits; i++){
				if((i>=highlight && i<highlight+bitCount && bitCount >= 0)){
					printf("\033[0;42m%d\033[0m", (val >> (bits-1-i))&1);
					
				}else if(i<highlight && bitCount < 0){
					printf("\033[0;41m%d\033[0m", (val >> (bits-1-i))&1);
				}else{
					printf("%d", (val >> (bits-1-i))&1);
				}
			}printf(" (%d)\n", val);
		}
		std::string dbg_getBin(int val, int bits, int highlight, int bitCount){
                        std::string ret = "";
                        for(int i=0; i<bits; i++){
                                if((i>=highlight && i<highlight+bitCount && bitCount >= 0)){
					ret += "\033[0;42m"+std::to_string((val >> (bits-1-i))&1)+"\033[0m";

                                }else if(i<highlight && bitCount < 0){
					ret += "\033[0;41m"+std::to_string((val >> (bits-1-i))&1)+"\033[0m";
                                }else{
					ret += std::to_string((val >> (bits-1-i))&1);
                                }
                        }
			return ret;
                }

		 /* * packByte Parameter Breakdown
		 * 1) The value that we want to pack into a location.
		 * 2) From lsb; number of bits within arg 1 that we care about.
		 * 3) Mask of an arg2 number of binary ones, 9 bit requires 0x1ff, or 0b1,1111,1111
		 * 4) the place to pack argument 1 into.
		 * 5) size of argument 4's location.
		 * 6) Starting element offset into argument 4's location.
		 * 7) Starting binary offset of argument 6's elemental location.
		 *  \
		 *  |_-> 6 to which byte in array of bytes, 7 points to which bit in selected byte.
		 * */
		bool packByte(int packingTarget, int targetBitCount, int targetOverflowMask, char *dstBuffer, size_t dstBufferSize, int *dstIndex, int *bitIndex){
			int binaryMax=8; // Pack byte, so we operate relative to a max container of 8
			int containerSize = packingTarget <= 0xff ? 1 : (((packingTarget/0xff)) + 1);
			bool maxOverflowed = binaryMax-targetBitCount < 0;
			if(maxOverflowed) containerSize--;
			int bitCount=targetBitCount;
			int bitIdx = bitIndex[0];
			int hi = dstIndex[0];

			int lvi = binaryMax - bitCount;
			int dte = (binaryMax-1) - lvi;
			int countFill 		= 0;	//bitIdx >= binaryMax-dte ? (binaryMax-bitIdx) : binaryMax;
			int countOverflow 	= 0;	//bitCount-countFill;
			int masterDifference 	= 0;	//binaryMax-bitCount-bitIdx;

			for(int chunkIdx=containerSize-1; chunkIdx>=0 && hi < dstBufferSize; chunkIdx--){
				int chunk = (packingTarget >> (chunkIdx*binaryMax)) & 0xff;
				countFill = (maxOverflowed || bitIdx >= binaryMax-dte) ? (binaryMax-bitIdx) : binaryMax;
				countOverflow = (bitCount-countFill) % binaryMax;
				masterDifference = binaryMax-bitCount-bitIdx;
				
				if(maxOverflowed){
                        		dstBuffer[hi] = (char)((packingTarget >> countOverflow)&0xff);
				}else if(masterDifference >= 0){
					dstBuffer[hi] += ((chunk) << (masterDifference));
				}else{
					masterDifference *= -1;
					dstBuffer[hi] += ((chunk) >> (masterDifference));
				}
				if(maxOverflowed){
					hi++;
                        		if(!(hi < dstBufferSize)){
                        		        this->setError(54545, "packByte() - hi out of bounds.");
                        		        return false;
                        		}
                        		dstBuffer[hi] = (char)(((packingTarget & (0x1ff>>countFill)) << countOverflow)&0xff);
				}else if(bitIdx >= binaryMax-dte){
					hi++;
					if(!(hi < dstBufferSize)){
						this->setError(345, "packByte() - hi is out of bounds.");
						return false;
					}
					dstBuffer[hi] = (char)(((chunk & (targetOverflowMask>>countFill)) & 0xff) << (binaryMax-countOverflow));
				}else if(bitIdx == lvi){
					hi++;
					if(!(hi<dstBufferSize)){
						this->setError(454, "packByte() - hi is out of bounds.");
						return false;
					}
					dstBuffer[hi] = 0x00;
				}
				bitIdx = (bitIdx+bitCount) % binaryMax;
			}
			bitIndex[0] =  bitIdx;
                        dstIndex[0] = hi;
			return true;
		}

		int packHeader(void){
			if(!this->validateFrequencies()){
				this->setError(1000, "packHeader(void) - failed to validate frequencies.");
				return -1;
			}
			// TODO: validate letters.
			if(this->frequencies_s != this->treeLetters_s){
				this->setError(12323, "packHeader() - frequency and tree letter arrays are missaligned.");
				return -1;
			}

			
			// Calculate header size.
			this->destroyHeader();
			// Padding is the bitIDX, so it's modulo 8, and because 0x08 = 0b1000, we only need 4 binary digits.
			int paddingBitCount = 4;
			// element max is 256, so because 256 is 0x1FF, and 0x1ff = 0b1,0000,0000, we need 9 binary digits.
			int elementCountBitCount = 9;
			// initalize the header bit count with p + en…(padding + element count)
			int headerBitCount = paddingBitCount + elementCountBitCount;
			// sizeof(int) = 4, so since 0x4 = 0b100, we only need 3 binary digits to store size.
			int containerSizeBitCount = 3;
			int byteBitCount = 8;
			for(int i=0; i<this->frequencies_s; i++){
				// byte is 8 bits, ((freq / 0b1111,1111) + 1) = bytes required to contain freqs value.
				int freqencyContainerSizeBitCount = byteBitCount * ((this->frequencies[i]/0xff)+ 1);
				// letter bitcount + freq bit count, + size of freq container bit count
				headerBitCount += byteBitCount + freqencyContainerSizeBitCount + containerSizeBitCount;
			}
			if(headerBitCount < paddingBitCount+elementCountBitCount+containerSizeBitCount+(2*byteBitCount)){
				this->setError(45345, "packHeader() - invalid header bit count.");
				return false;
			}
			// Conver the bits to bytes and allocate.
			this->header_s = (headerBitCount % 8) != 0 ? (headerBitCount/8)+1 : headerBitCount/8; 
			this->header = new char[this->header_s];

			// All we know is padding is 4 bits is size, so start beyond that.
			int bitIdx = 4;
			int hi=0; // header index.
			int elementCount = this->frequencies_s;
			/*
			 * 1) Number of different values used to create the tree.
			 * 2) Max different number of values are 2^8 = 256 = 0b1,0000,0000 = 9 bits
			 * 3) The only mask that can select 9 bits via AND is 0x1ff = 0b1,1111,1111
			 * 4) the place to pack argument 1 into.
			 * 5) size of argument 4's location.
			 * 6) Starting element offset into argument 4's location.
			 * 7) Starting binary offset of argument 6's elemental location.
			 * */
			this->packByte(elementCount, 9, 0x1ff, this->header, this->header_s, &hi, &bitIdx);
 
			for(int i=0; i<this->frequencies_s && hi<this->header_s; i++){
				int containerSize = (((this->frequencies[i]/0xff)) + 1);
				this->packByte(containerSize, 3, 0xff, this->header, this->header_s, &hi, &bitIdx);
				int freq = this->frequencies[i];
				this->packByte(freq, 8, 0xff, this->header, this->header_s, &hi, &bitIdx);
				char letter = this->treeLetters[i];
				this->packByte((int)letter&0xff, 8, 0xff, this->header, this->header_s, &hi, &bitIdx);
			}
			return bitIdx;
		}
		
		int getEncodeCharIndex(char target){
			// validate tree letters.
			for(int i=0; i<this->treeLetters_s; i++){
				if(this->treeLetters[i] == target) return i;
			}
	
			return -1;
		}	
	
		int packBody(int startingBitIndex, char *data, size_t dataSize){
			if(!this->validateFrequencies()){
				this->setError(345, "packBody() - failed to validate frequencies.");
				return -1;
			}
			// validate code table.
			// validate header.

			printf("[*] Packing Body Bits.\n");
			printf("[!] Starting Bit Index : %d\n", startingBitIndex);
			printf("[!] data address : %p\n", data);
			printf("[!] data size : %ld\n", dataSize);

			// calculate body size using the code table
			this->destroyBody();
			for(int i=0; i<this->frequencies_s; i++)
				this->body_s += this->codeTable[i] * this->frequencies[i];
			
			this->body_s = (this->body_s % 8) == 0 ? this->body_s/8 : (this->body_s/8)+1;
			if(startingBitIndex > 0)
				this->body_s++;
			
			this->body = new char[this->body_s];
			printf("[!] Calculated body Size : %ld\n", this->body_s);

			int bitIdx=startingBitIndex % 8;
			int bi=0;
			this->body[bi] = 0;
			for(int i=0; i<dataSize && bi<this->body_s; i++){
				int tableIdx = this->getEncodeCharIndex(data[i]);
				if(tableIdx == -1){
					this->setError(453445, "packBody() - failed to get char index.");
					return -1;
				}
				int bitCount = this->codeTable[tableIdx];
				int encodedChar = this->codeTable[tableIdx+this->frequencies_s];
				int mask = ~(~(0) << bitCount);
				int dbgA = bi;
				printf("[*] i: %d | bi: %d | bitIdx: %d\n", i, bi, bitIdx);
				this->packByte(encodedChar, bitCount, mask, this->body, this->body_s, &bi, &bitIdx);
				printf("[*]\tPacked %d(%d bits) into :\n", (int)encodedChar & 0xff, bitCount);
				for(int d=dbgA; d<=bi; d++){
					std::string bin = this->dbg_getBin(this->body[d], 8, 0, 0);
					printf("[*]\t\t[idx:%d] (%d) %s\n", d, (int)this->body[d]&0xff, bin.c_str());
				}
			}
			this->body_s -= (this->body_s-bi);
			printf("[*] final Size %ld, returning bit idx %d\n----------------\n", this->body_s, bitIdx);
			return bitIdx;
		}
		
		int unpackByte(char *src, size_t srcSize, int *srcIndex, int *bitIndex, int expectedBitCount, int byteMask, int expectedContainerSize){
			int ret = 0;
			int binaryMax=8; // Pack byte, so we operate relative to a max container of 8
                        int bitCount=expectedBitCount;
                        int bitIdx = bitIndex[0];
                        int hi = srcIndex[0];
                        int lvi = binaryMax - bitCount;
                        int dte = (binaryMax-1) - lvi;
                        int countFill           = 0;    //bitIdx >= binaryMax-dte ? (binaryMax-bitIdx) : binaryMax;
                        int countOverflow       = 0;    //bitCount-countFill;
                        int masterDifference    = 0;    //binaryMax-bitCount-bitIdx;

                        bool maxOverflowed = binaryMax-expectedBitCount < 0;
                        if(maxOverflowed) expectedContainerSize--;
			
		
			for(int chunkIdx=expectedContainerSize-1; chunkIdx>=0; chunkIdx--){
				countFill = (maxOverflowed || bitIdx >= binaryMax-dte) ? (binaryMax-bitIdx) : binaryMax;
                                countOverflow = (bitCount-countFill) % binaryMax;
                                masterDifference = binaryMax-bitCount-bitIdx;
				int chunk = 0;
				if(maxOverflowed){
					chunk = (((int)src[hi]) & (byteMask>>countOverflow)) << (countOverflow);
                                }else if(masterDifference >= 0){
					chunk = (((int)src[hi] & (0xff >> bitIdx)) >> masterDifference) & 0xff;
				}else{
					masterDifference*=-1;
					chunk = (((int)src[hi] & (0xff >> bitIdx)) << masterDifference) & 0xff;
				}

				if(maxOverflowed){
					hi++;
					if(!(hi < srcSize)){
						this->setError(2334, "unpackHeader() - hi overflows data.");
						return 0;
					}
					byteMask = 0xff;
					ret += (((int)src[hi]) & ((byteMask>>countOverflow)<<countOverflow)) >> (countOverflow);
				}else if(bitIdx >= binaryMax-dte){ // overflow
					hi++;
					if(!(hi<srcSize)){
						this->setError(654, "unpackHeader() - i is out of bounds.");
						return 0;
					}
					chunk += ((int)src[hi] & 0xff) >> (binaryMax-countOverflow);
				}else if(bitIdx == lvi || bitIdx+bitCount >= binaryMax){
					hi++;
                                        if(!(hi<srcSize)){
                                              this->setError(654, "unpackHeader() - i is out of bounds.");
                                              return 0;
                                        }
				}

				ret += chunk << (chunkIdx*binaryMax);
				bitIdx = (bitIdx + bitCount) % binaryMax; 
			}
			
			srcIndex[0] = hi;
			bitIndex[0] = bitIdx;
			return ret;
		}
		bool unpackHeader(char *data, size_t dataSize, int *ptr_indexOffset, int *ptr_bitOffset,  int *ptr_endPadding){
			if(data == NULL){
				this->setError(1100, "unpackHeader(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 2){
				this->setError(1101, "unpackHeader(char *data, size_t dataSize) - dataSize <= 2, which is invalid, treating as null.");
				return false;
			}

			this->destroyTreeLetters();
			this->destroyFrequencies();
			char entryChar=0x00;
                        int bitIdx = 0;// first 3 bits are reserved for padding.
			int entryContainerSize=0;
			int entryFreq=0;
                        int hi=0;
			int padding = unpackByte(data, dataSize, &hi, &bitIdx, 4, 0xf, 1);


			// clear up to bit idx, 
			int freqCount = unpackByte(data, dataSize, &hi, &bitIdx, 9, 0x1ff, 2);

			this->resizeTreeLetters(freqCount);
			this->resizeFrequencies(freqCount);

			// This loop is the problem. Need to loop frequency_s amount of times, and incrememnt hi speratately.
			for(int i=0; i<this->frequencies_s && hi < dataSize; i++){
				entryContainerSize=0;
				entryFreq=0;
				entryChar=0x00;

				/* get entry container size */
				entryContainerSize = unpackByte(data, dataSize, &hi, &bitIdx, 3, 0xff, 1);
				entryFreq=unpackByte(data, dataSize, &hi, &bitIdx, 8, 0xff, entryContainerSize);
				entryChar = (char)unpackByte(data, dataSize, &hi, &bitIdx, 8, 0xff, 1);
				
				/*We have our data, lets store it in our tables.*/
				this->treeLetters[i] = entryChar;
				this->frequencies[i] = entryFreq;
			}
			ptr_indexOffset[0] = hi+1; // +1 converts index position into size container.
			ptr_bitOffset[0] = bitIdx;
			ptr_endPadding[0] = padding;
			return true;
		}
		
		int getPackedBits(char *data, size_t dataSize, int *index, int *startBit, int numOfBitsToFetch, int bitsContainerSize){
			printf("[*] getPackedBits(data:%p, dataSize:%ld, index:%d, startBit:%d, numberOfBitsToFetch:%d, bitsContainerSize:%d)\n", data, dataSize, index[0], startBit[0], numOfBitsToFetch, bitsContainerSize);
			int ret = 0;
			int rb = numOfBitsToFetch; // remaining bits.
			int targetByteCount = bitsContainerSize;
			printf("[*]\tBits To fetch %d | target byte count : %d\n", rb, targetByteCount);
			printf("[*]\tLoop start index : %d, bit start index : %d\n", index[0], startBit[0]);
			for(int i=index[0]; i<dataSize && rb>0; i++){
				printf("[l]\tlooping %d\n", i);
				int d = (int)data[i]&0xff; // data
				printf("[*]\t\tIsolated Data : %d\n", d);
				int ab = (7-startBit[0])+1;// available bits
				printf("[*]\t\tBits available in this byte : %d\n", ab);
				int atf = rb-ab >= 0 ? ab : rb; // amount to fetch
				printf("[*]\t\tAmount of bits to fetch from this byte: %d\n", atf);
				int ats = atf;// amount to shift
				printf("[*]\t\tBit mask shift amount : %d\n", ats);
				int em = ~((~(0)>>ats)<<ats);// extraction mask
				printf("[*]\t\tExtraction Mask : 0x%x (%s)\n", em, this->dbg_getBin(em, ats, 0, 0).c_str());
				int ev = d & em; // extracted value
				printf("[*]\t\tExtracted Value : %d (%s)\n", ev, this->dbg_getBin(ev, ats, 0, 0).c_str());
				ats = (rb-atf); // amt to shift into ret
				printf("[*]\t\tExtracted Result Left Shift amount : %d\n", ats);
				ret += ev << ats;
				printf("[*]\t\tCalculated Ret : %d (%s)\n", ret, this->dbg_getBin(ret, numOfBitsToFetch, 0, 0).c_str());
				rb -= atf;
				index[0]++;
				startBit[0] = (startBit[0]+atf) % 8;
			}
			printf("[*]\tReturning : %d (%s)\n", ret, this->dbg_getBin(ret, numOfBitsToFetch, 0, 0).c_str());
			return ret;
		}

		bool unpackBody(char *data, size_t dataSize, int indexOffset, int bitOffset, int endPadding){
			// Calculate the output buffer size using code table.
			this->destroyBody();
			int body_i=0;
			this->body_s = dataSize - indexOffset;
			if(this->body_s <= 0){
				this->setError(534, "unpackBody() - out_s is out of bounds.");
				return false;
			}
			this->body = new char[this->body_s];
			printf("[*] data size : %ld\n", dataSize);
			printf("[*] header size : %d\n", indexOffset);
			printf("[*] Calculated body size : %ld\n", this->body_s);
			indexOffset--;
			// Determine smallest bit count in code table, and it's value.
			int smallestCount = this->codeTable[0];
			// determine largest bit count in code table, and it's value.
			int largestCount = this->codeTable[this->frequencies_s-1];
			int expectedContainerSize = largestCount <= 8 ? 1 : ( largestCount % 8 ) == 0 ? (largestCount/8) : (largestCount/8) + 1;
			int largestMask =  ~((~(0)>>largestCount) << largestCount);
			printf("[*] Largest Bit Count is %d\n", largestCount);
			printf("[*] Largest mask : 0x%x\n", largestMask);
			printf("[*] expected container size : %d\n", expectedContainerSize);
			printf("[*] first 4 packed bytes : %d %d %d %d\n", (int)data[indexOffset+0]&0xff, (int)data[indexOffset+1]&0xff, (int)data[indexOffset+2]&0xff, (int)data[indexOffset+3]&0xff);
			// Shift largest bit count of msb bits out of data and into a buffer variable.
			int calcRegister = this->getPackedBits(data, dataSize, &indexOffset, &bitOffset, largestCount, expectedContainerSize);
			int calcBitCount = largestCount;
			int calcMask = largestMask;

			// allocate the output Buffer 
			this->destroyOut();
			this->out_s = 0;
			/* The loop table i, from biggest to smallest. */
			for(int i=0; i<this->frequencies_s; i++){
				this->out_s += this->frequencies[i];
			}
			this->out = new char[this->out_s];
			printf("-----------DEBUGGING BODY UNPACK\n[*] Output buffer size : %ld\n", this->out_s);
			int tbleOff = this->frequencies_s;
			for(int i=0, tf=-1; i<this->out_s; i++){
				if(calcBitCount < smallestCount){
					this->setError(45345, "unpackBody() - calculated bits is less than smallest allowed.");
					return false;
				}
				/*
				 * Start Unpack decode algorithm.
				 * */
				printf("------------\n[%d] Starting Value : %d (%s)\n", i, calcRegister, this->dbg_getBin(calcRegister, calcBitCount, 0, 0).c_str());
				printf("[*]\tProcessing Encoded Bytes.\n");
				for(int f=0; f<this->frequencies_s; f++){
					
				}

			}
			
			return true;
		}

		bool encode(char *data, size_t dataSize){
			this->destroyOut();

			int headerPadding = this->packHeader();
			if(headerPadding <= -1){
				this->setError(1201, "encode() - failed to pack header.");
				return false;
			}

			int bodyPadding = this->packBody(headerPadding, data, dataSize);
			if(bodyPadding <= -1){
				this->setError(4324, "encode() - failed to pack body.");
				return false;
			}
			
			this->out_s = this->header_s + this->body_s;
			this->out = new char[this->out_s];
			printf("Packing bodyPadding %d\n", bodyPadding);
			// Join padding with header
			for(int i=0; i<this->header_s; i++){
				this->out[i] = i == 0 ? ((bodyPadding&0xf)<<4) + this->header[i] : this->header[i];
			}

			// Bind header tail to body head
			if(headerPadding != 0)
				this->out[this->header_s-1] += this->body[0]; 
			else
				this->out[this->header_s] = this->body[0];

			// fetch rest of body.
			for(int i=1, o=this->header_s; i<this->body_s && o<this->out_s; i++, o++){
				this->out[o] = this->body[i];
			}
			

			printf("\nHeader Size : %ld | Body Size : %ld\n", this->header_s, this->body_s);
			printf("Body Debug, first 4 bytes : %d %d %d %d\n", (int)body[0]&0xff, (int)body[1]&0xff, (int)body[2]&0xff, (int)body[3]&0xff);
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
						sum = tracer + this->treeData[i];
						if(t==targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = tracerIdx;
							return true;
						}
						t--;
						break;
					}
					sum = tracer + this->treeData[i];
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
			this->header = NULL;
                        this->header_s = 0;
                        this->body = NULL;
                        this->body_s = 0;

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
			this->destroyBody();
			this->destroyHeader();
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

		bool printCodeTable(void){
			printf("\nCode Table : \nduplicate\tindex(translated)\tbit count\tcode\tchar\tfrequency\n");
			if(this->frequencies == NULL || this->frequencies_s <= 0 || this->codeTable == NULL || this->codeTable_s <= 0 || this->treeLetters == NULL || this->treeLetters_s <=0){
                                printf("NULL\n");
                                return false;
                        }
			bool ret = true;
			for(int i=0; i<this->frequencies_s; i++){
				int entryCount = this->codeTable[i];
				std::string entryString = this->getCodeBinary(i);
				char entryLetter = this->treeLetters[i];
				int entryFrequency = this->frequencies[i];
				std::string duplicate = "\033[0;32m  valid\033[0m";
				for(int j=0; j<this->frequencies_s; j++){
					if(j==i) continue;
					if(entryCount == this->codeTable[j] && entryString == this->getCodeBinary(j)){
						duplicate = "\033[0;31minvalid\033[0m";
						ret = false;
					}
				}
				printf("%s - %d(%ld)\t%d\t%s\t%d\t%d\n", duplicate.c_str(), i, i+(this->treeData_s-this->frequencies_s), entryCount, entryString.c_str(), (int)entryLetter&0xff, entryFrequency);
			}printf("\n");
			return ret;
		}

		void printTreeOrigins(void){
			printf("\n\tTree Origins\n");
			if(!this->validateTreeData()){
				printf("NULL\n");
				return;
			}
			for(int t=this->treeDataLayerCount-1, i=0; t>=0; t--){
				printf("\n\tLayer %d\n", t);
				for(int j=0; j<this->treeLayerSizes[t]; j++){
					int zero=-1, one=-1;
					std::string topNode = this->treeDataTypes[i] == 1 ? "\033[30;43mtop node\033[0m" : "\033[30;47mbottom node\033[0m";
					if(this->getSubIndecies(i, &zero, &one)){
						int tz = zero == -1 ? -1 : this->treeData[zero];
						int to = one == -1 ? -1 : this->treeData[one];
						printf("%s \033[0;33mTree Idx:[%d]%d\t\033[0;35mZero:[%d]%d\t\033[0;36mOne:[%d]%d\033[0m\t%s\n", (this->treeData[i] == tz+to || (tz == -1 || to == -1)) ? "\033[0;32mvalid\033[0m" : "\033[0;31minvalid\033[0m", i, this->treeData[i], zero, tz, one, to, topNode.c_str());
					}else{
						int tz = zero == -1 ? -1 : this->treeData[zero];
						int to = one == -1 ? -1 : this->treeData[one];
						printf("%s \033[0;33mTree Idx:[%d]%d\t\033[0;35mZero:[%d]%d\t\033[0;36mOne:[%d]%d\033[0m\t%s\n", (this->treeData[i] == tz+to || (tz == -1 || to == -1)) ? "\033[0;32mvalid\033[0m" : "\033[0;31minvalid\033[0m", i, this->treeData[i], zero, tz, one, to, topNode.c_str());
					}
					i++;
				}
			}
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
			
			if(!this->generateCodeTable()){
				this->setError(5555, "compress() - failed to generate code table.");
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
			this->clearError();
                        this->destroyCodingTable();
                        this->destroyTreeLetters();
                        this->destroyFrequencies();
                        this->destroyOut();
                        this->tablesSorted = false;
			if(data == NULL){
                                this->setError(100, "decompress(char *data, size_t dataSize) - data is null.");
                                return false;
                        }
                        if(dataSize <= 0){
                                this->setError(101, "decompress(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
                                return false;
                        }

			int indexOffset = 0;
			int bitOffset = 0;
			int endPadding = 0;
			if(!this->unpackHeader(data, dataSize, &indexOffset, &bitOffset, &endPadding)){
				this->setError(102, "decompress(char *data, size_t dataSize) - faiiled to unpack header.");
				return false;
			}


			if(!this->plantTree()){
                                this->setError(103, "decompress(char *data, size_t dataSize) - failed to plant tree.");
                                return false;
                        }

			if(!this->generateCodeTable()){
				this->setError(1304, "decompress() - failed to generate code table.");
				return false;
			}

			if(!this->unpackBody(data, dataSize, indexOffset, bitOffset, endPadding)){
				this->setError(4535, "decompress() - failed to unpack body.");
				return false;
			}
			
			if(!this->decode(data, dataSize)){
				this->setError(104, "decompress(char *data, size_t dataSize) - failed to decode the data.");
				return false;
			}
			return true;
		}
};
