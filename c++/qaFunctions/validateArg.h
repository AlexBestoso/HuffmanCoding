
std::string argTag = "";
std::string argValue = "";
bool validateArg(char *arg){
	std::string tag = "";
	std::string value = "";
	int maxArgLen=100;
	int valueIndex=-1;
	for(int i=0; i<maxArgLen & arg[i] != 0x00; i++){
		if(arg[i] == '='){
			tag += arg[i];
			valueIndex = i+1;
			break;
			break;
		}else{
			tag += arg[i];
		}
	}
	for(int i=valueIndex; arg[i] != 0x00; i++){
		value += arg[i];
	}

	if(tag == "-t0rounds="){
		argTag = tag;
		argValue = value;
		return true;
	}
	
	argTag = "";
	argValue = "";
	return false;
}
