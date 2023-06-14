#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <unordered_set>
#include <vector>

using namespace std;
bool cmp(string s1, string s2)  {
    return (s1.compare(s2)==0)?true:false;
}

class undirected_graph {
private:
    map<string, set<string>> graph;


public:

    void addEdge(string u, string v) {
        graph[u].insert(v);
        graph[v].insert(u);
    }
    void printEnrolledUsers(){
        vector<string> ret;
        for(auto i : graph)
            ret.push_back(i.first);
        sort(ret.begin(), ret.end(), cmp);
        for (auto i : ret)
            cout<<i<<" ";
        cout<<endl;
    };
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
            if (visited.find(neighbor) == visited.end()) { //se non é stato ancora visitato
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
    void displayFriends(string u){
        for(auto it = graph[u].begin();it!=graph[u].end();it++)
            cout<<*it<<" ";
        cout<<endl;
    }

};
int main () {
    string line;
    ifstream file("input.txt");
    undirected_graph myGraph;
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            int pos=line.find(" ");
            // Add edges
            myGraph.addEdge(line.substr(0, pos), line.substr(pos+1, line.length()));
        }
        file.close();
        cout<<"display relationship"<<endl;
        myGraph.displayRelationship("mario", "gian");
        cout<<"display enrolled"<<endl;
        myGraph.printEnrolledUsers();
        myGraph.displayFriends("mario");
    }
    else{
        cout<<"file not open correctly";
    }
    return 0;
}