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

		void setOutBufferSize(size_t v){
        		this->data.outBufferSize = v;
		}
		size_t getOutBufferSize(void){
			return this->data.outBufferSize;
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
        		this->data.outBufferSize = 0;
        		this->data.outBuff = NULL;
        		this->data.stepCount = 0;
        		this->data.stepChain = NULL;
		}
		void reset(void){
			this->data.name = "";
        		this->data.description = "";
        		this->data.outputLoc = "";
        		this->data.outBufferSize = 0;
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
};
