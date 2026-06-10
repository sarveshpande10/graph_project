#include<bits/stdc++.h>
using namespace std;

class Node {
    public:
        vector<pair<int, int>> adj;
        bool alive;
        Node() {
            alive = true;
        }
};

class Graph {
    public:
        vector<Node> nodes;
        int aliveNodes;
        Graph() {
            aliveNodes = 0;
        }
};

void deleteEdge(Graph& g, const int& source, const int& destination);

void addNode(Graph& g) {
    cout << "\nAdding Node..." << endl;

    int index;  //Either index of dead node or new node
    int size = g.nodes.size();

    //Checking if any dead node is present
    if(g.aliveNodes < size) {
        for(int i=0;i<size;i++) {
            if(g.nodes[i].alive == false) {
                index = i;
                g.nodes[i].adj.clear();
                g.nodes[i].alive = true;
                break;
            }
        }
    }
    else {
        index = g.nodes.size();
        g.nodes.push_back({});
    }

    g.aliveNodes++;
    cout << "\nNode added successfully!" << endl;
}

void deleteNode(Graph& g, const int& node) {
    int size = g.nodes.size();

    //Check if node does not exist
    if(node >= size || g.nodes[node].alive == false) {
        cout << "\nNode " << node << " does not exist in the graph" << endl;
        return;
    }

    //Deleting incoming edges to the node
    for(int i=0;i<size;i++) {
        if(g.nodes[i].alive) {
            deleteEdge(g, i, node);
        }
    }

    //Deleting outgoing edges
    g.nodes[node].adj.clear();
    g.nodes[node].alive = false;
    g.aliveNodes--;

    cout << "\nNode " << node << " deleted successfully!" << endl;
}

void addEdge(Graph& g, const int& source, const int& destination, const int& weight=1) {
    int size = g.nodes.size();
    if(source >= size || destination >= size || !g.nodes[source].alive || !g.nodes[destination].alive) {
        cout << "Invalid source or destination node" << endl;
        return;
    }
    auto pos = lower_bound(g.nodes[source].adj.begin(), g.nodes[source].adj.end(), make_pair(destination, INT_MIN));
    
    //Check if the edge already exists
    if(pos != g.nodes[source].adj.end() && (*pos).first == destination) {
        cout << "Edge [" << source << "," << destination << "] already exists" << endl;
    }
    else {
        g.nodes[source].adj.insert(pos, {destination, weight});
        cout << "Edge [" << source << "," << destination << "] added successfully!" << endl;
    }
}

void deleteEdge(Graph& g, const int& source, const int& destination) {
    int size = g.nodes.size();
    if(source >= size || destination >= size || !g.nodes[source].alive || !g.nodes[destination].alive) {
        cout << "Invalid source or destination node" << endl;
        return;
    }

    //Check if the edge exists
    auto pos = lower_bound(g.nodes[source].adj.begin(), g.nodes[source].adj.end(), make_pair(destination, INT_MIN));
    if(pos != g.nodes[source].adj.end() && (*pos).first == destination) {
        g.nodes[source].adj.erase(pos);
        cout << "Edge [" << source << "," << destination << "] deleted successfully!" << endl;
    }
    else {
        cout << "Edge [" << source << "," << destination << "] does not exist" << endl;
    }
}

void displayAdjacencyLists(const Graph& g) {
    cout << endl;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive == true) {
            cout << i << ": ";
            for(int j=0;j<g.nodes[i].adj.size();j++) {
                cout << "(" << g.nodes[i].adj[j].first << "," << g.nodes[i].adj[j].second << ") ";
            }
            cout << endl;
        }
    }
}

void dfs(const Graph& g, int vertex, vector<int>& visited) {
    visited[vertex] = 1;
    cout << vertex << " ";
    for(pair<int, int> p : g.nodes[vertex].adj) {
        if(!visited[p.first]) {
            dfs(g, p.first, visited);
        }
    }
}

void dfsDisplay(const Graph& g) {
    vector<int> visited(g.nodes.size());
    cout << endl;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && !visited[i]) {
            dfs(g, i, visited);
        }
    }
}

void bfsDisplay(const Graph& g) {
    vector<int> visited(g.nodes.size());
    queue<int> q;
    cout << endl;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && !visited[i]) {
            visited[i] = 1;
            q.push(i);
        }
        while(!q.empty()) {
            int vertex = q.front();
            q.pop();
            cout << vertex << " ";
            for(pair<int, int> neighbor : g.nodes[vertex].adj) {
                if(!visited[neighbor.first]) {
                    visited[neighbor.first] = 1;
                    q.push(neighbor.first);
                }
            }
        }
    }
}

void dfsTopo(const Graph& g, int vertex, vector<int>& visited, stack<int>& st) {
    visited[vertex] = 1;
    for(pair<int, int> neighbor : g.nodes[vertex].adj) {
        if(!visited[neighbor.first]) {
            dfsTopo(g, neighbor.first, visited, st);
        }
    }
    st.push(vertex);
}

void topologicalDisplay(const Graph& g) {
    vector<int> visited(g.nodes.size());
    stack<int> st;
    cout << endl;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && !visited[i]) {
            dfsTopo(g, i, visited, st);
        }
    }
    while(!st.empty()) {
        cout << st.top() << " ";
        st.pop();
    }
}


int main() {
    Graph g;
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);
    addNode(g);


    addEdge(g, 0, 3);
    addEdge(g, 0, 4);
    addEdge(g, 1, 4);
    addEdge(g, 1, 5);
    addEdge(g, 2, 5);
    addEdge(g, 3, 6);
    addEdge(g, 4, 6);
    addEdge(g, 4, 7);
    addEdge(g, 5, 7);
    addEdge(g, 5, 8);
    addEdge(g, 6, 9);
    addEdge(g, 7, 9);
    addEdge(g, 8, 9);

    dfsDisplay(g);
    // deleteNode(g, 1);
    // dfsDisplay(g);
    cout << endl;
    bfsDisplay(g);
    topologicalDisplay(g);
    displayAdjacencyLists(g);

    deleteNode(g, 2);
    deleteNode(g, 9);
    cout << endl;
    bfsDisplay(g);
    topologicalDisplay(g);
    displayAdjacencyLists(g);

}