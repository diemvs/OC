#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#define FILE_PATH "lab_3_r.c"

#define IPC 45

struct Value {
	char str[200];
	pid_t pid;
};

int shmID;

char* getTime(){
	struct tm* ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);
	return asctime(ptr);
}

void MemoryDestructor(int signal){
	printf("Сработал деструктор\n");
	exit(0);
}



int main(void){
	signal(SIGINT, MemoryDestructor);
	key_t ipcKey = ftok(FILE_PATH, IPC);
	if(ipcKey == -1){
		printf("Ошибка  KEY: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	shmID = shmget(ipcKey, sizeof(struct Value), 0777 | IPC_EXCL);
	if (shmID == -1) {
		printf("Ошибка SHM: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	struct Value *recivedValue = (struct Value *) shmat(shmID, NULL, 0);
	if(recivedValue == (struct Value *) -1){
		printf("Ошибка  SHMAT: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	for(; ; ){
		struct Value myValue;

		strcpy( myValue.str, getTime());
		myValue.pid = getpid();

		printf("PID: %d, Время: %s \n", myValue.pid, myValue.str);
		printf("Полученные PID: %d, Время: %s \n", recivedValue->pid, recivedValue->str);
		sleep(2);
	}
	
}
