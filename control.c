#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include"info.h"

int main(int argc, char * argv[]) {
	char arg[2];
	int fd;
	int semd;
	int shmd;
	if (argc - 1) {
		strncpy(arg, argv[1], 2);
	}else {
		printf("please enter a command line argument\n");
		return 5;
	}
	if (!strcmp("-c", arg)) {
		//create semaphore
		semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0777);
		if (semd < 0) {
			printf("error creating semaphore: %s\n", strerror(errno));
			return errno;
		}else {
			union semun su;
			su.val = 1;
			semctl(semd, 0, SETVAL, su);
			printf("semaphore created\n");
		}
		//create shared memory
		shmd = shmget(SHMKEY, 1024, IPC_CREAT | IPC_EXCL | 0777);
		if (shmd < 0) {
			printf("error creating shared memory: %s\n", strerror(errno));
			return errno;
		}
		printf("shared memory created\n");
		//create file
		fd = open(FILENAME, 0777 | O_CREAT | O_TRUNC);
		if (fd < 0) {
			printf("error creating file: %s\n", strerror(errno));
			return errno;
		}
		printf("file created\n");
		close(fd);
		execlp("chmod", "chmod", "0777", FILENAME, NULL);
	}
	if (!strcmp("-r", arg)) {
		printf("trying to get in\n");
		//use semaphore
		semd = semget(SEMKEY, 1, 0777);
		if (semd < 0) {
			printf("error accessing semaphore: %s\n", strerror(errno));
			return errno;
		}else {
			struct sembuf sb;
			sb.sem_num = 0;
			sb.sem_op = -1;
			semop(semd, &sb, 1);
		}
		//print story
		printf("The story so far:\n");
		struct stat fileinfo;
		stat(FILENAME, &fileinfo);
		fd = open(FILENAME, O_RDONLY);
		char content[fileinfo.st_size];
		read(fd, content, fileinfo.st_size);
		write(STDOUT_FILENO, content, fileinfo.st_size);
		close(fd);
		//remove shared memory
		shmd = shmget(SHMKEY, 1024, 0777);
		if (shmd < 0) {
			printf("error accessing shared memory: %s\n", strerror(errno));
			return errno;
		}else {
			if (shmctl(shmd, IPC_RMID, 0)) {
				printf("error removing shared memory: %s\n", strerror(errno));
			}else {
				printf("shared memory removed\n");
			}
		}
		//remove file
		if (remove(FILENAME)) {
			printf("error removing file: %s\n", strerror(errno));
			return errno;
		}else {
			printf("file removed\n");
		}
		//remove semaphore
		if (semctl(semd, 0, IPC_RMID)) {
			printf("error removing semaphore: %s\n", strerror(errno));
			return errno;
		}else {
			printf("semaphore removed\n");
		}	
	}
	if (!strcmp("-v", arg)) {
		printf("The story so far:\n");
		struct stat fileinfo;
		stat(FILENAME, &fileinfo);
		fd = open(FILENAME, O_RDONLY);
		char content[fileinfo.st_size];
		read(fd, content, fileinfo.st_size);
		write(STDOUT_FILENO, content, fileinfo.st_size);
		close(fd);
	}
}
