class ThermalAlgorithm{
	private:
		struct thermalAlgorithm data;
	public:
		void setName(std::string v){
			this->data.name = v;
		}
		std::string getName(void){
			return this->data.name;
		}
		
		void setDescription(std::string v){
        		this->data.description = v;
		}
		std::string getDescription(void){
			return this->data.description;
		}

		void setOutputLoc(std::string v){
        		this->data.outputLoc = v;
		}
		std::string getOutputLoc(void){
			return this->data.outputLoc;
		}

		void setOutBuffSize(size_t *v){
        		this->data.outBuffSize = v;
		}
		size_t getOutBuffSize(void){
			if(this->data.outBuffSize == NULL) return 0;
			return this->data.outBuffSize[0];
		}
		
		void setOutBuff(uint8_t *v){
        		this->data.outBuff = v;
		}
		uint8_t *getOutBuff(void){
			return this->data.outBuff;
		}
		
		void setStepCount(size_t v){
        		this->data.stepCount = v;
		}
		size_t getStepCount(void){
			return this->data.stepCount;
		}
		
		void setStepChain(ThermalStep *v){
        		this->data.stepChain = v;
		}
		ThermalStep *getStepChain(void){
			return this->data.stepChain;
		}

		void init(void){
			this->data.name = "";
        		this->data.description = "";
        		this->data.outputLoc = "";
        		this->data.outBuffSize = NULL;
        		this->data.outBuff = NULL;
        		this->data.stepCount = 0;
        		this->data.stepChain = NULL;
		}
		void reset(void){
			this->data.name = "";
        		this->data.description = "";
        		this->data.outputLoc = "";
        		this->data.outBuffSize = NULL;
			if(this->data.outBuff != NULL)
				delete[] this->data.outBuff;
        		this->data.outBuff = NULL;
        		this->data.stepCount = 0;
			if(this->data.stepChain == NULL)
				delete[] this->data.stepChain;
        		this->data.stepChain = NULL;
			
		}
		ThermalAlgorithm(){
			this->init();
		}

		~ThermalAlgorithm(){

		}

		bool newStep(std::string name, std::string description, std::string srcFileName, int startingLineNumber, int endingLineNumber){
			if(this->data.stepCount <= 0 || this->data.stepChain == NULL){
				this->data.stepCount = 1;
				this->data.stepChain = new (std::nothrow) ThermalStep[1];
				if(this->data.stepChain == NULL)
					return false;

			}else{
				size_t tmpS = this->data.stepCount;
				ThermalStep *tmp = new (std::nothrow) ThermalStep[tmpS];
				if(tmp == NULL)
					return false;

				for(int i=0; i<tmpS; i++)
					tmp[i] = this->data.stepChain[i];

				delete[] this->data.stepChain;
				this->data.stepCount++;
				this->data.stepChain = new (std::nothrow) ThermalStep[this->data.stepCount];
				if(this->data.stepChain == NULL)
					return false;

				for(int i=0; i<tmpS; i++)
					this->data.stepChain[i] = tmp[i];

				delete[] tmp;
			}

			return this->data.stepChain[this->data.stepCount-1].newStep(name, description, srcFileName, startingLineNumber, endingLineNumber);
		}

		ThermalStep *getStep(int idx){
			if(idx < 0 || idx >= this->data.stepCount){
				return NULL;
			}

			return &this->data.stepChain[idx];
		}

		ThermalStep *getCurrentStep(void){
			return getStep(this->data.stepCount - 1);
		}
		
};
