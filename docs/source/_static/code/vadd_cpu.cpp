#include <cstdlib>

#define N 4096 // Size of arrays

// CPU Kernel to compute the addition
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
  vadd_cpu(A, B, C, N);

  // Check result
  int result = 0;
  for (int i = 0; i < N; i++)
    if (A[i] + B[i] != C[i])
      result = 1; // Causes program exit with error

  return result;
}