#include "./ThermalVariable.class.h"
struct thermalOperation{
	std::string name;
	std::string description;
	size_t variableCount;
	ThermalVariable *variables;
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
	size_t *outBuffSize;
	uint8_t *outBuff;
	size_t stepCount;
	ThermalStep *stepChain;
};
#include "./ThermalAlgorithm.class.h"

class ThermalDebugIntegrator{
	private:
		bool processingAlgo;
		int stepIdx;
		int operationIdx;
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

		ThermalAlgorithm *getAlgorithm(void){
			return &currentAlgo;
		}
		
		void setOutputBuffer(uint8_t *outAddr, size_t *outsizeAddr){
			this->currentAlgo.setOutBuffSize(outsizeAddr);
                        this->currentAlgo.setOutBuff(outAddr);
		}

		void finishAlgorithm(void){
			this->processingAlgo = false;
		}

		bool newStep(std::string name, std::string description, std::string srcFileName, int startingLineNumber, int endingLineNumber){
			return this->currentAlgo.newStep(name, description, srcFileName, startingLineNumber, endingLineNumber);
		}

		bool addOperator(){
			return true;
		}

};
