In Makefile, I first add the -lpthread flag after the target LDLIBS. Then, I switch
focus on main.c: In the main function, I notice that the outermost for loop iterate
through the image by x-coordinates. So I create threads to divide the tasks approximately
equally among the threads on the x-coordinates, i.e. divide into n columns. I grab the
whole loop into a function called threadFunc and call pthread_create on this function in
a loop iterates the number of threads of time, and pass in the index of the loop as parameter.

Since the original loop actually prints as the new data of individual pixel is computed,
I have to declare a global 3-d array to hold all the pixel data, and inside the function
threadFunc, I store the computed pixels inside this array. After the threads join later
in the main thread, I write a wrapped for loop to print out the pixel data in the 3-d
array in sequence. This prevents that the printed output might be wrong in sequence due
to the threads generally will not start and finish in the original sequence.

After the modification to Makefile and main.c, I build the source code with make clean check,
and generate the following output related to the performance gain of parallelism:

time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m40.823s
user	0m40.810s
sys	0m0.005s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m21.628s
user	0m42.504s
sys	0m0.002s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m14.651s
user	0m43.560s
sys	0m0.004s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m9.357s
user	0m44.284s
sys	0m0.003s
mv 8-test.ppm.tmp 8-test.ppm

It can be noticed that the performance gain is significant. As the number of threads
increases, the real time decreases drastically while user and sys time stay relatively
constant.
