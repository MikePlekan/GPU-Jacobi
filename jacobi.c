/*
  This class runs through the Jacobi Iterations until it hit the max iterations or 
max diff
Author: Michael Plekan
*/

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "timer.h"
#include <omp.h>
				    
int main(int argc, char *argv[]) {
struct timeval start, stop;
gettimeofday(&start, NULL);
if(!(argc==4 || argc==5)){
fprintf(stderr, "Usage: %s message count\n", argv[0]);
}
    int i=0;
    double a=0.0;
    
    int n =strtol(argv[1], NULL, 10);
    int maxpairs=strtol(argv[2], NULL, 10);
    int pairs=0;
    double maxdiff=strtod(argv[3], NULL);
    FILE* outfile;
    if(argc==5){
	outfile= fopen(argv[4], "w");
	}
    double diff=0;
    double current[n+2][n+2];
    double prev[n+2][n+2];
    //sets the borders
    int r,c;
    for(r=0;r<n+2;r++){
	for(c=0;c<n+2;c++){
	current[r][c]=0;
        prev[r][c]=0;
	}
     }

    for(r=0;r<n+2;r++){
      current[r][0]=1;
      current[0][r]=1;
      prev[r][0]=1;
      prev[0][r]=1;
    }

for(i=0;i<maxpairs;i++){
#pragma omp target teams distribute
	for(r=1;r<=n;r++){
#pragma parallel for
		for (c = 1; c <= n; c++)
		{
			prev[r][c] = (current[r - 1][c] + current[r + 1][c] + current[r][c - 1] + current[r][c + 1]) * 0.25;
		}
	}
	for(r=1;r<=n;r++){
#pragma parallel for
		for (c = 1; c <= n; c++)
		{
			current[r][c] = (prev[r - 1][c] + prev[r + 1][c] + prev[r][c - 1] + prev[r][c + 1]) * 0.25;
		}
	}
	diff=0;
	for(r=1;r<=n;r++){
	  for(c=1;c<=n;c++){
	   a=fabs(prev[r][c]-current[r][c]);
	   if(a>diff){
             diff=a;
           }
	   }
	}
pairs=pairs+1;
if(diff<maxdiff){break;}
}
double interval=1.00/((n)*2);
double x;
double y;
x=0;
y=0;
    for(r=0;r<n+2;r++){
	for(c=0;c<n+2;c++){

	if(r==0){
	y=0;
	}
	else if(r==(n+1)){
	y=1;
	}

	else{
	y=((interval*2)*(r))-interval;
	}
	
	if(c==0){
	x=0;
	}
	else if(c==(n+1)){
	x=1;
	}
	else{
	x=((interval*2)*(c))-interval;
	}
	
	double val=current[r][c];
	if(argc==5 && strcmp("-",argv[4])){
	fprintf(outfile,"%4.3f %4.3f %4.3f\n",x,y,val);
	}
	else if((argc==5)&& !strcmp("-",argv[4])){
	for(r=0;r<n+2;r++){
                for(c=0;c<n+2;c++){
                    printf("%.3f ",current[r][c]);
                }
                printf("\n");
            }
	}



	}
}
gettimeofday(&stop, NULL);

printf("Completed %d iteration pairs, last maxDiff %f, %f seconds",pairs,diff,diffgettime(start,stop));
  return 0;
}
