SeamCarving-AlgorithmsProject
=============================

A project for my Algorithms class to implement the Seam Carving algorithms of content aware image resizing

For this project, the image to be resized is in pgm (portable gray map) format. The pgm image file requires 4 entries followed by the greyscale values (some files include comments lines starting with the character #). The four entries are: the literal "P2", an integer representing the x dimension, an integer representing the y dimension, and an integer representing the maximum greyscale value. There should be x times y number of grey-level values after these 4 numbers.

Standard Input: a.exe image.pgm 5 10

Output will be saved as image_processed.pgm

To Compile run: g++ main.cpp SeamCarveImage.cpp -o SeamCarve
