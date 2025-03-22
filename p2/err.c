#include "err.h"

int call_error(int error) {
	if (error == NOFILEFOUND) {
		printf("No file was found\n");
	}
	else if (error == FILENOTWORK) {
		printf("Couldn't read file\n");
	}
	else if (error == NONRECOGNIZETOKEN) {
		printf("A nonrecognized token has been found on the code\n");
	}
	else {
		printf("An unknown error has occured\n");
	}

	return 1;
}