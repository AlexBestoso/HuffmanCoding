/*
 * This file isn't related to huffman coding. It's just used to better organize
 * the code used for testing.
 * */
struct timeElement{
	std::string info_name;

	clock_t time_start;
	clock_t time_end;
	double time_cpu;	

	int stat_count;
	double stat_sumation;
	double stat_average;
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
			e.info_name = "";

			e.time_start = 0;
			e.time_end = 0;
			e.time_cpu = 0.0;

			e.stat_count = 0;
			e.stat_sumation = 0.0;
			e.stat_average = 0.0;
			return e;
		}

		struct timeElement newElement(std::string name){
			struct timeElement ret;
			ret = this->initElement(ret);
			ret.info_name = name;
			return ret;
		}
		void startElementTimer(struct timeElement e){
			e.stat_count++;
			e.time_start = clock();
		}

		void endElementTimer(struct timeElement e){
			e.time_end = clock() - e.time_start;
			e.time_cpu = ((double)e.time_end) / CLOCKS_PER_SEC;
			e.stat_sumation += e.time_cpu;
			e.stat_average = e.stat_sumation / e.stat_count;
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
