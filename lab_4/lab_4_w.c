#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/sem.h>

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

void deleteSM(){
	if(shmctl(shmID, IPC_RMID, 0) == -1){
		printf("Ошибка удаления разделяемой памяти\n");
	}
	else {
		 printf("Разделяемая память удаленая успешно\n");
	}

}

void atexitFunction() {
	printf("\nСработал деструктор\n");
	struct shmid_ds *buffer = 0;
	shmctl(shmID, IPC_RMID, buffer);
		if(shmID >= 0 && buffer->shm_segsz != 0){
			deleteSM();
		}
	
}

int main(int argc, char* argv[]){
	if(atexit(atexitFunction)){
		printf("Ошибка atexit()\n");
	}
	struct Value *value;
	
	key_t key = ftok(FILE_PATH, IPC);
	if(key == -1){
		printf("Ошибка KEY: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	shmID = shmget(key, sizeof(value), 0666 | IPC_CREAT);
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
	
	key_t semkey = ftok(SEMAPHORE_NAME, IPC);
	
	int semID = semget(semkey, 1, 0666 | IPC_CREAT);
	if(semID == -1){
		printf("Ошибка SEMAPHORE ID: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	
	
	for(; ; ){
		int semSigRes = semop(semID, &semsignal, 1);
		if(semSigRes == -1){
			printf("Ошибка открытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}	
	
		struct Value temp;
		strcpy( temp.str, getTime());
		temp.pid = getpid();
	
		printf("Данные отправлены!\n");
		*value = temp;
		
		int semwaitRes = semop(semID, &semwait, 1);
		if(semwaitRes == -1){
			printf("Ошибка закрытия SEMAPHORE\n");
			exit(EXIT_FAILURE);
		}
		
		
		sleep(3);
	}

}

