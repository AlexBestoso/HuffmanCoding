/*
 * The Bestoso Free License
 * - You may use this file as you like.
 * - You may modify it and keep it For yourself.
 * - For it to be free, you shall be thankful :)
 *  - - - 
 **/

// Set these to 0 if you want to exclude them from the compile.
#define HUFFMAN_DEBUGGING 1
#define HUFFMAN_EXPERIMENTAL 1
class HuffmanCoding{
	private:
		/* QJ private vars */
		char *body;
		size_t body_s;

		size_t header_s;
		char *header;

		size_t codeTable_s;
		int *codeTable;

		size_t treeData_s;
		int *treeData;
		int *treeDataTypes;
		int treeDataLayerCount;
		int *treeLayerIndecies;
		int *treeLayerSizes;

		size_t treeLetters_s;
		char *treeLetters;

		size_t frequencies_s;
		int *frequencies;
		int frequencyMax;

		int *workQueue;
		size_t workQueue_s;

		size_t workBuffer_fill;
		size_t workBuffer_s;
		int *workBuffer;
		size_t workTypeBuffer_s;
		int *workTypeBuffer;

		int error;
		std::string error_msg;
		std::string errorCurrent;

		bool tablesSorted; // deprecated

		/* QJ init functions */
		void initBody(void){
			this->body = NULL;
			this->body_s = 0;
		}

		void initHeader(void){
			this->header_s = 0;
			this->header = NULL;
		}

		void initCodeTable(void){
			this->codeTable_s = 0;
			this->codeTable = NULL;
		}

		void initTreeData(void){
			this->treeData_s = 0;
			this->treeDataLayerCount = 0;
			this->treeData = NULL;
			this->treeDataTypes = NULL;
			this->treeLayerIndecies = NULL;
			this->treeLayerSizes = NULL;
		}
		
		void initTreeLetters(void){
			this->treeLetters_s = 0;
			this->treeLetters = NULL;
		}

		void initFrequencies(void){
			this->frequencies_s = 0;
			this->frequencies = NULL;
			this->frequencyMax = 0;
		}

		void initWorkQueue(void){
			this->workQueue_s = 0;
			this->workQueue = NULL;
		}

		void initWorkBuffers(void){
			this->workBuffer_fill = 0;
			this->workBuffer_s = 0;
			this->workBuffer = NULL;
			this->workTypeBuffer_s = 0;
			this->workTypeBuffer = NULL;
		}

		void initError(void){
			this->error = -1;
			this->error_msg = "";
		}

		void initOut(void){
			this->out = NULL;
			this->out_s = 0;
		}

		/* QJ destroy functions */
		void destroyBody(void){
			if(this->body != NULL)
				delete[] this->body;
			this->body_s = 0;
		}

		void destroyHeader(void){
			if(this->header != NULL)
				delete[] this->header;
			this->header_s = 0;
		}

