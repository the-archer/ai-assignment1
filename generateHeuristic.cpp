//Note : Compile with g++ -std=c++11 generateHeuristic.cpp -o generateHeuristic

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <climits>


using namespace std;


map<string, int> NodeMap;
vector<vector<pair<int, int>>> Graph;
int min_edge_weight;

void LoadGraph(void);
void GenerateHeuristic(void);
int GetMinEdgeWeight(void);
vector<int> ApplyBFS(int rootnode);


int main()
{

	LoadGraph();
	min_edge_weight = GetMinEdgeWeight();
	GenerateHeuristic();


	return 0;
}

void LoadGraph(void)
{

	ifstream file("file.csv"); // declare file stream
	string value, temp;
	int iter, cursrc, curdst, x;
	bool flag;
	
		
	while (file.good())
	{
		getline(file, value);
		iter = 0;
		flag = true;
		while (value.find(",", 0) != std::string::npos)
		{ //does the string have a comma in it?
			size_t  pos = value.find(",", 0);		//store the position of the delimiter
			temp = value.substr(0, pos);			//get the token
			value.erase(0, pos + 1);				//erase it from the source 
			flag = true;

			if (iter == 0){
				if (NodeMap.find(temp) == NodeMap.end()){
					NodeMap[temp] = Graph.size();
					vector<pair<int, int>> keepEmpty;
					Graph.push_back(keepEmpty);
				}
				cursrc = NodeMap[temp];
			}
			else if (iter % 2 == 1){
				if (NodeMap.find(temp) == NodeMap.end()){
					NodeMap[temp] = Graph.size();
					vector<pair<int, int>> keepEmpty;
					Graph.push_back(keepEmpty);
				}		
				curdst = NodeMap[temp];
				
				for (x = 0; x < Graph[curdst].size() && Graph[curdst][x].first != cursrc; x++);
				if (x == Graph[curdst].size()){
					Graph[curdst].push_back(std::make_pair(cursrc, -1));
					Graph[cursrc].push_back(std::make_pair(curdst, -1));
				}
				else
					flag = false;				
			}
			else if (flag){
				Graph[curdst].back().second = stoi(temp);
				Graph[cursrc].back().second = stoi(temp);
			}
			iter++;
		}

		if (iter == 0){
			if (NodeMap.find(value) == NodeMap.end()){
				NodeMap[value] = Graph.size();
				vector<pair<int, int>> keepEmpty;
				Graph.push_back(keepEmpty);
			}
		}
		else if (iter % 2 == 1){
			if (NodeMap.find(value) == NodeMap.end()){
				NodeMap[value] = Graph.size();
				vector<pair<int, int>> keepEmpty;
				Graph.push_back(keepEmpty);
			}
			curdst = NodeMap[value];

			for (x = 0; x < Graph[curdst].size() && Graph[curdst][x].first != cursrc; x++);
			if (x == Graph[curdst].size()){
				Graph[curdst].push_back(std::make_pair(cursrc, -1));
				Graph[cursrc].push_back(std::make_pair(curdst, -1));
			}
			else
				flag = false;	
		}
		else if (flag){
			Graph[curdst].back().second = stoi(value);
			Graph[cursrc].back().second = stoi(value);
		}
	}
	file.close();



	return;
}

void GenerateHeuristic(void)
{	
	ofstream f1;
	f1.open("heuristic.txt");
	for(int j=0; j<NodeMap.size(); j++)
	{ 
		vector<int> hvalues = ApplyBFS(j);
		f1 << j;
		for(int i=0; i<hvalues.size(); i++)
		{
			f1 << " " << hvalues[i];
		}
		f1 << endl;
	}
	f1.close();



	return;
}

int GetMinEdgeWeight(void)
{
	int minx=INT_MAX;
	for(auto x: Graph)
	{
		for(auto y: x)
		{
			if(y.second<minx)
			{
				minx=y.second;
			}
		}	
	}
	cout <<  minx << endl;
	return minx;

}

vector<int> ApplyBFS(int root_node)
{
	vector<int> hvalues(NodeMap.size());	
	queue<int> Q;
	set<int> explored;
	set<int> val_calculated;
	//int depth=0;
	//cout  << x.first << endl;
	Q.push(root_node);
	hvalues[root_node] = 0;
	val_calculated.insert(root_node);
	int cur_node;
	while(!Q.empty())
	{
		cur_node=Q.front();
		Q.pop();
		if(explored.find(cur_node) == explored.end())
		{
			

			for(auto x: Graph[cur_node])
			{
				if(val_calculated.find(x.first) == val_calculated.end())
				{
					hvalues[x.first] = hvalues[cur_node]+min_edge_weight;
					val_calculated.insert(x.first);
					Q.push(x.first);
				}
				
			}
			explored.insert(cur_node);


		}
	}

	return hvalues;
}