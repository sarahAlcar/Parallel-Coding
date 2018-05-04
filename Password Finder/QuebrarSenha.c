#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <omp.h>
typedef enum { false, true } bool;

FILE *popen(const char *command, const char *type);
int *broke;

char finalcmd[300] = "unzip -P%d -t %s 2>&1";
char filename[100];


double rtclock()
{
    struct timezone Tzp;
    struct timeval Tp;
    int stat;
    stat = gettimeofday (&Tp, &Tzp);
    if (stat != 0) printf("Error return from gettimeofday: %d",stat);
    return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}


void breakPass (int i, int * broke)
{
   FILE * fp;
   char ret[200];
   char cmd[400];

    sprintf((char*)&cmd, finalcmd, i, filename);
    fp = popen(cmd, "r");
    while (!feof(fp) && *broke == 0) {
	    	fgets((char*)&ret, 200, fp);
	    	if (strcasestr(ret, "ok") != NULL) {
	    	    printf("Senha:%d\n", i);
	    	    *broke = 1;
	    	}
	 }
	 pclose(fp);
}

int main ()
{
   broke = (int*) malloc (1*sizeof(int));
   *broke = 0;
   int nt;
   double t_start, t_end;

   int i;
   scanf("%d", &nt);
   scanf("%s", filename);
   bool abort = false;// done flag
   // creates one extra thread to have a thread master controler
   t_start = rtclock();
   #pragma omp parallel for shared (i, finalcmd, filename) schedule(dynamic) num_threads(nt+1)
   for(i=0; i < 500000; i++){
        int id = omp_get_thread_num ( );
        if (id == 0){ // busy waiting checking if flag is different
            while (*broke != 1){
                abort = false;
            }
            i = 500000;
            abort = true;
        } // other threads are responsible to run function    
        else if (abort == false){
            #pragma task in(i) out(broke)
	        breakPass (i, broke);
        }        

        
    }
  t_end = rtclock();
 
  fprintf(stdout, "%0.6lf\n", t_end - t_start);  
}
