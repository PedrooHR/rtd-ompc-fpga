extern "C" {
  // FPGA Kernel to compute the addition
  void vadd_fpga(int *A, int *B, int *C, int size) {
  #pragma HLS INTERFACE m_axi port = A
  #pragma HLS INTERFACE m_axi port = B
  #pragma HLS INTERFACE m_axi port = C
  #pragma HLS INTERFACE s_axilite port = return
    for (int i = 0; i < size; i++)
      C[i] = A[i] + B[i];
  }
}