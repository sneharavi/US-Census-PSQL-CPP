/*
 * CParseCensusFiles.h
 *
 *  Created on: Dec 27, 2018
 *      Author: Sneha Ravi Chandran
 */

#ifndef CCENSUSFILEMANAGER_H_
#define CCENSUSFILEMANAGER_H_

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <postgresql/libpq-fe.h>

#include <CRTCBoundData.h>

/*for the scope of the program */
using std::cout;
using std::string;
using std::cin;
using std::ifstream;
using std::cerr;
using std::vector;
using std::map;
using std::modf;

typedef std::vector<CRTCBoundData> rtc_bound_data_vector_t;

class CCensusFileManager
{

   private:
      rtc_bound_data_vector_t mStateBoundsDataVector;
      rtc_bound_data_vector_t mCountyBoundsDataVector;
      rtc_bound_data_vector_t mSubCountyBoundsDataVector;
      rtc_bound_data_vector_t mPlaceBoundDataVector;
      string censusFileDirectory;
      static PGconn *dbConnection;
      string trim(std::string str);
      int_least32_t openConnection();
      int_least32_t closeConnection();
      PGresult* processQuery(const string queryString);
      vector<string> listFiles( std::string& path, std::string& fileType );
      stringstream stringToInt;
   public:
      CCensusFileManager();
      ~CCensusFileManager();
      rtc_bound_data_vector_t GetStateBoundsDataVector();
      rtc_bound_data_vector_t GetCountyBoundsDataVector();
      rtc_bound_data_vector_t GetSubCountyBoundsDataVector();
      rtc_bound_data_vector_t GetPlaceBoundDataVector();
      int_least32_t saveCensusData();
      int_least32_t parseBoundData();
      int_least32_t parseRTCData();
};

#endif /* CCENSUSFILEMANAGER_H_ */
