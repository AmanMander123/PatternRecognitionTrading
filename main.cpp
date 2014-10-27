//
//  main.cpp
//  BinaryOptions
//
//  Created by Aman on 2014-09-15.
//  Copyright (c) 2014 Aman. All rights reserved.
//

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "unistd.h"

#include <iostream>
#include <cmath>
using std::cout;
using std::endl;
using std::ifstream;

using namespace std;

typedef vector<double> record_t;
typedef vector<record_t> data_t;

ofstream AccuracyFile;

////READ TEXT FILE
//Overload input operator to read fields
istream& operator >> (istream& ins, record_t& record) {
    
    //Clear record
    record.clear();
    
    //Read line into string
    string line;
    getline(ins, line);
    
    //Separate fields out of line
    stringstream ss(line);
    string field;
    while (getline(ss, field, ',')) {
        //Convert each field into a double
        stringstream fs(field);
        double f = 0.0;
        fs >> f;
        
        //Add to end of record
        record.push_back(f);
        
        
    }
    return ins;
}

//Overload input operator to read records
istream& operator >> (istream& ins, data_t& data) {
    
    //Clear data
    data.clear();
    
    //Read record and append to data
    record_t record;
    while (ins >> record) {
        data.push_back(record);
    }
    return ins;
}

//-----------------------------------------------------------------------------

//Percent Change calculator
double percentChange(double startPoint, double currentPoint)
{
    double x = ((currentPoint - startPoint) / abs(startPoint))*100;
    if (x == 0) {
        return 0.0000001;
    } else {
        return x;
    }
}
//-----------------------------------------------------------------------------

//////  STORE PATTERN  /////////
void storePattern(record_t &avgLine, record_t &performanceAr, data_t &patternAr, double pointsAhead, double storePoints) {
    record_t pattern;
    vector<double>::size_type x;
    x = avgLine.size() - pointsAhead;
    int y = storePoints + 1;
    while (y < x)
    {
        pattern.clear();
        for (int j = (storePoints - 1); j >= 0; --j)
        {
            pattern.push_back(percentChange(avgLine[y-storePoints], avgLine[y-j]));
        }
    
        double outcome = avgLine[(y+pointsAhead)];
        double currentPoint = avgLine[y];
        double futureOutcome = percentChange(currentPoint, outcome);
        
        patternAr.push_back(pattern);
    
        performanceAr.push_back(futureOutcome);
        y++;
    }
}
//-----------------------------------------------------------------------------

//////  CURRENT PATTERN  /////////
void currentPattern(record_t &avgLine, record_t &patForRec, double storePoints) {

    vector<double>::size_type lengthOfavgLine = avgLine.size();
    for (int i = storePoints; i > 0; --i)
    {
        patForRec.push_back(percentChange(avgLine[lengthOfavgLine-(storePoints + 1)],avgLine[lengthOfavgLine-i]));
    }
}
//-----------------------------------------------------------------------------

//////  PATTERN RECOGNITION  /////////
void patternRecognition(int toWhat, data_t &patternAr, record_t &patForRec, record_t &performanceAr, record_t &allData,
                        record_t &accuracyArray, double pointsAhead, double storePoints) {
    record_t sim;
    int patFound = 0;
    data_t plotPatAr;
    record_t eachPatt;
    record_t indexPlace;
    record_t predictedOutcomesAr;
    record_t predArray;
    plotPatAr.clear();
    indexPlace.clear();
    double howSim;
    for (int i = 0; i < patternAr.size(); i++)
    {
        sim.clear();
        eachPatt.clear();
        for (int j = 0; j < storePoints; j++)
        {
            sim.push_back(100 - abs(percentChange(patternAr[i][j], patForRec[j])));
            eachPatt.push_back(patternAr[i][j]);
        }
        
        double sum = 0;
        for (int k = 0; k < storePoints; k++)
        {
            if (!std::isinf(sim[k])) {
                sum += sim[k];
            }
        }
        howSim = sum / sim.size();
        
        if (howSim > 80) {
            patFound = 1;
            indexPlace.push_back(i);
            plotPatAr.push_back(eachPatt);
        }
    }

    //////IF PATTERN FOUND
    if (patFound == 1) {
        predictedOutcomesAr.clear();
        predArray.clear();
    
        for (int i = 0; i < plotPatAr.size(); i++)
        {
            if (performanceAr[indexPlace[i]] > patForRec[(storePoints-1)]) {
                predArray.push_back(1.00);
            } else {
                predArray.push_back(-1.00);
            }
            predictedOutcomesAr.push_back(performanceAr[indexPlace[i]]);
        }
    
        double realOutcome = allData[(toWhat + pointsAhead)];
        double realMovement = percentChange(allData[toWhat], realOutcome);
        
        double sum;
       
    
        vector<double>::size_type lengthOfPredArrray = predArray.size();
        sum = 0;
        for (int k = 0; k < lengthOfPredArrray; k++)
        {
            sum += predArray[k];
        }
        double predictionAverage = sum / predArray.size();
        if (predictionAverage < 0) {
            if (realMovement < 0) {
                accuracyArray.push_back(100);
            } else {
                accuracyArray.push_back(0);
            }
           
        }
        if (predictionAverage > 0) {
            if (realMovement > 0) {
                accuracyArray.push_back(100);
            } else {
                accuracyArray.push_back(0);
            }
        }
    }
}
//-----------------------------------------------------------------------------

