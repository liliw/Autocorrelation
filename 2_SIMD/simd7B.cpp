#include "simd.p5.h"

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	FILE *fp = fopen( "signal.txt", "r" );
	if( fp == NULL )
	{
		fprintf( stderr, "Cannot open file 'signal.txt'\n" );
		exit( 1 );
	}
	
	int Size ;
	fscanf( fp, "%d", &Size );
	float Array[2*Size];
	float Sums[1*Size];

	for( int i = 0; i < Size; i++ )
	{
		fscanf( fp, "%f", &Array[i] );
		Array[i+Size] = Array[i];		// duplicate the array
	}
	fclose( fp );

	double maxTime = 10000000000.;

	for(int cnt = 0; cnt < 10; cnt++)
	{
		double time0 = omp_get_wtime();

		for( int shift = 0; shift < Size; shift++ )
		{
			Sums[shift] = SimdMulSum( &Array[0], &Array[0+shift], Size );
		}

		double time1 = omp_get_wtime();

		if(time1-time0 < maxTime)
			maxTime = time1-time0;
	}

	double performance = Size*Size/maxTime/1000000.;
	fprintf(stderr, "performance= %10.2fl\n", performance);

	ofstream f;
	f.open("simd_graph.csv");

	for(int i = 1; i < 513; i++ )
	{
		f << i << "," << Sums[i] << "\n" ;
	}

	f.close();
}