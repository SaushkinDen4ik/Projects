#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h> 

// int N;
double eps = 0.00001;

double f(double x) {
    if (x > 1 || x < 0) {
        return 0;
    }
    return 4 / (1 + x*x);
}

bool compare(double a, double b) {
    return a <= b + eps;
}

int main(int argc, char* argv[]) {
    // if (argc < 2) {
        // return 1;
    // }

    std::setprecision(6);

    // N = atoi(argv[1]);

    int proc_i;
    int proc_size;

    std::ofstream out("data/data.txt", std::ios::app);

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &proc_i);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);

    if (proc_i == 0) {
        printf("\nProcess number: %d\n", proc_size);
    }
    std::vector<int> N_values = {1'000, 1'000'000, 100'000'000};
    for (int N : N_values) {
        if (proc_i == 0) {
            printf("\n-----  N = %d  -----\n", N);
        }
        double integral_val = 0;
        double t1 = MPI_Wtime();
        double h = N / proc_size;
        double step = 1 / (double)N;
        double left_board = proc_i * h * step;
        double right_board = left_board + h * step;
        if (proc_i == proc_size - 1) {
            right_board = 1;
        }

        for (double left_current = left_board; compare(left_current + step, right_board); left_current += step) { // TODO: с точностью чё?
            double darbu_sum = (f(left_current) + f(left_current + step)) * step / 2;
            integral_val += darbu_sum;
        }
        if (proc_i != 0) {
            MPI_Send(&integral_val, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        } else {
            printf("Integral value from process number %d: %f\n", proc_i, integral_val);
            
            int proc_received_from = 0;
            while (proc_received_from != proc_size - 1) {
                double integral_i = 0;

                MPI_Status status;
                MPI_Recv(&integral_i, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                ++proc_received_from;
                
                int from = status.MPI_SOURCE;
                printf("Integral value from process number %d: %f\n", from, integral_i);
                integral_val += integral_i;
            }

            double t2 = MPI_Wtime();
            double time = (t2 - t1) * 1000;

            printf("Final integral value: %f\n", integral_val);
            printf("Parallel execution time (ms): %f\n", time);
            out << N << " " << proc_size << " ";

            // part about consistent execution with 1 process
            double integral_val_0 = 0;
            t1 = MPI_Wtime();
            for (int i = 0; i < N; ++i) {
                integral_val_0 += (f(i * step) + f((i + 1) * step)) * step / 2;
            }
            t2 = MPI_Wtime();
            double one_time = (t2 - t1) * 1000;

            printf("Consistently computed Integral (I0) value with 1 process: %f\n", integral_val_0);
            printf("One process execution time (ms): %f\n", one_time);
            printf("I0 - I = %f\n", integral_val_0 - integral_val);
            printf("S = %f\n", one_time / time);
            out << one_time / time << "\n";
            sleep(1);
        }
    }

    out.close();

    MPI_Finalize();
    return 0;
}
