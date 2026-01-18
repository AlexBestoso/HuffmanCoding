void welcome(int argc, char *argv[]){
	qa.makePretty();
	printf("[~]-~ Huffman Codeing Test Program\n");
	printf(" |_-~ Version 0.0.0 qa\n");
	printf(" |\n");
	printf("[*]-~ Processing %d arguments...\n", argc);
	printf(" |_-~ Program Name : %s\n", argv[0]);
	for(int i=1; i<argc; i++){
		printf(" |_-~ Argument %d is ", i);
		if(validateArg(argv[i])){
			qa.color_greenFg();
			printf("valid ");
		}else{
			qa.color_redFg();
			printf("invalid ");
		}
		qa.makePretty();
		printf(": %s\n", argv[i]);
	}
	printf(" |\n");

}
