
class ThermalDebug{
	private:
		void color_reset(void){printf("\033[0m");}
		
		void color_bright(void){printf("\033[1m");}
		void color_underline(void){printf("\033[4m");}
		void color_flashing(void){printf("\033[5m");}

		void color_blackFg(void){printf("\033[30m");}
		void color_redFg(void){printf("\033[31m");}
		void color_greenFg(void){printf("\033[32m");}
		void color_yellowFg(void){printf("\033[33m");}
		void color_blueFg(void){printf("\033[34m");}
		void color_purpleFg(void){printf("\033[35m");}
		void color_cyanFg(void){printf("\033[36m");}
		void color_whiteFg(void){printf("\033[37m");}
		
		void color_blackBg(void){printf("\033[40m");}
		void color_redBg(void){printf("\033[41m");}
		void color_greenBg(void){printf("\033[42m");}
		void color_yellowBg(void){printf("\033[43m");}
		void color_blueBg(void){printf("\033[44m");}
		void color_purpleBg(void){printf("\033[45m");}
		void color_cyanBg(void){printf("\033[46m");}
		void color_whiteBg(void){printf("\033[47m");}
	
		int logFd=-1;
		std::string executionLog = "";
		std::string logLocation = "./thermal.log";
		std::string logCurrent="";
		bool freshLog(void){
			logFd = open(logLocation.c_str(), O_CREAT|O_TRUNC, S_IRUSR | S_IWUSR);
			if(logFd == -1){
				printf("[E]-~ failed to open log file.\n");
				return false;
			}
			close(logFd);
			return true;
		}
		bool lprint(std::string msg, size_t msgSize){
			for(int i=0; i<msgSize; i++) printf("%c", msg[i]);
			return this->log(msg, msgSize);
		}
		bool log(std::string data, size_t dataSize){
			logFd = open(logLocation.c_str(), O_APPEND | O_RDWR);
			if(logFd == -1){
				printf("[E]-~ failed to open log file.\n");
				return false;
			}
			if(write(logFd, data.c_str(), dataSize) != dataSize){
				printf("[E]-~ failed write log message.\n");
				close(logFd);
				return false;
			}
			close(logFd);
			return true;
		} 
	public:
		std::string binaryString(int src, int bitCount, bool invFormat){
			std::string dst = "";
			if(!invFormat){
				for(int i=0; i<bitCount && i<(sizeof(int)*8); i++){
					dst += (src & 1) == 0 ? '0' : '1';
					src >>= 1;
				}
			}else{
				for(int i=bitCount-1; i>=0 && i<(sizeof(int)*8); i--){
					dst += ((src >> i )&(src & 1)) == 0 ? '0' : '1';
				}

			}
			return dst;
		}

		int thermalAdd(){return 0;}
		int thermalSubtract(){return 0;}
		int thermalMultiply(){return 0;}
		int thermalDivide(){return 0;}
		int thermalAnd(){return 0;}
		int thermalOr(){return 0;}
		int thermalXor(){return 0;}
		int thermalNot(){return 0;}
		int thermalRightshift(){return 0;}
		int thermalLeftshift(){return 0;}
}thermalDbg;
