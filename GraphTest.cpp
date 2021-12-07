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



    ////analysis variables (testing) begin
    int orgCount = 0;
    int convCount = 0;

    int numData = 0;

    set<string> regions;

    set<string> datesForCity;

    set<string> pricesSpecific;
    string specificTime = "2015-03-08";

    unordered_map<string, int> dateFreq;

    int dateCount = 0;

    ////analysis variables end



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

            region.pop_back();  // remove line break character


            //analysis of csv file done inside:
            if(region == "Orlando"){

                datesForCity.emplace(date);

                dateFreq[date]++;

                if( date == specificTime ){

                    if(type == "organic")
                        orgCount++;
                    if(type == "conventional")
                        convCount++;

                    pricesSpecific.emplace(price);

                }

                //dateCount++;


            }

            //tempVertex = Vertex(region, stof(price), stof(volume) );

            tempVertex.volume= stof(volume);
            tempVertex.name = region;
            tempVertex.price =stof(price);


            //now we know which index(row) to match Dist-lookup column
            cityIndex = cityLookUp[region];

            int column; //will not traverse the columns contagiously: use iterator

            // never added this date "layer" before
            if(graphCollection[date].dateCount == 0 ) {

                graphCollection[date].dateCount++;

                //date count has been updated, now potentially insert vertex's edges

                if(graphCollection[date].adjList[region].empty()){  //if adjList @ index "region" is empty, add itself to index 0
                    graphCollection[date].InsertEdge(region, tempVertex,0);
                }

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

                // date previously added, now need to create unique date for hash table
            else if(graphCollection[date].dateCount <= 6){

                ////TODO <= 6 is probably incomplete logic:
                /*
                 *  Must guarantee no more than 7 (0-6) dates for a single CITY/REGION
                 *  problem: this code will only add 7 nodes to a graph
                 *  need extra logic for 54 unique cities * 7 layers or something similar
                 */

                int validDay = graphCollection[date].dateCount;
                date.append(to_string(validDay));                  //unique date

                graphCollection[date].dateCount++;

                if(graphCollection[date].adjList[region].empty()){  //if adjList @ index "region" is empty, add itself to index 0
                    graphCollection[date].InsertEdge(region, tempVertex,0);
                }

                //secondary use of cityLookUp, now using to iterate through columns (same ordering as rows)

                //unique key and original date count has been updated, now potentially insert vertices
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

                //date can no longer be used (7 layers already created of it = 7 days of the year)
            else{continue;}


        }


    }

    ////At this point, graphs and graph collection should be properly filled.

    //// Now until end is all analysis code

    // cout << "numDates Orlando: " << datesForCity.size() << endl;

    //cout << "num of dates for Orlando: " << dateCount << endl;

    //cout << "num data points: " << numData << endl;

    //cout << "num cities/regions: " << regions.size() << endl;


    /* cout << endl;

     int frequencyCount = 0;

     for(auto iter = datesForCity.begin(); iter != datesForCity.end(); iter++){

         cout << " date: " << *iter << endl;


     }

     cout << endl;
     cout << endl;

     cout << "on the data of " << specificTime << " ,there were: " << endl;
     cout << "Convetional: " << convCount << endl;
     cout << "Organic: " << orgCount << endl;



     for(auto iter = dateFreq.begin(); iter != dateFreq.end(); iter++){

         cout << "date: " << iter->first ;
         cout << ",  frequency : " << iter->second << endl;

         frequencyCount+=iter->second;


     }

     cout << "num Unique dates: " << dateFreq.size() << endl; // should be 169

     cout << "total number of dates " << frequencyCount << endl;

     float average = (float)frequencyCount / dateFreq.size();
     cout << "Average Frequency: " <<average << endl;

     cout << endl;

     cout << "number of prices asociated w/ : 2016-07-24: " << pricesSpecific.size() << endl;

     cout << endl;
     cout << endl;
     cout << endl;

     for (auto iter = pricesSpecific.begin(); iter != pricesSpecific.end(); iter++){

         cout << "price: " << *iter << endl;

     }*/


    string test = "2015-03-08";

    int six = 6;

    test.append(to_string(0));

    cout << "test: " << test << endl;





    /*for(auto iter = regions.begin(); iter != regions.end(); iter++){

        cout << *iter << endl;

    }*/

    /*cout << "date: " << date << endl;
    cout << "price: " << price << endl;
    cout << "volume: " << volume << endl;
    cout << "type: " << type << endl;
    cout << "region: " << region << endl;*/



    return 0;
}


