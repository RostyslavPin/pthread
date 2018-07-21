#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int globalres = 0;
int v1[100] = {1,2}, v2[100]= {3,4};
int ids[4] = {0,1,2,3};
pthread_t thrs[4];
pthread_mutex_t mutex;


void*	prod(void* me){
	int offset = *((int*)me);
	offset *= 25;
	int res = 0;
	int i = offset;
	while (i < offset + 25){
		res += v1[i]*v2[i];
		i++;
	}
	pthread_mutex_lock(&mutex);
	globalres += res;
	pthread_mutex_unlock(&mutex);
}

int		main(){
	pthread_attr_t attrs;
	int i = 0;

	if (0 != pthread_attr_init(&attrs)){
		perror("Cannot init attributes");
		abort();
	}
	if (0 != pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE)){
		perror("Error setting attributes");
		abort();
	}
	while (i < 4){
		if (0 != pthread_create(&thrs[i], &attrs, prod, &ids[i])){
			perror("Cannot create a thread");
			abort();
		}
		i++;
	}
	pthread_attr_destroy(&attrs);
	while (i < 4){
		if (0 != pthread_join(thrs[i], NULL)){
			perror("Cannot join");
			abort();
		}
		i++;
	}
	printf("%d\n", globalres);
	return (0);
}