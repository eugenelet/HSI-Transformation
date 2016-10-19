Image Transformation based on HSI 
=================================

This is an example of changing the characteristic of an image based on its **hue** **saturation** and **intensity**. There're two version of it, one containing slidebards to manually change the values of those 3 and another version that's a fixed transformation that works well with the image given to produce a dusk felling.

The containing executable file is compiled for x86 architecture under Linux.

To compile the version with manual adjustment of HSI values
> **make** 

To complile the fixed version of HSI values
> **make task**

To run the executable
> **./[executable file name] [input file]**

Demo
----
**Input**
![Alt text](input2.bmp?raw=true "Input Image")
**Output**
![Alt text](trans.bmp?raw=true "Output Image")
