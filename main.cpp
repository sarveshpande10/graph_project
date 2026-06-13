#include<bits/stdc++.h>
using namespace std;

class Institute {
    public:
        string instituteName;
        int startYear;
        int endYear;
        bool currentlyStudying;
};

class User {
    public:
        static int nextId;
        int id;
        string name;
        vector<Institute> educationalInstitutes;
        string hobby;
};

int User::nextId = 0;

class Node {
    public:
        vector<pair<int, int>> adj;  //{destination, weight}
        User user;
        bool alive;
        Node() {
            alive = true;
        }
};

class Graph {
    public:
        vector<Node> nodes;
        int aliveNodes;
        bool directed;
        Graph() {
            aliveNodes = 0;
        }
};

bool deleteEdge(Graph& g, const int& source, const int& destination);

int getIndexById(const Graph& g, int id) {
    int idx = -1;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && g.nodes[i].user.id == id) {
            idx = i;
            break;
        }
    }
    return idx;
}

void addUser(Graph& g, const User& user) {
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
    g.nodes[index].user = user;
    g.aliveNodes++;
}

void handleAddUser(Graph& g) {
    User user;
    user.id = ++User::nextId;
    cout << "\nEnter username: ";
    cin >> user.name;

    //Add Institutes
    char addIns;
    cout << "Do you want to add institutes(Y/N): ";
    cin >> addIns;
    int done = addIns == 'Y' ? 0 : 1;
    while(!done) {
        Institute ins;
        char currentChoice;
        char addMore;
        cout << "\nEnter institute name: ";
        cin >> ins.instituteName;
        cout << "Enter start year: ";
        cin >> ins.startYear;
        cout << "Enter end year: ";
        cin >> ins.endYear;
        cout << "Currently studying in this institute?(Y/N): ";
        cin >> currentChoice;
        ins.currentlyStudying = currentChoice == 'Y' ? true : false;
        user.educationalInstitutes.push_back(ins);
        cout << "Do you want to add more institutes(Y/N): ";
        cin >> addMore;
        done = addMore == 'Y' ? 0 : 1;
    }

    cout << "Enter hobby: ";
    cin >> user.hobby;
    addUser(g, user);
    cout << "\n'" << user.name << "' added successfully!" << endl;
}

bool deleteNode(Graph& g, const int& node) {
    int size = g.nodes.size();

    //Check if node does not exist
    if(node >= size || g.nodes[node].alive == false) {
        return false;
    }

    //Deleting incoming edges to the node
    if(g.directed) {
        for(int i=0;i<size;i++) {
            if(g.nodes[i].alive) {
                deleteEdge(g, i, node);
            }
        }
    }
    else {
        for(pair<int, int> edge : g.nodes[node].adj) {
            deleteEdge(g, edge.first, node);
        }
    }

    //Deleting outgoing edges
    g.nodes[node].adj.clear();
    g.nodes[node].alive = false;
    g.aliveNodes--;

    return true;
}

void handleDeleteNode(Graph& g, const int& node) {
    bool nodeDeleted = deleteNode(g, node);
    if(nodeDeleted) {
        cout << "\nNode " << node << " deleted successfully!" << endl;
    }
    else {
        cout << "\nNode " << node << " does NOT exist in the graph" << endl;
    }
}

bool connectFriend(Graph& g, const int userIndex, const int friendIndex, const int& weight=1) {

    auto pos = lower_bound(g.nodes[userIndex].adj.begin(), g.nodes[userIndex].adj.end(), make_pair(friendIndex, INT_MIN));
    
    //Check if the edge already exists
    if(pos != g.nodes[userIndex].adj.end() && (*pos).first == friendIndex) {
        return false;
    }

    g.nodes[userIndex].adj.insert(pos, {friendIndex, weight});
    if(!g.directed) {
        auto reversePos = lower_bound(g.nodes[friendIndex].adj.begin(), g.nodes[friendIndex].adj.end(), make_pair(userIndex, INT_MIN));
        g.nodes[friendIndex].adj.insert(reversePos, {userIndex, weight});
    }
    return true;
}

