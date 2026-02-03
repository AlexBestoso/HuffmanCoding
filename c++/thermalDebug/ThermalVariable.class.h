// NOTE: We may want to use static_cast<type>(this->value) to handle returning values.
class ThermalVariable{
	private:
		std::string name;
		std::string dataType;
		void *value;
	public:

		ThermalVariable(){
			this->name = "";
			this->dataType = "";
			this->value = NULL;
		}

		void create(std::string t, std::string n, int *v){
			this->create(t, n, (void *)v);
		}
		void create(std::string t, std::string n, char *v){
			this->create(t, n, (void *)v);
		}
		void create(std::string t, std::string n, float *v){
			this->create(t, n, (void *)v);
		}
		void create(std::string t, std::string n, double *v){
			this->create(t, n, (void *)v);
		}
		void create(std::string t, std::string n, size_t *v){
			this->create(t, n, (void *)v);
		}
		void create(std::string t, std::string n, long *v){
			this->create(t, n, (void *)v);
		}
		void create(std::string t, std::string n, void *v){
			this->name = n;
			this->value = v;
			this->dataType = t;
		}

		std::string getName(void){
			return this->name;
		}
		std::string getDataType(void){
			return this->dataType;
		}
		void *getValue(void){
			return this->value;
		}
		int getValueInt(void){
			int ret = static_cast<int *>(this->value)[0];
			return ret;
		}
		char getValueChar(void){
			char ret = static_cast<char *>(this->value)[0];
			return ret;
		}
		float getValueFloat(void){

			float *ret = static_cast<float*>(this->value);
			return ret[0];
		}
		double getValueDouble(void){
			double ret = static_cast<double *>(this->value)[0];
			return ret;
		}
		size_t getValueSize_t(void){
			size_t ret = ((size_t *)this->value)[0];
			return ret;
		}
		long getValueLong(void){
			long ret = ((long *)this->value)[0];
			return ret;
		}
		
		
	
};
