#ifndef GAUSSIAN_ELIMINATION_GUARD
#define GAUSSIAN_ELIMINATION_GUARD

/**
 * Solve the equation A*R = X for R.
 *
 * A is a square matrix stored as an array of pointers to arrays
 * holding the _rows_ of the matrix.  R is an vector of matching
 * height to A which will contain the results of the elimination.  X
 * is a vector of matching height to A that holds the RHS of the
 * equation.  This function works by reducing A to an upper triangular
 * matrix (i.e. the lower triangle is all zeros) by Gaussian
 * Elimination and then back substituting to extract R.  The contents
 * of A, X and R will all be modified by this function.  The cost of
 * this reducing each row is N comparisons (to find the best pivot),
 * two pointer swaps (to swap the pivot into the best position), N+1
 * divisions (to divide through by the pivot), size-N lots of N+1
 * subtractions and multiplications (to zero out the lower corner of
 * the matrix).  This cost is incurred for each row and then an
 * additional N*N/2 subtractions and multiplications to back
 * substitute the final values.
 */
void solve_simultaneous(double* X, double** A, double* R, int size);


/**
 * Invert matrix A and store the result in R.
 *
 * This function performs a full gaussian elimination on A to reduce
 * it to an identity matrix.  Simultaneously solves Ar(i)=b(i) for
 * r(i) where b(i) is the ith column of a appropriatly sized identity
 * matrix.  The inverse of A is then the matrix made up with rows
 * r(i). This function overwrites the contents of A and R (the
 * result). The matrix A is expected to be an array of arrays
 * containing _rows_.  The result R is an array of arrays
 * containing _rows_ of the result.
 *
 */
void invert_matrix(double** A, double **R, int size);

/**
 * Takes a matrix A and a matrix B and returns inv(A)*B stored in B.
 *
 * A and B be in row major format (array of arrays of rows) and
 * The parameter size is the size of the square matrix A, matrix B has
 * the same number of rows as A and a number of columns given by cols.
 * This function performs a gaussian elimination on A to find x in
 * inv(A)*B = X by solving A*X(i)=B(i) for each column in B
 * simultaneously.  This function destroys the contents of A and B.
 *
 */
void predivide(double** A, double **B, int size, int cols);

#endif//GAUSSIAN_ELIMINATION_GUARD