void handleConnectFriend(Graph& g) {
    int userId, friendId;
    cout << "\nEnter your ID: ";
    cin >> userId;
    int userIndex = getIndexById(g, userId);
    if(userIndex == -1) {
        cout << "\nUser not found!" << endl;
        return;
    }
    cout << "\nEnter your friend's ID: ";
    cin >> friendId;
    int friendIndex = getIndexById(g, friendId);
    if(friendIndex == -1) {
        cout << "\nFriend not found!\n" << endl;
        return;
    }
    
    bool friendConnected = connectFriend(g, userIndex, friendIndex);
    if(friendConnected) {
        cout << "\nUser with ID '" << userId << "' connected with ID '" << friendId << "' successfully!" << endl;
    }
    else {
        cout << "\nUser with ID '" << userId << "' is already connected with ID '" << friendId << "'" << endl;
    }
}

bool deleteEdge(Graph& g, const int& source, const int& destination) {
    int size = g.nodes.size();
    if(source >= size || destination >= size || !g.nodes[source].alive || !g.nodes[destination].alive) {
        return false;
    }
    //Check if the edge exists
    auto pos = lower_bound(g.nodes[source].adj.begin(), g.nodes[source].adj.end(), make_pair(destination, INT_MIN));
    if(pos != g.nodes[source].adj.end() && (*pos).first == destination) {
        g.nodes[source].adj.erase(pos);
        if(!g.directed) {
            auto reversePos = lower_bound(g.nodes[destination].adj.begin(), g.nodes[destination].adj.end(), make_pair(source, INT_MIN));
            g.nodes[destination].adj.erase(reversePos);
        }
        return true;
    }
    return false;
}

void handleDeleteEdge(Graph& g, const int& source, const int& destination) {
    int size = g.nodes.size();

    bool edgeDeleted = deleteEdge(g, source, destination);
    if(edgeDeleted) {
        cout << "Edge [" << source << "," << destination << "] deleted successfully!" << endl;
    }
    else {
        if(source >= size || destination >= size || !g.nodes[source].alive || !g.nodes[destination].alive) {
            cout << "Invalid source or destination node" << endl;
        }
        else {
            cout << "Edge [" << source << "," << destination << "] does not exist" << endl;
        }
    }
}

void displayAdjacencyLists(const Graph& g) {
    cout << "\nAdjacency List: " << endl;
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

void dfs(const Graph& g, int vertex, vector<int>& visited, vector<int>& ans) {
    visited[vertex] = 1;
    ans.push_back(vertex);
    for(pair<int, int> p : g.nodes[vertex].adj) {
        if(!visited[p.first]) {
            dfs(g, p.first, visited, ans);
        }
    }
}

void handleDFS(const Graph& g) {
    vector<int> visited(g.nodes.size());
    vector<int> ans;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && !visited[i]) {
            dfs(g, i, visited, ans);
        }
    }
    cout << "\nDFS: ";
    for(int node : ans) {
        cout << node << " ";
    }
    cout << endl;
}

vector<int> bfs(const Graph& g) {
    vector<int> visited(g.nodes.size());
    queue<int> q;
    vector<int> ans;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && !visited[i]) {
            visited[i] = 1;
            q.push(i);
        }
        while(!q.empty()) {
            int vertex = q.front();
            q.pop();
            ans.push_back(vertex);
            for(pair<int, int> neighbor : g.nodes[vertex].adj) {
                if(!visited[neighbor.first]) {
                    visited[neighbor.first] = 1;
                    q.push(neighbor.first);
                }
            }
        }
    }
    return ans;
}

void handleBFS(const Graph& g) {
    vector<int> ans = bfs(g);
    cout << "\nBFS: ";
    for(int node: ans) {
        cout << node << " ";
    }
    cout << endl;
}

