#include <stdio.h>
#include<string.h>
#include <stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
typedef struct route_and_distance{
	int distance;
	int route[50];
	char best_route_uptonow[200]; 
}rad;
rad ans = {2000000008};
int n,process_count =1, process_limit, route_count =0, parent_pid;;
int dist[50][50],fd[2];
bool visited[50] ={true,};
int length(rad arr, int k);
rad min(rad a, rad b);
rad pre_clear(rad arr, int k);
rad pre_add(rad arr, int k, int add_num);
rad fun(rad prefix);
int back(rad arr,int k);
void parent(int k,rad arr);
void handler(int sig);
void sigint_handler(int sig);
rad init_arr(rad arr);
int main(int argc, char *argv[] ){
	parent_pid = getpid();
	printf("new strucy type!\n");
	signal(SIGCHLD,handler);
	signal(SIGINT,sigint_handler);
	rad pref;
	pref = init_arr(pref);
	pref.distance= 0;
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

	ans=init_arr(ans);
//	if(n==13)
		parent(n-11,pref);
//	else
//		parent(n-12,pref);
	while( (wait(NULL)) > 0 );
//	pre_add(pre,n,3);
//	printf("%d %d\n",length(pre,n), pre[1]);
//	printf("%d\n",fun(0,pre));
	printf("\nbest: %d best route: %s \n",ans.distance,ans.best_route_uptonow);

}
char* int_to_string(rad arr){
	char *result;
	result = malloc(200);
	for(int i=0; i<length(arr,n);i++){
		char tmp[5];
		int num = arr.route[i];
		sprintf(tmp, "%d", num);
		strcat(result,tmp);
		strcat(result, "->");
	}	
	return result;
}
int length(rad arr, int k){
	int c=1;
	for(int i=1; i<k; i++){
		if(arr.route[i]!=0) c++;
		else break;
	}
	return c;
}
rad init_arr(rad arr){
	for(int i=0; i<50; i++){
		arr.route[i]=0;
	}
	return arr;
}
rad pre_add(rad arr, int k, int add_num){
	for(int i=1; i<k; i++){
		if(arr.route[i]==0){
			arr.route[i] = add_num;
			break;
		}
	}
	return arr;
}
rad pre_clear(rad arr, int k){
	arr.route[length(arr,n)-1] = 0;
	return arr;
}
rad min(rad a, rad b){
	if(a.distance<b.distance) return a;
	else return b;
}
rad fun(rad prefix){
	rad ret;
	ret.distance=1000000007;
	if(length(prefix,n)== n){
		prefix.distance +=dist[back(prefix,n)][0];
		return prefix;
	}
	for(int i=0; i<n; i++){
		if(visited[i]==true) continue;
		int here = back(prefix,n);
		if(dist[here][i]==0) continue ;
	//	printf("i: %d \n",i);
		visited[i]=true;
		prefix = pre_add(prefix,n,i);
	/*	for(int i=0; i<length(prefix,n); i++){
			printf("%d ",prefix[i]);

		}
		printf("\na: %d   dist: %d\n",a, dist[here][i]);
		for(int i =0; i<=n; i++){
			printf(visited[i] ? "true " :"false ");
		}
		printf("\n");
		printf("%d , %d , %d \n" ,a,i,c);*/
		prefix.distance +=dist[here][i];
		ret = min(fun(prefix),ret);
		//if(ret > a+dist[here][i]+dist[back(prefix,n)])
						
			
	//	printf("%d , %d , %d : %d\n" ,a+dist[k][i],i,c,ret);

		visited[i] = false;
		prefix.distance -= dist[here][i];
		prefix = pre_clear(prefix,n);
	}
	return ret;
}
int back(rad arr,int k){
	int tmp = arr.route[0];
	for(int i=1; i<k; i++){

		if(arr.route[i]==0) break;
		tmp = arr.route[i];
	}
	return tmp;
}
void parent(int k, rad arr){
	int pid;
	if(length(arr,n)==k){
		route_count++;
	
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
			printf("A child process is executing\n");	
			rad tmp = fun(arr);
			int current_distance = tmp.distance;
			write(fd[1],&current_distance,sizeof(int));
		//	printf("tmp: %d ", current_distance);
			char route_string[200];
		 	strcpy(route_string,int_to_string(tmp));
                  //      printf("%s \n", route_string);
              	  	write(fd[1], route_string,200);
			
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
	    		arr= pre_add(arr,n,i);
			arr.distance+=dist[here][i];
		//	for(int i=0; i<length(arr,n); i++){
		//		printf("k: %d ",arr.route[i]);
		//	}
		//	printf("%d\n",arr.distance);
			parent(k,arr);
		 	visited[i] = false;
			arr.distance -= dist[here][i];
			arr =pre_clear(arr,n);
	  }
	}

}

void handler (int sig){
	
		int tmp2;
		char best_route[200];
		process_count--;
		read(fd[0], &tmp2, sizeof(int));
		read(fd[0], best_route, 200);
		printf("A Child Process Dead. Its return values are distance:  %d route:  %s \n",tmp2,best_route);
		if(ans.distance>tmp2){	
			printf("--------------------------------------------  best route was updated  distanbe:  %d  route:  %s\n",tmp2,best_route);
			ans.distance = tmp2;
			strcpy(ans.best_route_uptonow,best_route);
		}
		
	
}
void sigint_handler(int sig){
	if(getpid()==parent_pid){
		printf("\nbest : %d\nroute: %s\n",ans.distance,ans.best_route_uptonow);}
	exit(0);
}
