# dijkstra-and-A--algorithm
Implementation of Dijkstra and A* algorithms in C++ using BOOST library

The purpose of this repository is to compare [Dijkstra](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) Algorithm with [A*](https://en.wikipedia.org/wiki/A*_search_algorithm) Algorithm, for the solution of finding the shortest path between two nodes s and t, in graph G = (V, E). And practice in BOOST library.

Dijkstra will terminate when we find the shortest path between s and t, because we don't care about the shortest path of s with all nodes, will call that methode Dijkstra-Sp. Let h(i) be the lower bound of distand of node i from t, h(i) <= h(j) + wt(i,j),  ∀(i, j) ∈ E , and ![CodeCogsEqn](https://user-images.githubusercontent.com/44173610/66704862-b0ab4580-ed28-11e9-8509-f2bd46431ed0.gif). 

A* algorithm will use h to change the cost of edge (i,j)∈ E  into wt'(i, j) = wt(i, j) + h(j) − h(i), and then use Dijkstra-SP. In the end the right distand from node s to node t, is the distand that A* calculate reduced by h(t) − h(s).

The graph will be a grid r x c(rows x columns),  (r, c) ∈ {(30, 1000), (60, 1000), (80, 1000)}. We let top-left node have (0,0) coordinates and bottom-right have (r-1, c-1) coordinates. The weight of the edges will be a)random between (1,..,10) b)random between (1, ..., 100). The node s will be choosen randomly from column 0 and the t node from column c - 1.
 
