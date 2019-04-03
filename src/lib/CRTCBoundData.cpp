/*
 * CRTCBoundData.cpp
 *
 *  Created on: Dec 27, 2018
 *      Author: Sneha Ravi Chandran
 */


#include <CRTCBoundData.h>

#include <math.h>

CRTCBoundData::CRTCBoundData(string regionID, string regionName,string minLatitude, string minLongitude, string maxLatitude, string maxLongitude)
{
   mRegionID = 0;
   std::stringstream stringToint;
   stringToint.str(regionID);
   stringToint >> mRegionID;
   mRegionName = regionName;
   mMinLatitude = minLatitude;
   mMinLongitude = minLongitude;
   mMaxLatitude = maxLatitude;
   mMaxLongitude = maxLongitude;
}


CRTCBoundData::CRTCBoundData(string regionID, string regionName)
{
   mRegionID = 0;
   std::stringstream stringToint;
   stringToint.str(regionID);
   stringToint >> mRegionID;
   mMinLatitude = "";
   mMinLongitude = "";
   mMaxLatitude = "";
   mMaxLongitude = "";
}


unsigned int CRTCBoundData::getRegionID()
{
   return mRegionID;
}
string CRTCBoundData::getRegionName()
{
   return mRegionName;
}
string CRTCBoundData::getMinLongitude()
{
   return mMinLongitude;
}
string CRTCBoundData::getMinLatitude()
{
   return mMinLatitude;
}
string CRTCBoundData::getMaxLatitude()
{
   return mMaxLatitude;
}
string CRTCBoundData::getMaxLongitude()
{
   return mMaxLongitude;
}

void CRTCBoundData::setMinLongitude(string minLongitude)
{
   mMinLongitude = minLongitude;
}
void CRTCBoundData::setMinLatitude(string minLatitude)
{
   mMinLatitude = minLatitude;
}
void CRTCBoundData::setMaxLatitude(string maxLatitude)
{
   mMaxLatitude = maxLatitude;
}
void CRTCBoundData::setMaxLongitude(string maxLongitude)
{
   mMaxLongitude = maxLongitude;
}
/********************************************************************************
 *                                                                              *
 * std::string formatLatitude(std::string degreeDecimal)                       *
 *                                                                              *
 *   std::string& path : String to format                                       *
 *                                                                              *
 *   Return std::string: Returns a populated string proper latitude format.    *
 *                                                                              *
 ********************************************************************************/
std::string CRTCBoundData::formatLatitude(std::string degreeDecimal)
{
   std::string degMinSec;
   /*Convert string into appropriate integer and float values*/
   unsigned int degrees = stoi(degreeDecimal.substr(1,2));
   /* extract and calculate minutes */
   float  minutes = (stof(degreeDecimal.substr(3,degreeDecimal.length()))*60/1000000);
   double minutes_double;
   float secondFractions= modf(minutes, &minutes_double);
   float seconds = secondFractions * 60;
   degMinSec = std::to_string(degrees) + " degrees " + std::to_string(minutes_double) + " minutes " + std::to_string(seconds) + " seconds";
   if(degreeDecimal.at(0) == '-')
   {
      degMinSec+= " South";
   }
   else if(degreeDecimal.at(0) == '+')
   {
      degMinSec += " North";
   }

   return degMinSec;

}

/********************************************************************************
 *                                                                              *
 *   std::string formatLongitude(std::string degreeDecimal)                     *
 *                                                                              *
 *   std::string& path : String to format                                       *
 *                                                                              *
 *   Return std::string: Returns a populated string proper longitude format.    *
 *                                                                              *
 ********************************************************************************/
std::string CRTCBoundData::formatLongitude(std::string degreeDecimal)
{
   std::string degMinSec;
   /*Convert string into appropriate integer and float values*/
   unsigned int degrees = stoi(degreeDecimal.substr(1,3));
   /* extract and calculate minutes */
   float  minutes = (stof(degreeDecimal.substr(4,degreeDecimal.length()))*60/1000000);
   double minutes_double;
   float secondFractions= modf(minutes, &minutes_double);
   float seconds = secondFractions * 60;
   degMinSec = std::to_string(degrees) + " degrees " + std::to_string(minutes_double) + " minutes " + std::to_string(seconds) + " seconds";
   /*Set east or west direct based on sign.*/
   if(degreeDecimal.at(0) == '-')
   {
      degMinSec += " West";
   }
   else if(degreeDecimal.at(0) == '+')
   {
     degMinSec += " East";
   }

   return degMinSec;
}
