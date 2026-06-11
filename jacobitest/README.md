# Jacobi EVD Test

This is a test for the accuracy of calling the jacobi eifenvalue solver with lower n_vals. 
This is tested because of my bachelor project, but i just added it here. 

`eigen.h` is from the lecture notes, it gets the right results for nvals but is slower. 

`jacobi.h` is my implimentation, which is for complex numbers and can stop early. 

To get the same results every time i had to use n_val=5 