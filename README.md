# Power-Iteration-Algorithm-for-Eigen-Values

  This program uses normalized power iteration algorithm with deflation to find two eigenvalues and corresponding eigenvector for the dominant eigenvalue of a given matrix A, where A is a square matrix.
  This program reads A from an input file and output the dominant eigenvalue, its corresponding eigenvector and the next eigenvalue as a text file.

A.txt file should be like:
    3.14 1.59 2.65 3.58
    9.79 3.23 8.46 2.64
    3.38 3.27 9.50 2.88
    4.19 7.16 9.39 9.37

Tolerance is given as 1e-6

Then the output file contains:
    Eigenvalue#1: 20.49
    0.35
    0.62
    0.55
    1.00
    Eigenvalue#2: 3.91
    
This program have three command line arguments for the parameters.The first argument is the name of the file you read the matrix from, the second argument is the tolerance, which will be used in the normalized power iteration algorithm, and the third argument is the name of your output file,
