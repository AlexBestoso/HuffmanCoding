class ThermalOperation{
	private:
		struct thermalOperation data;	
	public:
		void init(void){
			this->data.name = "";
			this->data.description = "";
			this->data.variableCount = 0;
			this->data.variables = NULL;

		}
		void reset(void){
			this->data.name = "";
			this->data.description = "";
			this->data.variableCount = 0;
			if(this->data.variables != NULL) delete[] this->data.variables;
			this->data.variables = NULL;
		}
		ThermalOperation(){
			this->init();
		}

		~ThermalOperation(){

		}
};
