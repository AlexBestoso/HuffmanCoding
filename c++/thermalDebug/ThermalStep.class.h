class ThermalStep{
	private:
		struct thermalStep data;
		bool stepOpen;
	public:
		void init(void){
			this->data.name = "";
			this->data.description = "";
			this->data.srcFileName = "";
			this->data.startingLineNumber = 0;
			this->data.endingLineNumber = 0;
			this->data.operationCount = 0;
			this->data.operationChain = NULL;
			this->stepOpen = false;
		}
		void reset(void){
			this->stepOpen = false;
			this->data.name = "";
			this->data.description = "";
			this->data.srcFileName = "";
			this->data.startingLineNumber = 0;
			this->data.endingLineNumber = 0;
			this->data.operationCount = 0;
			if(this->data.operationChain != NULL)
				delete[] this->data.operationChain;
			this->data.operationChain = NULL;

		}
		ThermalStep(){
			this->init();
		}
		
		~ThermalStep(){

		}

		bool newStep(std::string name, std::string desc, std::string srcFileLoc, int lineNumStart, int lineNumEnd){
			this->reset();
			this->data.name = name;
			this->data.description = desc;
			this->data.srcFileName = srcFileLoc;
			this->data.startingLineNumber = lineNumStart;
			this->data.endingLineNumber = lineNumEnd;

			return true;

		}
		
		void newOperation(){

		}
};
