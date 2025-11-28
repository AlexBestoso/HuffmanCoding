class HuffmanCoding{
	private:
		char *treeLetters;
		size_t treeLetters_s;
	
		int *frequencies;
		size_t frequencies_s;

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
				for(int i=0; i<this->treeLetters_s && i<oldSize){
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
                                for(int i=0; i<this->frequencies_s && i<oldSize){
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
					if(testChar == this->treeLetters[i]){
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

		bool encode(void){
			// sort frequency and letter tables so that the lower frequencies are first.
			// node count is frequencies_s - 1
			
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
			this->clearError();
		}
		~HuffmanCoding(){
			this->destroyTreeLetters();
			this->destroyFrequencies();
			this->destroyOut();
		}

		bool compress(char *data, size_t dataSize){
			this->clearError();
			this->destroyTree();
			this->destroyFrequencies();
			this->destroyOut();
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

			

			return true;
		}

		bool decompress(char *data, size_t dataSize){
			this->clearError();
			this->destroyTree();
			this->destroyFrequencies();
			this->destroyOut();
			return true;
		}
};