int main(int argc, const char * argv[])
{
    
    //Create data table
    data_t data;
    
    //Read data from file
    ifstream infile( <PATH TO INPUT FILE> );
    infile >> data;
    //Complain if something went wrong
    if (!infile.eof()) {
        cout << "Something went wrong!\n";
        return 1;
    }
    infile.close();
    
    //Average bid and ask into one price
    record_t allData;
    vector<double>::size_type length;
    length = data.size();
    
    for (int i = 0; i < length; i++) {
        allData.push_back(data[i][0]);
    }
    

    AccuracyFile.open ( < PATH TO OUTPUT FILE> );
    //Start main loop
    //Set loop variables
    
    
    record_t pointsAheadVec;
    pointsAheadVec.push_back(1);
    pointsAheadVec.push_back(5);
    pointsAheadVec.push_back(15);
    pointsAheadVec.push_back(30);
    pointsAheadVec.push_back(60);
    
    record_t storeVec;
    for (int upno = 10; upno < 120; upno=upno+5) {
       storeVec.push_back(upno);
  }
    
    for (int pointsCounter = 0; pointsCounter < pointsAheadVec.size(); pointsCounter++) {
        for (int storeCounter = 0; storeCounter < storeVec.size(); storeCounter++) {
        
        
        //Train for 3 quarters of the total data
        double toWhat = allData.size() - allData.size()/4;

        //Initialize variables
        double samps = 0;
        record_t accuracyArray;
        accuracyArray.clear();
        double accuracyAverage = 0;
        
        int pointsAhead = pointsAheadVec[pointsCounter];
        int storePoints = storeVec[storeCounter];
    
        while (toWhat < allData.size()) {
        
        //Extract prices until toWhat into avgLine
        record_t::const_iterator first = allData.begin();
        record_t::const_iterator last = allData.begin() + toWhat;
        record_t avgLine(first, last);
        
        //STORE PATTERN
        record_t performanceAr;
        data_t patternAr;
        performanceAr.clear();
        patternAr.clear();
        storePattern(avgLine, performanceAr, patternAr, pointsAhead, storePoints);
        
        //CURRENT PATTERN
        record_t patForRec;
        patForRec.clear();
        currentPattern(avgLine,patForRec,storePoints);
        
        patternRecognition(toWhat, patternAr, patForRec, performanceAr, allData, accuracyArray,pointsAhead, storePoints);
        
        avgLine.clear();
        toWhat++;
        samps++;
        vector<double>::size_type lengthOfaccuracyArray = accuracyArray.size();
        double sum = 0;
        for (int k = 0; k < lengthOfaccuracyArray; k++)
        {
            sum += accuracyArray[k];
        }
        accuracyAverage = sum / accuracyArray.size();
        
    } // ends while loop

            AccuracyFile << pointsAhead << ", " << storePoints << ", " << accuracyAverage << endl;
    } // ends store loop
        
    } // ends points ahead loop
    AccuracyFile.close();
    return 0;
}



