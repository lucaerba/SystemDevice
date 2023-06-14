#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
using namespace std;


class undirected_graph {
private:
    map<string, set<string>> graph;

public:
    void addEdge(string u, string v) {
        graph[u].insert(v);
        graph[v].insert(u);
    }
    void dfs(string start, string end, unordered_set<string>& visited, vector<string>& path) {
        visited.insert(start);
        path.push_back(start);
        if (start ==  end) {
            for (auto& vertex : path) {
                cout << vertex << " ";
            }
            cout << endl;
            return;
        }
        for (auto& neighbor : graph[start]) {
            if (visited.find(neighbor) == visited.end()) {
                dfs(neighbor, end, visited, path);
            }
        }
        path.pop_back();
    };
    void displayRelationship(string u, string v) {
        if (graph.find(u) == graph.end() || graph.find(v) == graph.end()) {
            cout << "No such person exists in the social network." << endl;
            return;
        }
        unordered_set<string> visited;
        vector<string> path;
        dfs(u, v, visited, path);
    }

};
int main () {
    string line;
    ifstream file("C:\\Users\\manuel\\Documents\\Politecnico\\SDP\\SDP_labs\\lab3\\es3\\input.txt.txt");
    undirected_graph myGraph;
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            int pos=line.find(" ");
            // Add edges
            myGraph.addEdge(line.substr(0, pos), line.substr(pos, line.length()));
        }
        file.close();
        myGraph.displayRelationship("simona", "luca");
    }
    else{
        cout<<"file not open correctly";
    }
    return 0;
}
