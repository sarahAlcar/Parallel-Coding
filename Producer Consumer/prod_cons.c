#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void producer_consumer(int *buffer, int size, int *vec, int n, int thd) {
	int i, j;
	long long unsigned int sum = 0;
    double start, end, time;
    start = omp_get_wtime();
    // pragma indicates outter for loop will be a parallel region.
    // Each thread have individual i and j variables; threads operate in shared buffer and vec structures
    // Sum has to synchronize individual thread results
    #pragma omp parallel num_threads (thd) private (i,j) shared(buffer,vec) reduction (+:sum)
	for(i=0;i<n;i++) {
		if(i % 2 == 0) {// PRODUCER
            // command used to recycle created threads in outter loop
            #pragma omp for 			
            for(j=0;j<size;j++) {
				buffer[j] = vec[i] + j*vec[i+1];
			}
		}
		else {// CONSUMER
            #pragma omp for 
			for(j=0;j<size;j++) {
				sum += buffer[j];
			}
		}
	}

    end = omp_get_wtime();
	time = end - start;

	printf("%llu\n",sum);
    printf("%lf\n", time);
}


// main receives variables as assignment commands
int main( ) {

   int threadNumb;
   int numberIterations;

   scanf("%d", &threadNumb);
   scanf("%d", &numberIterations);

   int bufferSize;

   scanf("%d", &bufferSize);
   int buffer[bufferSize];

   int values[numberIterations];
   for(int i=0;i<numberIterations;i++){
        scanf("%d", &values[i]);
    }

   producer_consumer (buffer, bufferSize, values,  numberIterations, threadNumb);  

   return(0);
}
