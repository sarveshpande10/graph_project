#include<bits/stdc++.h>
#include<algorithm>
using namespace std;

class Node {
    public:
        vector<int> adj;
        bool alive;
};

class Graph {
    public:
        vector<Node> node;
        int aliveNodes;
        Graph() {
            aliveNodes = 0;
        }
};

void addNode(Graph& g, const int& edges) {
    cout << "\nAdding Node..." << endl;
    if(edges > g.aliveNodes) {
        cout << "Cannot have " << edges << " edges as there are ";
        if(g.aliveNodes) {
            cout << "only " << g.aliveNodes << " vertices in the graph" << endl;
        }
        else {
            cout << "no vertices in the graph" << endl;
        }
        
        return;
    }

    int index;  //Either index of dead node or new node
    int size = g.node.size();

    //Checking if any dead node is present
    if(g.aliveNodes < size) {
        for(int i=0;i<size;i++) {
            if(g.node[i].alive == false) {
                index = i;
                g.node[i].adj.clear();
                g.node[i].alive = true;
                break;
            }
        }
    }
    else {
        index = g.node.size();
        g.node.push_back({{}, true});
    }
    int i=0;
    while(i < edges) {
        int vertex;
        cout << "\nEnter the vertex to connect the edge " << i+1 << " starting from the new node" << ": ";
        cin >> vertex;
        if(vertex < size) {
            if(g.node[vertex].alive == false) {
                cout << "Please enter valid vertex" << endl;
            }
            else {
                auto pos = lower_bound(g.node[index].adj.begin(), g.node[index].adj.end(), vertex);
    
                //Check if the edge already exists
                if(pos != g.node[index].adj.end() && *pos == vertex) {
                    cout << "Edge already exists" << endl;
                }
                else {
                    g.node[index].adj.insert(pos, vertex);
                    cout << "Edge added successfully!" << endl;
                    i++;
                }
            }
        }
        else {
            cout << "Please enter valid vertex" << endl;
        }
    } 

    g.aliveNodes++;
    cout << "\nNode added successfully!" << endl;
}

void deleteNode(Graph& g, const int& node) {
    int size = g.node.size();
    if(node >= size || g.node[node].alive == false) {
        cout << "\nNode " << node << " does not exist in the graph" << endl;
        return;
    }

    for(int vertex : g.node[node].adj) {
        erase(g.node[vertex].adj, node);
    }
    g.node[node].adj.clear();
    g.node[node].alive = false;
    g.aliveNodes--;
    cout << "\nNode " << node << " deleted successfully!" << endl;
}

void addEdge(Graph& g, const int& start, const int& end) {
    int size = g.node.size();
    if(start >= size || end >= size || !g.node[start].alive || !g.node[end].alive) {
        cout << "Invalid start or end node" << endl;
        return;
    }
    auto pos = lower_bound(g.node[start].adj.begin(), g.node[start].adj.end(), end);
    
    //Check if the edge already exists
    if(pos != g.node[start].adj.end() && *pos == end) {
        cout << "Edge [" << start << "," << end << "] already exists" << endl;
    }
    else {
        g.node[start].adj.insert(pos, end);
    }
    cout << "Edge [" << start << "," << end << "] added successfully!" << endl;
}

void deleteEdge(Graph& g, const int& start, const int& end) {
    int size = g.node.size();
    if(start >= size || end >= size || !g.node[start].alive || !g.node[end].alive) {
        cout << "Invalid start or end node" << endl;
        return;
    }

    //Check if the edge exists
    auto pos = lower_bound(g.node[start].adj.begin(), g.node[start].adj.end(), end);
    if(pos != g.node[start].adj.end() && *pos == end) {
        erase(g.node[start].adj, end);
        cout << "Edge [" << start << "," << end << "] deleted successfully!" << endl;
    }
    else {
        cout << "Edge [" << start << "," << end << "] does not exist" << endl;
    }
}

void displayAdjacencyLists(const Graph& g) {
    cout << endl;
    for(int i=0;i<g.node.size();i++) {
        if(g.node[i].alive == true) {
            cout << i << ": ";
            for(int j=0;j<g.node[i].adj.size();j++) {
                cout << g.node[i].adj[j] << " ";
            }
            cout << endl;
        }
    }
}

void dfs(const Graph& g, int vertex, vector<int>& visited) {
    visited[vertex] = 1;
    cout << vertex << " ";
    for(int v : g.node[vertex].adj) {
        if(!visited[v]) {
            dfs(g, v, visited);
        }
    }
}

void dfsDisplay(const Graph& g) {
    vector<int> visited(g.node.size());
    for(int i=0;i<g.node.size();i++) {
        if(g.node[i].alive && !visited[i]) {
            dfs(g, i, visited);
        }
    }
}

void bfsDisplay(const Graph& g) {
    vector<int> visited(g.node.size());
    queue<int> q;
    for(int i=0;i<g.node.size();i++) {
        if(g.node[i].alive && !visited[i]) {
            visited[i] = 1;
            q.push(i);
        }
        while(!q.empty()) {
            int vertex = q.front();
            q.pop();
            cout << vertex << " ";
            for(int neighbor : g.node[vertex].adj) {
                if(!visited[neighbor]) {
                    visited[neighbor] = 1;
                    q.push(neighbor);
                }
            }
        }
    }
}

int main() {
    Graph g;
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);
    addNode(g, 0);

    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 3);
    addEdge(g, 1, 4);
    addEdge(g, 2, 4);
    addEdge(g, 2, 5);
    addEdge(g, 3, 6);
    addEdge(g, 4, 1);
    addEdge(g, 4, 6);
    addEdge(g, 5, 2);
    addEdge(g, 5, 7);
    addEdge(g, 6, 0);
    addEdge(g, 7, 8);
    addEdge(g, 8, 5);
    addEdge(g, 9, 10);
    addEdge(g, 10, 11);
    addEdge(g, 11, 9);

    dfsDisplay(g);
    // deleteNode(g, 1);
    // dfsDisplay(g);
    cout << endl;
    bfsDisplay(g);
}