vector<int> topoSort(const Graph& g) {
    vector<int> inDegree(g.nodes.size());
    queue<int> q;

    //Initializing in-degrees of each node
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive) {
            for(pair<int, int> p : g.nodes[i].adj) {
                inDegree[p.first]++;
            }
        }
        else inDegree[i] = -1;
    }

    //Pushing nodes with in-degree = 0 to the queue
    for(int i=0;i<g.nodes.size();i++) {
        if(inDegree[i] == 0) {
            q.push(i);
        }
    }
    vector<int> ans;

    //Pushing nodes from the queue to the answer and decrementing the in-degrees of corr. destination nodes
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        ans.push_back(node); 
        for(pair<int, int> p : g.nodes[node].adj) {
            inDegree[p.first]--;
            if(inDegree[p.first] == 0) q.push(p.first);
        }
    }

    return ans;
}

void handleTopoSort(const Graph& g) {

    if(!g.directed) {
        cout << "Topological display only works on DIRECTED acyclic graphs" << endl;
        return;
    }
    vector<int> ans = topoSort(g);
    if(ans.size() != g.aliveNodes) {
        cout << "Cycle detected in the graph. Topological display only works on directed ACYCLIC graphs." << endl;
    }
    else {
        cout << "\nTopological Sort: ";
        for(int node : ans) {
            cout << node << " ";
        }
        cout << endl;
    }
}

vector<pair<int, int>> mst(const Graph& g) {
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;  //{wt, {node, parent}}
    vector<int> visited(g.nodes.size());
    vector<pair<int, int>> mst;
    int total = 0;
    pq.push({0, {0, -1}});
    while(!pq.empty()) {
        pair<int, pair<int, int>> edge = pq.top();
        int weight = edge.first;
        int node = edge.second.first;
        int parent = edge.second.second;
        pq.pop();
        if(!visited[node]) {
            visited[node] = 1;
            total += weight;
            if(parent != -1) mst.push_back({parent, node});
            for(pair<int, int> p : g.nodes[node].adj) {
                if(!visited[p.first]) {
                    pq.push({p.second, {p.first, node}});
                }
            }
        }
    }
    return mst;
}

void handleMST(const Graph& g) {
    //Check if graph is directed
    if(g.directed) {
        cout << "Minimum Spanning Tree is only valid for UNDIRECTED graphs" << endl;
        return;
    }
    vector<pair<int, int>> ans = mst(g);
    cout << "\nMinimum Spanning Tree: ";
    for(pair<int, int> p : ans) {
        cout << "[" << p.first << "," << p.second << "] ";
    }
    cout << endl;
}

