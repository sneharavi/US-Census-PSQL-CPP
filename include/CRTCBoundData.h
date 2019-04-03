/*
 * CRTCBoundData.h
 *
 *  Created on: Dec 27, 2018
 *      Author: Sneha Ravi Chandran
 */

#ifndef SRC_LIB_CRTCBOUNDDATA_H_
#define SRC_LIB_CRTCBOUNDDATA_H_

#include <string>
#include <sstream>

using std::string;
using std::stringstream;

class CRTCBoundData
{
   private:
      unsigned int mRegionID = 0;
      string mRegionName = "";
      string mMinLatitude = "";
      string mMinLongitude = "";
      string mMaxLatitude = "";
      string mMaxLongitude = "";

   public:
      CRTCBoundData(string regionID, string regionName,string minLatitude, string minLongitude, string maxLatitude, string maxLongitude);
      CRTCBoundData(unsigned int regionID, string regionName,string minLatitude, string minLongitude, string maxLatitude, string maxLongitude);
      CRTCBoundData(string regionID, string regionName);
      CRTCBoundData(){};
      ~CRTCBoundData(){};
      unsigned int getRegionID();
      string getRegionName();
      string getMinLatitude();
      string getMinLongitude();
      string getMaxLatitude();
      string getMaxLongitude();
      void setMinLatitude(string minLatitude);
      void setMinLongitude(string minLongitude);
      void setMaxLatitude(string maxLatitude);
      void setMaxLongitude(string maxLongitude);
      string formatLatitude(std::string degreeDecimal);
      string formatLongitude(std::string degreeDecimal);
};

#endif /* SRC_LIB_CRTCBOUNDDATA_H_ */
