#ifndef T_SALESMAN
#define T_SALESMAN

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>

mutex mtx;

int nest(int **graph, int n, int curr, bool *visited)
{
	//Copy version of visited to give to further recursions
	bool all_visited = true;
	bool *visit_nest = new bool[n-1];
	int val = INT16_MAX;

	for(int i = 0; i < n-1; i++)
	{
		visit_nest[i] = visited[i];
	}

	//On each ith node, add the distance b/w unvisited nodes and curr node
	for(int i = 0; i < n-1; i++)
	{
		if(!visited[i])
		{	//Need to return the minimum of what's recursed
			visit_nest[i] = true;
			all_visited = false;
			int best = nest(graph, n, i, visit_nest) + graph[i][curr];
			visit_nest[i] = false;
			if(best < val) val = best;
		}
	}

	//Max recursion depth reached, the current node meets back with base node
	if(all_visited) val = graph[curr][n-1];
	delete[] visit_nest;

	return val;
}

void trav_thread(int **graph, int n, int t, int *min, int id)
{
	//Node n-1 is always to be the assumed starting node
	//Each thread will then start a path from the remaining pool of nodes
	bool *visited = new bool[n-1];
	for(int i = 0; i < n-1; i++) visited[i] = false;
	for(int i = id; i < n-1; i += t)
	{
		//Visited needs to be reset for each continual loop
		visited[i] = true;
		int val = nest(graph, n, i, visited) + graph[i][n-1];
		visited[i] = false;

		//Mutex lock to make sure threads don't lead to race conditions
		mtx.lock();
		if(val < *min) 
		{
			*min = val;
		}
		mtx.unlock();
	}
	delete[] visited;
}

int trav_salesman(int **graph, int n, int threads)
{
	int *val = new int;
	*val = INT16_MAX;

	vector<thread> pool;
	for(int i = 0; i < threads; i++)
	{
		pool.push_back(thread(trav_thread, graph, n, threads, val, i));
	}
	
	//Join threads
	for(int i = 0; i < threads; i++)
	{
		pool[i].join();
	}

	int return_val = *val;
	delete val;

	return return_val;
}

#endif