vector<int> shortestPathFromFixedSource(const Graph& g, const int source) {
    vector<int> ans(g.nodes.size(), INT_MAX);
    vector<vector<string>> intermediates;
    ans[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;  //{currMin, node}
    pq.push({0, source});

    while(!pq.empty()) {
        int currMin = pq.top().first;
        int node = pq.top().second;
        pq.pop();
        if(currMin > ans[node]) continue;

        for(int i=0;i<g.nodes[node].adj.size();i++) {
            int destination = g.nodes[node].adj[i].first;
            int weight = g.nodes[node].adj[i].second;
            if(currMin + weight < ans[destination]) {
                ans[destination] = currMin + weight;
                pq.push({currMin + weight, destination});
            }
        }
    }
    return ans;
}

void handleShortestPathFromFixedSource(const Graph& g, const int source) {
    vector<int> ans = shortestPathFromFixedSource(g, source);
    cout << "\nShortest paths from " << source << " to" << endl;
    for(int i=0;i<ans.size();i++) {
        cout << i << " : " << ans[i] << endl;
    }
}

vector<string> shortestPathBetweenAnyPair(const Graph& g, const int source, const int destination) {
    vector<int> ans(g.nodes.size(), INT_MAX);
    vector<int> parent(g.nodes.size());
    parent[source] = source;
    ans[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;  //{currMin, node}
    pq.push({0, source});

    while(!pq.empty()) {
        int currMin = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if(node == destination) break;
        if(currMin > ans[node]) continue;
        for(int i=0;i<g.nodes[node].adj.size();i++) {
            int dest = g.nodes[node].adj[i].first;
            int weight = g.nodes[node].adj[i].second;
            if(currMin + weight < ans[dest]) {
                ans[dest] = currMin + weight;
                pq.push({currMin + weight, dest});
                parent[dest] = node;
            }
        }
    }
    vector<string> path;
    if(ans[destination] != INT_MAX) {
        path.push_back(g.nodes[destination].user.name);
        int i = destination;
        while(i != source) {
            path.push_back(g.nodes[parent[i]].user.name);
            i = parent[i];
        }
    }
    return path;
}

void handleIsConnected(const Graph& g) {
    int id1, id2;
    cout << "\nEnter ID of user 1: ";
    cin >> id1;
    int index1 = getIndexById(g, id1);
    if(index1 == -1) {
        cout << "\nUser not found!\n";
        return;
    }
    cout << "\nEnter ID of user 2: ";
    cin >> id2;
    int index2 = getIndexById(g, id2);
    if(index2 == -1) {
        cout << "\nUser not found!\n";
        return;
    }

    vector<string> path = shortestPathBetweenAnyPair(g, index1, index2);
    if(path.empty()) {
        cout << "\nUsers are not connected\n";
        return;
    }    
    cout << "\nDegree of separation: " << path.size() - 1 << endl;
    for(int i=path.size()-1;i>=0;i--) {
        cout << path[i];
        if(i != 0) {
            cout << " => ";
        }
    }
    cout << endl;
}

// bool searchByBFS(const Graph& g, const int value) {
//     vector<int> visited(g.nodes.size());
//     queue<int> q;
//     for(int i=0;i<g.nodes.size();i++) {
//         if(g.nodes[i].alive && !visited[i]) {
//             if(g.nodes[i].val == value) return true;
//             visited[i] = 1;
//             q.push(i);
//         }
//         while(!q.empty()) {
//             int vertex = q.front();
//             q.pop();
//             for(pair<int, int> neighbor : g.nodes[vertex].adj) {
//                 if(!visited[neighbor.first]) {
//                     if(g.nodes[neighbor.first].val == value) return true;
//                     visited[neighbor.first] = 1;
//                     q.push(neighbor.first);
//                 }
//             }
//         }
//     }
//     return false;
// }

// bool searchDFSHelper(const Graph& g, int vertex, vector<int>& visited, const int value) {
//     visited[vertex] = 1;
//     for(pair<int, int> p : g.nodes[vertex].adj) {
//         if(!visited[p.first]) {
//             if(g.nodes[p.first].val == value) return true;
//             if(searchDFSHelper(g, p.first, visited, value)) return true;
//         }
//     }
//     return false;
// }

// bool searchByDFS(const Graph& g, const int value) {
//     vector<int> visited(g.nodes.size());
//     for(int i=0;i<g.nodes.size();i++) {
//         if(g.nodes[i].alive && !visited[i]) {
//             if(g.nodes[i].val == value) return true;
//             if(searchDFSHelper(g, i, visited, value)) return true;
//         }
//     }
//     return false;
// }

bool detectCycle(const Graph& g) {
    if(g.directed) {
        vector<int> ts = topoSort(g);
        return ts.size() == g.nodes.size();
    }

    vector<int> visited(g.nodes.size());
    queue<pair<int, int>> q;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && !visited[i]) {
            visited[i] = 1;
            q.push({i, -1});
        }
        while(!q.empty()) {
            int node = q.front().first;
            int parent = q.front().second;
            q.pop();
            for(pair<int, int> neighbor : g.nodes[node].adj) {
                if(visited[neighbor.first] && neighbor.first != parent) return true;
                if(!visited[neighbor.first]) {
                    visited[neighbor.first] = 1;
                    q.push({neighbor.first, node});
                }
            }
        }
    }
    return false;
}

void getAllPathsHelper(const Graph& g, const int source, const int destination, vector<int>& visited, int node, vector<int>& vec, vector<vector<int>>& ans) {
    if(node == destination) {
        ans.push_back(vec);
    }
    else {
        visited[node] = 1;
        for(int i=0;i<g.nodes[node].adj.size();i++) {
            if(!visited[g.nodes[node].adj[i].first]) {
                vec.push_back(g.nodes[node].adj[i].first);
                getAllPathsHelper(g, source, destination, visited, g.nodes[node].adj[i].first, vec, ans);
                vec.pop_back();
            }
        }
    }
    visited[node] = 0;
}

