#include <iostream>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include <utility>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <set>
#include <math.h>
#include <ctime>

std::pair<int, int> FindCordinates(int numRow, int u){

	int count = -1;
	std::pair<int, int> x;

	for(int i = 0; i < numRow; i++){
			for (int j = 0; j < 1000; j++) {
				count++;
				if (count == u) {
					x = std::make_pair (i,j);
					break;
				}
			}
			if(count == u) break;
	}
	return x;
}

void printPrev(int target, int *prev){
	std::cout << target << "<--";
	if (prev[target] != -1) {
		printPrev(prev[target], prev);
	}
}


int main(int argc, char const *argv[]) {
	typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS,
																boost::no_property,
                                boost::property<boost::edge_weight_t, int>/
                                > Graph;

	Graph G;

	std::clock_t start, stop;
  double duration;


	std::cout << "Choose:\n1)30, 1000\n2)60, 1000\n3)80, 1000" << '\n';

	int input, maxRandom, numEdge, numNodes, numRow;
	std::cin >> input;

	std::cout << "Choose range for weight of edges:\n1)[1,10]\n2)[1,100]" << '\n';
	std::cin >> maxRandom;

	//initialize range of wieght
	if(maxRandom == 1) maxRandom = 10;
	else if(maxRandom == 2) maxRandom = 100;

	//initialize number of edges, nodes and rows
	if(input == 1) {
		numRow = 30;
		numNodes = 30000;
		numEdge = 58970;
	}
	else if(input == 2){
		numRow = 60;
		numNodes = 60000;
		numEdge = 118940; //60,1000
	}
	else if(input == 3) {
		numRow = 80;
		numNodes = 80000;
		numEdge = 158920; //80,1000
	}

	srand ( time(NULL) );
	int src = rand() % numRow;
	src = src*1000;
	srand ( time(NULL) + 10);
	int target = rand() % numRow;
	target = (target+1)*1000 - 1;

	int xArxiko, yArxiko;
	std::tie(xArxiko, yArxiko) = FindCordinates(numRow, src);

	int xTeliko, yTeliko;
	std::tie(xTeliko, yTeliko) = FindCordinates(numRow, target);

	boost::adjacency_list<>::vertex_descriptor V[numRow][1000];
	std::pair<Graph::edge_descriptor, bool> E[numEdge];


	//Create NODES
	for(int i = 0; i < numRow; i++){
		for(int j = 0; j < 1000; j++){
			V[i][j] = boost::add_vertex(G);
		}
	}

	//Create Edges
	int k = 0;
	for(int i = 0; i < numRow; i++){
		for(int j = 0; j < 1000; j++){
				if(i == numRow-1){
					if(j < 999){
						int rand1 = rand() % maxRandom + 1;
						boost::property<boost::edge_weight_t, int> edge_property(rand1);
						E[k] = boost::add_edge(V[i][j], V[i][j+1], edge_property, G);
						k++;
					}
				}
				else if(i <= numRow-2) {
					if(j<999){
						int rand1 = rand() % maxRandom + 1;
						boost::property<boost::edge_weight_t, int> edge_property(rand1);
						E[k] = boost::add_edge(V[i][j], V[i][j+1], edge_property, G);
						k++;
					}
					int rand1 = rand() % maxRandom + 1;
					boost::property<boost::edge_weight_t, int> edge_property(rand1);
					E[k] = boost::add_edge(V[i][j], V[i+1][j], edge_property, G);
					k++;
				}
		}
	}




	//PROPERTY MAPS DECLARE

	typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;
	IndexMap name_map = boost::get(boost::vertex_index, G);

	typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
	WeightMap weight_map = boost::get(boost::edge_weight, G);

	//ITERATOR DECLARE FOR OUT EDGES
	typedef boost::graph_traits<Graph>::out_edge_iterator oEdgeIter;
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	oEdgeIter oei, oei_end;



	std::cout << "\nFirst node: " << src <<"\nLast node:" << target << '\n';


start = std::clock();
/*
====================================================================================================
																						SP-Dijkstra
====================================================================================================
*/
	std::set<int> Q;

	//Make Q set with vertex
	for(int i = 0; i < numNodes; i++){
		Q.insert(i);
	}

	//initialize dist with INF, and previus with -2
	int dist[numNodes], prev[numNodes];
	std::fill_n(dist, numNodes, INT_MAX);
	std::fill_n(prev, numNodes, -2);

	dist[src] = 0; //source dist = 0
	prev[src] = -1;

	int u, visit = 0;
	while(!Q.empty()){

		//vertex in Q with min dist[u]
		std::set<int>::iterator min = Q.begin();
		for(std::set<int>::iterator it = Q.begin(); it!=Q.end(); ++it){
			if(dist[*it] < dist[*min]) min = it;
		}
		int u = *min;
		visit++;

		if(u == target) break;
		if(dist[u] == INT_MAX){
			std::cout << "No path from node: " << src << " to node: " << target << '\n';
			break;
		}
		//remove min from Q
		Q.erase(min);

		//FIND CORDINATES FOR u
		int xU, yU;
		std::tie(xU, yU) = FindCordinates(numRow, u);

		//CHECK ALL OUT EDGES OF u
		for(boost::tie(oei, oei_end) = boost::out_edges(V[xU][yU], G); oei != oei_end; ++oei){
			Vertex tmpTarget = boost::target(*oei, G);

			//FIND THE INDEX OF THE EDGE, THAT I WANT THE WEIGHT
			int keep;
			for(int i=0; i<numEdge; i++){
				if(boost::source(E[i].first, G) == u && boost::target(E[i].first, G) == name_map[tmpTarget])
					{
						keep = i;
						break;
					}
			}

			//FIND THE WEIGHT OF u ---> target
			int pWeight = 0;
			pWeight = get(weight_map, E[keep].first );

			//CALCULATE NEW SHORT DIST OF NEIGHBOR VERTICES
			int tmpDist;
			tmpDist = dist[u] + pWeight;
			if (tmpDist < dist[name_map[tmpTarget]]) {
				dist[name_map[tmpTarget]] = tmpDist;
				prev[name_map[tmpTarget]] = u;
			}
		}
	}
	std::cout << "\nDijkstra output: dist[" << target << "] = " << dist[target] << " and visit " << visit << " nodes" << '\n';


	stop = std::clock();
	duration = (stop - start) / (double)CLOCKS_PER_SEC;

  std::cout << "Runtime of Dijkstra is: " << duration*1000.0 << " ms \n";


	start = std::clock();


	/*
	====================================================================================================
																						A* Algorithm
	====================================================================================================
	*/

	std::cout << "\n\nA* START\n" << '\n';
	Q.clear();

	//Make Q set with vertex
	for(int i = 0; i < numNodes; i++){
		Q.insert(i);
	}

	//initialize dist with INF, and previus with -2
	std::fill_n(dist, numNodes, INT_MAX);
	std::fill_n(prev, numNodes, -2);

	dist[src] = 0; //source dist = 0
	prev[src] = -1;

	int visit2 = 0;
	while(!Q.empty()){

		//vertex in Q with min dist[u]
		std::set<int>::iterator min = Q.begin();
		for(std::set<int>::iterator it = Q.begin(); it!=Q.end(); ++it){
			if(dist[*it] < dist[*min]) min = it;
		}
		int u = *min;
		visit2++;
		//an einai to telikos kombos poy anazitame, brikame to dist poy thelame
		if(u == target) break;
		if(dist[u] == INT_MAX){
			std::cout << "No path from node: " << src << " to node: " << target << '\n';
			break;
		}

		//remove min from Q
		Q.erase(min);

		//FIND CORDINATES FOR u
		int xU, yU;
		std::tie(xU, yU) = FindCordinates(numRow, u);

		//CHECK ALL OUT EDGES OF u
		for(boost::tie(oei, oei_end) = boost::out_edges(V[xU][yU], G); oei != oei_end; ++oei){

			Vertex tmpTarget = boost::target(*oei, G);

			//FIND CORDINATES FOR TARGET OF u
			int xT, yT;
			std::tie(xT, yT) = FindCordinates(numRow, name_map[tmpTarget]);

			//FIND THE INDEX OF THE EDGE, THAT I WANT THE WEIGHT
			int keep;
			for(int i=0; i<numEdge; i++){
				if(boost::source(E[i].first, G) == u && boost::target(E[i].first, G) == name_map[tmpTarget])
					{
						keep = i;
						break;
					}
			}

			//FIND THE WEIGHT OF u ---> target
			int pWeight = 0;
			pWeight = get(weight_map, E[keep].first );
			pWeight = pWeight + sqrt(pow(xT-xTeliko, 2) + pow(yT-yTeliko, 2)) - sqrt(pow(xU-xTeliko, 2) + pow(yU-yTeliko, 2));
			//CALCULATE NEW SHORT DIST OF NEIGHBOR VERTICES
			int tmpDist;
			tmpDist = dist[u] + pWeight;
			if (tmpDist < dist[name_map[tmpTarget]]) {
				dist[name_map[tmpTarget]] = tmpDist;
				prev[name_map[tmpTarget]] = u;
			}
		}
	}
	stop = std::clock();
	duration = (stop - start) / (double)CLOCKS_PER_SEC; //dia to CLOCKS_PER_SEC pernoyme xrono

  std::cout << "Runtime of A* is: " << duration*1000.0 << " ms \n";

	std::cout << "\nA* output: dist[" << target << "] = " << dist[target] - sqrt(pow(xArxiko-xTeliko, 2) + pow(yArxiko-yTeliko, 2))
	<< " and visit " << visit2 << " nodes" << '\n';


	return 0;
}
