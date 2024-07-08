#include <cstdlib>

#define N 4096 // Size of arrays

// CPU Kernel to compute the addition
void vadd_fpga(int *A, int *B, int *C, int size);
#pragma omp declare variant(vadd_fpga) match(device={arch(alveo)})
void vadd_cpu(int *A, int *B, int *C, int size) {
  for (int i = 0; i < size; i++)
    C[i] = A[i] + B[i];
}

int main() {
  int A[N], B[N], C[N];

  // Initialize arrays
  for (int i = 0; i < N; i++) {
    A[i] = i + 1;
    B[i] = i + 2;
    C[i] = 0;
  }

  // Compute
  #pragma omp target map(to: A[:N], B[:N]) map(tofrom: C[:N]) nowait
  vadd_cpu(A, B, C, N);

  // OpenMP Barrier - Point of tasks execution
  #pragma omp taskwait

  // Check result
  int result = 0;
  for (int i = 0; i < N; i++)
    if (A[i] + B[i] != C[i])
      result = 1; // Causes program exit with error

  return result;
}