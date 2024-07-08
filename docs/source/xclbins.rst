Configuring XCLBIN Files
========================

XCLBIN files are the binaries of loaded into AMD FPGA folders. They contain the logic to be programmed into the boards. There are a few configurations for using XCLBIN files within the framework.

- **Default Behavior:**

  By default, each worker will load the first compatible XCLBIN found in the same directory as the application was launched.

- **Specifying XCLBIN file path**
  
  Users can specify a path which contains a compatible XCLBIN file. This overrides the previous configurations. This configuration is enabled through the environment variable:

  .. code-block:: bash
    
    export OMPTARGET_ALVEO_XCLBIN_DIR=/path/to/xclbin/file

- **Specifying workers' specific XCLBIN files**

  Users can also specify one specific XCLBIN file per worker node. This overrides the previous configurations. This is achieved by tpecifying the path to a `descriptor file` through the environment variable:

  .. code-block:: bash
    
    export OMPTARGET_ALVEO_XCLBIN_DESC=/path/to/descriptor/file

  The descriptor file consists in one entry per line specifying the full path for each XCLBIN file (including the XCLBIN file name). The descriptor file must have at least one path for each worker FPGAs. For example, if we have two FPGAs, a descriptor file could be:

  .. code-block:: none

    /home/user/sum.xclbin
    /home/user/mul.xclbin

