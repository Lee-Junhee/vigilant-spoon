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

int main() {
	int fd = open(FILENAME, O_WRONLY | O_APPEND, 0777);
	if (fd < 0) {
		printf("please run './control -c' before running this program\n");
		return 1;
	}
	int semd;
	int shmd;
	char * content;
	printf("trying to get in\n");
	semd = semget(SEMKEY, 1, 0);
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	semop(semd, &sb, 1);
	shmd = shmget(SHMKEY, 1024, 0);
	content = shmat(shmd, 0, 0);
	printf("Last addition: %s\n", content);
	printf("Your addition: ");
	fgets(content, 1024, stdin);
	write(fd, content, 1024);
	close(fd);
	shmdt(content);
	sb.sem_op = 1;
	semop(semd, &sb, 1);
}
