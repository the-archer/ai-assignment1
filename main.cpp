//Note : Compile with g++ -std=c++11 main.cpp

/*This program takes a graph (in csv format)  as input and runs 2 algos - Uniform Cost Search (Algo1) and A* (Algo2) to get the new node to be
added the k initial nodes.

Problem Definition
Consider the graph as a social network with the links being the people one
is in constant interaction with. Given the n people in the graph, there
are k people in them who have already formed a group of their own (say, a
project team) and they want to include one more person in their group, who
is most compatible with the existing members of the group. Find that person.


*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <climits>
#include <algorithm>


using namespace std;




map<string, int> NodeMap;  //This is a mapping from team names to node ids.  
vector<vector<pair<int, int>>> Graph; // The graph is stored as an adjacency list with the pair of node id and distance stored in vectors.
set<int> KNodes; // The set of k nodes given initially.
int K; // Value of k
vector<vector<int>> hvalues; //Unweighted edge ditancse between each pair of nodes. Used in A* as part of a heuristic.




int Algo1();
int Algo2();
float CalcAvgDist(int);
vector<int> UniformCostSearch(int);
void LoadGraph(void);
void LoadHeuristicValues();
bool GoalTest1(vector<bool>& explored);
bool GoalTest2(map<int, set<int>>& explored, int& ans);
int GetHValue(int node, vector<int>& minhvalues);
void PrintFrontier1(vector<int>& dist, vector<bool>& explored);
void PrintExplored1(vector<bool>& explored);
void PrintFrontier2(vector<vector<int>>& frontier);
void PrintExplored2(set<int>& explored);


int main(){

	LoadGraph(); /*Read the graph from csv*/
	

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

	cout << "Output of Algorithm 1:\t" << Algo1() << endl;
	cout << "Output of Algorithm 2:\t" << Algo2() << endl;

	return 0;
}

void LoadGraph(void) //Function to load the graph from file.csv
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

int Algo1() // The first algo which runs Uniform Cost Search taking each of the n-k nodes outside of the selected k nodes as the root.
{
	vector<float> averageDist = vector<float> (NodeMap.size(),INT_MAX); //Stores the average distance between each node and the 'k' selected nodes.
	for (int i = 0; i != averageDist.size(); i++){
		if (KNodes.find(i) != KNodes.end())
			averageDist[i] = INT_MAX;
		else
			averageDist[i] = CalcAvgDist(i); 
	}
	
	int min = INT_MAX, minNode=0;
	for (int i = 0; i < averageDist.size(); i++) //Finds the minimum of the average distances of the nodes.
	{
		if (min>averageDist[i]){
			min = averageDist[i];
			minNode = i;
		}
	}
	return minNode; //The node with minumum average distance with the 'k' selected nodes is returned by the function.
}

float CalcAvgDist(int NodeID) // This takes a particular node NodeID and calculates its average distance from  the k selected nodes.
{ 
	vector<int> Dist = UniformCostSearch(NodeID); //UniformCostSearch is applied to get all the distances. 
	float avg = 0;
	int num = 0;

	for (int i = 0; i < Dist.size(); i++) //Average of the distances is calculated.
	{
		if (KNodes.find(i) != KNodes.end()){
			avg += Dist[i];
			num++;
		}
	}
	if (num != 0)
		avg = avg / num;

	cout << "Avg of Node " << NodeID << ":\t\t" << avg << endl;
	cout << endl;
	return avg;
}

vector<int> UniformCostSearch(int NodeID) //Uniform Cost Search is applied taking NodeID as the root node.
{
	
	vector<int> Dist = vector<int>(NodeMap.size(),INT_MAX); //This stores the currently found least distance for the frontier nodes.
	vector<bool> explored = vector<bool>(NodeMap.size(), false); //The explored set.
	Dist[NodeID] = 0;
	cout << "Node: " << NodeID << endl; 
	int total = 0;
	while(!GoalTest1(explored)) //This loop is run until the goal test for this algo is fulfilled.
	{
		
		int min = INT_MAX;
		int minNode = 0;
		while (minNode < Dist.size() && explored[minNode])
			minNode++;
		for (int l = minNode; l < Dist.size(); l++) //Finds the node in the frontier with the least f value.
		{
			if (!explored[l] && min > Dist[l]){
				min = Dist[l];
				minNode = l;
			}
		}
		int i = minNode;
		explored[i] = true;
		for (int j = 0; j < Graph[i].size(); j++) //Updates the f values of the neighbours of the selected node.		
		{
			if (!explored[Graph[i][j].first] && Dist[i] != INT_MAX && Dist[Graph[i][j].first]>Dist[i] + Graph[i][j].second)
				Dist[Graph[i][j].first] = Dist[i] + Graph[i][j].second; 
		}
		total++;
		PrintExplored1(explored); //Printing the explored set.
		PrintFrontier1(Dist, explored); //Printing the frontier set.
		
	}
	return Dist;
}

bool GoalTest1(vector<bool>& explored) //Checks if the goal test condition is met by iterating through the k nodes and checking if they have been explored
{
	for(auto x: KNodes)
	{
		if(!explored[x])
			return false;
	}

	return true;
}


