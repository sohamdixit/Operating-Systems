#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define READERS 16
#define WRITERS 32

int shared_value = 0;
int reader_cnt = 0;
int i;
pthread_cond_t readerExited; 
pthread_mutex_t mutex;

void *reader(void * ptr) {

	int r_id = *((int*) ptr);

	pthread_mutex_lock(&mutex);
	reader_cnt++;
	printf("Reader %i read %i\n", r_id, shared_value);
	reader_cnt--;
	pthread_cond_signal(&readerExited);
	pthread_mutex_unlock(&mutex);

}

void *writer(void *ptr) {

	int w_id = *((int*) ptr);

	pthread_mutex_lock(&mutex);
	while(reader_cnt > 0) pthread_cond_wait(&readerExited, &mutex);
	shared_value = rand() %50;
	printf("Writer %i wrote %i\n", w_id, shared_value);
	pthread_mutex_unlock(&mutex);

}

int main() {

	int writer_id[WRITERS];
	int reader_id[READERS];
	pthread_t readers[READERS];
	pthread_t writers[WRITERS];
	pthread_mutex_init(&mutex, NULL);

	for(i=0 ; i<READERS ; i++)
		reader_id[i] = i+1;
	for(i=0 ; i<WRITERS ; i++)
		writer_id[i] = i+1;

	for(i=0 ; i<READERS ; i++)
		pthread_create(&readers[i], NULL, &reader, &reader_id[i]);
	for(i=0 ; i<WRITERS ; i++)
		pthread_create(&writers[i], NULL, &writer, &writer_id[i]);

	for (i = 0; i < READERS; ++i)
		pthread_join(readers[i], NULL);
	for (i = 0; i < WRITERS; ++i)
		pthread_join(writers[i], NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}