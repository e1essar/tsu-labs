#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "windows.h"

typedef struct {
	pthread_cond_t cond;
	pthread_mutex_t lock;
	bool ready;
	int value;
} Monitor;

Monitor monitor = {
	monitor.cond = PTHREAD_COND_INITIALIZER,
	monitor.lock = PTHREAD_MUTEX_INITIALIZER,
	monitor.ready = false,
	monitor.value = 0
};

void* produce(void* arg) {
	for (;;) {
		int value = rand();
		pthread_mutex_lock(&monitor.lock);
		
		if (monitor.ready) {
			pthread_mutex_unlock(&monitor.lock);
			continue;
		}
		
		monitor.value = value;
		monitor.ready = true;
		printf("produce value: %d\n", monitor.value);
		Sleep(100); // for clarity

		pthread_cond_signal(&monitor.cond);
		pthread_mutex_unlock(&monitor.lock);
	}
}

void* consume(void* arg) {
	for(;;) {
		pthread_mutex_lock(&monitor.lock);

		while (monitor.ready == false) {
			pthread_cond_wait(&monitor.cond, &monitor.lock);
		}

		monitor.ready = false;
		printf(" consume value: %d\n", monitor.value);
		Sleep(100); // for clarity

		pthread_mutex_unlock(&monitor.lock);
	}
}

int main() {
	pthread_t provider;
	pthread_t consumer;

	int status = 0;

	status = pthread_create(&provider, NULL, produce, NULL);
	if (status != 0) {
		cout << "main error: can't create thread\n";
		exit(ERROR_CREATE_THREAD);
	}

	status = pthread_create(&consumer, NULL, consume, NULL);
	if (status != 0) {
		cout << "main error: can't create thread\n";
		exit(ERROR_CREATE_THREAD);
	}

	pthread_join(provider, NULL);
	if (status != SUCCESS) {
		cout << "main error: can't join thread\n";
		exit(ERROR_JOIN_THREAD);
	}

	pthread_join(consumer, NULL);
	if (status != SUCCESS) {
		cout << "main error: can't join thread\n";
		exit(ERROR_JOIN_THREAD);
	}
	return 0;
}
