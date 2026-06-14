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
        vector<int> adj;
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
        Graph() {
            aliveNodes = 0;
        }
};

int getIndexById(const Graph& g, const int id) {
    int idx = -1;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && g.nodes[i].user.id == id) {
            idx = i;
            break;
        }
    }
    return idx;
}

bool deleteEdge(Graph& g, const int source, const int destination) {
    int size = g.nodes.size();
    if(source >= size || destination >= size || !g.nodes[source].alive || !g.nodes[destination].alive) {
        return false;
    }
    //Check if the edge exists
    auto pos = lower_bound(g.nodes[source].adj.begin(), g.nodes[source].adj.end(), destination);
    if(pos != g.nodes[source].adj.end() && *pos == destination) {
        g.nodes[source].adj.erase(pos);
        auto reversePos = lower_bound(g.nodes[destination].adj.begin(), g.nodes[destination].adj.end(), source);
        g.nodes[destination].adj.erase(reversePos);
        return true;
    }
    return false;
}

int verifyIdAndGetIndex(const Graph& g, const string& option="User") {
    int id;
    cout << "\nEnter " << option << " ID: ";
    cin >> id;
    int idx = getIndexById(g, id);
    if(idx == -1) {
        cout << endl << option << " not found!\n";
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

bool deleteUser(Graph& g, const int& id) {
    int idx = getIndexById(g, id);

    if(idx == -1) return false;

    //Deleting incoming edges to the node
    vector<int> neighbors = g.nodes[idx].adj;
    for(const int neighbor : neighbors) {
        deleteEdge(g, neighbor, idx);
    }

    //Deleting outgoing edges
    g.nodes[idx].adj.clear();
    g.nodes[idx].user = {};
    g.nodes[idx].alive = false;
    g.aliveNodes--;

    return true;
}

void handleDeleteUser(Graph& g) {
    int id;
    cout << "\nEnter user ID: ";
    cin >> id;

    bool userDeleted = deleteUser(g, id);
    if(userDeleted) {
        cout << "\nUser deleted successfully!\n";
    }
    else {
        cout << "\nUser not found!\n";
    }
}

bool connectFriend(Graph& g, const int userIndex, const int friendIndex) {
    auto pos = lower_bound(g.nodes[userIndex].adj.begin(), g.nodes[userIndex].adj.end(), friendIndex);
    
    //Check if the edge already exists
    if(pos != g.nodes[userIndex].adj.end() && *pos == friendIndex) {
        return false;
    }
    //Add in user's adj list
    g.nodes[userIndex].adj.insert(pos, friendIndex);
    //Add in friend's adj list
    auto reversePos = lower_bound(g.nodes[friendIndex].adj.begin(), g.nodes[friendIndex].adj.end(), userIndex);
    g.nodes[friendIndex].adj.insert(reversePos, userIndex);
    return true;
}

void handleConnectFriend(Graph& g) {
    int userIndex = verifyIdAndGetIndex(g);
    if(userIndex != -1) {
        int friendIndex = verifyIdAndGetIndex(g, "Friend");
        if(friendIndex != -1) {
            if(userIndex == friendIndex) {
                cout << "\nUser and friend should not be same!\n";
                return;
            }
            bool friendConnected = connectFriend(g, userIndex, friendIndex);
            if(friendConnected) {
                cout << "\n'" << g.nodes[userIndex].user.name << "' connected with '" << g.nodes[friendIndex].user.name << "' successfully!" << endl;
            }
            else {
                cout << "\n'" << g.nodes[userIndex].user.name << "' is already connected with '" << g.nodes[friendIndex].user.name << "'!" << endl;
            }
        }
    }
}

void handleDisconnectFriend(Graph& g) {
    int userIdx = verifyIdAndGetIndex(g);
    if(userIdx != -1) {
        int friendIdx = verifyIdAndGetIndex(g, "Friend");
        if(friendIdx != -1) {
            bool friendDisconnected = deleteEdge(g, userIdx, friendIdx);
            if(friendDisconnected) {
                cout << "\n'" << g.nodes[friendIdx].user.name << "' disconnected successfully!\n";
            }
            else {
                cout << "\nFriend not connected with user!\n";
            }
        }
    }
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

void handleCurrentMates(const Graph& g) {
    int idx = verifyIdAndGetIndex(g);
    if(idx != -1) {
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

void handlePastMates(const Graph& g) {
    int idx = verifyIdAndGetIndex(g);
    if(idx != -1) {
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
}

pair<vector<int>, vector<int>> getShortestPathTree(const Graph& g, const int source, const int destination = -1) {
    vector<int> distance(g.nodes.size(), INT_MAX);
    distance[source] = 0;
    queue<int> q;
    q.push(source);
    vector<int> parent(g.nodes.size(), -1);
    parent[source] = source;

    while(!q.empty()) {
        int node = q.front();
        q.pop();
        if(destination != -1 && node == destination) break;
        for(int i=0;i<g.nodes[node].adj.size();i++) {
            int neighbor = g.nodes[node].adj[i];
            if(g.nodes[neighbor].alive && distance[neighbor] == INT_MAX) {
                distance[neighbor] = distance[node] + 1;
                parent[neighbor] = node;
                q.push(neighbor);
            }
        }
    }

    return {distance, parent};
}

vector<string> getConnectionPath(const Graph& g, const int source, const int destination) {
    vector<int> parent = getShortestPathTree(g, source, destination).second;
    if(parent[destination] == -1) return {};
    vector<string> path;
    path.push_back(g.nodes[destination].user.name);
    int i = destination;
    while(i != source) {
        path.push_back(g.nodes[parent[i]].user.name);
        i = parent[i];
    }
    return path;
}

void handleIsConnected(const Graph& g) {
    int index1 = verifyIdAndGetIndex(g, "User 1");
    if(index1 != -1) {
        int index2 = verifyIdAndGetIndex(g, "User 2");
        if(index2 != -1) {
            vector<string> path = getConnectionPath(g, index1, index2);
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
    }

}

vector<string> getContactList(const Graph& g, const int idx) {
    vector<string> contacts;
    for(const int neighbor : g.nodes[idx].adj) {
        contacts.push_back(g.nodes[neighbor].user.name);
    }
    return contacts;
}

void handleContactList(const Graph& g) {
    int idx = verifyIdAndGetIndex(g);

    if(idx != -1) {
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

}

vector<vector<string>> getCompleteNetwork(const Graph& g, const int idx) {

    vector<vector<string>> connections;
    vector<int> parent = getShortestPathTree(g, idx).second;
    for(int destination=0;destination<parent.size();destination++) {
        if(destination != idx && parent[destination] != -1) {
            vector<string> path;
            path.push_back(g.nodes[destination].user.name);
            int i = destination;
            while(i != destination) {
                path.push_back(g.nodes[parent[i]].user.name);
                i = parent[i];
            }  
            connections.push_back(path);
        }
    }
    return connections;
}

void handleGetCompleteNetwork(const Graph& g) {
    int idx = verifyIdAndGetIndex(g);

    if(idx != -1) {
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

}

vector<string> getMaximumPathOfSeparation(const Graph& g, const int idx) {
    pair<vector<int>, vector<int>> connections = getShortestPathTree(g, idx);
    vector<int> distances = connections.first;
    int maxi = 0, maxIdx = -1;
    for(int i=0;i<distances.size();i++) {
        if(distances[i] != INT_MAX && distances[i] > maxi) {
            maxi = distances[i];
            maxIdx = i;
        }
    }

    if(maxIdx == -1) return {};
    vector<int> parent = connections.second;
    vector<string> path;
    path.push_back(g.nodes[maxIdx].user.name);
    int i = maxIdx;
    while(i != idx) {
        path.push_back(g.nodes[parent[i]].user.name);
        i = parent[i];
    }
    return path;
}

void handleMaximumPathOfSeparation(const Graph& g) {
    int idx = verifyIdAndGetIndex(g);

    if(idx != -1) {
        vector<string> maxPath = getMaximumPathOfSeparation(g, idx);
        if(maxPath.empty()) {
            cout << "\nNo connections found!\n";
            return;
        }
        cout << "\nMaximum degree of separation: " << maxPath.size() - 1 << endl;
        for(int i=maxPath.size()-1;i>=0;i--) {
            cout << maxPath[i];
            if(i != 0) {
                cout << " => ";
            }
        }
        cout << endl;
    }

}

vector<string> searchByHobby(const Graph& g, const string hobby) {
    vector<string> names;
    for(int i=0;i<g.nodes.size();i++) {
        if(g.nodes[i].alive && g.nodes[i].user.hobby == hobby) {
            names.push_back(g.nodes[i].user.name);
        }
    } 

    return names;
}

void handleSearchByHobby(const Graph& g) {
    string hobby;
    cout << "\nEnter hobby: ";
    cin >> hobby;
    vector<string> users = searchByHobby(g, hobby);
    if(users.empty()) {
        cout << "\nNo users found!\n";
        return;
    }
    cout << "\nUsers:\n";
    for(int i=0;i<users.size();i++) {
        cout << i+1 << ". " << users[i] << endl;
    }
    cout << endl;
}

int main() {
    Graph g;

    int done = 0;
    while(!done) {
        int choice;
        cout << "\n1.Add new user\n"
            << "2.Connect with a friend\n"
            << "3.Display current school/college mates\n"
            << "4.Display past school/college mates\n"
            << "5.Check if two users are connected\n"
            << "6.Get contact list\n"
            << "7.Check complete network\n"
            << "8.Maximum path of separation\n"
            << "9.Search users by hobby\n"
            << "10.Disconnect with a friend\n"
            << "11.Delete user\n"
            << "12.Exit\n";
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
            handleMaximumPathOfSeparation(g);
            break;

        case 9:
            handleSearchByHobby(g);
            break;

        case 10:
            handleDisconnectFriend(g);
            break;

        case 11:
            handleDeleteUser(g);
            break;

        case 12:
            done = 1;
            break;

        default:
            cout << "\nInvalid choice!\n";
            break;
        }
    }
}