/*
 * This file isn't related to huffman coding. It's just used to better organize
 * the code used for testing.
 * */
struct timeElement{
	std::string elementName;
	int roundCount;
	clock_t startTime;
	clock_t endTime;
	double duration;	
	double durationSumation;
	double durationAverage;	
};

class QualityAssurance{
	private:
		int logFd=-1;
		std::string executionLog = "";
		std::string logLocation = "./qa.log";
	public:
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
		struct timeElement initElement(struct timeElement e){
			e.elementName = "";
			e.roundCount = 0;
			e.startTime = 0;
			e.endTime = 0;
			e.duration = 0;	
			e.durationSumation = 0;
			e.durationAverage = 0;	
			return e;
		}

		struct timeElement newElement(std::string name){
			struct timeElement ret;
			ret = this->initElement(ret);
			ret.elementName = name;
			return ret;
		}
		void startElementTimer(struct timeElement *e){
			e->roundCount++;
			e->startTime = clock();
		}

		void stopElementTimer(struct timeElement *e){
			e->endTime = clock() - e->startTime;
			e->duration = ((double)e->endTime) / CLOCKS_PER_SEC;
			e->durationSumation += e->duration;
			e->durationAverage = e->durationSumation / e->roundCount;
		}

		void makePretty(void){
			this->color_blackBg();
			this->color_bright();
			this->color_purpleFg();
		}
		void quit(int errorCode){
			this->color_reset();
			printf("\n");
			close(this->logFd);
			exit(errorCode);
		}

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
}qa;
