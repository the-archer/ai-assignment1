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
vector<int> ApplyBFS(std::pair<const std::basic_string<char>, int>);


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
	for(auto x: NodeMap)
	{ 
		vector<int> hvalues = ApplyBFS(x);
	}


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
	cout <<  minx;
	return minx;

}

vector<int> ApplyBFS(std::pair<const std::basic_string<char>, int> x)
{
	vector<int> hvalues(NodeMap.size());	
	queue<int> frontier;
	set<int> explored;
	//cout  << x.first << endl;
	return hvalues;
}