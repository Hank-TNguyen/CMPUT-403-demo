#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <string.h>
#include <list>
#include <vector>

using namespace std;

struct Edge {
    int u, v, weight;
};


// A C++ program print Eulerian Trail in a given Eulerian or Semi-Eulerian Graph
class Graph
{
  int V;    // No. of vertices
  list<int> *adj;    // A dynamic array of adjacency lists
public:
    // Constructor and destructor
  Graph(int V)  { this->V = V;  adj = new list<int>[V];  }
  ~Graph()      { delete [] adj;  }

  // functions to add and remove edge
  void addEdge(int u, int v) {  adj[u].push_back(v); adj[v].push_back(u); }
  void rmvEdge(int u, int v);

  // Methods to print Eulerian tour
  vector<Edge>  printEulerTour();
  void printEulerUtil(int s, vector<Edge> &save);

  // This function returns count of vertices reachable from v. It does DFS
  int DFSCount(int v, bool visited[]);

  // Utility function to check if edge u-v is a valid next edge in
  // Eulerian trail or circuit
  bool isValidNextEdge(int u, int v);
};

/* The main function that print Eulerian Trail. It first finds an odd
   degree vertex (if there is any) and then calls printEulerUtil()
   to print the path */
vector<Edge> Graph::printEulerTour()
{
  // Find a vertex with odd degree
  int u = 0;
  for (int i = 0; i < V; i++)
      if (adj[i].size() & 1)
        {   u = i; break;  }

    vector<Edge> s;

  // Print tour starting from oddv
  printEulerUtil(u, s);
  cout << endl;

  return s;
}

// Print Euler tour starting from vertex u
void Graph::printEulerUtil(int u, vector<Edge> &save)
{
  // Recur for all the vertices adjacent to this vertex
  list<int>::iterator i;
  for (i = adj[u].begin(); i != adj[u].end(); ++i)
  {
      int v = *i;

      // If edge u-v is not removed and it's a a valid next edge
      if (v != -1 && isValidNextEdge(u, v))
      {
          cout << u << "-" << v << "  ";
          save.push_back(Edge());
          save.back().u = u;
          save.back().v = v;
          rmvEdge(u, v);
          printEulerUtil(v, save);
      }
  }
}

// The function to check if edge u-v can be considered as next edge in
// Euler Tout
bool Graph::isValidNextEdge(int u, int v)
{
  // The edge u-v is valid in one of the following two cases:

  // 1) If v is the only adjacent vertex of u
  int count = 0;  // To store count of adjacent vertices
  list<int>::iterator i;
  for (i = adj[u].begin(); i != adj[u].end(); ++i)
     if (*i != -1)
        count++;
  if (count == 1)
    return true;


  // 2) If there are multiple adjacents, then u-v is not a bridge
  // Do following steps to check if u-v is a bridge

  // 2.a) count of vertices reachable from u
  bool visited[V];
  memset(visited, false, V);
  int count1 = DFSCount(u, visited);

  // 2.b) Remove edge (u, v) and after removing the edge, count
  // vertices reachable from u
  rmvEdge(u, v);
  memset(visited, false, V);
  int count2 = DFSCount(u, visited);

  // 2.c) Add the edge back to the graph
  addEdge(u, v);

  // 2.d) If count1 is greater, then edge (u, v) is a bridge
  return (count1 > count2)? false: true;
}

// This function removes edge u-v from graph.  It removes the edge by
// replacing adjcent vertex value with -1.
void Graph::rmvEdge(int u, int v)
{
  // Find v in adjacency list of u and replace it with -1
  list<int>::iterator iv = find(adj[u].begin(), adj[u].end(), v);
  *iv = -1;

  // Find u in adjacency list of v and replace it with -1
  list<int>::iterator iu = find(adj[v].begin(), adj[v].end(), u);
  *iu = -1;
}

// A DFS based function to count reachable vertices from v
int Graph::DFSCount(int v, bool visited[])
{
  // Mark the current node as visited
  visited[v] = true;
  int count = 1;

  // Recur for all vertices adjacent to this vertex
  list<int>::iterator i;
  for (i = adj[v].begin(); i != adj[v].end(); ++i)
      if (*i != -1 && !visited[*i])
          count += DFSCount(*i, visited);

  return count;
}


