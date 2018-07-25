/*

https://www.ics.uci.edu/~eppstein/161/960206.html
Boruvka's Algorithm
make a list L of n trees, each a single vertex
while (L has more than one tree)
    for each T in L, find the smallest edge connecting T to G-T
    add all those edges to the MST
    (causing pairs of trees in L to merge)

d(c1 ,c2) = nearestint(√(x1−x2 )^2+(y1 − y2 )^2)

http://www.geeksforgeeks.org/greedy-algorithms-set-9-boruvkas-algorithm/

https://github.com/beckysag/traveling-salesman
*/

#include <iostream>     //cout statements
#include <fstream>      //file i/o
#include <sstream>      //sstream function
#include <vector>       //vector functions
#include <string>       //string functions
#include <omp.h>        //parallel functions
#include <math.h>       //square root function
#include <stdlib.h>     //atoi function
#include <map>          //hash map functions
#include <cassert>      //assert functions
using namespace std;

struct Edge{
   int source, destination, weight;
};

struct Graph{
   int V;
   int E;
   Edge* edge;
};

struct subset{
   int parent;
   int rank;
};

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V, int E)
{
    Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}

int noOfVertices;
int noOfEdges;
// Finally there will be one tree that will be MST
int numTrees = 0;
int MSTweight = 0;
int noOfEdge;
Edge *edge;
subset *subsets;
int *cheapSets;

//setting a map for 2 values https://stackoverflow.com/questions/24532164/c-stl-map-one-key-two-values
//possible adjacency matrix to minimize tree creation http://www.geeksforgeeks.org/graph-and-its-representations/
//https://stackoverflow.com/questions/37957080/can-i-use-2-or-more-delimiters-in-c-function-getline
void buildList(string filename, map<int, pair<int,int> > &vertices){
   string line;
   ifstream inputFile(filename.c_str());
   vector <int> newVector;

   if (inputFile.is_open()){
      while(getline(inputFile,line)){
         stringstream ss(line);
         while(getline(ss,line,' ')){
            int currentValue = atoi(line.c_str());
            newVector.push_back(currentValue);
            //cout << currentValue << endl;
         }
      }
   }
   else
      cout << "Unable to open File";

   for (int i = 0; i < newVector.size(); i++){
      if(i%3 == 0){
         vertices[newVector[i]] = make_pair(newVector[i+1],newVector[i+2]);
      }
   }

   return;
}

//d(c1 ,c2) = nearestint(√(x1−x2 )^2+(y1 − y2 )^2)
void makeTree(Graph* graph, map<int, pair<int,int> > &vertices){
   int y = 0;
   int i = 0;
   int weight,x1,x2,y1,y2;
   for(i; i < vertices.size(); i++){
      #pragma omp parallel for
      for(int x = 0; x < vertices.size(); x++){
         if(i == x){
            graph->edge[y].source = i;
            graph->edge[y].destination = x;
            graph->edge[y].weight = 0;
         }
         else{
            graph->edge[y].source = i;
            graph->edge[y].destination = x;
            x1 = vertices[i].first;
            x2 = vertices[x].first;
            y1 = vertices[i].second;
            y2 = vertices[x].second;
            weight = sqrt(pow(x1-x2,2)+pow(y1-y2,2));
            graph->edge[y].weight = weight;
         }
         y++;
      }
   }
}

// A utility function to find set of an element i
// (uses path compression technique)
int find(struct subset subsets[], int i)
{
    // find root and make root as parent of i
    // (path compression)
    if (subsets[i].parent != i)
      subsets[i].parent =
             find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high
    // rank tree (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and
    // increment its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// The main function for MST using Boruvka's algorithm
void boruvkaMST(struct Graph* graph)
{
   // Get data of given graph
   int V = graph->V, E = graph->E;
   Edge *edge = graph->edge;

   // Allocate memory for creating V subsets.
   struct subset *subsets = new subset[V];

   // An array to store index of the cheapest edge of
   // subset.  The stored index for indexing array 'edge[]'
   int *cheapest = new int[V];

   // Create V subsets with single elements
   for (int v = 0; v < V; ++v)
   {
      subsets[v].parent = v;
      subsets[v].rank = 0;
      cheapest[v] = -1;
   }

   // Initially there are V different trees.
   // Finally there will be one tree that will be MST
   int numTrees = V;
   int MSTweight = 0;

   // Keep combining components (or sets) until all
   // compnentes are not combined into single MST.
   while (numTrees > 1)
   {
      // Traverse through all edges and update
      // cheapest of every component
      for (int i=0; i<E; i++)
      {
           // Find components (or sets) of two corners
           // of current edge
           int set1 = find(subsets, edge[i].source);
           int set2 = find(subsets, edge[i].destination);

           // If two corners of current edge belong to
           // same set, ignore current edge
           if (set1 == set2)
               continue;

           // Else check if current edge is closer to previous
           // cheapest edges of set1 and set2
           else
           {
              if (cheapest[set1] == -1 ||
                  edge[cheapest[set1]].weight > edge[i].weight)
                cheapest[set1] = i;

              if (cheapest[set2] == -1 ||
                  edge[cheapest[set2]].weight > edge[i].weight)
                cheapest[set2] = i;
           }
      }

      // Consider the above picked cheapest edges and add them
      // to MST
      for (int i=0; i<V; i++)
      {
           // Check if cheapest for current set exists
           if (cheapest[i] != -1)
           {
               int set1 = find(subsets, edge[cheapest[i]].source);
               int set2 = find(subsets, edge[cheapest[i]].destination);

               if (set1 == set2)
                   continue;
               MSTweight += edge[cheapest[i]].weight;
               printf("Edge %d-%d included in MST\n",
                      edge[cheapest[i]].source, edge[cheapest[i]].destination,
                      edge[cheapest[i]].weight);

               // Do a union of set1 and set2 and decrease number
               // of trees
               Union(subsets, set1, set2);
               numTrees--;
           }
      }

          for (int i=0; i<V; i++)
        {
            cheapSets[i]=-1;

        }
    }

    printf("Weight of MST is %d\n", MSTweight);
    return;
}

//trying to print out the map
int main(int argc, char const *argv[]) {
   #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
   #endif

   omp_set_num_threads(24);
   //cout << "Enter textfile name: NOT WORKING FOR NOW" << endl;
   //cin >> filename;
   string filename = "tsp_example_1.txt";
   map<int, pair<int,int> > currentVertices;
   buildList(filename, currentVertices);
   int V = currentVertices.size();
   int E = pow(V,2);
   struct Graph *graph = createGraph(V, E);
   makeTree(graph, currentVertices);
   boruvkaMST(graph);

   return 0;
}
