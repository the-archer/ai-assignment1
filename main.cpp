#include <iostream>
#include <fstream>
#include <string>
#include <hash_map>
#include <set>
#include <queue>

using namespace std;


hash_map<string, int> NodeMap; //Hash map is deprecated. Can use unordered_map / map instead
vector<vector<pair<int, int>>> Graph;
set<int> KNodes;
int K;
/*struct comparator{
	bool operator() (const pair<int, int>& lhs, pair<int, int>& rhs){
		return lhs.second < rhs.second;
	};*/

int Algo1();
//int Algo2();
float CalcAvgDist(int);
vector<int> Dijkstra(int);

int main(){


	/*Read the graph from csv*/
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


	/* User I/O */
	cout << "Enter value of K:";
	cin >> K;

	cout << "Enter the ids for the k nodes from the following nodes:" << endl;
	for (auto i = NodeMap.begin(); i != NodeMap.end(); i++){
		cout << i->second << "\t\t" << i->first << endl;
	}
	for (x = 0; x < K; x++){
		cin >> iter;
		KNodes.insert(iter);
	}

	cout << "Output of Algorithm 1:\t" << Algo1() << endl;
	//cout << "Output of Algorithm 2:\t" << Algo2() << endl;

	cin >> value;
	return 0;
}

int Algo1(){
	vector<float> averageDist = vector<float> (NodeMap.size(),INT_MAX);
	for (int i = 0; i != averageDist.size(); i++){
		if (KNodes.find(i) != KNodes.end())
			averageDist[i] = INT_MAX;
		else
			averageDist[i] = CalcAvgDist(i);
	}
	
	int min = INT_MAX, minNode=0;
	for (int i = 0; i < averageDist.size(); i++){
		if (min>averageDist[i]){
			min = averageDist[i];
			minNode = i;
		}
	}
	return minNode;
}

float CalcAvgDist(int NodeID){
	vector<int> Dist = Dijkstra(NodeID);
	float avg = 0;
	int num = 0;
	for (int i = 0; i < Dist.size(); i++){
		if (KNodes.find(i) != KNodes.end()){
			avg += Dist[i];
			num++;
		}
	}
	if (num != 0)
		avg = avg / num;

	cout << "Avg of Node " << NodeID << ":\t\t" << avg << endl;

	return avg;
}

vector<int> Dijkstra(int NodeID){
	//priority_queue<pair<int,int>, std::vector<pair<int,int>>, comparator> minHeap;
	vector<int> Dist = vector<int>(NodeMap.size(),INT_MAX);
	vector<bool> explored = vector<bool>(NodeMap.size(), false);
	Dist[NodeID] = 0;
	/*minHeap.push(makepair(NodeID, 0));
	for (auto i = NodeMap.begin(); i != NodeMap.end(); i++)
		if (i->second!=NodeID)
			minHeap.push(makepair(i->second, INT_MAX));*/
	
	//set<int> Explored;
	int total = 0;
	while(total != NodeMap.size()){
		
		int min = INT_MAX;
		int minNode = 0;
		while (minNode < Dist.size() && explored[minNode])
			minNode++;
		for (int l = minNode; l < Dist.size(); l++){
			if (!explored[l] && min > Dist[l]){
				min = Dist[l];
				minNode = l;
			}
		}
		int i = minNode;
		explored[i] = true;
		for (int j = 0; j < Graph[i].size(); j++){
			if (!explored[Graph[i][j].first] && Dist[i] != INT_MAX && Dist[Graph[i][j].first]>Dist[i] + Graph[i][j].second)
				Dist[Graph[i][j].first] = Dist[i] + Graph[i][j].second;
		}
		total++;
	}
	return Dist;
}