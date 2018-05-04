#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double count_sort_serial(double a[], int n, int thd) {
    
	int i, j, count;
	double *temp;
	double start, end, duracao;


	temp = (double *)malloc(n*sizeof(double));

	start = omp_get_wtime();

// command to break the operation into sets and parallelize code segment
  #pragma omp parallel for num_threads(thd) private(i, j, count) shared(a, temp) 
	for (i = 0; i < n; i++) {
		count = 0;
		for (j = 0; j < n; j++)
			if (a[j] < a[i])
				count++;
			else if (a[j] == a[i] && j < i)
				count++;
		temp[count] = a[i];
	}
	end = omp_get_wtime();

	duracao = end - start;

     for(int k = 0; k < n; k++) {
        printf("%.2f ", temp[k]);
    }

	memcpy(a, temp, n*sizeof(double));
	free(temp);

	return duracao;
}

// main receives variables as assignment commands
int main( ) {
   int threadNumb;
   int numberElement;
   scanf("%d", &threadNumb);
   scanf("%d", &numberElement);
   double elements[numberElement];
   for(int i=0;i<numberElement;i++){
        scanf("%lf", &elements[i]);
    }
   
   double time = count_sort_serial(elements, numberElement, threadNumb);
   printf("\n");
   printf("%lf\n", time);
   printf("\n");  

   return(0);
}
