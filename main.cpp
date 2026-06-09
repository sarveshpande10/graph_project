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
        cout << "\nEnter the vertex to connect the edge " << i+1 << ": ";
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
                    auto posInVertex = lower_bound(g.node[vertex].adj.begin(), g.node[vertex].adj.end(), index);
                    g.node[vertex].adj.insert(posInVertex, index);
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
        cout << "Node " << node << " does not exist in the graph" << endl;
        return;
    }

    for(int vertex : g.node[node].adj) {
        erase(g.node[vertex].adj, node);
    }
    g.node[node].adj.clear();
    g.node[node].alive = false;
    g.aliveNodes--;
    cout << "Node " << node << " deleted successfully!" << endl;
}

void displayGraph(const Graph& g) {
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

int main() {
    Graph g;
    addNode(g, 0);
    addNode(g, 1);
    addNode(g, 2);
    addNode(g, 4);
    displayGraph(g);
    deleteNode(g, 1);

    displayGraph(g);
}