Accelerating Communication
==========================

In HPC, communication is a common concern. When working with accelerators like FPGA, we have a new memory domain apart from the Host memory (the device memory). Exchanging data between worker nodes using the framework has two modes: MPI and ACCL modes.

.. important::

  Exchanging data between nodes are only available if using OMPC Data Manager. More information about it can be found `in here <exchange.html>`_.

- **MPI (default mode):**

  MPI is the default communication mechanism for OMPC. Since the FPGA extension uses OMPC under the hood, it leverages that system automatically, so don't have to care about moving data between FPGAs. **That will be done automatically by the framework**.

- **ACCL:**
  
  Communication can be accelerated by using the Alveo Collective Communication Library. A library that should be build together with applicationg kernels in the FPGA XCLBIN file.

  The version of ACCL used in this project can be found in `this repository <https://github.com/PedrooHR/ACCL/tree/dev>`_.

  Steps to build applications with ACCL can be found in the repository.

  The detection and usage of ACCL by the framework is also automatized, the user only need to specify FPGA IPs for configuration. This is set in the framework through the environment variable:

  .. code-block:: bash
    
    export OMPTARGET_ALVEO_ACCL_IPS=/path/to/ips/file

  This file is a simple text file containing one IP entry per line (one line per FPGA in the system). In a system with two FPGAs, an example could be:

  .. code-block:: none

    10.0.0.1
    10.0.0.2