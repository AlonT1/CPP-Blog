#include <stdio.h> 
#include <stdlib.h> 
#include <pthread
#include <unistd.h> 
#include <time.h>

void* my_func(void*);

//----------------------------------------------------


int main() {
	pthread_t thread_data;
	int a = 1, b = 2;
	int status;
	srand((unsigned)time(NULL));
	status = pthread_create(&thread_data, NULL,
		my_func, (void*)&a);

	if (status != 0) {
		fputs("pthread_create failed in main", stderr);
		exit(EXIT_FAILURE);
	}

	my_func((void*)&b);
	puts("we do not reach here");
	return(EXIT_SUCCESS);
}

//----------------------------------------------------

void* my_func(void* args) {
	int i;
	int* val = (int*)args;
	for (i = 0; i < 5; i++) {
		printf("process = %d thread= %d (%u) i= %d\n", getpid(), *val,
			(unsigned int)pthread_self(), i);

		sleep(rand() % 5 + 1);
	}
	pthread_exit(NULL);
}