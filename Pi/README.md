# 512 Hyperloop Pod

## Intro
There is a single Makefile in the <code>src/</code> directory that compiles all <code>.cpp</code> and <code>.h</code> files. The entry point can be found in <code>main.cpp</code>. When compiled, the output file will be called <code>podex</code> (as in "pod executable") in the generated <code>bin/</code> directory.

## Folders
All the code is in the <code>src</code> folder. Once inside that, the following folders exist:

- <code>cpp/</code> - all <code>.cpp</code> source files will be stored here
- <code>include/</code> - all <code>.h</code> header files will be stored here

The <code>bin/</code> and <code>obj</code> directories will be created by make while compiling, but are in the gitignore so they won't be pushed to the repository.

## Commands
Once in the <code>src/</code> directory (the directory with the Makefile), the following commands can be run:

- <code>make</code> - builds podex
- <code>make clean</code> - remove binaries, object files, and temp files

## Files
We have two python files meant to be the frontend interaction for SpaceX.

- <code>back.py</code> Tkinter GUI for monitoring (and e-stopping) the pod
- <code>cli.py</code> UDP wrapper for sending data from the pod executable to the Network Access Panel (provided by SpaceX) meant to comminucate with the GUI
