#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "Graph.h"
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <sstream>


#include <math.h>
#include <cmath>
#define earthRadiusKm 6371.0
using namespace std;


//HAVERSINE FUNCTION PROVIDED BY MrTJ on Stackoverflow
//used to calculate the distance between two cities (via distance formula on a sphere (HAVERSINE))
//https://stackoverflow.com/questions/10198985/calculating-the-distance-between-2-latitudes-and-longitudes-that-are-saved-in-a

// This function converts decimal degrees to radians
double deg2rad(double deg) {
    return (deg * M_PI / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
    return (rad * 180 / M_PI);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}


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
    //create look-up city index for the distance look-up table
    unordered_map<string, int> cityLookUp;
    int cityIndex;

    string tempCityName;
    int tempEdgeDistance;

    ReadCityLookUP(cityLookUp);




    //BEGINNING OF DISTANCE-TABLE CREATION
vector<vector<double>> coordinates;
string FileName = "Distance.csv";


string location, lati, longi;
double lat = 0;
double lon = 0;
ifstream CSVcoord;


CSVcoord.open(FileName);

if(CSVcoord.is_open()){

    while(!CSVcoord.eof()){
        getline(CSVcoord, location, ',');

        getline(CSVcoord, lati, ',');
        lat = stof(lati);


        getline(CSVcoord, longi);
        lon = stof(longi);

        vector<double> coords;
        coords.push_back(lat);
        coords.push_back(lon);
                                    //create a vector with lat, long coordinate pair and add it to the vector

        coordinates.push_back(coords);


    }

}


    int distLookUp[54][54];
    for(int i = 0; i < 54; i++){
        for(int j = 0; j < 54; j++){
            double lat1 = coordinates[i][0];
            double long1 = coordinates[i][1];
            double lat2 = coordinates[j][0];
            double long2 = coordinates[j][1];
            distLookUp[i][j] = int (distanceEarth(lat1, long1, lat2, long2));    //call upon our function to calculate the distance
        }
    }
    //END OF DISTANCE CREATION TABLE

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

                            if(distLookUp[cityIndex][column] < 965){
                                //capture cities within 965 kilometers (600 miles)

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

                            if(distLookUp[cityIndex][column] < 965){
                                //capture cities within 965 km (600 miles)

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
