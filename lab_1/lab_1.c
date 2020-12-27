/* #1 Установить обработчик atexit() при завершении процесса */
/* #2 Из текущего процесса создать дочерний */
/* #3 Каждый процесс должен вывести на экран признак дочерний/родительский */
/* #4 Родительский процесс должен дождаться завершения дочернего  */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void funcExitParentProcess(){
	printf("Завершил работу родителский процесс\n");
}

void funcExitChildProcess(){
	printf("Завершил работу дочерний процесс\n");
}


int main ()
{
	pid_t pid = fork();

	switch(pid){
	case -1: 
		perror("fork");
		exit(0);
	case 0:
		printf("Это процесс потомок\n");
		printf("PID: %d\n", getpid());
		printf("PPID: %d\n", getppid());
		
		atexit(funcExitChildProcess);
		exit(0);
	default: 
		printf("Это процесс родитель\n");
		printf("PID: %d\n", getpid());
		printf("PPID: %d\n", getppid());
		printf("Жду когда потомок вызовет  exit()...\n");
		wait(0);	
	
		atexit(funcExitParentProcess);
		exit(0);
	}
	return 0;
}