		void destroyCodingTable(void){
			if(this->codeTable != NULL)
				delete[] this->codeTable;
			this->codeTable = NULL;
			this->codeTable_s = 0;
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

		void destroyTreeLayers(void){
			if(this->treeLayerIndecies != NULL)
				delete[] this->treeLayerIndecies;
			if(this->treeLayerSizes != NULL)
				delete[] this->treeLayerSizes;
			this->treeLayerIndecies = NULL;
			this->treeLayerSizes = NULL;
			this->treeDataLayerCount = 0;
		}

		void destroyTreeLetters(void){
			if(this->treeLetters != NULL)
				delete[] this->treeLetters;
			this->treeLetters = NULL;
			this->treeLetters_s = 0;
		}

		void destroyFrequencies(void){
			if(this->frequencies != NULL)
				delete[] this->frequencies;
			this->frequencies = NULL;
			this->frequencies_s = 0;
		}

		void destroyWorkQueue(void){
			if(this->workQueue != NULL)
				delete[] this->workQueue;
			this->workQueue = NULL;
			this->workQueue_s = 0;
		}

		void destroyWorkBuffer(void){
			if(this->workBuffer != NULL)
				delete[] this->workBuffer;
			this->workBuffer = NULL;
			this->workBuffer_s=0;
			this->workBuffer_fill=0;
		}

		void destroyWorkTypeBuffer(void){
			if(this->workTypeBuffer != NULL)
				delete[] this->workTypeBuffer;
			this->workTypeBuffer = NULL;
			this->workTypeBuffer_s = 0;
		}

		void destroyOut(void){
			if(this->out != NULL)
				delete[] this->out;
			this->out = NULL;
			this->out_s = 0;
		}

		/* QJ validate functions */
		bool validateBody(void){
			this->errorCurrent = "validateBody() - ";
			if(this->body == NULL){
				this->setError(0, this->errorCurrent + "body is null.");
				return false;
			}
			if(this->body_s <= 0){
				this->errorCurrent += "body_s:"+std::to_string(this->body_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateHeader(void){
			this->errorCurrent = "validateHeader() - ";
			if(this->header == NULL){
				this->setError(0, this->errorCurrent+"header is null.");
				return false;
			}
			if(this->header_s <= 0){
				this->errorCurrent = "header_s:"+std::to_string(this->header_s)+" is empty";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateCodeTable(void){
			this->errorCurrent = "validateCodeTable() - ";
			if(this->codeTable == NULL){
				this->setError(0, this->errorCurrent+"codeTable is null.");
				return false;
			}
			if(this->codeTable_s <= 0){
				this->errorCurrent += "codeTableSize:"+std::to_string(this->codeTable_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateTreeData(void){
			this->errorCurrent = "validateTreeData() - ";
			if(this->treeData == NULL){
				this->setError(0, this->errorCurrent + "treeData is null.");
				return false;
			}
			if(this->treeDataTypes == NULL){
				this->setError(1, this->errorCurrent + "treeDataTypes is null.");
				return false;
			}	
			if(this->treeData_s <= 0){
				this->errorCurrent += "treeData_s:"+std::to_string(this->treeData_s) + "is empty.";
				this->setError(2, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateTreeLayers(void){
			this->errorCurrent = "validateTreeLayers() - ";
			if(this->treeLayerIndecies == NULL){
				this->setError(0, this->errorCurrent + "treeLayerIndecies is null.");
				return false;
			}
			if(this->treeLayerSizes == NULL){
				this->setError(1, this->errorCurrent + "treeLayerSizes is null.");
				return false;
			}
			if(this->treeDataLayerCount <= 0){
				this->errorCurrent += "treeDataLayerCount:"+std::to_string(this->treeDataLayerCount)+" is empty";
				this->setError(2, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateTreeLetters(void){
			this->errorCurrent = "validateTreeLetters";
			if(this->treeLetters == NULL){
				this->setError(0, this->errorCurrent+ "treeLetters is null.");
				return false;
			}
			if(this->treeLetters_s <= 0){
				this->errorCurrent += "treeLetters_s:"+std::to_string(this->treeLetters_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}	

		bool validateFrequencies(void){
			this->errorCurrent = "validateFrequencies() - ";
			if(this->frequencies == NULL){
				this->setError(0, this->errorCurrent + "frequencies is null.");
				return false;
			}
			if(this->frequencies_s <= 0){
				this->errorCurrent += "frequencies_s:" + std::to_string(this->frequencies_s) + " is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateWorkQueue(void){
			this->errorCurrent = "validateWorkQueue";
			if(this->workQueue == NULL){
				this->setError(0, this->errorCurrent+"workQueue is null.");
				return false;
			}
			if(this->workQueue_s <= 0){
				this->errorCurrent += "workQueue_s:" + std::to_string(this->workQueue_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateWorkBuffer(void){
			this->errorCurrent = "validateWorkBuffer - ";
			if(this->workBuffer == NULL){
				this->setError(0, this->errorCurrent + "workBuffer is null.");
				return false;
			}
			if(this->workBuffer_s <= 0){
				this->errorCurrent += "workBuffer_s:" + std::to_string(this->workBuffer_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateWorkTypeBuffer(void){
			this->errorCurrent = "validateWorkTypeBuffer() - ";
			if(this->workTypeBuffer == NULL){
				this->setError(0, this->errorCurrent+"workTypeBuffer is null.");
				return false;
			}
			if(this->workTypeBuffer_s <= 0){
				this->errorCurrent += "workTypeBuffer_s:" + std::to_string(this->workTypeBuffer_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}

		bool validateOut(void){
			this->errorCurrent = "validateOut() - ";
			if(this->out == NULL){
				this->setError(0, this->errorCurrent + "out is null.");
				return false;
			}
			if(this->out_s <= 0){
				this->errorCurrent += "out_s:" + std::to_string(this->out_s)+" is empty.";
				this->setError(1, this->errorCurrent);
				return false;
			}
			return true;
		}
			
		/* QJ resize functions */
		bool resizeTreeLetters(size_t size){
			this->errorCurrent = "resizeTreeLetters() - ";
			if(size < 0){
				this->setError(0, this->errorCurrent+"size:"+std::to_string(size)+" is negative.");
				return false;
			}else if(size == 0){
				this->destroyTreeLetters();
			}else if(this->treeLetters == NULL){
				this->treeLetters = new (std::nothrow) char[size];
				if(!this->treeLetters){
					this->setError(1, this->errorCurrent+"failed to allocate treeLetters.");
					return false;
				}
				this->treeLetters_s = size;
			}else{
				size_t oldSize = this->treeLetters_s;
				char *transfer = new (std::nothrow) char[oldSize];
				if(!transfer){
					this->setError(2, this->errorCurrent+"failed to allocate transfer.");
					return false;
				}

				for(int i=0; i<oldSize; i++)
					transfer[i] = this->treeLetters[i];
				this->destroyTreeLetters();

				this->treeLetters = new (std::nothrow) char[size];
				if(!this->treeLetters){
					this->setError(3, this->errorCurrent+"failed to allocate treeLetters.");
					return false;
				}

				this->treeLetters_s = size;
				for(int i=0; i<this->treeLetters_s; i++){
					if(i<oldSize)
						this->treeLetters[i] = transfer[i];
					else
						this->treeLetters[i] = 0;
				}

				delete[] transfer;
			}
			return true;
		}

		bool resizeFrequencies(size_t size){
			this->errorCurrent = "resizeFrequencies() - ";
			if(size == 0){
				this->destroyFrequencies();
			}else if(this->frequencies == NULL){
				this->frequencies = new (std::nothrow) int[size];
				if(!this->frequencies){
					this->setError(0, this->errorCurrent+"failed to allocate frequencies.");
					return false;
				}
				this->frequencies_s = size;
			}else{
				size_t oldSize = this->frequencies_s;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(1, this->errorCurrent+"failed to allocate transfer.");
					return false;
				}
				for(int i=0; i<oldSize; i++)
					transfer[i] = this->frequencies[i];
				this->destroyFrequencies();
				this->frequencies = new (std::nothrow) int[size];
				if(!this->frequencies){
					this->setError(2, this->errorCurrent+"failed to allocate frequencies.");
					return false;
				}
				this->frequencies_s = size;
				for(int i=0; i<this->frequencies_s; i++){
					if(i<oldSize)
						this->frequencies[i] = transfer[i];
					else
						this->frequencies[i] = 0;
				}
				delete[] transfer;
			}
			return true;
		}

		bool resizeWorkBuffer(size_t size){
			this->errorCurrent = "resizeWorkBuffer() - ";
			if(size == 0){
				this->destroyWorkBuffer();
			}else if(this->workBuffer == NULL || this->workBuffer_s <= 0){
				this->destroyWorkBuffer();
				this->workBuffer = new (std::nothrow) int[size];
				if(!this->workBuffer){
					this->setError(0, this->errorCurrent+"failed to allocate workBuffer");
					return false;
				}
				this->workBuffer_s = size;
			}else{
				size_t oldSize = this->workBuffer_s;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(1, this->errorCurrent+"failed to allocate transfer buffer");
					return false;
				}
				for(int i=0; i<oldSize; i++){
					transfer[i] = this->workBuffer[i];
				}
				this->destroyWorkBuffer();
				this->workBuffer = new (std::nothrow) int[size];
				if(!this->workBuffer){
					this->setError(2, this->errorCurrent+"failed to allocate workBuffer.");
					return false;
				}
				this->workBuffer_s = size;
				for(int i=0; i<this->workBuffer_s; i++){
					if(i<oldSize)
						this->workBuffer[i] = transfer[i];
					else
						this->workBuffer[i] = 0;
				}
				delete[] transfer;
			}
			return true;
		}

		bool resizeWorkTypeBuffer(size_t size){
			this->errorCurrent = "resizeWorkTypeBuffer() - ";
			if(size == 0){
				this->destroyWorkTypeBuffer();
			}else if(this->workTypeBuffer == NULL || this->workTypeBuffer_s <= 0){
				this->destroyWorkTypeBuffer();
				this->workTypeBuffer = new (std::nothrow) int[size];
				if(!this->workTypeBuffer){
					this->setError(0, this->errorCurrent+"failed to allocate workTypeBuffer.");
					return false;
				}
				this->workTypeBuffer_s = size;
			}else{
				size_t oldSize = this->workTypeBuffer_s;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(1, this->errorCurrent+"failed to allocate transfer.");
					return false;
				}
				for(int i=0; i<oldSize; i++){
					transfer[i] = this->workTypeBuffer[i];
				}
				this->destroyWorkTypeBuffer();
				this->workTypeBuffer = new (std::nothrow) int[size];
				if(!this->workTypeBuffer){
					this->setError(2, this->errorCurrent+"failed to allocate workTypeBuffer.");
					return false;
				}
				this->workTypeBuffer_s = size;
				for(int i=0; i<this->workTypeBuffer_s; i++){
					if(i<oldSize)
						this->workTypeBuffer[i] = transfer[i];
					else
						this->workTypeBuffer[i] = 0;
				}

				delete[] transfer;
			}
			return true;
		}
		
		bool resizeTreeData(size_t size){
			this->errorCurrent = "resizeTreeData() - ";
			if(size == 0){
				this->destroyTreeData();
			}else if(this->treeData == NULL){
				this->treeData = new (std::nothrow) int[size];
				if(!this->treeData){
					this->setError(0, this->errorCurrent+"treeData failed to allocate.");
					return false;
				}
				this->treeDataTypes = new (std::nothrow) int[size];
				if(!this->treeDataTypes){
					this->setError(1, this->errorCurrent+"treeDataTypes failed to allocate.");
					return false;
				}
				this->treeData_s = size;
			}else{
				size_t oldSize = this->treeData_s;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(2, this->errorCurrent+"failed to allocate transfer.");
					return false;
				}
				int *transferTwo = new (std::nothrow) int[oldSize];
				if(!transferTwo){
					this->setError(3, this->errorCurrent+"failed to allocate transferTwo.");
					return false;
				}
				for(int i=0; i<oldSize; i++){
					transfer[i] = this->treeData[i];
					transferTwo[i] = this->treeDataTypes[i];
				}
				this->destroyTreeData();
				this->treeData = new (std::nothrow) int[size];
				if(!this->treeData){
					this->setError(4, this->errorCurrent+"failed to allocate treeData.");
					return false;
				}
				this->treeDataTypes = new (std::nothrow) int[size];
				if(!this->treeDataTypes){
					this->setError(5, this->errorCurrent+"failed to allocate treeDataTypes.");
					return false;
				}
				this->treeData_s = size;
				for(int i=0; i<this->treeData_s; i++){
					if(i<oldSize){
						this->treeData[i] = transfer[i];
						this->treeDataTypes[i] = transferTwo[i];
					}else{
						this->treeData[i] = 0;
						this->treeDataTypes[i] = 0;
					}
				}

				delete[] transfer;
				delete[] transferTwo;
			}
			return true;
		}

		bool resizeTreeLayers(size_t size){
			this->errorCurrent = "resizeTreeLayers() - ";
			if(size == 0){
				this->destroyTreeLayers();
			}else if(this->treeLayerIndecies == NULL || this->treeLayerSizes == NULL){
				this->destroyTreeLayers();
				this->treeLayerIndecies = new (std::nothrow) int[size];
				if(!this->treeLayerIndecies){
					this->setError(0, this->errorCurrent+"failed to allocate treeLayerIndecies.");
					return false;
				}
				this->treeLayerSizes = new (std::nothrow) int[size];
				if(!this->treeLayerSizes){
					this->setError(1, this->errorCurrent+"failed to allocate treeLayerSizes.");
					return false;
				}
				this->treeDataLayerCount = size;
			}else{
				size_t oldSize = this->treeDataLayerCount;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(2, this->errorCurrent+"failed to allocate transfer.");
					return false;
				}
				int *transferTwo = new (std::nothrow) int[oldSize];
				if(!transferTwo){
					this->setError(3, this->errorCurrent+"failed to allocate transferTwo.");
					return false;
				}
				for(int i=0; i<oldSize; i++){
					transfer[i] = this->treeLayerIndecies[i];
					transferTwo[i] = this->treeLayerSizes[i];
				}
				this->destroyTreeLayers();
				this->treeLayerIndecies = new (std::nothrow) int[size];
				if(!this->treeLayerIndecies){
					this->setError(4, this->errorCurrent+"failed to allocate treeLayerIndecies.");
					return false;
				}
				this->treeLayerSizes = new (std::nothrow) int[size];
				if(!this->treeLayerSizes){
					this->setError(5, this->errorCurrent+"failed to allocate treeLayerSizes.");
					return false;
				}
				this->treeDataLayerCount = size;
				for(int i=0; i<this->treeDataLayerCount; i++){
					if(i<oldSize){
						this->treeLayerIndecies[i] = transfer[i];
						this->treeLayerSizes[i] = transferTwo[i];
					}else{
						this->treeLayerIndecies[i] = 0;
						this->treeLayerSizes[i] = 0;
					}
				}

				delete[] transfer;
				delete[] transferTwo;
			}
			return true;
		}

		bool resizeCodeTable(size_t size){
			this->errorCurrent = "resizeCodeTable() - ";
			if(size < 0){
				this->setError(0, this->errorCurrent+"size:"+std::to_string(size)+" is negative.");
				return false;
			}else if(size == 0){
				this->destroyCodingTable();
			}else if(this->codeTable == NULL){
				this->codeTable = new (std::nothrow) int[size];
				if(!this->codeTable){
					this->setError(1, this->errorCurrent+"failed to allocate codeTable.");
					return false;
				}
				this->codeTable_s = size;
			}else{
				size_t oldSize = this->codeTable_s;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(2, this->errorCurrent+"failed to allocate transfer.");
					return false;
				}

				for(int i=0; i<oldSize; i++)
					transfer[i] = this->codeTable[i];
				this->destroyCodingTable();

				this->codeTable = new (std::nothrow) int[size];
				if(!this->codeTable){
					this->setError(3, this->errorCurrent+"failed to allocate codeTable.");
					return false;
				}

				this->codeTable_s = size;
				for(int i=0; i<this->codeTable_s; i++){
					if(i<oldSize)
						this->codeTable[i] = transfer[i];
					else
						this->codeTable[i] = 0;
				}

				delete[] transfer;
			}
			return true;
		}

		bool resizeWorkQueue(size_t size){
			this->errorCurrent = "resizeWorkQueue() - ";
			if(size == 0){
				this->destroyWorkQueue();
			}else if(this->workQueue == NULL){
				this->workQueue = new (std::nothrow) int[size];
				if(!this->workQueue){
					this->setError(0, this->errorCurrent+"failed to allocate workQueue.");
					return false;
				}
				this->workQueue_s = size;
			}else{
				size_t oldSize = this->workQueue_s;
				int *transfer = new (std::nothrow) int[oldSize];
				if(!transfer){
					this->setError(1, this->errorCurrent+"transfer failed to allocate.");
					return false;
				}
				for(int i=0; i<oldSize; i++){
					transfer[i] = this->treeData[i];
				}
				this->destroyWorkQueue();
				this->workQueue = new (std::nothrow) int[size];
				if(!this->workQueue){
					this->setError(2, this->errorCurrent+"failed to allocate workQueue");
					return false;
				}
				this->workQueue_s = size;
				for(int i=0; i<this->workQueue_s && i<oldSize; i++){
					this->treeData[i] = transfer[i];
				}

				delete[] transfer;
			}
			return true;
		}


		/* QJ misc buffer functions */
		/*
			allocates work queue, 
			pushes the provided value relative to the provided number of items in the queue.
			Returns the number of items int queue
		*/
		int pushWorkQueue(int val, int queueCount){
			this->errorCurrent = "pushWorkQueue() - ";
			if(!this->validateFrequencies()){
				this->setError(0, this->errorCurrent+"invalid frequencies.");
				return -1;

			}else if(!this->validateWorkQueue()){
				this->clearError();
				if(!this->resizeWorkQueue(this->frequencies_s)){
					this->setError(1, this->errorCurrent+"failed to resize workQueue.");
					return -1;
				}

				for(int i=0; i<this->workQueue_s; i++)
					this->workQueue[i] = -1;

			}else if(this->workQueue[this->workQueue_s-1] != -1){
				// if queue is full, expand by one element.
				if(!this->resizeWorkQueue(this->workQueue_s+1)){
					this->setError(2, this->errorCurrent+"failed to resize workQueue.");
					return -1;
				}

				this->workQueue[this->workQueue_s-1] = -1;
			}

			int start = queueCount >= 0 && queueCount < workQueue_s ? queueCount : 0;
			int newQueueCount = start;
			bool failure=true;
			for(int i=start; i<this->workQueue_s; i++){
				if(this->workQueue[i] == -1){
					failure=false;
					this->workQueue[i] = val;
					newQueueCount++;
					break;
				}
				newQueueCount++;
			}
			if(failure){
				this->setError(3, this->errorCurrent+"failed to push value.");
				return -1;
			}
			return newQueueCount;
		}

		bool calculateLayerIndecies(void){
			this->errorCurrent = "calculateLayerIndecies() - ";
			if(!this->validateTreeData()){
				this->setError(0, this->errorCurrent+"failed to valied treeData.");
				return false;
			}else if(!this->validateTreeLayers()){
				this->setError(1, this->errorCurrent+"failed to validate treeLayers");
				return false;
			}

			int dataRemainder = this->treeData_s-1;
			for(int i=0; i<this->treeDataLayerCount; i++){
				this->treeLayerIndecies[i] = dataRemainder;
				dataRemainder -= this->treeLayerSizes[i];
			}
			return true;
		}
		
		bool isBaseIndex(int target){
			this->errorCurrent = "isBaseIndex() - ";
			if(!this->validateTreeData()){
				this->setError(0, this->errorCurrent + "invalid tree data.");
				return false;
			}else if(!this->validateTreeLayers()){
				this->setError(1, this->errorCurrent + "invalid tree layers.");
				return false;
			}

			// 0 is the seeded layer, the actual index value of layer 0 is treeData_s - 1
			int baseLayerStartIndex = this->treeLayerIndecies[0];
			// index value minus the size of the layer positions us at the start of the next layer.
			int baseLayerEndIndex = baseLayerStartIndex - this->treeLayerSizes[0];

			// is in the base node range?
			if(target <= baseLayerStartIndex && target > baseLayerEndIndex){
				return true;
			}
			return false;
		}
		
		int getBaseLayerIndex(int target){
			this->errorCurrent = "getBaseLayerIndex() - ";
			if(!this->validateTreeLayers()){
				this->setError(0, this->errorCurrent+"invalid tree layer data.");
				return -1;
			}
			for(int i=0; i<this->treeDataLayerCount; i++){
				int start = this->treeLayerIndecies[i];
				int end = start-this->treeLayerSizes[i];
				if(target <= start && target > end){
					return i;
				}
			}
			this->setError(1, this->errorCurrent+"target isn't within layer range.");
			return -1;
		}

		/*
		 * This function is limited by sizeof(int)
		 * */
		bool addBitToCodeTable(int targetIndex, int bit){
			this->errorCurrent = "addBitToCodeTable() - ";
			if(!this->validateFrequencies()){
				this->setError(0, this->errorCurrent+"failed to validate frequencies.");
				return false;
			}else if(!this->validateCodeTable()){
				this->setError(1, this->errorCurrent+"failed to validate code table.");
				return false;
			}else if(targetIndex < 0 || targetIndex >= this->codeTable_s){
				this->setError(2, this->errorCurrent+"targetIndex:"+std::to_string(targetIndex)+"out of bounds, max:"+std::to_string(this->codeTable_s));
				return false;
			}
			
			// increment bit count
			this->codeTable[targetIndex]++;
			
			// left shift encoded entry by 1, and ad the lsb of bit. 
			int codeIdx = targetIndex+this->frequencies_s;
			this->codeTable[codeIdx] = (this->codeTable[codeIdx] << 1) + (bit&0x01);
			if(this->codeTable[targetIndex] > 32){
				this->setError(3, this->errorCurrent+"codeTable["+std::to_string(targetIndex)+"]:"+std::to_string(this->codeTable[targetIndex])+" bigger than the supported 32 bits.");
				return false;
			}
			return true;
		}

		bool getSubIndecies(int targetIndex, int *zeroIndex, int *oneIndex){
			this->errorCurrent = "getSubIndecies() - ";
			if(!this->validateTreeData()){
				this->setError(0, this->errorCurrent+"invalid tree data.");
				return false;
			}else if(!this->validateFrequencies()){
				this->setError(1, this->errorCurrent+"invalid frequencies.");
				return false;
			}else if(!this->validateTreeLayers()){
				this->setError(2, this->errorCurrent+"invalid tree layers.");
				return false;
			}else if(targetIndex < 0 || targetIndex >= this->treeData_s){
				this->errorCurrent += "targetIndex:"+std::to_string(targetIndex)+" is out of bounds, treeData_s:"+std::to_string(this->treeData_s);
				this->setError(3, this->errorCurrent);
				return false;
			}

			// Determine which layer our targetIndex is a part of.
			int targetLayer = this->getBaseLayerIndex(targetIndex);
			if(this->failed()){
				this->setError(4, this->errorCurrent+"failed to get targetLayer.");
				return false;
			}else if(targetLayer >= this->treeDataLayerCount || targetLayer < 0){
				this->setError(5, this->errorCurrent+"targetLayer:"+std::to_string(targetLayer)+"is out of bounds, treeDataLayerCount:"+std::to_string(this->treeDataLayerCount));
				return false;
			}

			// ensures that odd-sized buffers are handled.
			if(targetLayer == 0){
				zeroIndex[0] = targetIndex;
				oneIndex[0] = -1;
				return true;
			}


			// identify the start and end of the target layer.
			int targetLayerStart = this->treeLayerIndecies[targetLayer];
			if(targetLayerStart < 0 || targetLayerStart >= this->treeData_s){
				this->setError(6, this->errorCurrent+"targetLayerStart:"+std::to_string(targetLayerStart)+" is out of bounds, treeData_s:"+std::to_string(this->treeData_s));
				return false;
			}
			int targetLayerEnd = targetLayerStart - this->treeLayerSizes[targetLayer];
			if(!(targetLayerEnd >= -1) || targetLayerEnd >= (int)this->treeData_s){
				this->setError(7, this->errorCurrent+"targetLayerEnd:"+std::to_string(targetLayerEnd)+"is out of bounds, treeData_s:"+std::to_string(this->treeData_s));
				return  false;
			}
			
			// Identify the source layer, and its start and end.
			int sourceLayer = targetLayer - 1;
			if(sourceLayer < 0 || sourceLayer >= this->treeDataLayerCount){
				this->setError(8, this->errorCurrent+"sourceLayer:"+std::to_string(sourceLayer)+"is out of bounds, treeDataLayerCount:"+std::to_string(this->treeDataLayerCount));
				return false;
			}
			int sourceLayerStart = this->treeLayerIndecies[sourceLayer];
			if(sourceLayerStart < 0 || sourceLayerStart >= (int)this->treeData_s){
				this->setError(9, this->errorCurrent+"sourceLayerStart:"+std::to_string(sourceLayerStart)+"is out of bounds, treeData:"+std::to_string(this->treeData_s));
				return false;
			}
			int sourceLayerEnd = sourceLayerStart - this->treeLayerSizes[sourceLayer];
			if(sourceLayerEnd < 0 || sourceLayerStart > (int)this->treeData_s){
				this->setError(10, this->errorCurrent+"sourceLayerEnd:"+std::to_string(sourceLayerEnd)+"is out of bounds, treeData_s:"+std::to_string(this->treeData_s));
				return false;
			}

			// source tree layer size should always be > because of how the huffman tree is structured.
			if(this->treeLayerSizes[sourceLayer] <= this->treeLayerSizes[targetLayer]){
				this->errorCurrent += "Layers out of order. ";
				this->errorCurrent += "src treeLayerSizes["+std::to_string(sourceLayer)+"]:";
				this->errorCurrent += std::to_string(this->treeLayerSizes[sourceLayer])+" <= target treeLayerSizes[";
				this->errorCurrent += std::to_string(targetLayer)+"]:"+std::to_string(this->treeLayerSizes[targetLayer]);
				this->setError(11, this->errorCurrent);
				return false;
			}
			
			// Extract the source indecies.
			int tracerIdx = 0;
			int tracer = -1;
			int sum = -1;
			for(int i=sourceLayerStart, t=targetLayerStart; i>sourceLayerEnd && t>targetLayerEnd; i--){
				// Ensure we're not making calculations using bottom nodes.
				if(this->treeDataTypes[i] == 0)
					continue;
				bool finalLoop = i == sourceLayerEnd + 1;
				// Handle edge case for final loop/interation
				if(finalLoop && tracer == -1){
						if(sum == -1){
							this->setError(12, this->errorCurrent+"final iteration impossible context.");
							return false;
						}
						if(t == targetIndex){
							zeroIndex[0] = i;
							oneIndex[0] = t+1;
							return true;
						}
						t--;
						break;
				}else if(finalLoop && this->treeData[i] > sum && sum != -1){
					if(t == targetIndex){
						zeroIndex[0] = tracerIdx;
						oneIndex[0] = t+1;
						return true;
					}

					t--;
					if(!(t>targetLayerEnd)){
						this->setError(13, this->errorCurrent+"miss-aligned tree.");
						return false;
					}

					if(t == targetIndex){
						zeroIndex[0] = i;
						oneIndex[0] = t+1;
						return true;
					}

					t--;
					break;
				}

				// Ensure that we have a tracer
				if(tracer == -1){
					tracer = this->treeData[i];
					tracerIdx=i;
					continue;
				}

				// Determine source indecies and cycle.
				if(this->treeData[i] <= sum || sum == -1){
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
				}else if(this->treeData[i] > sum){
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
			this->errorCurrent += "Failed to find sub index for targetIndex:";
			this->errorCurrent += std::to_string(targetIndex)+", sourceLayer:";
			this->errorCurrent += std::to_string(sourceLayer)+", targetLayer:"+std::to_string(targetLayer);
			this->setError(14, this->errorCurrent);
			return false;
		}

		int popWorkQueue(void){
			this->errorCurrent = "popWorkQueue() - ";
			if(!this->validateWorkQueue()){
				this->setError(0, "invalid work queue.");
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

		/* QJ tree functions */
		// seed function expects value and type buffers to be equal to frequencies_s
		bool seedLayers(void){
			this->errorCurrent = "seedLayers() - ";
			if(!this->validateFrequencies()){
				this->setError(0, this->errorCurrent+"failed to validate frequenncies.");
				return false;
			}else if(this->treeData != NULL){
				this->setError(1, this->errorCurrent+"treeData already seeded.");
				return false;
			}

			if(!this->resizeTreeData(this->frequencies_s)){
				this->setError(2, this->errorCurrent+"failed to resize treeData.");
				return false;
			}
			for(int i=0; i<this->treeData_s; i++){
				this->treeData[i] = this->frequencies[i];
				this->treeDataTypes[i] = 1; // 1 states that index i is a "top node"
			}

			if(!this->resizeTreeLayers(1)){
				this->setError(3, this->errorCurrent+"failed to resize tree layers.");
				return false;
			}
			this->treeLayerSizes[0] = this->frequencies_s;

			return true;
		}

		
		/* //
			returns true when more layers can be grown, false when done growing, and false with the error set if there's an error.
		// */
		bool growLayer(int *valueBuffer, size_t valueBuffer_s, int *typeBuffer, size_t typeBuffer_s){
			this->errorCurrent = "growLayer() - ";
			if(valueBuffer == NULL){
				this->setError(0, this->errorCurrent+"valueBuffer is null.");
				return false;
			}else if(valueBuffer_s <= 0){
				this->setError(1, this->errorCurrent+"valueBuffer_s:"+std::to_string(valueBuffer_s)+" is empty.");
				return false;
			}else if(typeBuffer == NULL){
				this->setError(2, this->errorCurrent+"typeBuffer is null.");
				return false;
			}else if(typeBuffer_s <= 0){
				this->setError(3, this->errorCurrent+"typeBuffer_s:"+std::to_string(typeBuffer_s)+" is empty.");
				return false;
			}else if(!this->validateFrequencies()){
				this->setError(4, this->errorCurrent+"invalid frequencies.");
				return false;
			}

			// Check if buffers need to be seeded.
			if(this->treeData == NULL || this->treeData_s <= 0){
				if(!this->seedLayers()){
					this->setError(5, this->errorCurrent+"failed to seed layers.");
					return false;
				}
				return true;
			}else if(!this->calculateLayerIndecies() || this->treeDataLayerCount <= 0){
				this->setError(6, this->errorCurrent+"failed to calculate Layer indecies.");
				return false;
			}

			// top layer information
			int topLayerStart = this->treeLayerIndecies[this->treeDataLayerCount - 1];
			size_t topLayerSize = this->treeLayerSizes[this->treeDataLayerCount - 1];
			if(topLayerSize <= 0){
				this->setError(7, this->errorCurrent+"invalid top size");
				return false;
			}else if(topLayerStart < 0 || topLayerStart >= this->treeData_s){
				this->setError(8, this->errorCurrent+"topLayerStart:"+std::to_string(topLayerStart)+" is out of bounds, treeData_s:"+std::to_string(this->treeData_s));
				return false;
			}else if(this->treeDataTypes[0] == 0){
				this->setError(9, this->errorCurrent+"invalid seed data, treeDataTypes[0] == 0 is a bad bottom node.");
				return false;
			}

			// finalization checks
			if(topLayerSize == 1){
				// one byte in top layer, no more data to process
				return false;
			}else if(topLayerSize == 2 && this->treeDataTypes[0] == 1 && this->treeDataTypes[1] == 0){
				// top layer is 2 bytes, and the types of each bytes means no m ore data to process
				return false;
			}else if(this->treeData[0] == this->frequencyMax){
				// the top most tree node is max freq, no more data to process.
				return false;
			}

			// Ready to grow the layer.
			this->workBuffer_fill=0;
			for(int i=topLayerStart, tracer=-1, sum=-1; i>=0 && i<this->treeData_s; i--){
				if(this->workBuffer_fill < 0 || this->workBuffer_fill >= valueBuffer_s || this->workBuffer_fill >= typeBuffer_s){
					this->errorCurrent += "workBuffer_fill:"+std::to_string(this->workBuffer_fill)+" is out of bounds, ";
					this->errorCurrent += "valueBuffer_s:"+std::to_string(valueBuffer_s)+" or ";
					this->errorCurrent += "typeBuffer_s:"+std::to_string(typeBuffer_s);
					this->setError(10, this->errorCurrent);
					return false;
				}
				if(this->frequencyMax <= this->treeData[i]) break;
				if(this->treeDataTypes[i] == 0){ // it's a bottom node, don't use it.
					continue;
				}
				if(tracer == -1){
					if(i == 0){
						if(sum == -1){
							this->setError(11, this->errorCurrent+"Impossible condition!");
							return false;
						}
						sum = this->treeData[i] + sum;
						valueBuffer[this->workBuffer_fill] = sum;
						typeBuffer[this->workBuffer_fill] = 1;
						int widx = this->workBuffer_fill-1;
						if(widx >= 0 && widx < typeBuffer_s)
							typeBuffer[widx] = 0;
						this->workBuffer_fill++;
						break;
					}
					tracer = this->treeData[i];
					continue;
				}
				if(sum == -1){
					sum = this->treeData[i] + tracer;
					valueBuffer[this->workBuffer_fill] = sum;
					typeBuffer[this->workBuffer_fill] = 1;
					int widx=this->workBuffer_fill-1;
					if(widx >= 0 && widx < typeBuffer_s)
						typeBuffer[widx] = 1;
					this->workBuffer_fill++;
					tracer = -1;
					continue;
				}
				if(this->treeData[i] == sum){
					if(i==0){
						sum = tracer + this->treeData[i];
						valueBuffer[this->workBuffer_fill] = sum;
						typeBuffer[this->workBuffer_fill] = 1;
						int widx = this->workBuffer_fill-1;
						if(widx >= 0 && widx < typeBuffer_s)
							typeBuffer[widx] = 1;
						this->workBuffer_fill++;
						break;
					}
					sum = tracer + this->treeData[i];
					valueBuffer[this->workBuffer_fill] = sum;
					typeBuffer[this->workBuffer_fill] = 1;
					int widx = this->workBuffer_fill-1;
					if(widx >= 0 && widx < typeBuffer_s)
						typeBuffer[widx] = 1;

					this->workBuffer_fill++;
					tracer = -1;
					continue;
				}
				if(this->treeData[i] < sum){
					sum = this->treeData[i] + tracer;
					valueBuffer[this->workBuffer_fill] = sum;
					typeBuffer[this->workBuffer_fill] = 1;
					int widx = this->workBuffer_fill-1;
					if(widx >= 0 && widx < typeBuffer_s)
						typeBuffer[widx] = 1;
					this->workBuffer_fill++;
					tracer = -1;
				}
				if(this->treeData[i] > sum){
					if(i==0){
						sum = tracer + sum;
						valueBuffer[this->workBuffer_fill] = sum;
						typeBuffer[this->workBuffer_fill] = 0;
						int widx = this->workBuffer_fill-1;
						if(widx >= 0 && widx < typeBuffer_s)
							typeBuffer[widx] = 0;
						this->workBuffer_fill++;

						sum = this->treeData[i] + sum;
						valueBuffer[this->workBuffer_fill] = sum;
						typeBuffer[this->workBuffer_fill] = 1;

						this->workBuffer_fill++;
						break;
					}
					sum = tracer + sum;
					valueBuffer[this->workBuffer_fill] = sum;
					typeBuffer[this->workBuffer_fill] = 0;
					int widx = this->workBuffer_fill - 1;
					if(widx >= 0 && widx < typeBuffer_s)
						typeBuffer[widx] = 0;

					this->workBuffer_fill++;
					tracer = this->treeData[i];
				}
			}

			if(this->workBuffer_fill <= 0){
				// loop didn't happen, no more data to process
				return false;
			}else if(this->workBuffer_fill - 1 < 0 || this->workBuffer_fill >= typeBuffer_s){
				this->errorCurrent += "workBuffer_fill:"+std::to_string(this->workBuffer_fill)+" is out of bounds, ";
				this->errorCurrent += "typeBuffer_s:"+std::to_string(typeBuffer_s)+" unless/or ";
				this->errorCurrent += "workBuffer_fill-1 < 0";
				this->setError(12, this->errorCurrent);
				return false;
			}
			// the last layer value to be grown is a top node.
			typeBuffer[this->workBuffer_fill-1] = 1;

			// Append workbuffer fill treeLayerSizes, layerCount validated not to be <= 0 earlier
			if(!this->resizeTreeLayers(this->treeDataLayerCount + 1)){
				this->setError(13, this->errorCurrent+"failed to resize treeLayers");
				return false;
			}
			this->treeLayerSizes[this->treeDataLayerCount - 1] = this->workBuffer_fill;
					
			size_t originalSize = this->treeData_s;
			if(!this->resizeTreeData(originalSize + this->workBuffer_fill)){
				this->setError(14, this->errorCurrent+"failed to resize treeData.");
				return false;
			}
			
			// Make room for new data via right shift of array positions by workBuffer_fill positions.
			// Loop I until 0; but break out ouf the loop after originalSize iterations.
			for(int i=this->treeData_s-1, ogSizeTrack=0;  ogSizeTrack<originalSize && i>=0; i--, ogSizeTrack++){
				int offsetIdx = i - this->workBuffer_fill;
				if(offsetIdx < 0 || offsetIdx >= this->treeData_s){
					this->errorCurrent += "offsetIdx:"+std::to_string(offsetIdx)+" is out of bounds, ";
					this->errorCurrent += "treeData_s:"+std::to_string(this->treeData_s);
					this->setError(15, this->errorCurrent);
					return false;
				}
				this->treeData[i] = this->treeData[offsetIdx];
				this->treeDataTypes[i] = this->treeDataTypes[offsetIdx];
			}

			// populate front of array with new data
			for(int i=0; i<this->workBuffer_fill && i<this->treeData_s; i++){
				int workIdx = this->workBuffer_fill-1-i;
				if(workIdx < 0 || workIdx >= valueBuffer_s){
					this->errorCurrent += "workIdx:"+std::to_string(workIdx)+" is out of bounds, ";
					this->errorCurrent += "valueBuffer_s:"+std::to_string(valueBuffer_s);
					this->setError(16, this->errorCurrent);
					return false;
				}
				if(workIdx < 0 || workIdx >= typeBuffer_s){
					this->errorCurrent += "workIdx:"+std::to_string(workIdx)+" is out of bounds, ";
					this->errorCurrent += "typeBuffer_s:"+std::to_string(typeBuffer_s);
					this->setError(17, this->errorCurrent);
					return false;
				}
				this->treeData[i] = valueBuffer[workIdx];
				this->treeDataTypes[i] = typeBuffer[workIdx];
			}

			return true;
		}

		bool plantTree(void){
			this->errorCurrent = "plantTree() - ";
			if(!this->validateFrequencies()){
				this->setError(0, this->errorCurrent+"invalid frequencies.");
				return false;
			}
			this->destroyTreeData();
			this->destroyWorkTypeBuffer();
			this->destroyWorkBuffer();
			if(!this->resizeWorkBuffer(this->frequencies_s)){
				this->setError(1, this->errorCurrent+"failed to resize work buffer.");
				return false;
			}else if(!this->resizeWorkTypeBuffer(this->workBuffer_s)){
				this->setError(2, this->errorCurrent+"failed to resize work Type buffer.");
				this->destroyWorkBuffer();
				return false;
			}

			// seed the tree, and begin coding table.
			int startSize = this->treeData_s;
			this->treeDataLayerCount=0;
			int timeoutError = 0;
			while(this->growLayer(this->workBuffer, this->workBuffer_s, this->workTypeBuffer, this->workTypeBuffer_s)){
				if(this->failed()){
					this->setError(3, this->errorCurrent+"failed to grow layer.");
					this->destroyWorkBuffer();
					this->destroyWorkTypeBuffer();
					return false;
				}
				if(timeoutError >= this->workTypeBuffer_s){
					this->setError(4, this->errorCurrent+"grow layer has timed out.");
					this->destroyWorkBuffer();
					this->destroyWorkTypeBuffer();
					return false;
				}
				timeoutError++;
			}

			this->destroyWorkBuffer();
			this->destroyWorkTypeBuffer();
			if(this->failed()){
				this->setError(5, this->errorCurrent+"failed to grow layer");
				return false;
			}

			this->calculateLayerIndecies();
			if(this->treeData[0] != this->frequencyMax){
				this->errorCurrent += "failed to grow tree, frequencyMax:"+std::to_string(this->frequencyMax)+" != ";
				this->errorCurrent += "treeData[0]:"+std::to_string(this->treeData[0]);
				this->setError(6, this->errorCurrent);
				return false;
			}
			return true;
		}

		/* QJ bit packing functions */
		int deriveChunkIndex(int maxChunkBitCount, int bitsLeft){
			if(maxChunkBitCount <= 0){
				this->setError(0, "deriveChunkIndex() - maxChunkBitCount must be > than 0.");
				return -1;
			}else if(bitsLeft <= 0){
				return -1;
			}

			int ret = bitsLeft / maxChunkBitCount;

			if((bitsLeft % maxChunkBitCount) == 0) ret--;

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
		bool packByte(int packingTarget, int targetBitCount, char *dstBuffer, size_t dstBufferSize, int *dstIndex, int *bitIndex){
			this->errorCurrent = "packByte() - ";
			int binaryMax=8; // Pack byte, so we operate relative to a max container of 8
			if(targetBitCount <= 0){
				this->setError(0, this->errorCurrent+"targetBitCount:"+std::to_string(targetBitCount)+" is empty.");
				return false;
			}else if(dstBuffer == NULL){
				this->setError(1, this->errorCurrent+"dstBuffer is null,");
				return false;
			}else if(dstBufferSize <= 0){
				this->setError(2, this->errorCurrent+"dstBufferSize:"+std::to_string(dstBufferSize)+" is empty.");
				return false;
			}else if(dstIndex == NULL){
				this->setError(3, this->errorCurrent+"dstIndex is null.");
				return false;
			}else if(dstIndex[0] >= dstBufferSize || dstIndex[0] < 0){
				this->errorCurrent += "dstIndex:"+std::to_string(dstIndex[0])+" is out of bounds, ";
				this->errorCurrent += "dstBufferSize:"+std::to_string(dstBufferSize);
				this->setError(4, this->errorCurrent);
				return false;
			}else if(bitIndex == NULL){
				this->setError(5, this->errorCurrent+"bitIndex is null.");
				return false;
			}else if(bitIndex[0] >= binaryMax || bitIndex[0] < 0){
				this->errorCurrent += "bitIndex:"+std::to_string(bitIndex[0])+" is out of bounds, ";
				this->errorCurrent += "binaryMax:"+std::to_string(binaryMax);
				this->setError(6, this->errorCurrent);
				return false;
			}

			int bitsRemaining = targetBitCount;
			for(int i=this->deriveChunkIndex(binaryMax, bitsRemaining); i>=0 && dstIndex[0] < dstBufferSize; i=this->deriveChunkIndex(binaryMax, bitsRemaining)){
				int chunk = (packingTarget >> (i*binaryMax)) & 0xff;
				
				int msbPos = (bitsRemaining % binaryMax);
				msbPos = msbPos == 0 ? binaryMax - 1 : msbPos - 1;
				int lsbPos = 0;

				int sherrection = (7 - bitIndex[0]) - msbPos;
				if(sherrection < 0){
					// Negative, right Shift, preserve lost bits.
					sherrection *= -1;
					chunk = chunk >> sherrection;
					msbPos -= sherrection;
					lsbPos = 0;
				}else if(sherrection > 0){
					// Positive, Left shift, fetch extra bits
					chunk = chunk << sherrection;
					msbPos += sherrection;
					lsbPos += sherrection;	
				} 

				int bitsUsed = (msbPos - lsbPos) + 1;

				bitsRemaining -= bitsUsed;
				dstBuffer[dstIndex[0]] += chunk;
				bitIndex[0] = (bitIndex[0] + bitsUsed);
				
				if(bitIndex[0] >= binaryMax){
					bitIndex[0] = bitIndex[0] % binaryMax;
					dstIndex[0]++;

					if(!(dstIndex[0] < dstBufferSize)) return true;
					
					dstBuffer[dstIndex[0]] = 0x00;
				}

				if(bitsRemaining <= 0) return true;
			}
			return true;
		}

		int packHeader(void){
			this->errorCurrent = "packHeader() - ";
			if(!this->validateFrequencies()){
				this->setError(0, this->errorCurrent+"failed to validate frequencies.");
				return -1;
			}else if(!this->validateTreeLetters()){
				this->setError(1, this->errorCurrent+"failed to validate tree letters.");
				return -1;
			}else if(this->frequencies_s != this->treeLetters_s){
				this->errorCurrent += "frequencies_s:"+std::to_string(this->frequencies_s)+" != ";
				this->errorCurrent += "treeLetters_s:"+std::to_string(this->treeLetters_s);
				this->setError(2, this->errorCurrent);
				return -1;
			}

			int paddingBitCount = 4; // only 4 bits needed to represent : TODO shrink to 3
			int elementCountBitCount = 9; // max of 9 bits required to represent
			int headerBitCount = paddingBitCount + elementCountBitCount;
			int containerSizeBitCount = 3; // only 3 bits required to rep int's byte usage.
			int byteBitCount = 8; // a byte is 8 bits
			int bitIdx = 4; // index to a byte's binary diget. where 0 is the MSB 2⁷, left side of the binary number.
			int headerIdx = 0; // literal array index.

			for(int i=0; i<this->frequencies_s; i++){
				int freqBitCount = byteBitCount * ((this->frequencies[i] / 0xff) + 1);
				headerBitCount += byteBitCount + freqBitCount + containerSizeBitCount;
			}

			// ensure bit count isn't less than the smallest expected count.
			int minimumBitCount = paddingBitCount + elementCountBitCount + containerSizeBitCount + (2 * byteBitCount);
			if(headerBitCount < minimumBitCount){
				this->errorCurrent += "headerBitCount:"+std::to_string(headerBitCount)+" is less than the expected ";
				this->errorCurrent += "minimumBitCount:"+std::to_string(minimumBitCount);
				this->setError(3, this->errorCurrent);
				return -1;
			}

			this->destroyHeader();
			this->header_s = headerBitCount / 8;
			if ((headerBitCount % 8) != 0) this->header_s++;

			this->header = new (std::nothrow) char[this->header_s];
			if(!this->header){
				this->setError(4, this->errorCurrent+"failed to allocate header.");
				return -1;
			}

			this->header[0] = 0x0;

			if(!this->packByte(this->frequencies_s, 9, this->header, this->header_s, &headerIdx, &bitIdx)){
				this->setError(4, this->errorCurrent+"failed to pack element count.");
				return -1;
			}

			for(int i=0; i<this->frequencies_s && headerIdx<this->header_s; i++){
				int containerSize = (((this->frequencies[i]/0xff)) + 1); // rel to sizeof(int) data type
				if(!this->packByte(containerSize, 3, this->header, this->header_s, &headerIdx, &bitIdx)){
					this->setError(5, this->errorCurrent+"failed to pack sizeof frequency int.");
					return -1;
				}

				int freq = this->frequencies[i];
				if(!this->packByte(freq, 8, this->header, this->header_s, &headerIdx, &bitIdx)){
					this->setError(6, this->errorCurrent+"failed to pack frequency value.");
					return -1;
				}

				char letter = this->treeLetters[i];
				if(!this->packByte((int)letter&0xff, 8, this->header, this->header_s, &headerIdx, &bitIdx)){
					this->setError(7, this->errorCurrent+"failed to pack tree letter.");
					return -1;
				}
			}
			return bitIdx;
		}
		
		int getCodeTableIndex(char target){
			if(!this->validateTreeLetters()){
				this->setError(0, "getCodeTableIndex() - failed to validate tree letters.");
				return -1;
			}
			for(int i=0; i<this->treeLetters_s; i++){
				if(this->treeLetters[i] == target) return i;
			}

			return -1;
		}

		int packBody(int startingBitIndex, char *data, size_t dataSize){
			this->errorCurrent = "packBody() - ";
			if(!this->validateFrequencies()){
				this->setError(0, this->errorCurrent+"failed to validate frequencies.");
				return -1;
			}else if(!this->validateCodeTable()){
				this->setError(1, this->errorCurrent+"failed to validate code table.");
				return -1;
			}else if(!this->validateHeader()){
				this->setError(2, this->errorCurrent+"failed to validate header data.");
				return -1;
			}else if(startingBitIndex < 0 || startingBitIndex >= 8){
				this->errorCurrent += "startingBitIndex:"+std::to_string(startingBitIndex)+"is out of bounds, 8";
				this->setError(3, this->errorCurrent);
				return -1;
			}else if(data == NULL){
				this->setError(4, this->errorCurrent+"data is null.");
				return -1;
			}else if(dataSize <= 0){
				this->setError(5, this->errorCurrent+"dataSize:"+std::to_string(dataSize)+" is empty");
				return -1;
			}

			this->destroyBody();

			// Calculate the number of bits in the encouded result.
			for(int i=0; i<this->frequencies_s; i++)
				this->body_s += this->codeTable[i] * this->frequencies[i];

			// add extra byte if there's a remainder from our conversion
			if((this->body_s % 8) != 0) 
				this->body_s = (this->body_s / 8) + 1;
			else
				this->body_s = this->body_s / 8;

			// Add extra byte to account for the shifted start location of body bytes.
			if(startingBitIndex > 0) this->body_s++;

			if(this->body_s <= 0){
				this->setError(6, this->errorCurrent+"body_s:"+std::to_string(this->body_s)+" has underflowed.");
				return -1;
			}

			this->body = new (std::nothrow) char[this->body_s];
			if(!this->body){
				this->setError(7, this->errorCurrent+"failed to allocate body.");
				return -1;
			}
			this->body[0] = 0;

			int bitIdx = startingBitIndex % 8;
			int bi=0;
			for(int i=0; i<dataSize && bi<this->body_s; i++){
				int tableIdx = this->getCodeTableIndex(data[i]);
				if(tableIdx <= -1){
					this->setError(8, this->errorCurrent+"failed to get char index.");
					return -1;
				}else if(tableIdx >= this->frequencies_s){
					this->errorCurrent += "tableIdx:"+std::to_string(tableIdx)+" is out of bounds, ";
					this->errorCurrent += "frequencies_s:"+std::to_string(this->frequencies_s);
					this->setError(8, this->errorCurrent);
					return -1;
				}
				int bitCount = this->codeTable[tableIdx];
				int encodedChar = this->codeTable[tableIdx + this->frequencies_s];
				if(!this->packByte(encodedChar, bitCount, this->body, this->body_s, &bi, &bitIdx)){
					this->setError(9, this->errorCurrent+"failed to pack body byte.");
					return -1;
				}
			}
			return bitIdx;
		}

		/* QJ bit unpacking functions */
		int unpackByte(char *src, size_t srcSize, int *srcIndex, int *bitIndex, int expectedBitCount){
			this->errorCurrent = "unpackByte() - ";
			if(src == NULL){
				this->setError(0, this->errorCurrent+"src is null.");
				return 0;
			}else if(srcSize <= 0){
				this->setError(1, this->errorCurrent+"srcSize:"+std::to_string(srcSize)+" is empty.");
				return 0;
			}else if(srcIndex == NULL){
				this->setError(2, this->errorCurrent+"srcIndex is null.");
				return 0;
			}else if(srcIndex[0] < 0 || srcIndex[0] >= (int)srcSize){
				this->errorCurrent += "srcIndex:"+std::to_string(srcIndex[0])+" is out of bounds, ";
				this->errorCurrent += "srcSize:"+std::to_string(srcSize);
				this->setError(3, this->errorCurrent);
				return 0;
			}else if(bitIndex == NULL){
				this->setError(4, this->errorCurrent+"bitIndex is null.");
				return 0;
			}else if(bitIndex[0] < 0 || bitIndex[0] >= 8){
				this->setError(5, this->errorCurrent+"bitIndex:"+std::to_string(bitIndex[0])+" is out of bounds, 8");
				return 0;
			}else if(expectedBitCount <= 0){
				this->setError(6, this->errorCurrent+"expectedBitCount:"+std::to_string(expectedBitCount)+" is empty.");
				return 0;
			}

			int ret = 0;
			int binaryMax=8; // Pack byte, so we operate relative to a max container of 8
			int bitsRemaining = expectedBitCount;
			for(int i=this->deriveChunkIndex(binaryMax, bitsRemaining); i >= 0 && srcIndex[0] < srcSize; i=this->deriveChunkIndex(binaryMax, bitsRemaining)){
				if(bitsRemaining <= 0) break;

				int bitsUsed = 0;

				int msb = bitsRemaining % binaryMax;
				msb = msb == 0 ? binaryMax - 1 : msb - 1;

				int lsb = 0;
				int data = ((int)src[srcIndex[0]] & 0xff);

				int sherrection = (7 - bitIndex[0]) - msb;
				if(sherrection < 0){
					// Negative, right Shift, preserve lost bits.
					sherrection *= -1;
					int mask = ~((~(0) >> (msb - lsb + 1)) << (msb - lsb + 1));
					data = (data & (mask >> sherrection)) << sherrection;
					lsb += sherrection;
				}else if(sherrection > 0){
					// Positive, Left shift, fetch extra bits
					int mask = ~((~(0) >> (msb - lsb + 1)) << (msb - lsb + 1));
					data = (data & (mask << sherrection)) >> sherrection;
				}

				bitsUsed = (msb - lsb) + 1;
				bitsRemaining -= bitsUsed;
				ret += (data & ( ~(( ~(0) >> expectedBitCount) << expectedBitCount) )) << (i * 8);

				bitIndex[0] += bitsUsed;
				if(bitIndex[0] >= binaryMax){
					bitIndex[0] = bitIndex[0] % binaryMax;
					srcIndex[0]++;
				}
			}
			if(this->failed()){
				this->setError(7, this->errorCurrent+"failed to derive chunk index.");
				return 0;
			}
			return ret;
		}

		bool unpackHeader(char *data, size_t dataSize, int *bodyStart, int *bodyPadding, int *headerPadding){
			this->errorCurrent = "unpackHeader() - ";
			if(data == NULL){
				this->setError(0, this->errorCurrent+"data is null.");
				return false;
			}else if(dataSize <= 2){
				this->setError(1, this->errorCurrent+"dataSize:"+std::to_string(dataSize)+" is too small, min 3");
				return false;
			}else if(bodyStart == NULL){
				this->setError(2, this->errorCurrent+"bodyStart is null.");
				return false;
			}else if(bodyPadding == NULL){
				this->setError(3, this->errorCurrent+"bodyPadding is null.");
				return false;
			}else if(headerPadding == NULL){
				this->setError(4, this->errorCurrent+"headerPadding is null.");
				return false;
			}

			this->destroyTreeLetters();
			this->destroyFrequencies();

			int byteIdx = 0;
			int bitIdx = 0;
			int elementCount = 0;

			// NOTE: are we even actually using body padding?
			bodyPadding[0] = this->unpackByte(data, dataSize, &byteIdx, &bitIdx, 4);
			if(this->failed()){
				this->setError(5, this->errorCurrent+"failed to unpack body padding.");
				return false;
			}
			
			elementCount = this->unpackByte(data, dataSize, &byteIdx, &bitIdx, 9);
			if(this->failed()){
				this->setError(6, this->errorCurrent+"failed to unpack element count.");
				return false;
			}

			if(!this->resizeTreeLetters(elementCount)){
				this->setError(7, this->errorCurrent+"failed to resize tree letters.");
				return false;
			}else if(!this->resizeFrequencies(elementCount)){
				this->setError(8, this->errorCurrent+"failed to resize frequencies.");
				return false;
			}

			for(int i=0; i<this->frequencies_s; i++){
				int containerSize = this->unpackByte(data, dataSize, &byteIdx, &bitIdx, 3);
				if(this->failed()){
					this->setError(9, this->errorCurrent+"failed to unpack container size.");
					return false;
				}

				int freqValue = this->unpackByte(data, dataSize, &byteIdx, &bitIdx, containerSize * 8);
				if(this->failed()){
					this->setError(10, this->errorCurrent+"failed to unpack frequency value.");
					return false;
				}

				int freqLetter = this->unpackByte(data, dataSize, &byteIdx, &bitIdx, 8);
				if(this->failed()){
					this->setError(11, this->errorCurrent+"failed to unpack frequency letter.");
					return false;
				}

				this->frequencies[i] = freqValue;
				this->treeLetters[i] = (char)freqLetter&0xff;
			}

			headerPadding[0] = bitIdx;
			bodyStart[0] = byteIdx;
			return true;
		}

		bool unpackBody(char *data, size_t dataSize, int bodyStart, int bitOffset, int endPadding){
			this->errorCurrent = "unpackBody() - ";
			if(data == NULL){
				this->setError(0, this->errorCurrent+"data is null.");
				return false;
			}else if(dataSize <= 0){
				this->setError(1, this->errorCurrent+"dataSize:"+std::to_string(dataSize)+" is empty.");
				return false;
			}else if(bodyStart < 0 || bodyStart >= dataSize){
				this->errorCurrent += "bodyStart:"+std::to_string(bodyStart)+" is out of bounds, ";
				this->errorCurrent += "dataSize:"+std::to_string(dataSize);
				this->setError(2, this->errorCurrent);
				return false;
			}else if(bitOffset < 0 || bitOffset >= 8){
				this->setError(3, this->errorCurrent+"bitOffset:"+std::to_string(bitOffset)+" is out of bounds, 8");
				return false;
			}else if(!this->validateTreeData()){
				this->setError(4, this->errorCurrent+"failed to validate tree data.");
				return false;
			}else if(!this->validateFrequencies()){
				this->setError(5, this->errorCurrent+"failed to validate frequencies.");
				return false;
			}else if(!this->validateCodeTable()){
				this->setError(6, this->errorCurrent+"failed to validate code table.");
				return false;
			}

			this->destroyOut();
			this->out_s = this->frequencyMax;
			this->out = new (std::nothrow) char[this->out_s];
			if(!this->out){
				this->setError(7, this->errorCurrent+"failed to allocat out.");
				return false;
			}

			if(!this->codeTableSortByBitCount()){
				this->setError(8, this->errorCurrent+"failed to sort code table.");
				return false;
			}

			for(int o=0; o<out_s; o++){
				int maxBitCount = this->codeTable[this->frequencies_s-1];
				int encoded = this->unpackByte(data, dataSize, &bodyStart, &bitOffset, maxBitCount);
				if(this->failed()){
					this->setError(9, this->errorCurrent+"failed to unpack compressed chunk.");
					return false;
				}	
				int tableCode = 0;
				int bitBackTrack = 0;
				bool success = false;
				for(int f=this->frequencies_s-1; f>=0; f--){
					tableCode = this->codeTable[f+this->frequencies_s];
					if(maxBitCount != this->codeTable[f]){
						int diff = maxBitCount - this->codeTable[f];
						encoded >>= diff;
						maxBitCount = this->codeTable[f];
						bitBackTrack += diff;
						encoded &= ~(( ~(0) >> maxBitCount) << maxBitCount);
					}
					int mask = ~(( ~(0) >> maxBitCount) << maxBitCount);	
					if((encoded & mask) == (tableCode & mask)){
						this->out[o] = this->treeLetters[f];
						bitOffset -= bitBackTrack;
						if(bitOffset < 0){
							bitOffset *= -1;
							bitOffset = 8 - (bitOffset % 8);
							bodyStart--;
						}
						// this->reduceFrequency(f); <-- could work to reduce decode time. but needs to be calculated.
						success = true;
						break;
					}
				}
				if(!success){
					this->errorCurrent += "failed to decode body at index o:"+std::to_string(o);
					this->setError(10, this->errorCurrent);
					return false;
				}
			}
			return true;
		}

		/* QJ sort functions */
		bool codeTableSortByBitCount(void){
			this->errorCurrent = "codeTableSortByBitCount() - ";
			if(!this->validateTreeData()){
				this->setError(0, this->errorCurrent+"failed to validate tree data.");
				return false;
			}else if(!this->validateFrequencies()){
				this->setError(1, this->errorCurrent+"failed to validate frequencies.");
				return false;
			}else if(!this->validateCodeTable()){
				this->setError(2, this->errorCurrent+"failed to validate code table.");
				return false;
			}

			this->destroyWorkBuffer();
			if(!this->resizeWorkBuffer(this->frequencies_s)){
				this->setError(3, this->errorCurrent+"failed to resize workbuffer.");
				return false;
			}

			for(int i=0, grab=this->codeTable[0], grabIdx=0; i<this->workBuffer_s && i<this->codeTable_s; i++){
				if(grab > this->codeTable[i]){
					// get the index of grab, 
					int biggerIdx = grabIdx;
					if((biggerIdx+this->frequencies_s < 0 || biggerIdx+this->frequencies_s >= this->codeTable_s) ||
					   (biggerIdx < 0 || biggerIdx >= this->codeTable_s)){
						this->errorCurrent += "biggerIdx:"+std::to_string(biggerIdx)+" is out of bounds, ";
						this->errorCurrent += "codeTable_s:"+std::to_string(this->codeTable_s);
						this->setError(4, this->errorCurrent);
						return false;
					}

					// get the index of code table.
					int smallerIdx = i;
					if((smallerIdx+this->frequencies_s < 0 || smallerIdx+this->frequencies_s >= this->codeTable_s) || 
					   (smallerIdx < 0 || smallerIdx >= this->codeTable_s)){ // insanity check
						this->errorCurrent += "smallerIdx:"+std::to_string(smallerIdx)+" is out of bounds, ";
						this->errorCurrent += "codeTable_s:"+std::to_string(this->codeTable_s);
						this->setError(5, this->errorCurrent);
						return false;
					}

					// swap grabI and codeTableI,
					int tmpG = grab;
					int tmpT = this->codeTable[i];
					this->codeTable[biggerIdx] = tmpT;
					this->codeTable[smallerIdx] = tmpG;

					// swap grabI+freq and codeTableI + freq
					tmpG = this->codeTable[biggerIdx+this->frequencies_s];
					tmpT = this->codeTable[smallerIdx+this->frequencies_s];
					this->codeTable[biggerIdx+this->frequencies_s] = tmpT;
					this->codeTable[smallerIdx+this->frequencies_s] = tmpG;

					// swap grabI and treeLetterI
					tmpG = (int)this->treeLetters[biggerIdx] & 0xff;
					tmpT = (int)this->treeLetters[smallerIdx] & 0xff;
					this->treeLetters[biggerIdx] = ((char)tmpT) & 0xff;
					this->treeLetters[smallerIdx] = ((char)tmpG) & 0xff;

					// swap grabI and freqeuncieI
					tmpG = this->frequencies[biggerIdx];
					tmpT = this->frequencies[smallerIdx];
					this->frequencies[biggerIdx] = tmpT;
					this->frequencies[smallerIdx] = tmpG;

					i = 0;
					grab = this->codeTable[0];
					grabIdx = 0;
				}else{
					grab = this->codeTable[i];
					grabIdx = i;
				}
			}

			this->destroyWorkBuffer();
			return true;
		}

		/* QJ encode functions */
		/* //
			Algorithm breakdown:
			1) make sure we have data
			2) get index i from data until no more data.
			3) 	check if data i is inside our tree letters array.
			4)	if data i not in array, resize array by +1
			5)		Append data i to the array
		// */
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
					if(!this->resizeTreeLetters(this->treeLetters_s + 1)){
						this->setError(4, "createTreeLetters() - Failed to resize tree letters.");
						return false;
					}
					if(this->treeLetters_s - 1 < 0){
						this->setError(5, "createTreeLetters() - Unexpected underflow detected.");
						return false;
					}
					this->treeLetters[this->treeLetters_s-1] = testChar;
				}
			}

			return true;
		}

		bool createFrequency(char *data, size_t dataSize){
			if(data == NULL){
				this->setError(302, "createFrequency(char *data, size_t dataSize) - data is null.");
				return false;
			}
			if(dataSize <= 0){
				this->setError(303, "createFrequency(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
				return false;
			}

			if(!this->validateTreeLetters()){
				this->setError(0, "createFrequency() - Invalid tree letters.");
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
				if(!this->resizeFrequencies(this->frequencies_s + 1)){
					this->setError(1, "createFrequency() - failed to resize frequencies array.");
					return false;
				}
				if(this->frequencies_s - 1 < 0){
					this->setError(2, "createFrequency() - Unexpected buffer underflow.");
					return false;
				}
				this->frequencies[this->frequencies_s - 1] = freq;
				this->frequencyMax += freq;
			}
			return true;
		}
		
		// most to least frequent.
		bool sortFreqencies(void){
			if(!this->validateFrequencies()){
				this->setError(400, "sortFreqencies(void) - failed to validate frequencies.");
				return false;
			}
			if(!this->validateTreeLetters()){
				this->setError(1, "sortFrequencies() - failed to validate tree letters.");
				return false;
			}
			if(this->treeLetters_s != this->frequencies_s){
				this->setError(402, "sortFreqencies(void) - Table Corruption, treeLetters_s != frequencies_s.");
				return false;
			}

			int reserve = this->frequencies[0];
			for(int i=0; i<this->frequencies_s; i++){
				if(reserve < this->frequencies[i]){
					int a = this->frequencies[i];
					int b = this->frequencies[i-1];
					this->frequencies[i-1] = a;
					this->frequencies[i] = b;

					a = (int)this->treeLetters[i] & 0xff;
					b = (int)this->treeLetters[i-1] & 0xff;
					this->treeLetters[i-1] = (char)a;
					this->treeLetters[i] = (char)b;

					i = -1;
					reserve = this->frequencies[0];
					continue;
				}
				reserve = this->frequencies[i];
			}
			return true;
		}

		bool generateCodeTable(void){
			if(!this->validateTreeData()){
				this->setError(44456, "generateCodeTable() - failed to validate tree data.");
				return false;
			}else if(!this->validateFrequencies()){
				this->setError(665434, "generateCodeTable() - failed to validate frequencies.");
				return false;
			}else if(!this->validateTreeLetters()){
				this->setError(2, "generateCodeTable() - failed to validate tree letters.");
				return false;
			}else if(!this->validateTreeLayers()){
				this->setError(3, "generateCodeTable() - failed to validate tree layers.");
				return false;
			}

			this->destroyCodingTable();
			if(!this->resizeCodeTable(this->frequencies_s*2)){
				this->setError(5, "generateCodeTable() - failed to resize code table.");
				return false;
			}
			for(int i=0; i<this->codeTable_s; i++)
				this->codeTable[i] = 0; // init

			int converter = this->treeData_s - this->frequencies_s;
			int zero=-1;
			int one=-1;
			int queueFill=0;
			int start = this->treeLayerIndecies[1];
			int bitArray[2] = {0};
			for(int i=0; i<=start; i++){
				if(this->isBaseIndex(i)){
					this->setError(3234, "generateCodeTable() - huffman tree missaligned. Cannot start processing from base node.");
					return false; 
				}else if(!this->getSubIndecies(i, &zero, &one) && this->failed()){
					this->setError(666, "generateCodeTable() - failed to get inital sub indecies.");
					return false;
				}

				bitArray[0] = zero;
				bitArray[1] = one;
				for(int bit=0; bit<2; bit++){
					if(this->isBaseIndex(bitArray[bit])){
						int newIndex = bitArray[bit] - converter; // convert to bitarray[x] into a value between 0 and frequencies_s
						if(!this->addBitToCodeTable(newIndex, bit)){
							this->setError(45423, "generateCodeTable() - failed to add bit to code table.");
							return false;
						}
						continue;
					}
					queueFill = 0;
					queueFill = this->pushWorkQueue(bitArray[bit], queueFill);
					if(this->failed()){
						this->setError(3234, "generateCodeTable() - failed to push data to queue.");
						return false;
					}
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
							if(this->failed()){
								this->setError(234, "generateCodeTable() - failed to push data to queue.");
								return false;
							}
						}

						if(this->isBaseIndex(one)){
							int newIndex = one - converter; // convert to 0 to frequencie_s
							if(!this->addBitToCodeTable(newIndex, bit)){
								this->setError(435, "generateCodeTable() - failed to add bit to one index.");
								return false;
							}
						}else{
							queueFill = this->pushWorkQueue(one, queueFill);
							if(this->failed()){
								this->setError(234, "generateCodeTable() - failed to push data to queue.");
								return false;
							}
						}
					}
				}
			}
			return true;
		}
		
		bool encode(char *data, size_t dataSize){

			int headerPadding = this->packHeader();
			if(headerPadding <= -1){
				this->setError(1201, "encode() - failed to pack header.");
				return false;
			}

			int bodyPadding = this->packBody(headerPadding, data, dataSize);
			if(bodyPadding <= -1 || this->failed()){
				this->setError(4324, "encode() - failed to pack body.");
				return false;
			}

			int a=0, b=0;
			// TODO: change padding to 3 bits.
			if(!this->packByte(bodyPadding, 4, this->header, this->header_s, &a, &b)){
				this->setError(234, "encode() - failed to pack body padding.");
			}

			this->destroyOut();

			// Add in the header size
			this->out_s = this->header_s;

			// If the header didn't use the full byte, total size minus 1 consolidates empty space.
			if(headerPadding != 0) this->out_s--;
			
			this->out_s += this->body_s;

			this->out = new (std::nothrow) char[this->out_s];
			if(!this->out){
				this->setError(3, "encode() = failed to allocate out.");
				return false;
			}

			for(int o=0, h=0, b=0; o<this->out_s && (h<this->header_s || b<this->body_s); o++){
				if(h<this->header_s){
					this->out[o] = this->header[h];
					h++;
					if(!(h<this->header_s) && headerPadding != 0){
						// add body[0] directly to the out[o] we just occupied, which reduces total size by 1
						this->out[o] += this->body[b];
						b++;
					}
				}else if(b<this->body_s){
					this->out[o] = this->body[b];
					b++;
				}else{
					break;
				}
			}
			return true;
		}

		/* QJ error functions private */
		void setError(int c, std::string m){
			this->error = c;
			this->error_msg += "["+std::to_string(c)+"] " + m+"\n";
		}
		
	public:
		/* QJ public vars */
		char *out;
		size_t out_s;

		/* QJ constructors */
		HuffmanCoding(){
			this->initBody();
			this->initHeader();
			this->initCodeTable();
			this->initTreeData();
			this->initTreeLetters();
			this->initFrequencies();
			this->initWorkQueue();
			this->initWorkBuffers();
			this->initError();
			this->initOut();
		}

		~HuffmanCoding(){
			this->destroyBody();
			this->destroyHeader();
			this->destroyCodingTable();
			this->destroyTreeData();
			this->destroyTreeLayers();
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyWorkQueue();
			this->destroyWorkBuffer();
			this->destroyWorkTypeBuffer();
			this->clearError();
			this->destroyOut();
		}

		/* QJ error functions public */
		bool failed(void){
			return this->error != -1 ? true : false;
		}

		int getError(void){
			return this->error;
		}
		std::string getErrorMessage(void){
			return this->error_msg;
		}
		void clearError(void){
			this->error = -1;
			this->error_msg = "";
		}

		/* QJ compress main */
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

		/* QJ decompress main */
		bool decompress(char *data, size_t dataSize){
			this->clearError();
			this->destroyCodingTable();
			this->destroyTreeLetters();
			this->destroyTreeLayers();
			this->destroyFrequencies();
			this->destroyOut();

			this->tablesSorted = false; // possibley deprecated variable

			if(data == NULL){
				this->setError(100, "decompress(char *data, size_t dataSize) - data is null.");
				return false;
			}else if(dataSize <= 0){
				this->setError(101, "decompress(char *data, size_t dataSize) - dataSize is <= 0, treating data as null.");
				return false;
			}

			int bodyStart = 0;
			int bodyPadding = 0;
			int headerPadding = 0;
			if(!this->unpackHeader(data, dataSize, &bodyStart, &bodyPadding, &headerPadding)){
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

			if(!this->unpackBody(data, dataSize, bodyStart, headerPadding, bodyPadding)){
				this->setError(4535, "decompress() - failed to unpack body.");
				return false;
			}

			return true;
		}

#if HUFFMAN_EXPERIMENTAL == 1
		/* QJ experimental functions */
		bool popTables(int freqIndex){
			if(!this->validateFrequencies()){
				this->setError(4345, "reduceFrequency()- frequencies table is invalid.");
				return false;
			}else if(!this->validateTreeLetters()){
				this->setError(366, "popTables() - failed to validate tree letters.");
				return false;
			}else if(!this->validateCodeTable()){
				this->setError(222, "popTables() - failed to validate code table.");
				return false;
			}else if(!(freqIndex < this->frequencies_s) || freqIndex < 0){
				this->setError(345, "reduceFrequency() - freqIndex out of bounds.");
				return false;
			}

			for(int i=freqIndex+1; i<this->frequencies_s; i++){
				// remove index from frequencies
				this->frequencies[i-1] = this->frequencies[i];
				// remove index from treeLetters
				this->treeLetters[i-1] = this->treeLetters[i];
				// remove index from codeTable
				this->codeTable[i-1] = this->codeTable[i];
				this->codeTable[this->frequencies_s + i - 1] = this->codeTable[this->frequencies_s + i];
			}

			for(int i=this->frequencies_s+1; i<this->codeTable_s; i++)
				this->codeTable[i-1] = this->codeTable[i];
			
			this->codeTable_s -= 2;
			this->frequencies_s -= 1;
			return true;
		}

		bool reduceFrequency(int freqIndex){
			if(!this->validateFrequencies()){
				this->setError(4345, "reduceFrequency()- frequencies table is invalid.");
				return false;
			}else if(!(freqIndex < this->frequencies_s) || freqIndex < 0){
				this->setError(345, "reduceFrequency() - freqIndex out of bounds.");
				return false;
			}

			this->frequencies[freqIndex]--;
			if(this->frequencies[freqIndex] < 0)
				return this->popTables(freqIndex); // devnote: this doesn't seem right.
			return true;
		}
#endif

#if HUFFMAN_DEBUGGING == 1
		/* QJ debug functions */
		void printTreeLetters(void){
			printf("Tree Letters : ");
			if(this->treeLetters == NULL || this->treeLetters_s <=0){
				printf("NULL\n");
				return;
			}
			for(int i=0; i<treeLetters_s; i++){
				printf("[%d]%c ", i, this->treeLetters[i]);
			}
			printf("\n");
		}

		void printFrequencies(void){
			printf("Frequencies (mex:%d): ", this->frequencyMax);
			if(this->frequencies == NULL || this->frequencies_s <= 0){
				printf("NULL\n");
				return;
			}
			for(int i=0; i<this->frequencies_s; i++){
				printf("[%d]%d ", i, this->frequencies[i]);
			}
			printf("\n");
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
			}
			printf("\n");
			return ret;
		}

		void printTreeOrigins(void){
			printf("\n\tTree Origins\n");
			if(!this->validateTreeData() || !this->validateTreeLayers()){
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
			if(this->treeData == NULL || this->treeData_s <= 0 || !this->validateTreeLayers()){
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
			}
			printf(" (%d)\n", val);
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
#endif
};
