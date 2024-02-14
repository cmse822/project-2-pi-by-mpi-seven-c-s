#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mpi.h>

void srandom (unsigned seed);  
double dboard (int darts);

//#define DARTS 10000   	/* number of throws at dartboard */
#define ROUNDS 5    	/* number of times "darts" is iterated */

int main(int argc, char *argv[])
{
   double pi;          	/* average of pi after "darts" is thrown */
   double avepi;       	/* average pi value for all iterations */
   int i, n;
   int numtasks, rank;
   int DARTS;
   double global_ave;
   // Initialize MPI environment

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);


   // printf("Starting serial version of pi calculation using dartboard algorithm...\n");
   srandom (rank);            /* seed the random number generator */
   avepi = 0;
   double start_time = MPI_Wtime();
   for (i = 0; i < ROUNDS; i++) {
      /* Perform pi calculation on serial processor */
      DARTS = atoi(argv[1])/numtasks/ROUNDS;
      pi = dboard(DARTS);
      avepi += pi;
      }
   double end_time = MPI_Wtime();
   //printf("Average pi for process %d, avg =%10.12f\n",rank, avepi/ROUNDS); 

   double total_time;
   MPI_Reduce(&start_time, &total_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    /*if (rank == 0) {
        std::cout << "Total runtime: " << end_time - total_time << " seconds." << std::endl;
    }*/   
   
   MPI_Reduce(&avepi, &global_ave, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   // Print the result
   if (rank == 0) {
   printf("%f,%f", end_time - total_time,
            global_ave / numtasks/ROUNDS );
   //printf("\nReal value of PI: 3.1415926535897 \n");

   }
   MPI_Finalize();
   
}

   /*****************************************************************************
    * dboard
    *****************************************************************************/
   #define sqr(x)	((x)*(x))
   long random(void);

   double dboard(int darts)
   {
      double x_coord,       /* x coordinate, between -1 and 1  */
            y_coord,       /* y coordinate, between -1 and 1  */
            pi,            /* pi  */
            r;             /* random number scaled between 0 and 1  */
      int score,            /* number of darts that hit circle */
         n;
      unsigned int cconst;  /* must be 4-bytes in size */
   /*************************************************************************
    * The cconst variable must be 4 bytes. We check this and bail if it is
    * not the right size
    ************************************************************************/
      if (sizeof(cconst) != 4) {
         printf("Wrong data size for cconst variable in dboard routine!\n");
         printf("See comments in source file. Quitting.\n");
         exit(1);
         }
      /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
      cconst = 2 << (31 - 1);
      score = 0;

   /***********************************************************************
    * Throw darts at board.  Done by generating random numbers
    * between 0 and 1 and converting them to values for x and y
    * coordinates and then testing to see if they "land" in
    * the circle."  If so, score is incremented.  After throwing the
    * specified number of darts, pi is calculated.  The computed value
    * of pi is returned as the value of this function, dboard.
    ************************************************************************/

      for (n = 1; n <= darts; n++) {
         /* generate random numbers for x and y coordinates */
         r = (double)random()/cconst;
         x_coord = (2.0 * r) - 1.0;
         r = (double)random()/cconst;
         y_coord = (2.0 * r) - 1.0;

         /* if dart lands in circle, increment score */
         if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
            score++;
         }

      /* calculate pi */
      pi = 4.0 * (double)score/(double)darts;
      return(pi);
   }
