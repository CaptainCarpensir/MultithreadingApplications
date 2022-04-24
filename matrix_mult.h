#ifndef MATRIX_MULT
#define MATRIX_MULT

#include <thread>
#include <vector>

void mult_thread(int **m1, int **m2, int **res, int m, int p, int n, int t, int id)
{
	//Rows to be calculated by the thread
	for (int i = id; i < m; i += t)
	{
		//Loop through columns
		for(int j = 0; j < n; j++)
		{
			int val = 0;
			for(int k = 0; k < p; k++)
			{
				val += m1[i][k] * m2[k][j];
			}
			res[i][j] = val;
		}
	}
}

int** matrix_mult(int **m1, int **m2, int m, int p, int n, int threads)
{
	/*
	*	Philosophy of the multithreads:
	*		- create a pool of threads, each one will multiply rows (n%threads)+thread_id
	*		- no need for critical section control because separate rows are controlled independently
	*/

	//Instantiate matrix
	int **result = new int*[m];
	for (int i = 0; i < m; i++)
	{
		result[i] = new int[n];
	}

	//Create thread pool to multiply
	vector<thread> pool;
	for(int i = 0; i < threads; i++)
	{
		pool.push_back(thread(mult_thread, m1, m2, result, m, p, n, threads, i));
	}
	
	//Join threads
	for(int i = 0; i < threads; i++)
	{
		pool[i].join();
	}

	return result;
}

#endif