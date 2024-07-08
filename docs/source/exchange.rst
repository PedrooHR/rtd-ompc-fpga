Data Transfer Between Workers
=============================

By default, the OpenMP Offloading library does not provide communication between worker nodes (or between FPGAs). So any data exchange would have to be moved from one worker to another using the Head Node as intermediary.

OMPC implements the **Data Manager** (DM). The DM tracks the data movements and enables OMPC to move data directly from one worker (or FPGA) to another.

Using one or another type depends on how the application is programmed, to better understand that, let's take a look at both types.

.. _defaultexchange:

Default Exchange Mechanism
++++++++++++++++++++++++++

When using OMPC the default mode will be used whenever the data is exposed **locally** to the task. This means we map the data only in the target task creation. For example, when we were calling the ``vadd_cpu`` function in the `Quick Start <quickstart.html>`_ tutorial:

.. code-block:: c++
   :linenos:
   :emphasize-lines: 1
   
   #pragma omp target map(to: A[:N], B[:N]) map(tofrom: C[:N]) nowait
   vadd_cpu(A, B, C, N);

Mapping data like in the emphasized line maps a task within the scope of the task, meaning it will be moved from/to the Head Node according to the clauses selected (``to/tofrom``).

.. _ompcexchange: 

Using Data Manager (DM)
+++++++++++++++++++++++

As said before, the DM enables exchanging data between workers. This can be also understood as exchanging data between tasks, so whenever 