wh_draw
========

Repository for plotting and table-making utilities combining the flexibility of analysis_code's simple and flexible string-based plotting with the speed of ra4_macros' "looping" scripts.

#### Code documentation
Doxygen-based documentation is available at [Adam's UCSB-HEP webpage](http://hep.ucsb.edu/people/ald77/documentation/doc_ra4_draw/).

#### Setup and compilation
Compilation requires c++11 and ROOT, but not CMSSW. To compile, simply run

    ./compile.py

#### Making histograms
An example script is available under src/test.cxx. To execute, compile and then run

    ./run/core/test.exe
    
Another example for WH is here:
    ```
    ./run/wh/plot_datamc.exe
    ```
##### Adding branches

The list of branches available for plotting must be specified in txt/variables/full

Then, at compile-time, a baby file will be generated to load/set addresses for all of the variables listed.
Modifying this file should cause the whole repository to be recompiled.



