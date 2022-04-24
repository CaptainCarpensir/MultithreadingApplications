// Imports & Namespace
using namespace std;
#include "travelling_salesman.h"
#include "matrix_mult.h"
#include "merge_sort.h"
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>

/*
*	Main Function
*/

//Function that makes two nxn matrices, then returns the time taken to multiply them
long matrix(int n, int threads);

//Function that creates a random n size array, then returns the time taken to sort
long sort(int n, int threads);

//Function that creates a graph, then returns time taken to brute force travelling salesman
long travel(int n, int threads);

//Function that polls for the 
void poll_simulation(int &max_t, int &sort_size, int &mult_size, int &trav_size, int &trials);

int main()
{
	srand(time(NULL)); // Seeding random

	int min_thrds = 1;
	int max_thrds = 1;

	int sort_size = 1;
	int mult_size = 1;
	int trav_size = 1;

	int trials = 1;

	poll_simulation(max_thrds, sort_size, mult_size, trav_size, trials);

	ofstream sort_writer;
	ofstream matrix_writer;
	ofstream travel_writer;

	sort_writer.open("sort_results.csv");
	matrix_writer.open("matrix_results.csv");
	travel_writer.open("travel_results.csv");
	sort_writer << "prob size=" << sort_size << ",trials=" << trials << endl;
	sort_writer << "threads,time(milliseconds)" << endl; 
	matrix_writer << "prob size=" << mult_size << ",trials=" << trials << endl;
	matrix_writer << "threads,time(milliseconds)" << endl; 
	travel_writer << "prob size=" << trav_size << ",trials=" << trials << endl;
	travel_writer << "threads,time(milliseconds)" << endl; 
	for(int t = min_thrds; t <= max_thrds; t++)
	{
		long long sort_avg = 0;
		long long matrix_avg = 0;
		long long travel_avg = 0;
		for(int i = 0; i < trials; i++)
		{
			//Merge Sort
			sort_avg += sort(sort_size, t);

			//Matrix Multiplication
			matrix_avg += matrix(mult_size, t);

			//Travelling Salesman
			travel_avg += travel(trav_size, t);
		}
		sort_avg /= trials;
		matrix_avg /= trials;
		travel_avg /= trials;
		sort_writer << t << "," << (float)(sort_avg)/1000000 << endl;
		matrix_writer << t << "," << (float)(matrix_avg)/1000000 << endl;
		travel_writer << t << "," << (float)(travel_avg)/1000000 << endl;
	}
	sort_writer.close();
	matrix_writer.close();
	travel_writer.close();

	return 0;
}

long sort(int n, int threads)
{
	//Declare max and min for values inside array
	const int MAX_SIZE = 9;
	const int MIN_SIZE = 0;

	//Initialize unsorted array
	int* arr = new int[n];
	for(int i=0; i<n; i++)
	{
		arr[i] = rand()%(MAX_SIZE-MIN_SIZE+1)+MIN_SIZE;
	}

	auto start = chrono::high_resolution_clock::now();
	merged_quick_sort(arr, n, threads);
	auto dur = chrono::high_resolution_clock::now() - start;

	//Delete array
	delete[] arr;

	return dur.count();
}

long matrix(int n, int threads)
{
	//Create matrices
	int **m1 = new int*[n];
	int **m2 = new int*[n];
	for(int i = 0; i < n; i++)
	{
		m1[i] = new int[n];
		m2[i] = new int[n];
		for(int j = 0; j < n; j++)
		{
			m1[i][j] = rand() % 10;
			m2[i][j] = rand() % 10;
		}
	}

	auto start = chrono::high_resolution_clock::now();
	int **result = matrix_mult(m1, m2, n, n, n, threads);
	auto dur = chrono::high_resolution_clock::now() - start;

	//Properly delete matrices
	for(int i = 0; i < n; i++)
	{
		delete[] m1[i];
		delete[] m2[i];
		delete[] result[i];
	}
	delete[] m1;
	delete[] m2;
	delete[] result;

	return dur.count();
}

long travel(int n, int threads)
{
	//Create adjacency matrix
	int **adj_matrix = new int*[n];
	for(int i = 0; i < n; i++)
	{
		adj_matrix[i] = new int[n];
		for(int j = 0; j < i; j++)
		{
			int num = (rand() % 30) + 1;
			adj_matrix[i][j] = num;
			adj_matrix[j][i] = num;
		}
		adj_matrix[i][i] = 0;
	}

	auto start = chrono::high_resolution_clock::now();
	trav_salesman(adj_matrix, n, threads);
	auto dur = chrono::high_resolution_clock::now() - start;

	//Delete matrices
	for(int i = 0; i < n; i++)
	{
		delete[] adj_matrix[i];
	}
	delete[] adj_matrix;

	return dur.count();
}

void poll_simulation(int &max_t, int &sort_size, int &mult_size, int &trav_size, int &trials)
{
	max_t = 0;
	mult_size = 0;
	sort_size = 0;
	trav_size = 0;
	trials = 0;

	while(max_t <= 0)
	{
		cout << "Enter the max number of threads: ";
		cin >> max_t;
		cout << endl;
	}	

	while(sort_size <= 0)
	{
		cout << "Enter the problem size for sorting: ";
		cin >> sort_size;
		cout << endl;
	}

	while(mult_size <= 0)
	{
		cout << "Enter the problem size for matrix multiplication: ";
		cin >> mult_size;
		cout << endl;
	}

	while(trav_size <= 0)
	{
		cout << "Enter the problem size for the travelling salesman: ";
		cin >> trav_size;
		cout << endl;
	}

	while(trials <= 0)
	{
		cout << "Enter the number of trials: ";
		cin >> trials;
		cout << endl;
	}
}