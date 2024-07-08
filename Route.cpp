#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

// Constants for transportation modes
const int TRAIN = 0;
const int CAR = 1;
const int AIRPLANE = 2;

// Class to represent a city
class City {
public:
    int id;
    string name;

    City(int _id, const string& _name) : id(_id), name(_name) {}
};

// Class to represent a mode of transportation
class Transportation {
public:
    int mode;
    int cost;

    Transportation(int _mode, int _cost) : mode(_mode), cost(_cost) {}
};
class CityGraph {
private:
    // Adjacency list for graph representation
    map<int, vector<pair<int, pair<int, Transportation>>>> adjacencyList;

public:
    // Function to add an edge between two cities with transportation details
    void addEdge(int from, int to, int mode, int distance, int cost) {
        adjacencyList[from].push_back({to, {distance, Transportation(mode, cost)}});
        adjacencyList[to].push_back({from, {distance, Transportation(mode, cost)}});
    }

    // Dijkstra's algorithm to find shortest path from source to destination
    pair<int, vector<int>> shortestPath(int source, int destination, int mode) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        map<int, int> dist;
        map<int, int> parent;
        vector<int> path;

        // Initialize distances
        for (auto& entry : adjacencyList) {
            dist[entry.first] = numeric_limits<int>::max();
        }

        dist[source] = 0;
        pq.push({0, source});

        // Variables to store best cost and path for the selected mode
        int best_cost = numeric_limits<int>::max();
        vector<int> best_path;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (u == destination) {
                if (dist[u] < best_cost) {
                    best_cost = dist[u];
                    int node = u;
                    best_path.clear();
                    while (node != source) {
                        best_path.push_back(node);
                        node = parent[node];
                    }
                    best_path.push_back(source);
                    reverse(best_path.begin(), best_path.end());
                }
            }

            for (auto& neighbor : adjacencyList[u]) {
                int v = neighbor.first;
                int distance = neighbor.second.first;
                int cost = neighbor.second.second.cost;

                if (neighbor.second.second.mode == mode) {
                    if (dist[v] > dist[u] + cost) {
                        dist[v] = dist[u] + cost;
                        parent[v] = u;
                        pq.push({dist[v], v});
                    }
                }
            }
        }

        return {best_cost, best_path};
    }

    // Function to display all cities
    void displayCities(const vector<City>& cities) {
        for (const City& city : cities) {
            cout << city.id << ": " << city.name << endl;
        }
    }

    // Function to display all routes with transportation details
    void displayRoutes(const vector<City>& cities) {
        for (auto& entry : adjacencyList) {
            int from = entry.first;
            for (auto& neighbor : entry.second) {
                int to = neighbor.first;
                int distance = neighbor.second.first;
                int mode = neighbor.second.second.mode;
                int cost = neighbor.second.second.cost;

                cout << cities[from].name << " to " << cities[to].name << " - Distance: " << distance << " km, ";
                if (mode == TRAIN) cout << "Train";
                else if (mode == CAR) cout << "Car";
                else if (mode == AIRPLANE) cout << "Airplane";
                cout << " - Cost: $" << cost << endl;
            }
        }
    }
};
int main() {
    // Initialize cities
    vector<City> cities = {
        {0, "City A"},
        {1, "City B"},
        {2, "City C"},
        {3, "City D"},
        {4, "City E"}
    };

    // Initialize graph
    CityGraph graph;

    // Add transportation routes with distances and costs
    graph.addEdge(0, 1, TRAIN, 100, 50);
    graph.addEdge(0, 1, CAR, 120, 80);
    graph.addEdge(0, 1, AIRPLANE, 80, 200);

    graph.addEdge(1, 2, TRAIN, 150, 70);
    graph.addEdge(1, 2, CAR, 130, 90);
    graph.addEdge(1, 2, AIRPLANE, 100, 250);

    graph.addEdge(2, 3, TRAIN, 200, 90);
    graph.addEdge(2, 3, CAR, 180, 120);
    graph.addEdge(2, 3, AIRPLANE, 150, 300);

    graph.addEdge(3, 4, TRAIN, 120, 60);
    graph.addEdge(3, 4, CAR, 100, 80);
    graph.addEdge(3, 4, AIRPLANE, 90, 200);

    // Display all cities
    cout << "Cities:" << endl;
    graph.displayCities(cities);
    cout << endl;

    // Display all routes with transportation details
    cout << "Routes:" << endl;
    graph.displayRoutes(cities);
    cout << endl;

    // User interface to select source and destination cities
    int source, destination;
    cout << "Select source city (0-4): ";
    cin >> source;
    cout << "Select destination city (0-4): ";
    cin >> destination;

    // Display available transportation modes
    cout << "Select transportation mode:" << endl;
    cout << "0. Train" << endl;
    cout << "1. Car" << endl;
    cout << "2. Airplane" << endl;
    int mode;
    cin >> mode;

    // Validate user input
    if (source < 0 || source >= cities.size() || destination < 0 || destination >= cities.size() || mode < 0 || mode > 2) {
        cout << "Invalid input. Exiting..." << endl;
        return 1;
    }

    // Calculate shortest path and cost using Dijkstra's algorithm
    auto result = graph.shortestPath(source, destination, mode);

    // Output shortest path and cost
    int total_cost = result.first;
    vector<int> path = result.second;

    if (total_cost == numeric_limits<int>::max()) {
        cout << "No path found from city " << cities[source].name << " to city " << cities[destination].name << endl;
    } else {
        cout << "Shortest path from " << cities[source].name << " to " << cities[destination].name << " using ";
        if (mode == TRAIN) cout << "train";
        else if (mode == CAR) cout << "car";
        else if (mode == AIRPLANE) cout << "airplane";
        cout << ":" << endl;
        
        for (int i = 0; i < path.size(); ++i) {
            cout << cities[path[i]].name;
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
        cout << "Total cost: $" << total_cost << endl;
    }

    return 0;
}