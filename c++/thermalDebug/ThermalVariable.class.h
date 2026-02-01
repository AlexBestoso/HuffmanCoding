
class ThermalVariable{
	private:
		std::string name;
		std::string value;
		int dataType;
	public:

		ThermalVariable(){
			this->name = "";
			this->value = "";
			this->dataType = -1;
		}

		void create(std::string t, std::string n, std::string v){
			this->name = n;
			this->value = v;
			this->dataType = this->dataTypeByName(t);
		}

		std::string rawValue(void){
			return this->value;
		}
		std::string getName(void){
			return this->name;
		}
		float castValue_float(void){
			float ret = 0.0;

			return ret;
		}
		void *getValue(void){
			void *ret = NULL;
			switch(this->dataType){
				case 0:{
					float data = std::stof(this->value);
					ret = (void *)&data;
					break;
				}
				case 3:{
					double data = std::stod(this->value);
					ret = (void *)&data;
					break;
				}
				case 6:{
					int data = std::stoi(this->value);
					ret = (void *)&data;
					break;
				}
		/*		case 7:{
					ret = (void *)&data;
					break;
				//	return "char";
				}
				case 8:{
					ret = (void *)&data;
					break;
				//	return "signed char";
				}
				case 9:{
					ret = (void *)&data;
					break;
				//	return "unsigned char";
				}
				case 10:{
					ret = (void *)&data;
					break;
				//	return "signed int";
				}
				case 11:{
					ret = (void *)&data;
					break;
				//	return "unsigned int";
				}*/
			}
			return ret;
		}
		int dataTypeByName(std::string type){
			if(type == "float"){return 0;}
			if(type == "signed float"){return 1;}
			if(type == "unsigned float"){return 2;}
			if(type == "double"){return 3;}
			if(type == "signed double"){return 4;}
			if(type == "unsigned double"){return 5;}
			if(type == "int"){return 6;}
			if(type == "char"){return 7;}
			if(type == "signed char"){return 8;}
			if(type == "unsigned char"){return 9;}
			if(type == "signed int"){return 10;}
			if(type == "unsigned int"){return 11;}
			
			return -1;
		}

		std::string dataTypeById(int id){
			switch(id){
				case 0:
					return "float";
				case 1:
					return "signed float";
				case 2:
					return "unsigned float";
				case 3:
					return "double";
				case 4:
					return "signed double";
				case 5:
					return "unsigned double";
				case 6:
					return "int";
				case 7:
					return "char";
				case 8:
					return "signed char";
				case 9:
					return "unsigned char";
				case 10:
					return "signed int";
				case 11:
					return "unsigned int";
			}
			return "";
		}
	
};
