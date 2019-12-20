#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include"key.h"

int main(int argc, char * argv[]) {
	char arg[2];
	int semd;
	if (argc - 1) {
		strncpy(arg, argv[1], 2);
	}else {
		printf("please enter a command line argument\n");
		return 5;
	}
	if (!strcmp("-c", arg)) {
		semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL);
		if (semd < 1) {
			printf("%s\n", strerror(errno));
			return errno;
		}else {
			printf("semaphore created\n");
		}
	}
	if (!strcmp("-r", arg)) {
		printf("removing...");
	}
	if (!strcmp("-v", arg)) {
		printf("viewing...");
	}
}
