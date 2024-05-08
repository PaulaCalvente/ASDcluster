#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	unsigned short xi[3] = { 1, 2, 3 };
	unsigned long long local_count = 0;
	unsigned long long global_count = 0;
	long long i;
	long long samples;
	double x, y;
	double start_time, end_time;

	samples = 3000000;
	if (argc > 1)
		samples = atoll(argv[1]);
	srand(xi[rank]);

	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();

	for (i = 0; i < samples; ++i) {
		x = ((double)rand()) / ((double)RAND_MAX);
		y = ((double)rand()) / ((double)RAND_MAX);
		if (x * x + y * y <= 1.0) {
			++local_count;
		}
	}
	MPI_Reduce(&local_count, &global_count, 1, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	end_time = MPI_Wtime();

	if (rank == 0) {
		printf("Valor estimado de pi: %.7f\n", 4.0 * global_count / (samples * size));
		printf("Tiempo de ejecución: %.6f segundos\n", end_time - start_time);
	}

	MPI_Finalize();
	return 0;
}
