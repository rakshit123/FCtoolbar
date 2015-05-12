Functional Connectivity Toolbar for AFNI
Download the AFNI source code from http://afni.nimh.nih.gov/pub/dist/tgz/AFNI_ARCHIVE/AFNI_2011_12_21_1014.tgz
The scrpit prereq.sh install all the libraries required for compiling AFNI source code on your machine
Use the command "make totality" for compiling the AFNI source code.
Currently, Makefile does not include the option for using multicore.
In the other_builds directory in the source code, a makefile for enabling OpenMP is available. This can be used as a reference for making changes to current makefile for using OpenMP.
