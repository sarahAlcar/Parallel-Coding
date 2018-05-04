#include<pthread.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

// Pthreads requires the serial code variables to become global
int threadNumb;
int numberElement;
int binNumb;

double min = INFINITY;
double max = -INFINITY;
double h;

double *elements;
int *vet;
// count will receive only the created threads insted of the previous serial version parameters
void * count(void* thread) {
	int i, j, count;
	double min_t, max_t;
    long rank = (long) thread; //thread id = rank
    int local_m = binNumb/threadNumb;// bin number for each thread
    int first = rank*local_m; 
    int last = (rank+1)*local_m;
// the first and last variables are defined for each working thread so that they can know their working scope, similar to a private in omp
	for(j=first;j<last;j++) {
		count = 0;
		min_t = min + j*h;
		max_t = min + (j+1)*h;
		for(i=0;i<numberElement;i++) {
			if( (elements[i] <= max_t && elements[i] > min_t) || (j == 0 && elements[i] <= min_t) ) {
				count++;
			}
		}
		vet[j] = count;
	}
}
// main receives variables as assignment commands
int main( ) {
   
   scanf("%d", &threadNumb);
   scanf("%d", &numberElement);
   scanf("%d", &binNumb);

   elements = malloc(numberElement*sizeof(double));
   for(int i=0;i<numberElement;i++){
        scanf("%lf", &elements[i]);
        if (elements[i] > max){
            max = elements[i];
        }
        if (elements[i] < min){
            min = elements[i];
        }
    }

    max = ceil (max);
    min = floor (min);

   vet = malloc(binNumb*sizeof(int));;
   for(int k=0;k<binNumb;k++){
        vet[k] = 0;
    }
 
   h = (max-min)/(float)binNumb;
   double temp = min;
   for(int k=0;k<binNumb;k++){   
        printf ("%.2lf ", temp);
        temp = temp+h;
   }
    printf("%.2lf\n", max);
   struct timeval start;
   gettimeofday(&start,NULL);
    // inicialize and create the threads  
   pthread_t* thread_handles = malloc(threadNumb*sizeof(pthread_t));
   long thread;
    // the creation also executes the function count for each thread
   for (thread = 0; thread < threadNumb; thread++)
        pthread_create(&thread_handles[thread], NULL, count, (void*)thread);
    // synchronizes the process in the end of execution
   for (thread = 0; thread < threadNumb; thread++)
        pthread_join(thread_handles[thread], NULL);
   free(thread_handles);

    for(int k=0;k<binNumb-1;k++){
        printf ("%d ", vet[k]);
    }
    printf ("%d\n", vet[binNumb-1]);
    struct timeval end;
    gettimeofday(&end,NULL);
    long unsigned duracao = ((end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec));
    printf ("%ld\n", duracao);
   return(0);
}

