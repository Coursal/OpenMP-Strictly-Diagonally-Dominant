# OpenMP-Strictly-Diagonally-Dominant
A simple OpenMP program that checks parallelly if a NxN input matrix is strictly diagonally dominant and plays a bit with it

##### More specifically, for a user-given number of threads:

1. The program checks if the _A_ NxN matrix given by user is [strictly diagonally dominant](https://en.wikipedia.org/wiki/Diagonally_dominant_matrix "wiki") or not
2. If so, it searches for the **max** element of the main diagonal of _A_ and prints it
3. Based on **max**, a new _B_ NxN matrix is being created, where:
* `B[i][j] = max–abs(A[i][j]), i<>j`
* `B[i][j] = max, i=j`
4. On _B_ matrix, the **min** element along with its row and column gets found and printed

##### Tested on Lubuntu 16.04.5 (Xenial Xerus)

##### Compiled and executed with:
* `gcc -o sdd -fopenmp sdd.c` 
* `./sdd`
