#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char *argv[])
{
	if(argc < 2) {
		fprintf(stderr,"usage: <text>");
		return EXIT_FAILURE;
	}

	int input_text_len = strlen(argv[1]);
	int mirror_text_len = (input_text_len+input_text_len)-1;
	char mirror_text[mirror_text_len];
	
	//Copies original text into the mirror text array
	for(int x=0; x<input_text_len; x++) {
		mirror_text[x] = argv[1][x];
	}
	int counter = 0;
	for(int y=input_text_len-2; y >=0; y--) {
		mirror_text[input_text_len+counter] = argv[1][y];
		counter++;
	}

	for(int i=0; i < mirror_text_len; i++) {
		printf("%c",mirror_text[i]);
	}
	printf("\n");
	return EXIT_SUCCESS;
}
