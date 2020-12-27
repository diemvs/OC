#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#define FIFO_PATH "fifo0001"

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
	unlink(FIFO_PATH);

	if((mkfifo(FIFO_PATH, 0666)) == -1) {
		fprintf(stderr, "Невозможно сделать FIFO\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid;
	pid = fork();
	if( pid > 0) {				
		int pid = getpid();
		char *str = "Текущее время в род. процессе: ";
 		str = concat(str, getTime());
		str = concat(str, "PID род. процесса: ");
		str = concat(str, toChar(pid));
		str = concat(str, "\n\0");
		
		int w_fifo = open(FIFO_PATH, O_WRONLY);
		if (w_fifo == -1) {
			fprintf(stderr, "Невозможно считать fifo\n");
			exit(EXIT_FAILURE);
		}
                write(w_fifo, str, strlen(str) + 1);
		close(w_fifo);
		wait(0);
		
	} else {
		int r_fifo = open(FIFO_PATH, O_RDONLY);
		if(r_fifo == -1){
			fprintf(stderr, "Невозможно открыть FIFO\n");
			exit(EXIT_FAILURE);
		}		
		char buf[200];
		read(r_fifo, &buf, sizeof(buf));
		printf("%s", buf);
	       	close(r_fifo);	
		
		sleep(5);
		printf("Текущее время в доч. процессе: %s", getTime());
	}
return 0;
}