vector<vector<int>> getAllPaths(const Graph& g, const int source, const int destination) {
    vector<int> visited(g.nodes.size());
    vector<int> vec;
    vector<vector<int>> ans;
    vec.push_back(source);
    getAllPathsHelper(g, source, destination, visited, source, vec, ans);

    return ans;
}

vector<User> searchCurrentMates(const Graph& g, const User& user) {
    string currIns = "";
    //Find the current institute of user
    for(const Institute& i : user.educationalInstitutes) {
        if(i.currentlyStudying) {
            currIns = i.instituteName;
            break;
        }
    }
    if(currIns == "") return {};
    vector<User> mates;
    for(const Node& node : g.nodes) {
        if(node.alive && node.user.id != user.id) {
            for(const Institute& i : node.user.educationalInstitutes) {
                if(i.currentlyStudying) {
                    if(i.instituteName == currIns) {
                        mates.push_back(node.user);
                    }
                    break;
                }
            }
        }
    }

    return mates;
}

vector<User> searchPastMates(const Graph& g, const User& user) {
    vector<Institute> pastIns;
    //Find all past institutes
    for(const Institute& i : user.educationalInstitutes) {
        if(!i.currentlyStudying) {
            pastIns.push_back(i);
        }
    }

    //Currently this adds duplicate mates
    if(pastIns.empty()) return {};
    vector<User> mates;
    for(const Node& node : g.nodes) {
        if(node.alive && node.user.id != user.id) {
            for(const Institute& otherIns : node.user.educationalInstitutes) {
                if(!otherIns.currentlyStudying) {
                    for(const Institute& userIns : pastIns) {
                        if(otherIns.endYear == userIns.endYear && otherIns.instituteName == userIns.instituteName) {
                            mates.push_back(node.user);
                            break;
                        }
                    }
                }
            }
        }
    }

    return mates;
}

void handleCurrentMates(const Graph& g) {
    int id;
    cout << "\nEnter user ID: ";
    cin >> id;
    int idx = getIndexById(g, id);
    if(idx == -1) {
        cout << "\nUser not found!\n";
        return;
    }
    vector<User> mates = searchCurrentMates(g, g.nodes[idx].user);
    if(mates.empty()) {
        cout << "\nNo mates found for the user!\n";
        return;
    }
    cout << "\nCurrent mates:\n";
    for(int i=0;i<mates.size();i++) {
        cout << i+1 << ". " << mates[i].name << endl;
    }
}

void handlePastMates(const Graph& g) {
    int id;
    cout << "\nEnter user ID: ";
    cin >> id;
    int idx = getIndexById(g, id);
    if(idx == -1) {
        cout << "\nUser not found!\n";
        return;
    }
    vector<User> mates = searchPastMates(g, g.nodes[idx].user);
    if(mates.empty()) {
        cout << "\nNo mates found for the user!\n";
        return;
    }
    cout << "\nPast mates:\n";
    for(int i=0;i<mates.size();i++) {
        cout << i+1 << ". " << mates[i].name << endl;
    }
}

vector<string> getContactList(const Graph& g, const int idx) {
    vector<string> contacts;
    for(const auto& adj : g.nodes[idx].adj) {
        contacts.push_back(g.nodes[adj.first].user.name);
    }
    return contacts;
}

void handleContactList(const Graph& g) {
    int id;
    cout << "\nEnter user ID: ";
    cin >> id;
    int idx = getIndexById(g, id);

    if(idx == -1) {
        cout << "\nUser not found!\n";
        return;
    }

    vector<string> contacts = getContactList(g, idx);
    if(contacts.empty()) {
        cout << "\nNo contacts found!\n";
        return;
    }
    cout << "\nContact List:\n";
    for(int i=0;i<contacts.size();i++) {
        cout << i+1 << ". " << contacts[i] << endl;
    }
    cout << endl;
}

vector<vector<string>> getCompleteNetwork(const Graph& g, const int idx) {

    vector<vector<string>> connections;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && i != idx) {
            vector<string> path = shortestPathBetweenAnyPair(g, idx, i);
            if(!path.empty()) connections.push_back(path);
        }
    }
    return connections;
}

