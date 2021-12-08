#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "Graph.h"
#include <vector>
#include <unordered_map>

using namespace std;

void ReadCityLookUP(unordered_map<string, int> &_cityLook){

    //used for distance lookup table
    int index = 0;

    //key
    string tempCity;

    string cityListFile = "UniqueCities.txt";

    ifstream inCSV;  //re-use ifstream obj name

    inCSV.open(cityListFile);


    if(inCSV.is_open()){

        while(!inCSV.eof()){

            getline(inCSV, tempCity);


            _cityLook[tempCity] = index;
            index++;


        }



    }




}

int main() {

    //creat look-up city index for the distance look-up table
    unordered_map<string, int> cityLookUp;
    int cityIndex;

    string tempCityName;
    int tempEdgeDistance;

    ReadCityLookUP(cityLookUp);

    ////TODO: fill in code to make distLookUP
    vector< vector<int> > distLookUp(54, vector<int>(54));    //54 x 54

    //this is where we store the graphs objects into "layers" based on date
    unordered_map<string, Graph> graphCollection;


    string csvFileName = "Augmented_avocado_Compress.csv";
    ifstream inCSV;

    string date, price, volume, type , region;

    Vertex tempVertex;      //used for index  0 of adjList, represents source data
    Vertex dummyVertex;    // used for other indices of adjList, only using Vertex for consistency with index 0

    string charConv = "C";
    string charOrganic = "O";

    inCSV.open(csvFileName);


    //Begin reading in data

    if(inCSV.is_open()){

        getline(inCSV, date);  //get rid of header


        while(!inCSV.eof()){

            getline(inCSV, date, ',');

            getline(inCSV, price, ',');

            getline(inCSV, volume, ',');

            getline(inCSV, type, ',');

            getline(inCSV, region);

            region.pop_back();  // remove line break character


            //tempVertex = Vertex(region, stof(price), stof(volume) );

            tempVertex.volume= stof(volume);
            tempVertex.name = region;
            tempVertex.price =stof(price);


            //now we know which index(row) to match Dist-lookup column
            cityIndex = cityLookUp[region];


            //will not traverse the columns contagiously: use iterator
            int column;

            if(type == "organic"){date.append(charOrganic);}         //add 'O' to date, make date(key) dependent on type
            else{date.append(charConv);}

            // never added this date "layer" before
            if(graphCollection[date].dateCount == 0 ) {


                // now potentially insert vertex's edges, there may be duplicates

                if(graphCollection[date].adjList.size() < 54){

                    /*  if adjList @ index "region" is empty, add itself to index 0
                     *  index 0 stores the source vertex infro like: price, volume, distance = 0 bec it is itself
                     *  then add its adj: this process only done once per layer, per region
                     */

                    if(graphCollection[date].adjList[region].empty()){

                        graphCollection[date].InsertEdge(region, tempVertex,0);

                        ////remember: cityLookUp is helper lookup for dist table look up

                        //secondary use of cityLookUp, now using to iterate through columns (same ordering as rows)
                        for(auto iter = cityLookUp.begin(); iter != cityLookUp.end(); iter++){

                            column = iter->second;

                            if(distLookUp[cityIndex][column] < 600){
                                //capture cities within 600 miles

                                //need to create dummy Vertex (only use name attribute)
                                dummyVertex.name = iter->first;

                                tempEdgeDistance = distLookUp[cityIndex][column];

                                graphCollection[date].InsertEdge(region,dummyVertex,tempEdgeDistance);

                            }

                        }

                    }

                }
                else{

                    //once the graph has been filled (all 54 city/region), increment to next "layer" : next date
                    // the beginning of the next if-branch will apend this date count
                    graphCollection[date].dateCount++;
                }


            }

                // date previously added, now need to create unique date for hash table
            else if(graphCollection[date].dateCount <= 6){

                //current layer is filled, augment the date(key) to progress to next layer
                if(graphCollection[date].adjList.size() == 54){
                    int validDay = graphCollection[date].dateCount;    //should have been incremented in a previous iteration
                    date.append(to_string(validDay));                  //unique date
                }

                // now potentially insert vertex's edges, there may be duplicates

                if(graphCollection[date].adjList.size() < 54){

                    /*  if adjList @ index "region" is empty, add itself to index 0
                     *  index 0 stores the source vertex infro like: price, volume, distance = 0 bec it is itself
                     *  then add its adj: this process only done once per layer, per region
                     */

                    if(graphCollection[date].adjList[region].empty()){

                        graphCollection[date].InsertEdge(region, tempVertex,0);

                        ////remember: cityLookUp is helper lookup for dist table look up

                        //secondary use of cityLookUp, now using to iterate through columns (same ordering as rows)
                        for(auto iter = cityLookUp.begin(); iter != cityLookUp.end(); iter++){

                            column = iter->second;

                            if(distLookUp[cityIndex][column] < 600){
                                //capture cities within 600 miles

                                //need to create dummy Vertex (only use name attribute)
                                dummyVertex.name = iter->first;

                                tempEdgeDistance = distLookUp[cityIndex][column];

                                graphCollection[date].InsertEdge(region,dummyVertex,tempEdgeDistance);

                            }

                        }

                    }

                }
                else{

                    //once the graph has been filled (all 54 city/region), increment to next "layer" : next date
                    graphCollection[date].dateCount++;
                }


            }

                //date can no longer be used (7 layers already created of it = 7 days of the year)
            else{continue;}


        }


    }

    ////At this point, graphs and graph collection should be properly filled.



    //how I make the keys unique:

    string test = "2015-03-08";

    int six = 6;

    test.append(to_string(six));

    cout << "test: " << test << endl;



    return 0;
}
