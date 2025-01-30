#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue>  // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"

using namespace std;

// include this class as stated in the project doc
class prioritize {
 public:
  bool operator()(const pair<long long, double>& p1,
                  const pair<long long, double>& p2) const {
    return p1.second > p2.second;
  }
};

double INF = numeric_limits<double>::max();

void buildGraph(istream& input, graph<long long, double>& g,
                vector<BuildingInfo>& buildings) {
  // open the JSON file
  nlohmann::json jsonFile;
  input >> jsonFile;

  // read and add the buildings
  for (auto building : jsonFile["buildings"]) {
    // create a building of type BuildingInfo with id, location (a struct with
    // lon and lat), name, and abbr
    BuildingInfo myBuilding;
    myBuilding.id = building["id"];
    myBuilding.location.lat = building["lat"];
    myBuilding.location.lon = building["lon"];
    myBuilding.abbr = building["abbr"];
    myBuilding.name = building["name"];

    // store the building in the buildings array, which is a parameter
    buildings.push_back(myBuilding);

    // add the building's ID as a vertex into the graph, which is a parameter
    g.addVertex(myBuilding.id);
  }

  // create a map to store the waypoints
  unordered_map<long long, Coordinates> waypoints;

  // read and add the waypoints
  for (auto waypoint : jsonFile["waypoints"]) {
    Coordinates waypointCoords(waypoint["lat"], waypoint["lon"]);

    // store the waypoint coords
    waypoints[waypoint["id"]] = waypointCoords;

    // add the waypoint to the graph
    g.addVertex(waypoint["id"]);
  }

  // link the buildings to the waypoints if their distance is less than 0.036
  // miles
  for (auto currBuilding : buildings) {
    for (auto currWaypoint : waypoints) {
      // the building's location stores the Corrdinate
      // the waypoint's second entry stores the Corrdinate
      double distance =
          distBetween2Points(currBuilding.location, currWaypoint.second);
      // if the their distance is less than 0.036, add an edge
      if (distance < 0.036) {
        g.addEdge(currBuilding.id, currWaypoint.first, distance);
        g.addEdge(currWaypoint.first, currBuilding.id, distance);
      }
    }
  }

  // go thru each footway, which is an array of coordinates
  for (auto currFootway : jsonFile["footways"]) {
    // for each footway, create an edge between each coordinate pair
    for (int i = 0; i < currFootway.size() - 1; i++) {
      double distance = distBetween2Points(waypoints[currFootway[i]],
                                           waypoints[currFootway[i + 1]]);
      g.addEdge(currFootway[i], currFootway[i + 1], distance);
      g.addEdge(currFootway[i + 1], currFootway[i], distance);
    }
  }
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo>& buildings,
                             const string& query) {
  for (const BuildingInfo& building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo>& buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo& building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

// go from start to the target and ignore the specified nodes
vector<long long> dijkstra(const graph<long long, double>& G, long long start,
                           long long target,
                           const set<long long>& ignoreNodes) {
  // create the path
  vector<long long> path;

  // create the priority queue as stated in the project info guide
  priority_queue<pair<long long, double>, vector<pair<long long, double>>,
                 prioritize>
      worklist;

  // worklist requires: distance from start, predecessor, and seen nodes
  map<long long, double> distance;
  map<long long, long long> predecessor;
  set<long long> seen;

  // if the start and target are the same, return a vector of just the start
  if (start == target) {
    return {start};
  }

  // add the start node to the worklist
  worklist.push({start, 0});
  // the current distance from the start is 0, so set it as such
  distance[start] = 0;

  // while your worklist is not empty
  while (!worklist.empty()) {
    // store the info of the shortest distance element
    long long currNode = worklist.top().first;
    double currDistance = worklist.top().second;
    // pop the shortest distance or "time" as covered in lecture
    worklist.pop();

    // check to ensure the current node hasn't been visited
    // set.find() returns index or the last element if not found
    if (seen.find(currNode) == seen.end()) {
      seen.insert(currNode);
    }
    // if you found the target, stop
    if (currNode == target) {
      break;
    }

    // if you haven't found the target, keep working thru the nodes
    for (auto neighbor : G.neighbors(currNode)) {
      // if the neighbor is not to be ignored or if it's the target
      if (ignoreNodes.find(neighbor) == ignoreNodes.end() ||
          neighbor == target) {
        // get the weight from the current node to that node
        double edgeWeight;
        G.getWeight(currNode, neighbor, edgeWeight);
        // store the distance from the start to that node
        double nodeDistance = currDistance + edgeWeight;
        // if the neighbor has not been mapped in the distance map
        // or the node's distance is less than the already stored distance for
        // that node (for overriding)
        if (distance.find(neighbor) == distance.end() ||
            nodeDistance < distance[neighbor]) {
          // set/reset the distance for that neighbor
          distance[neighbor] = nodeDistance;
          // set/reset the predecessor for that neighbor
          predecessor[neighbor] = currNode;
          // add the neighbor to the worklist
          worklist.push({neighbor, nodeDistance});
        }
      }
    }
  }

  // if the target is unreachable, aka it has no distance from the start node,
  // return an empty set
  if (distance.find(target) == distance.end()) {
    return {};
  }

  // create the path from target to start using the predecessors
  long long currNode = target;
  while (currNode != start) {
    path.push_back(currNode);
    currNode = predecessor[currNode];
  }
  path.push_back(start);

  // the path is currently target to start, so reverse it
  reverse(path.begin(), path.end());

  // return the path from start to target
  return path;
}

double pathLength(const graph<long long, double>& G,
                  const vector<long long>& path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long>& path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

void application(const vector<BuildingInfo>& buildings,
                 const graph<long long, double>& G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto& building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}