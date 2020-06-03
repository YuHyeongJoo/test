#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

void clean_up(void * a ){
	printf("cleanup\n");
}

void *fun(void * a){
	 pthread_cleanup_push(clean_up,NULL);	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	for(int i=0; i<10; i++){
		sleep(1);
		printf("%d\n",i);
	}
	 pthread_cleanup_pop(0);

}

int main(){
	pthread_t p;
	pthread_create(&p,NULL,fun,NULL);
	sleep(4);
	pthread_cancel(p);
	pthread_join(p,NULL);
	printf("end\n");
}
