/*
 * Simple program to add two numbers (given in as arguments). Used to
 * test argument passing to child processes.
 *
 * Intended for the basic system calls assignment; this should work
 * once execv() argument handling is implemented.
 */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int
main(int argc, char *argv[])
{
	printf("IN ADD- There are %d args\nAttempting to print them...", argc);
	int whatever;
	for(whatever = 0; whatever < argc; whatever++){
		char *arg = argv[whatever];
		printf("%s\n", arg);
	}


	int i, j;

	if (argc != 3) {
		printf("Incorrect usage of add\n");
		errx(1, "Usage: add num1 num2");
	}

	i = atoi(argv[1]);
	j = atoi(argv[2]);

	printf("Answer: %d\n", i+j);

	return 0;
}
