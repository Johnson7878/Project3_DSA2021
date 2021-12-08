#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <map>
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


int main() {

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


    int distanceTable[54][54];
    for(int i = 0; i < 54; i++){
        for(int j = 0; j < 54; j++){
            double lat1 = coordinates[i][0];
            double long1 = coordinates[i][1];
            double lat2 = coordinates[j][0];
            double long2 = coordinates[j][1];
            distanceTable[i][j] = int (distanceEarth(lat1, long1, lat2, long2));    //call upon our function to calculate the distance
        }
    }


    return 0;
}
