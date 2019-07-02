#include <stdlib.h>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#include<bits/stdc++.h> 

using namespace std;

bool HasPath(int V, int** edges, int* visited, int v1, int v2)
{
    if (edges[v1][v2] == 1)
        return true;

    for (int i = 0; i<V; i++)
    {
        if (visited[i] != 1 && edges[v1][i] == 1)
        {
            visited[i] = 1;
            if (HasPath(V, edges, visited, i, v2))
                return true;
        }
    }
    return false;
}

void addEdge(vector<int> adj[],int u,int v)
{
    adj[u].push_back(v); 
    adj[v].push_back(u); 
}

void printGraph(vector<int> adj[],int V)
{
	for (int v = 0; v < V; ++v) 
    { 
        cout << "\n Adjacency list of vertex "
             << v << "\n head "; 
        for (auto x : adj[v]) 
           cout << "-> " << x; 
        printf("\n"); 
    } 	
}