int Algo2() //The second algo which applies A* on each of the k selected nodes.
{
	LoadHeuristicValues(); //Loads unweighted edge distances between each pair of nodes from heuristic.txt (precalculated).

	
	map<int, vector<vector<int>>> frontier; //A map of frontier sets for each of the k nodes.
	map<int,set<int>> explored; //A map of explored sets for each of the k nodes.
	
	int stepno=0; 

	cout << endl;
	
	cout << "Step No: " << stepno << endl;

	vector<int> minhvalues(NodeMap.size(), 0); //For each node, stores the minimum of all the hvalues to the n-k nodes, excluding itself.
	for(auto x: KNodes) //Initialization
	{
		set<int> temp;
		explored.insert(pair<int, set<int>>(x, temp));
		vector<vector<int>> temp2;
		vector<int> temp3;
		temp3.push_back(x);
		temp3.push_back(0); //g value of root node
		int h = GetHValue(x, minhvalues);
		temp3.push_back(h+0); //f = g+h
		temp2.push_back(temp3);
		frontier.insert(pair<int, vector<vector<int>>>(x,  temp2));
		cout << "Node: " << x << endl;
		PrintExplored2(explored[x]);
		PrintFrontier2(frontier[x]);
		
		

	}

	int ans=-1;
	stepno++;
	while(!GoalTest2(explored, ans)) //This loop runs which the goal test for this algo is not fulfilled.
	{
		cout << endl;
		cout << "Step No: " << stepno << endl;
		

		for(auto x: KNodes) //For each of the k selected nodes, a single step of A* is run at a time.
		{
			cout << "Node: " << x << endl;
			int minx=INT_MAX;
			int selected=-1;
			int parentg=0;
			for(auto y: frontier[x]) //Selects the node from the frontier set with the least f value to be added to the explored set.
			{
				if(y[2] < minx)
				{
					minx = y[2];
					selected = y[0];
					parentg=y[1];
				}
			}

			
			explored[x].insert(selected); //Adds the selected node to the explored set.
			
			
			for(auto y: Graph[selected]) //Iterates through all its neigbours update their f values.
			{
				if(explored[x].find(y.first)!=explored[x].end()) //Skips neighbours which are already in the explored set.
				{					
					continue;
				}
				int g = parentg + y.second;
				int h = GetHValue(y.first, minhvalues);
				int newf=g+h; //Calculates the new f value for the node.
				bool flag=false;
				for(auto z: frontier[x]) //Checks if the node is already in the frontier and updates its f value if its found to be smaller.
				{
					if(z[0]==y.first)
					{
						z[2] = min(z[2], newf);
						flag=true;

						break;
					}
				}
				if(!flag) //Otherwise adds the node to the frontier.
				{
					vector<int> temp;
					temp.push_back(y.first);
					temp.push_back(g);
					temp.push_back(newf);
					frontier[x].push_back(temp);
					
				}

				
			}
			for(int i=0; i<frontier[x].size(); i++) //Removes the node which was added to the explored set from the frontier. 
			{
				if(frontier[x][i][0]==selected)
				{
					frontier[x].erase(frontier[x].begin()+i);
					break;
				}

			}
			PrintExplored2(explored[x]);
			PrintFrontier2(frontier[x]);
			
			

		}
		stepno++;

	}

	return ans;

}

int GetHValue(int node, vector<int>& minhvalues) //Gets the Heuristic value from the hvalues.
{

	if(minhvalues[node]==0)
	{
		int minx=INT_MAX;	
		for(int i=0; i<NodeMap.size(); i++) //The heuristic value is calculated by taking the minimum of the distances from the node to the other n-k nodes.
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

void LoadHeuristicValues() //Loads unweighted edge distances between each pair of nodes from heuristic.txt (precalculated).
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
		hvalues.push_back(temp); //These values are stored in hvalues.

	}


	return;
}

bool GoalTest2(map<int, set<int>>& explored, int& ans) //The goal test for the second algo. This is fulfilled when the intersection of the explored 
{													// sets of all the k node has at least a single node outside of the k nodes.
	vector<int> freqcount(NodeMap.size(), 0);
	for(auto x: explored) //Gets the frequency count of all the nodes in the explored sets.
	{
		for(auto y: x.second)
		{
			freqcount[y]++; 
		}

	}

	int maxm=INT_MIN;

	for(int i=0; i<NodeMap.size(); i++) //Finds the node with maximum frequency.
	{
		if(freqcount[i]>maxm && KNodes.find(i)==KNodes.end())
		{
			maxm=freqcount[i];
			ans=i;
		}
	}	

	if(maxm == K) //If the node with the maximum frequency occurs all the k explored sets, then the function returns true and that mpde is set as the answer.
		return true;
	else
		return false;
}

void PrintFrontier2(vector<vector<int>>& frontier) //Iterates through the frontier vector and prints it.
{
	cout << "Frontier: ";
	for(auto x: frontier)
	{
		cout << x[0] << " "; 
	}
	cout << endl;
	

}

void PrintExplored2(set<int>& explored) //Iterates through the explored set and prints it.
{
	cout << "Explored: ";
	for(auto x: explored)
	{
		cout << x << " ";
	}
	cout << endl;

}

void PrintExplored1(vector<bool>& explored) //Iterates through the explored set and prints the nodes which have been explored.
{
	cout << "Explored: ";
	for(int i=0; i<explored.size(); i++)
	{
		if(explored[i])
			cout << i << " ";
	}
	cout << endl;

}

void PrintFrontier1(vector<int>& dist, vector<bool>& explored) //Iterates through the distance vector and prints the nodes which have been visited but
{												// are not in the explored set, and hence belong to the frontier.
	cout << "Frontier: ";
	for(int i=0; i<dist.size(); i++)
	{
		if(dist[i]!=INT_MAX && !explored[i])
		{
			cout << i << " ";
		}

	}
	cout << endl;

}