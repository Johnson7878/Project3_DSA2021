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

    Vertex tempVertex;



    ////analysis variables (testing) begin
    int orgCount = 0;
    int convCount = 0;

    int numData = 0;

    set<string> regions;

    set<string> datesForCity;

    set<string> pricesSpecific;
    string specificTime = "2015-03-08";

    int dateCount = 0;

    ////analysis variables end



    inCSV.open(csvFileName);

    //Begin reading in data

    unordered_map<string, int> dateFreq;

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

            cityIndex = cityLookUp[region];  //now we know which index to lookup

            int column; //will not traverse the columns contagiously





            // never added this date "layer" before
            if(graphCollection[date].dateCount == 0 ) {

                graphCollection[date].dateCount++;

                //date count has been updated, now potentially insert vertex's edges

                for(auto iter = cityLookUp.begin(); iter != cityLookUp.end(); iter++){

                    column = iter->second;

                    if(distLookUp[cityIndex][column] < 600){
                        //capture cities within 600 miles

                        tempEdgeDistance = distLookUp[cityIndex][column];
                        tempCityName = iter->first;

                        graphCollection[date].InsertEdge(tempVertex,tempCityName,tempEdgeDistance);

                    }

                }


            }

                // date previously added, now need to create unique date for hash table
            else if(graphCollection[date].dateCount <= 6){

                int validDay = graphCollection[date].dateCount;
                date.append(to_string(validDay));                  //unique date

                graphCollection[date].dateCount++;

                //unique key and original date count has been updated, now potentially insert vertices
                for(auto iter = cityLookUp.begin(); iter != cityLookUp.end(); iter++){

                    column = iter->second;

                    if(distLookUp[cityIndex][column] < 600){
                        //capture cities within 600 miles

                        tempEdgeDistance = distLookUp[cityIndex][column];
                        tempCityName = iter->first;

                        graphCollection[date].InsertEdge(tempVertex,tempCityName,tempEdgeDistance);

                    }

                }

            }

                //date can no longer be used (7 layers already created of it = 7 days of the year)
            else{continue;}


        }


    }

    ////At this point, graphs and graph collection should be properly filled.

    // cout << "numDates Orlando: " << datesForCity.size() << endl;

    //cout << "num of dates for Orlando: " << dateCount << endl;

    //cout << "num data points: " << numData << endl;

    //cout << "num cities/regions: " << regions.size() << endl;


    //// Now until end is all analysis code

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