// Union-find data structure
class UF    {
    int *id, cnt, *sz;
public:
  // Create an empty union find data structure with N isolated sets.
    UF(int N)   {
        cnt = N;
  id = new int[N];
  sz = new int[N];
        for(int i=0; i<N; i++)  {
            id[i] = i;
        sz[i] = 1;
  }
    }
    ~UF() {
  delete [] id;
  delete [] sz;
    }
  // Return the id of component corresponding to object p.
    int find(int p) {
        int root = p;
        while (root != id[root])
            root = id[root];
        while (p != root) {
            int newp = id[p];
            id[p] = root;
            p = newp;
        }
        return root;
    }
  // Replace sets containing x and y with their union.
    void merge(int x, int y)  {
        int i = find(x);
        int j = find(y);
        if (i == j) return;

    // make smaller root point to larger one
        if   (sz[i] < sz[j]) { id[i] = j; sz[j] += sz[i]; }
        else                 { id[j] = i; sz[i] += sz[j]; }
        cnt--;
    }
  // Are objects x and y in the same set?
    bool connected(int x, int y)    {
        return find(x) == find(y);
    }
  // Return the number of disjoint sets.
    int count() {
        return cnt;
    }
};

// comparator function for sorting edges in ascending order by weight
bool comp(Edge *a, Edge *b)   {
    return a->weight < b->weight;
}

int main()  {
    int V, E, i, N, u, v, cost;
    int cost_table[31][31];
    Edge **edges, **mst;

    cin >> V;
    E = V * V;
    edges = new Edge*[E];
    // for(i=0; i<E; i++)  {   // Enter E undirected edges (u, v, weight)
    for (int i = 0; i < V; i++) {
      for (int j = 0; j < V; j++) {
        edges[V * i + j] = new Edge;
        edges[V * i + j]->u = i;
        edges[V * i + j]->v = j;
        cin >> cost_table[i][j];
        edges[V * i + j]->weight = cost_table[i][j];
      }
    }

    // cout << edges[22]->u << " " << edges[22]-> v << " " << edges[22]->weight << endl;
    sort(edges, edges + E, comp);

    UF uf(V);
    mst = new Edge*[V-1];
    for(N=i=cost=0; i<E && N<V-1; i++) {
        u = edges[i]->u; v = edges[i]->v;
        if(!uf.connected(u, v)) {
            uf.merge(u, v);
            mst[N++] = edges[i];
            cost += edges[i]->weight;
        }
    }

    printf("Total cost of MST : %d\n", cost);
    // printf("Edges in MST :\n");

    // for(i = 0; i < N; i++)
    //   cout << mst[i]->u << " " << mst[i]->v << " " << mst[i]->weight << endl;

    Graph g(V);
    for(i = 0; i < N; i++) {
        g.addEdge(mst[i]->u, mst[i]->v);
        g.addEdge(mst[i]->u, mst[i]->v);
    }

    vector<Edge> s;
    cout << ">>>>>>>>>\nThe path is: ";
    s = g.printEulerTour();

    vector<int> visited;
    int curr = -1, weight = 0;
    for (auto e = s.begin() + 1; e != s.end(); e++) {
        if (find(visited.begin(), visited.end(), e->u) == visited.end()) {
            visited.push_back(e->u);
            if (curr >= 0)
                weight += cost_table[curr][e->u];
            curr = e->u;

        }

        if (find(visited.begin(), visited.end(), e->v) == visited.end()) {
            visited.push_back(e->v);
            if (curr >= 0)
                weight += cost_table[curr][e->v];
            curr = e->v;
        }
    }

    weight += cost_table[curr][visited[0]];
    cout << "The reduced path: " << endl;
    for (auto i = visited.begin(); i != visited.end(); i++)
        cout << (*i) << " ";

    cout << endl;
    cout << endl << "The cost is: " << weight << endl;

    if (weight > 2 * cost) {
        cout << "Oops... it cost more than twice MST. ";
        cout << "Don't panic! Triangle inequality does not apply!" << endl;
    }

    return 0;
}