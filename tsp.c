#include <stdio.h>
#include<string.h>
#include <stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
typedef struct route_and_distance{
	int distance;
	int route[50];
	char best_route_uptonow[200]; 
}rad;
rad ans = {2000000008};
int n=0,process_count =0, process_limit=0, parent_pid,thread_num ,finish[2],thread_fd[2];
unsigned long long route_count =0;
int dist[50][50],consumerid[200] , fd[2],child_id[2], rad_data[2], subtask_done[2], child_done[2];
bool visited[50] ={true,};
int length(rad arr, int k);
rad min(rad a, rad b);
rad pre_clear(rad arr, int k);
rad pre_add(rad arr, int k, int add_num);
rad chld(rad prefix);
int back(rad arr,int k);
void parent(int k,rad arr);
void handler(int sig);
void sigint_handler(int sig);
rad init_arr(rad arr);
int main(int argc, char *argv[] ){
	parent_pid = getpid();
//	signal(SIGCHLD,handler);
	signal(SIGINT,sigint_handler);
	rad pref;
	pref = init_arr(pref);
	pref.distance= 0;
	pipe(fd);
	pipe(thread_fd);
	pipe(child_id);
	pipe(rad_data);
	pipe(finish);
	pipe(subtask_done);
	pipe(child_done);
	fcntl(thread_fd[0],F_SETFL, O_NONBLOCK);
	FILE *fp = fopen(argv[1],"r");
	char strTemp[255];
	fgets(strTemp,sizeof(strTemp),fp);
	char *ptr = strtok(strTemp, " ");
	while(ptr!=NULL){
		ptr = strtok(NULL, " ");
		n++;
	}
	
	rewind(fp);

	for(int i=0; i<n; i++){
		 for(int j=0; j<n; j++){
      			fscanf(fp,"%d", &dist[i][j]);
  		 }
	 }
	process_limit = atoi(argv[2]);
	thread_num = process_limit;

	ans=init_arr(ans);
	int init_pid,menu;
	init_pid=fork();
	if(init_pid==0){
		parent(n-10,pref);
		int child_done_int =0;
		for(int i=0; i<8; i++){
			write(child_done[1],&child_done_int,sizeof(int));	
		}
		exit(0);

	}
	while(1){
		printf("Input number 1~4 1 : stat, 2: treads, 3: num N  else: quit ");
		scanf("%d",&menu);
		if(menu==1){

		}
		else if(menu==2){
			for(int i=0; i<thread_num; i++){
				read(child_id[0], &consumerid[i], 4);
				printf("pid : %d\n",consumerid[i]);
			}

		}
		else if(menu==3){
			int thr_num;
                        printf("input thread num : ");
                        scanf("%d", &thr_num);
                        if(thr_num>8){
                                printf("Thread number error!\n");
                                continue;
                        }
                        else if(process_limit>thr_num){
                                thread_num= thread_num + (process_limit-thr_num);
				int child_done_int=0;
				for(int i=0; i<process_limit-thr_num; i++){
					write(child_done[1],&child_done_int,4);
				}
                                process_limit = thr_num;
                        }
                        else if(thr_num>=process_limit){
                                thread_num = thread_num +(thr_num - process_limit);
                                process_limit = thr_num;
                        }

			write(thread_fd[1],&thr_num,4);
			
			}
		else
			break;
			
		
	}
	while( (wait(NULL)) > 0 );
	printf("\nShortest distance: %d  The number of route: %lld   \nShortest route: %s \n",ans.distance,route_count*2*3*4*5*6*7*8*9*10*11*12 ,ans.best_route_uptonow);

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
void string_to_int(rad a, char *s){
	char *ptr = strtok(s, "->");
	int i=0;
	while(ptr !=NULL){
		a.route[i]=atoi(ptr);
		i++;
		ptr = strtok(NULL,"->");
	}
}
rad chld(rad prefix){
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
		visited[i]=true;
		prefix = pre_add(prefix,n,i);
		prefix.distance +=dist[here][i];
		ret = min(chld(prefix),ret);

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
		printf("a bubtasks created\n");
		pid=-1;
		read(thread_fd[0], &process_limit,4);
		if(process_count<process_limit){
                        pid = fork();
			printf("a process created\n");
			if(pid!=0){
				int child_done_int=1;
				write(child_id[1],&pid,4);
				consumerid[process_count] = pid;
				write(child_done[1], &child_done_int,sizeof(int));
				int tmp_dis = arr.distance;
				char tmp_route[200];
				strcpy(tmp_route,int_to_string(arr));
                 	      	 process_count++;
				write(rad_data[1],&tmp_dis,sizeof(tmp_dis));
				write(rad_data[1], tmp_route,200);
			}
		}
		else{
			int child_done_int =1, subtask;
			
			char subtask_route[200];
			write(child_done[1], &child_done_int, sizeof(int));
			read(subtask_done[0],&subtask, sizeof(int));
			read(subtask_done[0],subtask_route, 200);
			if(ans.distance>subtask){
				ans.distance= subtask;
				strcpy(ans.best_route_uptonow,subtask_route);
			}
			subtask = arr.distance;
			strcpy(subtask_route,int_to_string(arr));
			write(rad_data[1],&subtask,sizeof(int));
			write(rad_data[1], subtask_route,200);
		}
		if(pid ==0){
			while(1){
				int child_done_int;
				read(child_done[0],&child_done_int, sizeof(int));
				if(child_done_int ==0) {
					process_count--;
					int getid = getpid();
					write(child_id[1],&getid,4);
					exit(0);}
				rad tmp;
				int tmp_dis;
				char tmp_route[200];
				read(rad_data[0],&tmp_dis,sizeof(int));
				read(rad_data[0],&tmp_route,200);
				tmp.distance = tmp_dis;
				string_to_int(tmp, tmp_route);
                       		rad return_rad = chld(tmp);
				tmp_dis =return_rad.distance;
				strcpy(tmp_route,int_to_string(return_rad));
				write(subtask_done[1],&tmp_dis,sizeof(int));
				write(subtask_done[1],tmp_route,200);

			}
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
		printf("A Child Process is Dead. It returned valuses 1. Distance:%d  2. Path:  %s \n",tmp2,best_route);
		if(ans.distance>tmp2){	
			printf("\nThe shortest route was updated  1.  Distanbe: %d   2. Path: %s\n\n",tmp2,best_route);
			ans.distance = tmp2;
			strcpy(ans.best_route_uptonow,best_route);
		}
		
	
}
void sigint_handler(int sig){
	if(getpid()==parent_pid){
		if(ans.distance == 2000000008)
			printf("No child processes have been terminated. So there is no path found\n");
		printf("\nShortest distance upto now: %d  The number of route upto now : %lld  \nShortest route upto now: %s\n",ans.distance,route_count*2*3*4*5*6*7*8*9*10*11*12 ,ans.best_route_uptonow);}
	exit(0);
}
