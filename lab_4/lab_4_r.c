#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>

#define IPC 90
#define FILE_PATH "lab_4_r.c"
#define SEMAPHORE_NAME "sem_1"

int shmID;

struct sembuf semsignal = { 0, 1, SEM_UNDO };
struct sembuf semwait = { 0, -1, SEM_UNDO };

struct Value {
	char str[200];
	pid_t pid;
};

char* getTime(){
	struct tm* ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);
	return asctime(ptr);
}

int main(){
	struct Value *recivedValue;
	key_t key = ftok(FILE_PATH, IPC);
	
	int shmID = shmget(key, sizeof(struct Value *), 0666 | IPC_CREAT);
	if(shmID == -1){
		printf("Ошибка SHM: %d\n", errno);
		exit(-1);
	}
	
	recivedValue = (struct Value *) shmat(shmID, NULL, 0);
	if (recivedValue == (struct Value *) - 1){
		printf("Ошибка SHMAT: %d\n", errno);
		exit(-1);
	}
	
	key_t semkey = ftok(SEMAPHORE_NAME, IPC);
	
	int semID = semget(semkey, 1, 0666 | IPC_CREAT);
	if(semID == -1){
		printf("Ошибка SEMAPHORE ID: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	for(; ; ){
		struct Value myValue;
		
		myValue.pid = getpid();
		strcpy( myValue.str, getTime());
		printf("PID: %d, Время: %s \n", myValue.pid, myValue.str);
		printf("Полученные PID: %d, Время: %s \n", recivedValue->pid, recivedValue->str);
		int semSigRes = semop(semID, &semsignal, 1);
		if(semSigRes == -1){
			printf("Ошибка открытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}	

		sleep(1);
	}
	


}
