# include <iostream>
# include <pthread.h>
# include <unistd.h>

using namespace std;

# define MAX 500

int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX]; // Result of Addition
int matD[MAX][MAX]; // Result of Multiplication

//number of threads
int quantity=6;
//array to store the sum of matA+matB
long *sum=new long[quantity];
//total sum of matA+matB
long long int total_sum=0;
//array to store the sum of matD
long *sumD=new long [quantity];
//total sum of matD
long long int total_sumD=0;

//multiplication function
void* multiply(void *arg)
{	
	long step;
	step=(long) arg;
	//starting row
	int start=step*MAX/quantity;
	//ending row+1
	int end=(step+1)*MAX/quantity;
	//if remainder exist, store the remainders to the last quantity
	if(step==(quantity-1)&&MAX%quantity!=0)
		end==MAX;
		
	//calculate matD and store each value to sumD
	
	//row i in matD	
	for(int i=start;i<end;i++){
		//jth item in row i->D[i][j]
		for(int j=0;j<MAX;j++){
			for(int k=0;k<MAX;k++)
				matD[i][j]+=matA[i][k]*matB[k][j];
			sumD[step]+=matD[i][j];
		}
	}
	
	pthread_exit(NULL);
}

//addition function
void* add(void *arg)
{
	long step;
	step=(long) arg;
	//starting row
	int start=step*MAX/quantity;
	//ending row+1
	int end=(step+1)*MAX/quantity;
	//if remainder exist, store the remainders to the last quantity
	if(step==(quantity-1)&&MAX%quantity!=0)
		end==MAX;
		
	//store each value of matA and matB to sum
	
	for(int i=start;i<end;i++){
		for(int j=0;j<MAX;j++){
			sum[step]+=matA[i][j]+matB[i][j];
		}
	}
	
	pthread_exit(NULL);
}

int main()
{
	//get input
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) 
		    cin >> matA[i][j];
	}
	for (int i = 0; i < MAX; i++) {
		for (int j = 0; j < MAX; j++) 
		    cin >> matB[i][j];
	}
	
	//number of rows each thread contains
	long subsize=MAX/quantity;
		
	//thread initialization	
	pthread_t thread_add[quantity];
	pthread_t thread_multiply[quantity];
	
	//thread creation
	for(long i = 0;i < quantity;i++) {
		pthread_create(&thread_add[i], NULL,add,(void *)i);
		pthread_create(&thread_multiply[i], NULL,multiply,(void *)i);
	}
	
	//wait to join threads
	for(int i=0;i<quantity;i++){
		pthread_join(thread_add[i], NULL);
		pthread_join(thread_multiply[i], NULL);
	}
	
	/*
	this work too, but the method above is slightly faster
	
	for(long i = 0;i < quantity;i++) 
		pthread_create(&thread_multiply[i], NULL,multiply,(void *)i);
		
	for(int i=0;i<quantity;i++)
		pthread_join(thread_multiply[i], NULL);
	*/
		
	for(int i=0;i<quantity;i++){
		total_sum+=sum[i];
		total_sumD+=sumD[i];
	}
	//cout<<"quantity="<<quantity<<endl;
	cout<<total_sum<<endl;
	cout<<total_sumD;
	return 0;
}
