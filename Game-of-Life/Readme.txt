gameoflife.c is developed using MPICH2.
1. Install MPICH2
The system I use is Windows7 and Cygwin. To install MPICH2, I downloaded mpich2-1.4.1p1.tar.gz and put it in /usr/local/bin/
Then using the following commands:
1) tar zxvf mpich2-1.4.1p1.tar.gz;
2) mkdir /home/myname/mpich-install
3) cd /usr/local/bin/mpich2-1.4.1p1
4)./configure --disable-f77 --disable-fc --prefix = /home/myname/mpich-install
5)make
6)make install
In the end, I added the following line to .bash_profile
export PATH =/home/myname/mpich-install/bin:${PATH}

2. Compile and run the program
The compile command is:
mpicxx -o gameoflife gameoflife.c
To run, e.g. using four processes (This program can only be executed by less than 16 processes):
mpiexec -n 4 ./gameoflife.exe
