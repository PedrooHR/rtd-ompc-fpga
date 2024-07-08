Quick Start
===========

**Summary**

1. `Check the Requirements <requirements_>`_
2. `Download the Container <dockercontainer_>`_
3. `Get the Application <getapplication_>`_
4. `Find an FPGA Kernel <fpgakernel_>`_
5. `Integrate Application and FPGA Kernel <integrate_>`_
6. `Run on FPGAs <runonfpgas_>`_


.. _requirements:

1. Check the Requirements
+++++++++++++++++++++++++

To be able to quick start with the framework you would need:

* One (or more) servers with
   a. Singularity CE - `Singularity Download <https://docs.sylabs.io/guides/latest/user-guide/quick_start.html>`_
   b. Compatible MPICH installation (MPICH v. 4.0.2) - `MPICH Download <https://www.mpich.org/static/downloads/4.0.2/>`_
   c. AMD Vitis Tools 2022.1 (or superior) - `Vitis Page <https://www.xilinx.com/products/design-tools/vitis.html>`_
   d. AMD XRT version 2.13.466 (or superior) - `XRT Page <https://www.xilinx.com/products/design-tools/vitis/xrt.html/>`_
   e. One (or more) AMD Alveo Board

.. _dockercontainer:

2. Download the Container
++++++++++++++++++++++++++++

For this tutorial, we will be using the OMPC FPGA container. It includes all the tools needed to run applications using the framework. We suggest using `Singularity <https://docs.sylabs.io/guides/latest/user-guide/quick_start.html>`_.

**Download the container using Singularity:**

.. code-block:: bash

   singularity pull docker://pedroohr/runtime-fpga:latest

.. _getapplication:

3. Get the Application
++++++++++++++++++++++

As an example, let's start with a basic Vector Addition example. Following figure shows the application:

.. image:: img/app-show.gif
   :alt: Element-wise vector Addition

A basic CPU kernel to execute the application can be implemented as the following:

.. code-block:: c
   :linenos:

   void vadd_cpu(int *A, int *B, int *C, int size) {
      for (int i = 0; i < size; i++)
         C[i] = A[i] + B[i];
   }

And it is called somewhere in the application as:

.. code-block:: c
   :linenos:
   
   vadd(A, B, C, N);

.. note::
   This application is already available in the container on the path `/examples/vadd/vadd_cpu.cpp`. 

   But you can find a fully functional implementation example here: `vadd_cpu <_static/code/vadd_cpu.cpp>`_

.. _fpgakernel:

4. Find an FPGA Kernel
++++++++++++++++++++++

The framework facilitates the usage of any kernel that can be used as an alternative to a defined CPU function (`i.e.:` share equivalent prototypes).

.. tip::
   Application developers can always change the CPU functions to match a desired FPGA kernel (even if the arguments will not be used in the CPU implementation).

So, let's say we found an FPGA implementation for the vadd kernel (`in this case, an HLS version of the kernel`):

.. code-block:: c++
   :linenos:
   
   void vadd_fpga(int *A, int *B, int *C, int size) {
   #pragma HLS INTERFACE m_axi port = A
   #pragma HLS INTERFACE m_axi port = B
   #pragma HLS INTERFACE m_axi port = C
   #pragma HLS INTERFACE s_axilite port = return
      for (int i = 0; i < size; i++)
         C[i] = A[i] + B[i];
   }

That implementation can be compiled using the AMD Vitis\ :sup:`TM` Compiler. The code below shows how to compile for the AMD Alveo u55c board. 

.. code-block:: bash
   
   v++ 

.. note::
   This kernel implementation is already available in the container on the path `/examples/vadd/fpga_kernel.cpp`. 

   But you can find the kernel implementation here: `vadd_cpu.cpp <_static/code/fpga_kernel.cpp>`_


.. _integrate:

5. Integrate Application and FPGA Kernel
++++++++++++++++++++++++++++++++++++++++

The integration of the FPGA kernel can be done with just a few lines of code.

To make the program understand we want to use the FPGA kernel as an alternative to the CPU kernel we need two lines of code (lines 1 and 2):

.. code-block:: c++
   :linenos:
   :emphasize-lines: 1, 2
   
   void vadd_fpga(int *A, int *B, int *C, int size);
   #pragma omp declare variant(vadd_fpga) match(device={arch(alveo)})
   void vadd_cpu(int *A, int *B, int *C, int size) {
      for (int i = 0; i < size; i++)
         C[i] = A[i] + B[i];
   }

Finally, in the line we call that function in the code we need to create an OpenMP Target task (line 1) and establish a syncronization point (line 4), so the program knows when to execute the kernels.

.. code-block:: c++
   :linenos:
   :emphasize-lines: 1, 4
   
   #pragma omp target map(to: A[:N], B[:N]) map(tofrom: C[:N]) nowait
   vadd_cpu(A, B, C, N);

   #pragma omp taskwait

.. important::

   Observe how the original call to ``vadd_cpu`` do not change even if using FPGAs!

.. note::
   This application is already available in the container on the path `/examples/vadd/vadd_fpga.cpp`. 

   But you can find a fully functional implementation example here: `vadd_fpga.cpp <_static/code/vadd_fpga.cpp>`_

.. _runonfpgas:

6. Run on FPGAs
+++++++++++++++

To run the application using the FPGA kernel, one need to compile first, and then run, using the provided container:

**Compiling it using Singularity:**

.. code-block:: bash

   singularity exec runtime-fpga_latest.sif clang++ -fopenmp -fopenmp-targets=alveo -fno-openmp-new-driver vadd_fpga.cpp -o vadd_fpga

**Run it using Singularity:**

.. code-block:: bash

   # Runs using 1 worker node containing FPGAs
   mpirun -np 2 singularity exec runtime-fpga_latest.sif ./fpga_vadd

.. important::

   Currently, we run the applications using **mpirun**, the number of nodes will always be: 1 + number of workers


**That is it! Happy coding with FPGAs**
