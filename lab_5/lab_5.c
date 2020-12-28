#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

#define SIZE_OF_ARR 100
#define NUMBER_OF_THREADS 10

int* arr;
int counter;


void* readT(){
	for(; ; ){
		pthread_mutex_lock(&mutex);
		printf("ID потока: %ld, Размер массива: %d, Счетчик: %d\n", (long)pthread_self(), arr[counter], counter);
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 5);
	}

}

void* writeT(){
	for(; ; ){
		pthread_mutex_lock(&mutex);
		counter++;
		if(counter < SIZE_OF_ARR){
			arr[counter] = counter;
		}
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}

}

int main(){
	arr = (int*)calloc(SIZE_OF_ARR, sizeof(int));
	pthread_t threads[NUMBER_OF_THREADS];
	
	int i;
	
	for(i = 0; i < 1; i++){
		pthread_create(&threads[i], NULL, writeT, NULL);
	}
	
	while(i < NUMBER_OF_THREADS){
		pthread_create(&threads[i], NULL, readT, NULL);
		i++;
	}

	for(i = 0; i < NUMBER_OF_THREADS; i++){
		pthread_join(threads[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);

	free(arr);
	return 0;

}
