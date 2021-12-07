#pragma once
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct Vertex{

    //dont need Date, Date used to distinguish the many graph variables

    string name;
    float price;
    float volume; //need volume?


    Vertex(){name = ""; price = 0; volume = 0;}

    Vertex(string _name, float _price, float _volume){
        name = _name; price = _price; volume = _volume;
    }

};

class Graph{


    //// organic attribute of a graph could be used to easily double the number of vertices in the graph
    // bool isOrganic;

    //used if graph is adj Matrix, won't need it for adjList
    // int gridSize = 200000; //200k

    //// Distance Matrix should have access in main (maybe make it static?)
    //vector< vector<int> > distLookUp(54, vector<int>(54));    //54 x 54

    //Key is a vertex, Value is the adjList, Pair of: destination name, then edge weight
    unordered_map< Vertex, vector< pair<string, int> > > adjList;


public:

    int dateCount;
    /*
    * from a valid date, Exp: 11/04/15  , 1st time we read this date, date count = 0,
    * second time we read the date, date count = 1, meaning : 11/ 05 / 15,
    * this way we can get dates that are not actually part of the data set,
    * also a dateCount is much simpler than creating and verifying that a valid date exists
    */

    Graph( int _dateCount );

    void InsertEdge(Vertex &from, string &to, int distance);




};


Graph::Graph(int _dateCount){dateCount = _dateCount;}

void Graph::InsertEdge(Vertex &from, string &to, int distance) {

    pair<string, int> edge;
    edge.first = to;
    edge.second = distance;

    adjList[from].push_back(edge);

}

/*int Graph::getDateCount() const {
    return dateCount;
}

void Graph::setDateCount(int _dateCount) {
    Graph::dateCount = _dateCount;
}*/
