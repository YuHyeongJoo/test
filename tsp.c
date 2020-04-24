#include <stdio.h>
#include <stdbool.h>
int n;
int dist[50][50],pre[50];
bool visited[50] ={true,};
int length(int *arr, int k);
int min(int a, int b);
void pre_clear(int *arr, int k);
void pre_add(int *arr, int k, int add_num);
int fun(int a, int k, int* prefix);
int back(int* arr,int k);
void parent(int k,int *arr, int c);

int main(){

	scanf("%d", &n);
	for(int i=0; i<n; i++){
		pre[i]=0;
	}
	for(int i=0; i<n; i++){
		for(int j =0; j<n; j++){
			scanf("%d",&dist[i][j]);
		}
	}
	parent(4,pre,0);
//	pre_add(pre,n,3);
//	printf("%d %d\n",length(pre,n), pre[1]);
//	printf("%d\n",fun(0,0,pre));

}

int length(int *arr, int k){
	int c=1;
	for(int i=1; i<k; i++){
		if(arr[i]!=0) c++;
		else break;
	}
	return c;
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
int fun(int a, int k, int* prefix){
	if(length(prefix,n)== n) return a+dist[back(prefix,n)][0];

	int ret= 1000000007;
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
		ret = min(fun(a+dist[here][i],i,prefix),ret);

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
	if(length(arr,n)==k){
		for(int i=0; i<length(arr,n); i++){
			printf("%d ", arr[i]);
		}
		printf("\n");
	}
	else{
		for(int i=0; i<n; i++){
	    if(visited[i]==true) continue;
			int here = back(arr,n);
			if(dist[here][i]==0) continue ;
	    visited[i] = true;
	    pre_add(arr,n,i);
			parent(k,arr,c);
	    visited[i] = false;
			pre_clear(arr,n);
	  }
	}

}
