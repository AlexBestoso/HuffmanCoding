#include "./ThermalInput.class.h"
#include "./ThermalOutput.class.h"
struct thermalOperation{
	std::string name;
	std::string description;
	size_t inputCount;
	ThermalInput *inputs;
	size_t outputCount;
	ThermalOutput *outputs;
};
#include "./ThermalOperation.class.h"

struct thermalStep{
	std::string name;
	std::string description;
	std::string srcFileName;
	int startingLineNumber;
	int endingLineNumber;
	size_t operationCount;
	ThermalOperation *operationChain;
	
};
#include "./ThermalStep.class.h"

struct thermalAlgorithm{
	std::string name;
	std::string description;
	std::string outputLoc;
	size_t outBufferSize;
	uint8_t *outBuff;
	size_t stepCount;
	ThermalStep *stepChain;
};
#include "./ThermalAlgorithm.class.h"

class ThermalDebugIntegrator{
	private:
		bool processingAlgo;
		ThermalAlgorithm currentAlgo;
	public:
	
		ThermalDebugIntegrator(){
			this->processingAlgo = false;
		}
		
		~ThermalDebugIntegrator(){

		}

		bool newAlgorithm(std::string name, std::string desc, std::string outFile){
			if(this->processingAlgo) return false;
			this->processingAlgo = true;
			this->currentAlgo.reset();
			this->currentAlgo.setName(name);
			this->currentAlgo.setDescription(desc);
			this->currentAlgo.setOutputLoc(outFile);
			return true;
		}

		void finishAlgorithm(void){
			this->processingAlgo = false;
		}

		bool newStep(std::string name, std::string description, std::string srcFileName, int startingLineNumber, int endingLineNumber){
			
			return true;
		}

};
