#pragma once
#include <string>
#include <vector>
#include "../datastructures/Graph.h"
#include "../models/Kota.h"
using namespace std;

namespace SwiftExpedition {

class RoutingService {
private:
    Graph graph;                       
    vector<Kota> edgesList;       

public:
    RoutingService();

    void loadKota(const string& filename);

    void addRoute(const string& asal, const string& tujuan, int jarak);

    void bfsSearch(const string& asal, const string& tujuan);

    void dfsSearch(const string& start);
    void findAllRoutes(const string& asal, const string& tujuan);

    void displayEdges() const;
    void displayGraph() const;
    void displayCities() const;

    Graph& getGraph();
    int getCityCount() const;
    vector<string> getAllCities() const;
};

} 