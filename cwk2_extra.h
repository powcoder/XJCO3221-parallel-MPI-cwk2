https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
//
// This file contains functions used in the second item of coursework. Do not alter these routines
// (or copy-and-paste their content to cwk2.c and change it there), as a different version of
// this file will be used as part of the assessment.
//

// Get the problem size N from the command line, checking it is divisible by the number of processes.
int getNFromCommandLine( int argc, char **argv, int rank, int numprocs )
{
	if( argc!=2 )
	{
		if( rank==0 ) printf( "Error: Must enter the matrix and vector size N on the command line.\n" );
		MPI_Finalize();
		return -1;
	}

	int N = atoi( argv[1] );		// Note argv[0] is the executable name! 
	if( N<=0 )						// atoi() converts ASCII to an integer value; returns 0 for non-numeric strings.
	{
		if( rank==0 ) printf( "Could not convert the command line argument '%s' to a positive integer.\n", argv[1] );
		MPI_Finalize();
		return -1;
	}
	if( N%numprocs )
	{
		if( rank==0 ) printf( "The problem size N=%i is not a multiple of the number of processes=%i.\n", N, numprocs );
		MPI_Finalize();
		return -1;
	}

	return N;
}

// Initialise (allocate and assign values to) the full matrix, and initialise (allocate only) the full solution vector.
// The (pointers to) pointers malloc'd here will need to be free'd before program termination.
void initialiseMatrixAndSolution( float **A, float **b, float *x, int N )
{
	int row, col;

	// Set the seed for the random number generator.
	srand( time(0) );

	// Allocate memory for the full matrix, which is allocated as a one-dimensional array, even though it is used
	// like a two-dimensional array - this ensures it the entire matrix is stored in one memory block.
	*A = (float*) malloc( N * N * sizeof(float) );

	// Allocate memory for the full solution vector.
	*b = (float*) malloc( N * sizeof(float) );

	// Put random values into the matrix and the vector x to be multiplied.
	for( row=0; row<N; row++ )
		for( col=0; col<N; col++ )
			(*A)[row*N+col] = 1.0f * rand() / RAND_MAX;
	
	for( row=0; row<N; row++ )
		x[row] = 1.0f * rand() / RAND_MAX;
}

// Free up all memory allocated for x, A_perProc and b_perProc, and (on rank 0 only) A and b.
void freeAllMemory( int rank, float *A, float *b, float *x, float *A_perProc, float *b_perProc )
{
	free( x );
	free( A_perProc );
	free( b_perProc );
	if( rank==0 )
	{
		free( A );
		free( b );
	}
}