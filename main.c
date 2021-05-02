#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// max memory is 4095(available) - 512(reserved for intepreter only)
#define MAX_MEMORY 3583


int load_binary_file(unsigned char **buffer, char *filepath) {
	FILE *fileptr;

	if((fileptr = fopen(filepath, "rb")) == NULL) {
		printf("Load file error, invalid file\n");
		exit(1);
	}

	fseek(fileptr, 0L, SEEK_END);
	const int filesize = ftell(fileptr);
	rewind(fileptr);

	if(filesize > MAX_MEMORY) {
		printf("Binary file exceeds max value\n");
		exit(1);
	}
	*buffer = (unsigned char *)malloc(filesize * sizeof(**buffer));
	if(*buffer == NULL) {
		printf("Allocation failed\n");
		exit(1);
	}
	fread(*buffer, filesize, 1, fileptr);
	fclose(fileptr);
	return filesize;
}


int main(int argc, char const *argv[])
{
	#define MAX_FILEPATH 1000
	char filepath[MAX_FILEPATH];
	unsigned char *buffer = NULL;

	printf("File to load: ");

	fgets(filepath, MAX_FILEPATH, stdin);
	// remove new line
	filepath[strcspn(filepath, "\n")] = 0;

	if(ferror(stdin)) {
		printf("Read failed\n");
		exit(1);
	}
	const int filesize = load_binary_file(&buffer, filepath);
	printf("Fetched %d bytes\n", filesize);
	for(int i = 0; i < filesize; i++) {
		printf("%x ", buffer[i]);
	}
	free(buffer);
	return 0;
}
