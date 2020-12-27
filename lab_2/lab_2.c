/* #1 Создать pipe
 * #2 Создать дочерний процесс 
 * #3 Передать строчку через pipe из родительского в дочерний процесс с текущим временем и pid предыдущего процесса.
 * #4 Дочерний процесс должен вывести на экран его текущее время и полученную через pipe строчку.
 * #5 Время из род и доч должно различаться как минимум на 5 секунд
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* getTime(){
	struct tm* ptr;
	time_t lt;
	lt = time(NULL);
	ptr = localtime(&lt);
	return asctime(ptr);
}

char* toChar(int num){
	char* c;
	c = (char*)malloc(10 * sizeof(char));
	int numOfDigits = 0;
	while(num > 9){
		c[numOfDigits++] = (num % 10) + '0';
		num = num / 10;
	}
	c[numOfDigits++] = num + '0';
	c[numOfDigits] = '\0';
	char t;

	for (int i = 0; i < numOfDigits/2; i++){
		t = c[i];
		c[i] = c[numOfDigits - 1 - i];
		c[numOfDigits - 1 - i] = t;
	}
	numOfDigits = 0;
	return c;

}

int main (int argc, char * argv[])
{
	int pipedes[2];
	pid_t pid;
	pipe(pipedes);
	pid = fork();
	if( pid > 0) {
		long int currentTime;
		int pid = getpid();

		char *str = "Текущее время в род. процессе: ";
 		str = concat(str, getTime());
		str = concat(str, "PID род. процесса: ");
		str = concat(str, toChar(pid));
		str = concat(str, "\n");
		
		close(pipedes[0]);
		write(pipedes[1], (void *) str, strlen(str) + 1);
		close(pipedes[1]);
		wait(0);
	} else {
		char buf[1024];
		int len;
		close(pipedes[1]);
		while((len = read(pipedes[0], buf, 1024)) != 0)
		write(2, buf, len);
		close(pipedes[0]);
		sleep(5);
		printf("Текущее время в доч. процессе: %s", getTime());
	}
return 0;
}
