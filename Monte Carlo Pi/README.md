# Parallel Monte Carlo Pi 

The code was parallelized with Pthreads. For a parallel code, the monte carlo pi calculation requires the use of locks so the final result gets calculated correctly. Insted of locking the number of cicles it is possible to only lock the hits count making it possible for each thread to run in the for loop without having to lock the critical section.
