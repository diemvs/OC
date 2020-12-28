#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

int* arr;
int counter;
int numOfThreads = 10;

void* readT(){
	for(; ; ){
		pthread_mutex_lock(&mutex);
		printf("ID потока: %ld, Размер массива: %d, Счетчик: %d\n", pthread_self(), arr[counter], counter);
		
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}

}

void* writeT(){
	for(; ; ){
		pthread_mutex_lock(&mutex);
		counter++;
		if(counter < 100){
			arr[counter] = counter;
		}
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}

}

int main(){
	arr = (int*)calloc(100, sizeof(int));
	pthread_t threads[numOfThreads];
	
	int i;
	
	for(i = 0; i < 1; i++){
		pthread_create(&threads[i], NULL, writeT, NULL);
	}
	
	while(i < numOfThreads){
		pthread_create(&threads[i], NULL, readT, NULL);
		i++;
	}

	for(i = 0; i < numOfThreads; i++){
		pthread_join(threads[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);

	free(arr);
	return 0;

}
