#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_rwlock_t rwlock;

int* arr;
int counter = 0;
int numOfThreads = 10;

void* readT(){
	for(; ; ){
		pthread_rwlock_wrlock(&rwlock);
		printf("ID потока: %ld, Размер массива: %d, Счетчик: %d\n", pthread_self(), arr[counter], counter);
		
		fflush(stdout);
		pthread_rwlock_unlock(&rwlock);
		sleep(3);
	}

}

void* writeT(){
	for(; ; ){
		pthread_rwlock_rdlock(&rwlock);
		counter++;
		if(counter < 100){
			arr[counter] = counter;
		}
		fflush(stdout);
		pthread_rwlock_unlock(&rwlock);
		sleep(3);
	}

}

int main(){
	arr = (int*)calloc(100, sizeof(int));
	
	pthread_t threads[numOfThreads];
	pthread_rwlock_init(&rwlock, NULL);
	
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
	
	pthread_rwlock_destroy(&rwlock);

	free(arr);
	return 0;

}
