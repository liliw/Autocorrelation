#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

#define NUMT 8

int Size;
float *Array;
float *Sums;

int main()
{
	FILE *fp = fopen( "signal.txt", "r" );
	if( fp == NULL )
	{
		fprintf( stderr, "Cannot open file 'signal.txt'\n" );
		exit( 1 );
	}
	
	fscanf( fp, "%d", &Size );
	Array = new float[2*Size];
	Sums  = new float[1*Size];

	for( int i = 0; i < Size; i++ )
	{
		fscanf( fp, "%f", &Array[i] );
		Array[i+Size] = Array[i];		// duplicate the array
	}
	fclose( fp );

	omp_set_num_threads(NUMT);
	double maxTime = 100000.;

	for(int cnt=0; cnt<10; cnt++) 
	{
		double time0 = omp_get_wtime();

		#pragma omp parallel for
		for( int shift = 0; shift < Size; shift++ )
		{
			float sum = 0.;
			for( int i = 0; i < Size; i++ )
			{
				sum += Array[i] * Array[i + shift];
			}
			Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
		}

		double time1 = omp_get_wtime();
		
		if(	time1-time0 < maxTime )
			maxTime = time1-time0;
	} // for

	double performance = Size*Size/maxTime/1000000.;
	fprintf(stderr, "performance= %10.2fl\n", performance);

	ofstream f;
	f.open("openMP.csv");

	for(int i = 1; i < 513; i++ )
	{
		f << i << "," << Sums[i] << "\n" ;
	}

	f.close();
}