void handleGetCompleteNetwork(const Graph& g) {
    int id;
    cout << "\nEnter user ID: ";
    cin >> id;
    int idx = getIndexById(g, id);

    if(idx == -1) {
        cout << "\nUser not found!\n";
        return;
    }

    vector<vector<string>> connections = getCompleteNetwork(g, idx);
    if(connections.empty()) {
        cout << "\nNo connections found!\n";
        return;
    }
    cout << "\nConnections:\n";
    for(int i=0;i<connections.size();i++) {
        cout << i+1 << ". ";
        for(int j=connections[i].size()-1;j>=0;j--) {
            cout << connections[i][j];
            if(j != 0) {
                cout << " => ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    Graph g;
    g.directed = false;

    int done = 0;
    while(!done) {
        int choice;
        cout << "\n1.Add new user\n"
            << "2.Connect with a friend\n"
            << "3.Display current school/college mates\n"
            << "4.Display past school/college mates\n"
            << "5.Check if two users are connected\n"
            << "6.Get contact list\n"
            << "7.Check complete network\n";
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            handleAddUser(g);
            break;
        
        case 2:
            handleConnectFriend(g);
            break;

        case 3:
            handleCurrentMates(g);
            break;
        
        case 4:
            handlePastMates(g);
            break;

        case 5:
            handleIsConnected(g);
            break;

        case 6:
            handleContactList(g);
            break;

        case 7:
            handleGetCompleteNetwork(g);
            break;

        case 8:
            done = 1;
            break;

        default:
            cout << "\nInvalid choice!\n";
            break;
        }
    }
    // addUser(g, 3);
    // addUser(g, 1);
    // addUser(g, 8);
    // addUser(g, 7);
    // addUser(g, 2);
    // addUser(g, 2);
    // addUser(g, 2);
    // addUser(g, 2);
    // addUser(g);


    // addEdge(g, 0, 1, 4);
    // addEdge(g, 0, 2, 3);
    // addEdge(g, 0, 3, 6);
    // addEdge(g, 1, 4, 4);
    // addEdge(g, 1, 3, 2);
    // addEdge(g, 2, 0, 3);
    // addEdge(g, 2, 5, 1);
    // addEdge(g, 2, 4, 5);
    // addEdge(g, 3, 6, 5);
    // addEdge(g, 3, 1, 2);
    // addEdge(g, 3, 2, 1);
    // addEdge(g, 3, 4, 7);
    // addEdge(g, 4, 6, 5);
    // addEdge(g, 4, 7, 7);
    // addEdge(g, 5, 4, 7);
    // addEdge(g, 5, 7, 7);
    // addEdge(g, 6, 7, 7);
    // addEdge(g, 6, 2, 7);


    // vector<vector<int>> paths = getAllPaths(g, 0, 7);

    // cout << endl << "paths:" << endl;
    // for(vector<int> path : paths) {
    //     for(int node : path) {
    //         cout << node << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    
    // handleDFS(g);
    // deleteNode(g, 1);
    // dfsDisplay(g);
    // cout << endl;
    // handleBFS(g);
    // handleTopoSort(g);
    // handleMST(g);
    // displayAdjacencyLists(g);
    // handleShortestPathFromFixedSource(g, 0);
    // cout << endl << shortestPathBetweenAnyPair(g, 0, 3) << endl;
    // cout << searchByBFS(g, 8) << endl;
    // cout << endl << searchByDFS(g, 8) << endl;
    // cout << detectCycle(g);

    // deleteNode(g, 2);
    // deleteNode(g, 9);
    // cout << endl;
    // handleBFS(g);
    // handleTopoSort(g);
    // handleMST(g);
    // displayAdjacencyLists(g);
    // handleShortestPathFromFixedSource(g, 0);
    // cout << shortestPathBetweenAnyPair(g, 0, 3);
    // cout << endl << searchByBFS(g, 7) << endl;
    // cout << endl << searchByDFS(g, 2) << endl;
    // cout << detectCycle(g);

}