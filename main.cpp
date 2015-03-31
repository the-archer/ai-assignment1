//Note : Compile with g++ -std=c++11 main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <climits>
#include <algorithm>


using namespace std;


struct compare2 {
        bool operator()(std::pair<int, int> lhs, std::pair<int, int> rhs) const {
            return lhs.second < rhs.second;
        }
};


map<string, int> NodeMap; //Hash map is deprecated. Can use unordered_map / map instead
vector<vector<pair<int, int>>> Graph;
set<int> KNodes;
int K;

vector<vector<int>> hvalues;

/*struct comparator{
	bool operator() (const pair<int, int>& lhs, pair<int, int>& rhs){
		return lhs.second < rhs.second;
	};*/




int Algo1();
//int Algo2();
float CalcAvgDist(int);
vector<int> Dijkstra(int);
void LoadGraph(void);
int Algo2(double threshold);
void LoadHeuristicValues();
bool GoalTest(map<int, set<int>>& explored, double threshold, int& ans);
int GetHValue(int node, vector<int>& minhvalues);


int main(){

	LoadGraph();
	/*Read the graph from csv*/
	

	string value;
	int iter;
	/* User I/O */
	cout << "Enter value of K:";
	cin >> K;

	cout << "Enter the ids for the k nodes from the following nodes:" << endl;
	for (auto i = NodeMap.begin(); i != NodeMap.end(); i++){
		cout << i->second << "\t\t" << i->first << endl;
	}
	for (int x = 0; x < K; x++){
		cin >> iter;
		KNodes.insert(iter);
	}

	//cout << "Output of Algorithm 1:\t" << Algo1() << endl;
	cout << "Output of Algorithm 2:\t" << Algo2(1) << endl;

	cin >> value;
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

int Algo2(double threshold)
{
	LoadHeuristicValues();

	/*for(int i=0; i<NodeMap.size(); i++)
	{
		for(int j=0; j<NodeMap.size(); j++)
		{
			cout << hvalues[i][j] << " " ;
		}
		cout << endl;
	}*/
	//map<int, priority_queue<pair<int, int>, std::vector<int>, compare2>> frontier; //Issues with priority queue. Will try to solve them later.
	//map<int, vector<tuple<int, int, int>>> frontier;
	map<int, vector<vector<int>>> frontier;
	map<int,set<int>> explored;
	//priority_queue<pair<int, int>, std::vector<int>, compare2> temp2;
	vector<int> minhvalues(NodeMap.size(), 0);
	for(auto x: KNodes)
	{
		set<int> temp;
		//temp.insert(x);
		explored.insert(pair<int, set<int>>(x, temp));
		vector<vector<int>> temp2;
		//vector<tuple<int, int, int>> temp2;
		
		/*for(auto y: Graph[x])
		{
			int h = GetHValue(y.first, minhvalues);
			vector<int> temp3;
			temp3.push_back(y.first);
			temp3.push_back(0);
			temp2.push_back(make_tuple(y.first, h+y.second, y.second));


		}*/
		//priority_queue<pair<int, int>, std::vector<int>, compare2> temp2;
		
		vector<int> temp3;
		temp3.push_back(x);
		temp3.push_back(0); //g
		int h = GetHValue(x, minhvalues);
		temp3.push_back(h+0);
		temp2.push_back(temp3);
		//frontier.insert(pair<int, vector<tuple<int, int, int>>>(x, temp2));
		frontier.insert(pair<int, vector<vector<int>>>(x,  temp2));
		//frontier.insert(pair<int, priority_queue<pair<int, int>, std::vector<int>, compare2>>(x, temp2));

	}

	int ans=0;
	while(!GoalTest(explored, threshold, ans))
	{
		for(auto x: KNodes)
		{
			int minx=INT_MAX;
			int selected=-1;
			for(auto y: frontier[x])
			{
				if(y[2] < minx)
				{
					minx = y[2];
					selected = y[0];
				}
			}

			explored[x].insert(selected);

			for(auto y: Graph[selected]) //vector<vector<pair<int, int>>> Graph; map<int, vector<vector<int>>> frontier;
			{
				if(explored[x].find(y)!=explored[x].end())
					continue;
				int g = frontier[x][selected][1] + y.second;
				int h = GetHValue(y.first, minhvalues);
				int newf=g+h;
				bool flag=false;
				for(auto z: frontier[x])
				{
					if(z[0]==y)
					{
						z[2] = min(z[2], newf);
						flag=true;

						break;
					}
				}
				if(!flag)
				{
					vector<int> temp;
					temp.push_back(y);
					temp.push_back(g);
					temp.push_back(newf);
					frontier[x].push_back(temp);
				}

				
			}
			frontier.erase(x);


		}

	}




	return ans;

}

int GetHValue(int node, vector<int>& minhvalues)
{

	if(minhvalues[node]==0)
	{
		int minx=INT_MAX;	
		for(int i=0; i<NodeMap.size(); i++)
		{
			if(KNodes.find(i)==KNodes.end() && i!=node)
			{
				if(hvalues[node][i]<minx)	
				minx = hvalues[node][i];
			}
		}

		minhvalues[node]=minx;

	}

	return minhvalues[node];

}

void LoadHeuristicValues()
{
	ifstream f1("heuristic.txt");

	for(int i=0; i<NodeMap.size(); i++)
	{
		vector <int> temp;
		int discard;
		f1 >> discard;
		for(int j=0; j<NodeMap.size(); j++)
		{
			int t;
			f1 >> t;
			temp.push_back(t);
		}
		hvalues.push_back(temp);

	}


	return;
}

bool GoalTest(map<int, set<int>>& explored, double threshold, int& ans)
{
	vector<int> freqcount(NodeMap.size(), 0);
	for(auto x: explored)
	{
		for(auto y: x.second)
		{
			freqcount[y]++;
		}

	}

	int maxm=INT_MIN;

	for(int i=0; i<NodeMap.size(); i++)
	{
		if(freqcount[i]>maxm)
		{
			maxm=freqcount[i];
			ans=i;
		}
	}	

	if(maxm >= threshold/double(K))
		return true;
	else
		return false;
}
