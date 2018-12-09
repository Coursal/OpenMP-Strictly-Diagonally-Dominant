#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main( int argc,char *argv[])
{
	int t;				//number of threads for the program

	int N;				//the main dimension for "A", "B" matrices

	int **A;
	int **B;

	int num_of_lines; 		//number of lines each thread has work with

	int m=0;			//max absolute diagonal element of "A"

	int min;			//min element of "B" with its row and column
	int min_row;
	int min_column;

	int flag=1;			//flag that indicates if the "A" matrix is strictly diagonally dominant (1) or not (0)

	int row_sum=0;			//a row's sum of elements' values except the diagonal one


	printf("Type the number of threads you'd like to use: ");
	scanf("%d", &t);

	omp_set_num_threads(t);

	printf("Type the main dimension (N) for your \"A\" input matrix: ");
	scanf("%d", &N);

	//allocating memory dynamically to both of the matrices we will work on
	A = (int **)malloc(N*sizeof(int *));
	for (int i=0; i<N; i++)
         	A[i]=(int *)malloc(N*sizeof(int));

	B = (int **)malloc(N*sizeof(int *));
	for (int i=0; i<N; i++)
         	B[i]=(int *)malloc(N*sizeof(int));

	printf("\n");

	//asking the user to type the values of the "A" matrix
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			printf("A[%d][%d]= ", i, j);
			scanf("%d", &A[i][j]);
		}

		printf("\n");
	}

	//checking "A" matrix if it's strictly diagonally dominant
	#pragma omp parallel shared(A, N, num_of_lines, flag)
	{
		num_of_lines=N/omp_get_num_threads();

		#pragma omp for schedule(static, num_of_lines)
			for(int i=0;i<N;i++)
			{
				int row_sum=0;

				for(int j=0;j<N;j++)
				{
					if(i!=j)
						row_sum+=abs(A[i][j]);
				}

				if(abs(A[i][i])<=row_sum)
					flag=0;
			}
	}

	if(!flag)
	{
		printf("\"A\" matrix is not strictly diagonally dominant\n");
	}
	else
	{
		//printing the "A" matrix
		printf("\n********************************************************\n");
		printf("********************************************************\n");
		printf("The \"A\" %dx%d matrix:\n", N, N);
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
				printf("%d \t", A[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		//finding the absolute maximum value of the diagonal elements in "A"
		#pragma omp parallel shared(A, N, num_of_lines, m)
		{
			num_of_lines=N/omp_get_num_threads();

			#pragma omp for schedule(static, num_of_lines) reduction(max:m)
				for(int i=0;i<N;i++)
					for(int j=0;j<N;j++)
						if(i==j)
							if(abs(A[i][j])>m)
								m=abs(A[i][j]);
		}

		printf("Total (absolute) maximum diagonal element of \"A\" is %d\n", m);
		printf("********************************************************\n");

		//calculating the "B" matrix
		#pragma omp parallel shared(A, B, N, num_of_lines, m)
		{
			#pragma omp for schedule(static, num_of_lines) collapse(2)
				for(int i=0;i<N;i++)
					for(int j=0;j<N;j++)
						if(i==j)
							B[i][j]=m;
						else
							B[i][j]=m-abs(A[i][j]);
		}

		//printing the "B" matrix
		printf("The \"B\" %dx%d matrix:\n", N, N);
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
				printf("%d \t", B[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		//searching for the smallest value and its element's indexes
		#pragma omp parallel shared(B, N, num_of_lines)
		{
			min=9999;	//setting the min variable to an extremely big value just so every value of B is definetely smaller that it
			min_row=0;
			min_column=0;

			#pragma omp for schedule(static, num_of_lines) collapse(2)
				for(int i=0;i<N;i++)
					for(int j=0;j<N;j++)
							if(B[i][j]<min)
							{
								#pragma omp critical (inc_min)
								{
									min=B[i][j];
									min_row=i;
									min_column=j;
								}
							}
		}

		printf("Minimum element of \"B\" is %d at %dx%d\n", min, min_row, min_column);
		printf("********************************************************\n");
		printf("********************************************************\n");
	}

	return(0);
}
