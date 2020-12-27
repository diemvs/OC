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

int shmID;

char* getTime(){
	struct tm* ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);
	return asctime(ptr);
}

struct Value {
	char str[200];
	pid_t pid;
};

void MemoryDestructor(int signal){
	printf("Сработал деструктор\n");
	struct shmid_ds *buffer = 0;
	shmctl(shmID, IPC_RMID, buffer);
	
	exit(0);
}

int main(void){
	signal(SIGINT, MemoryDestructor);
	struct Value *value;
	
	key_t ipcKey = ftok(FILE_PATH, IPC);
	if(ipcKey == -1){
		printf("Ошибка KEY: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	shmID = shmget(ipcKey, sizeof(value), 0666 | IPC_CREAT);
	if(shmID == -1){
		printf("Ошибка SHM: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	value = (struct Value *) shmat(shmID, NULL, 0);
	if (value == (struct Value *) -1){
		printf("Ошибка SHMAT: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	if(value->pid != 0){
		printf("Процесс уже запущен!\n");
		exit(0);
	}	
	
	for(; ; ){
	struct Value temp;
	strcpy( temp.str, getTime());
	temp.pid = getpid();
	
	printf("Данные отправлены!\n");
	*value = temp;

	
	sleep(3);
	}
return 0;
}
