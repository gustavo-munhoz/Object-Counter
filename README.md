# Object-Counter
A counter of objects in a PPM (portable pixel map) image.

When executing, the algorithm will consider the top left pixel of the image as background, and anything different as an object.

The main function of the code receives two parameters: name of the file and bool (0 for false, else for true).
If the second parameter is true, the image will be printed in the console.
Along with the source code, you will find an Images directory, which contains 20 examples of ppm images.


Example of execution in cmd:

input: ObjectCounter.exe 01.ppm 1
output: 
................................
................................
................................
........*.......................
.......***.............*........
......*****............*........
.....*******..........***.......
....*********.........***.......
...***********.......*****......
....*********........*****......
.....*******........*******.....
......*****.........*******.....
.......***.........*********....
........*..........*********....
..................***********...
................................
...**********...................
...**********...................
...**********.......*****.......
...**********......*******......
...**********.....*********.....
...**********....***********....
...**********....***********....
...**********....***********....
...**********....***********....
...**********....***********....
..................*********.....
...................*******......
....................*****.......
................................
................................
................................

Objects counted: 4