class ThermalOperation{
	private:
		struct thermalOperation data;	
	public:
		void init(void){
			this->data.name = "";
			this->data.description = "";
			this->data.inputCount = 0;
			this->data.inputs = NULL;
			this->data.outputCount = 0;
			this->data.outputs = NULL;

		}
		void reset(void){
			this->data.name = "";
			this->data.description = "";
			this->data.inputCount = 0;
			if(this->data.inputs != NULL) delete[] this->data.inputs;
			this->data.inputs = NULL;
			this->data.outputCount = 0;
			if(this->data.outputs != NULL) delete[] this->data.outputs;
			this->data.outputs = NULL;
		}
		ThermalOperation(){
			this->init();
		}

		~ThermalOperation(){

		}
};
