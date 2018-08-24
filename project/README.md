# Scala saoclib
 Scala wrapper of 'Intel FPGA SDK for OpenCL'.
 
# How to Run

There are several examples in 'com.pzque.sparkdl.examples' package, feel free to run them.

Before you run the examples, you should ensure you have successfully compiled the 'cpp' project and have a shared library named
'saoclib.so' in the '../bin/' directory. 

Then remember to add an option `-Djava.library.path=../bin` to your vm launch options.
In idea, just click "Run" label in menus bar and paste the option text above to the "VM options" input field.

