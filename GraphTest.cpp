#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "Graph.h"
#include <vector>

using namespace std;

int main() {

    ////reminder how to initialize 2-D vector (from minesweeper ) :

    //int numRows = 200000;
    //int numColumn = numRows;
    //vector< vector<int> > grid (numRows, vector<int> (numColumn) );

    string csvFileName = "Augmented_avocado_Compress.csv";
    ifstream inCSV;

    string date, price, volume, type , region;

    int numData = 0; // just making sure I have read all the data , not neccessary

    set<string> regions;



    inCSV.open(csvFileName);

    //Begin reading in data

    if(inCSV.is_open()){

        getline(inCSV, date);  //get rid of header



        while(!inCSV.eof()){

            getline(inCSV, date, ',');
            numData++;

            getline(inCSV, price, ',');
            numData++;

            getline(inCSV, volume, ',');
            numData++;

            getline(inCSV, type, ',');
            numData++;
            getline(inCSV, region);
            numData++;

            if(region == "")

                //crete vertex here, pass it to the "insert edge" function

                //or pass it to the "verify distance" : it would prob return bool, then pass to insert function

                //regardless need to insert the vertex in the graph, add it on the x and y axis

                regions.emplace(region);
            numData++;


        }


    }

    cout << "num data points: " << numData << endl;

    cout << "num cities/regions: " << regions.size() << endl;


    cout << endl;

    for(auto iter = regions.begin(); iter != regions.end(); iter++){

        cout << *iter << endl;

    }

    /*cout << "date: " << date << endl;
    cout << "price: " << price << endl;
    cout << "volume: " << volume << endl;
    cout << "type: " << type << endl;
    cout << "region: " << region << endl;*/



    return 0;
}


