#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 8
#define N 10;

pthread_mutex_t mutex;
pthread_cond_t cc, cp;
int n = 0, queue[MAX], head = 0, tail = 0;
volatile int to_consume = N;
volatile int to_produce = N;

void * producer(void *ptr) {

	pthread_mutex_lock(&mutex);

	while(to_produce) {
		if(n == MAX) pthread_cond_wait(&cp, &mutex);

		queue[tail] = rand();
		printf("Produced %i\n", queue[tail]);
		tail = (tail+1) %MAX;
		n++; to_produce--;
		pthread_cond_signal(&cc);
	}

	pthread_mutex_unlock(&mutex);
}

void * consumer(void *ptr) {

	pthread_mutex_lock(&mutex);

	while(to_consume) {
		if(n == 0) pthread_cond_wait(&cc, &mutex);
		printf("Consuming %i\n", queue[head]);
		head = (head+1) %MAX;
		n--; to_consume--;
		pthread_cond_signal(&cp);
	}

	pthread_mutex_unlock(&mutex);
}

int main() {

	int N1, N2, i;
//	pthread_t *consumert, *producert;
	pthread_t consumert[16], producert[16];

	printf("Enter Number of producers: ");
	scanf("%i", &N1);
//	producert = (pthread_t*) malloc(sizeof(pthread_t)*N1);
	printf("Enter Number of consumers: ");
//	consumert = (pthread_t*) malloc(sizeof(pthread_t)*N2);
	scanf("%i", &N2);

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cc, NULL);
	pthread_cond_init(&cp, NULL);

	for (i = 0; i < N1; ++i)
		pthread_create(&producert[i], NULL, &producer, NULL);
	for (i = 0; i < N2; ++i)
		pthread_create(&consumert[i], NULL, &consumer, NULL);

	for (i = 0; i < N1; ++i)
		pthread_join(producert[i], NULL);
	for (i = 0; i < N2; ++i)
		pthread_join(consumert[i], NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cc);
	pthread_cond_destroy(&cp);

	return 0;
}