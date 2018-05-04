#include<pthread.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>


// Pthreads requires the serial code variables to become global
int threadNumb;
int numberPoints;
int hits = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void * pi(void* thread) {
    unsigned int random = rand( );
    int local_m = numberPoints/threadNumb;

    double x, y, distSqr;
    int numbCicle = 0;
    for ( int toss = 0; toss < local_m; toss++){
        x = ((double)rand_r(&random)/RAND_MAX)*2.0-1.0;
        y = ((double)rand_r(&random)/RAND_MAX)*2.0-1.0;
        distSqr = (x*x) + (y*y);
        if ( distSqr <= 1){
            numbCicle++;
        }
    }
    // Pthread lock to increment number of hits, does not have to be ordered
    pthread_mutex_lock(&mutex);
    hits += numbCicle;
    pthread_mutex_unlock(&mutex);// unlock 

}



// main receives variables as assignment commands
int main( ) {
   
   scanf("%d", &threadNumb);
   scanf("%d", &numberPoints);
    
   struct timeval start;
   gettimeofday(&start,NULL);

   // allocation
   pthread_t* thread_handles = malloc(threadNumb*sizeof(pthread_t));
   long thread;
    // pi calculation divided by thread
   for (thread = 0; thread < threadNumb; thread++){
        pthread_create(&thread_handles[thread], NULL, pi, (void*)thread);
    }
    // pi calculation join
   for (thread = 0; thread < threadNumb; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    pthread_mutex_destroy(&mutex);
    
    // calculates final pi 
    double piEst = 4* hits/ ((double)numberPoints);
    printf ("%lf\n", piEst);

    // calculate time 
    struct timeval end;
    gettimeofday(&end,NULL);
    long unsigned duracao = ((end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec));
    
    printf ("%ld\n", duracao);

   return(0);
}
