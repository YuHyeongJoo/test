#include <stdio.h>
#include <stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
typedef struct route_and_distance{
	int distance;
	int* route[50]; 
}rad;
int n,process_count =1, process_limit, ans=2047400008, route_count =0;;
int dist[50][50],pre[50], fd[2];
bool visited[50] ={true,};
int length(int *arr, int k);
int min(int a, int b);
void pre_clear(int *arr, int k);
void pre_add(int *arr, int k, int add_num);
int fun(int a, int* prefix);
int back(int* arr,int k);
void parent(int k,int *arr, int c);
void handler(int sig);
void init_arr(int *arr);
int main(int argc, char *argv[] ){
	signal(SIGCHLD,handler);
	init_arr(pre);
	init_arr(bestroute);
	pipe(fd);
	FILE *fp = fopen(argv[1],"r");
	fscanf(fp,"%d", &n);
	for(int i=0; i<n; i++){
   		 for(int j=0; j<n; j++){
      			fscanf(fp,"%d", &dist[i][j]);
   		 }
 	 }
	process_limit = atoi(argv[2]);

/*	int state = pipe(fd);
	int pid = fork();
	if(pid==0){
		int kk =10;
		write(fd[1],&kk,sizeof(kk));
		exit(0); 	
	}
	else{
		read(fd[0], &n, sizeof(int));
		printf("written n is : %d\n",n);
	}*/
/*	scanf("%d", &n);
	for(int i=0; i<n; i++){
		pre[i]=0;
	}*	for(int i=0; i<n; i++){
		for(int j =0; j<n; j++){
			scanf("%d",&dist[i][j]);
		}

	}*/
	if(n==13)
		parent(3,pre,0);
	else
		parent(n-12,pre,0);
//	pre_add(pre,n,3);
//	printf("%d %d\n",length(pre,n), pre[1]);
//	printf("%d\n",fun(0,pre));

}

int length(int *arr, int k){
	int c=1;
	for(int i=1; i<k; i++){
		if(arr[i]!=0) c++;
		else break;
	}
	return c;
}
void init_arr(int *arr){
	for(int i=0; i<50; i++){
		arr[i]=0;
	}
}
void pre_add(int *arr, int k, int add_num){
	for(int i=1; i<k; i++){
		if(arr[i]==0){
			arr[i] = add_num;
			break;
		}
	}
}
void pre_clear(int *arr, int k){
	arr[length(arr,n)-1] = 0;
}
int min(int a, int b){
	if(a<b) return a;
	else return b;
}
int fun(int a, int* prefix){
	int ret= 1000000007;
	if(length(prefix,n)== n) return a+dist[back(prefix,n)][0];
	for(int i=0; i<n; i++){
		if(visited[i]==true) continue;
		int here = back(prefix,n);
		if(dist[here][i]==0) continue ;
	//	printf("i: %d \n",i);
		visited[i]=true;
		pre_add(prefix,n,i);
	/*	for(int i=0; i<length(prefix,n); i++){
			printf("%d ",prefix[i]);

		}
		printf("\na: %d   dist: %d\n",a, dist[here][i]);
		for(int i =0; i<=n; i++){
			printf(visited[i] ? "true " :"false ");
		}
		printf("\n");
		printf("%d , %d , %d \n" ,a,i,c);*/
		ret = min(fun(a+dist[here][i],prefix),ret);
		if(ret > a+dist[here][i]+dist[back(prefix,n)])
						
			
		//	printf("%d , %d , %d : %d\n" ,a+dist[k][i],i,c,ret);

		visited[i] = false;
		pre_clear(prefix,n);
	}
	return ret;
}
int back(int* arr,int k){
	int tmp = arr[0];
	for(int i=1; i<k; i++){

		if(arr[i]==0) break;
		tmp = arr[i];
	}
	return tmp;
}
void parent(int k,int *arr, int c){
	int sending=0, pid;
	if(length(arr,n)==k){
i		route_count++;
	
		while(process_count>process_limit){
			//	printf("in while process:  %d\n", process_count);
                               // read(fd[0], &sending, sizeof(int));
                               // process_count = process_count-sending;
			//	printf("count: %d\n",process_count);
                               // sending=0;
                        };
                        pid = fork();
                        process_count++;
		if(pid ==0){
			int tmp = fun(c,arr);
			ans = min(ans,tmp);
			printf("%d ",tmp);
			for(int i=0; i<length(arr,n); i++){
                        	printf("%d ", arr[i]);
              	  	}
			printf("\n");
		//	int send=1;
		//	write(fd[1],&send,sizeof(int));
			exit(0) ;	
		}
	}
	else{	
		for(int i=0; i<n; i++){
	    		if(visited[i]==true) continue;
			int here = back(arr,n);
			if(dist[here][i]==0) continue ;
	    		visited[i] = true;
	    		pre_add(arr,n,i);
			parent(k,arr,c+dist[here][i]);
		 	visited[i] = false;
			pre_clear(arr,n);
	  }
	}

}

void handler (int sig){
	if(sig == SIGCHLD){
		process_count--;
		
	}
}

