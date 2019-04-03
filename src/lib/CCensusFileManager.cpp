/*************************************************************************************
 * CCensusFileManager.cpp                                                             *
 *                                                                                   *
 * Created on: Dec 27, 2018                                                          *
 * Author: Sneha Ravi Chandran                                                       *
 *************************************************************************************/

#include <CCensusFileManager.h>

#include <dirent.h>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <utility>
#include <cstdio>

CCensusFileManager::CCensusFileManager()
{

}

rtc_bound_data_vector_t CCensusFileManager::GetStateBoundsDataVector()
{
   return mStateBoundsDataVector;
}
rtc_bound_data_vector_t CCensusFileManager::GetCountyBoundsDataVector()
{
   return mCountyBoundsDataVector;
}
rtc_bound_data_vector_t CCensusFileManager::GetSubCountyBoundsDataVector()
{
   return mSubCountyBoundsDataVector;
}
rtc_bound_data_vector_t CCensusFileManager::GetPlaceBoundDataVector()
{
   return mPlaceBoundDataVector;
}

int_least32_t CCensusFileManager::saveCensusData()
{
   if(openConnection() != 0)
   {
      return -1;
   }

   char format_str[4096];
   for(auto stateData : mStateBoundsDataVector)
   {
      string queryString = "INSERT INTO z1856678.STATE (id,iname,minlatitude,maxlatitude,minlongitude,maxlongitude) values (" +
               std::to_string(stateData.getRegionID()) +"," + stateData.getRegionName() +","+
               stateData.getMinLatitude()+","+stateData.getMaxLatitude()+","+
               stateData.getMinLongitude()+","+stateData.getMaxLongitude()+ ") on conflict (id) do nothing;";
      processQuery(format_str);
   }

   for(auto countyData : mCountyBoundsDataVector)
   {
      string queryString = "INSERT INTO z1856678.COUNTY (id,iname,minlatitude,maxlatitude,minlongitude,maxlongitude) values (" +
               std::to_string(countyData.getRegionID()) + "," + countyData.getRegionName() +","+
               countyData.getMinLatitude()+ "," + countyData.getMaxLatitude()+","+
               countyData.getMinLongitude()+ "," + countyData.getMaxLongitude()+ ") on conflict (id) do nothing;";
      processQuery(format_str);
   }

   for(auto subCountyData : mSubCountyBoundsDataVector)
   {
      string queryString = "INSERT INTO z1856678.SUBCOUNTY (id,iname,minlatitude,maxlatitude,minlongitude,maxlongitude) values (" +
               std::to_string(subCountyData.getRegionID()) +"," + subCountyData.getRegionName() +","+
               subCountyData.getMinLatitude() + "," + subCountyData.getMaxLatitude()+","+
               subCountyData.getMinLongitude() + "," + subCountyData.getMaxLongitude()+ ") on conflict (id) do nothing;";
      processQuery(format_str);
   }

   for(auto placeData : mPlaceBoundDataVector)
   {
      string queryString = "INSERT INTO z1856678.PLACE (id,iname,minlatitude,maxlatitude,minlongitude,maxlongitude) values (" +
               std::to_string(placeData.getRegionID()) + "," + placeData.getRegionName() +","+
               placeData.getMinLatitude() + "," + placeData.getMaxLatitude()+","+
               placeData.getMinLongitude() + "," + placeData.getMaxLongitude() + ") on conflict (id) do nothing;";
      processQuery(format_str);
   }

   if(closeConnection() != 0)
   {
      return -1;
   }

   return 0;
}

PGresult* CCensusFileManager::processQuery(const string queryString)
{

   PGresult* queryResult = PQexec(dbConnection, queryString.c_str());
   if(( PQresultStatus(queryResult) == PGRES_COMMAND_OK) || ( PQresultStatus(queryResult) == PGRES_TUPLES_OK))
   {
      cout << "Completed: " << queryString << std::endl;
      return queryResult;
   }
   cout << "Failed: " << queryString << std::endl;

   PQclear(queryResult);
   return NULL;
}

int_least32_t CCensusFileManager::openConnection()
{
   dbConnection = PQconnectdb("host=localhost dbname=z1856678 password=123456");
   if(PQstatus(dbConnection) != CONNECTION_OK)
   {
      return -1;
   }
   return 0;
}

int_least32_t CCensusFileManager::closeConnection()
{
   if(PQstatus(dbConnection) == CONNECTION_OK)
   {
      PQfinish(dbConnection);
      return 0;
   }
   return -1;
}


/********************************************************************************
 *                                                                              *
 * std::vector<string> listFiles( std::string& path, std::string& fileType )    *
 *                                                                              *
 *   std::string& path : Directory path to search                               *
 *   std::string& fileType:  extension to search for                            *
 *                                                                              *
 *   Returns std::vector<string>: Returns a populated string of file names of    *
 *   that extension.                                                            *
 * ******************************************************************************/
std::vector<string> CCensusFileManager::listFiles( std::string& path, std::string& fileType )
{
   DIR* dirFile = opendir( path.c_str() );
   std::vector<string> filesFound;
   if ( dirFile )
   {
      struct dirent* hFile;
      errno = 0;
      while (( hFile = readdir( dirFile )) != NULL )
      {
         if (( strcmp( hFile->d_name, "."  ) != 0) && (strcmp( hFile->d_name, ".." ) != 0) && (  hFile->d_name[0] != '.' ))
         {
            string fileString = std::string(hFile->d_name);
            if ((fileString.substr(fileString.length()-4,4)).compare(fileType) == 0)
            {
               filesFound.push_back(hFile->d_name);
            }
         }
      }
      closedir( dirFile );
   }
   return filesFound;
}


/********************************************************************************
 *                                                                              *
 * std::string trim(std::string str)                                            *
 *                                                                              *
 *   std::string& path : String to strip whitespace from.                       *
 *                                                                              *
 *   Return std::string: Returns a populated string with spaces removed.        *
 *                                                                              *
 ********************************************************************************/
std::string CCensusFileManager::trim(std::string str)
{
   string returnString;
   for ( int i=0; i< str.size(); i++)
   {
      if((str[i] == ' ') && (str[i+1] == ' '))
      {
         break;
      }
      else
      {
         returnString = returnString  + str[i];
      }
    }
   return returnString;
}

int_least32_t CCensusFileManager::parseRTCData()
{
   string rtcFileType(".RTC");
   string userOption;
   string rtcFileName;
   std::stringstream stringToint;
   ifstream regionIdNameFileStream;
   bool try_again = true;
   do
   {
      unsigned int itemNumber = 1;
      unsigned int chosenOption;
      std::vector<string> rtcFiles = listFiles(censusFileDirectory, rtcFileType);
      cout << "\nFound " + std::to_string(rtcFiles.size()) + " RTC files in " + censusFileDirectory + "\n";
      chosenOption = rtcFiles.size() + 1;
      for (string rtcFile : rtcFiles)
      {
         cout << "\t\t" << std::to_string(itemNumber) << ")" << rtcFile << std::endl;
         itemNumber++;
      }
      userOption.clear();
      cout << "\t\t" << std::to_string(itemNumber) << ")Exit" << std::endl;
      cout << "\nPlease select an option to proceed:  ";
      getline(cin, userOption);
      stringToint.str(userOption);
      stringToint >> chosenOption;
      if (!stringToint.fail())
      {
         int option = chosenOption - 1;
         if ((option < rtcFiles.size()) && (option >= 0))
         {
            rtcFileName = censusFileDirectory + "/" + rtcFiles.at(option);
            try_again = false;
         }
         else if (chosenOption == (rtcFiles.size() + 1))
         {
            cout << "Thank you for using the tool" << std::endl;
            exit(0);
         }
         else
         {
            cerr << "Invalid input try again.\n" << std::endl;
         }
         stringToint.clear();
      }
      else
      {
         cerr << "Invalid input try again.\n" << std::endl;
         stringToint.clear();
      }
   }
   while (try_again);

   /* open the file stream */

   regionIdNameFileStream.open(rtcFileName);
   string regionIDNameString;
   std::cout << "Adding Regions Data to tables... \n";
   string stateID, countyID, subCountyID, placeID, regionName;
   unsigned int regionID;
   // reads the input file line by line
   while (getline(regionIdNameFileStream, regionIDNameString))
   {
      /* ignores the records having 1990 and choose only records having 2000 or blank spaces*/
      if (regionIDNameString[10] == '2' || regionIDNameString[10] == ' ')
      {
         /* Parse and add state*/
         if (regionIDNameString[24] == 'S')
         {
            stateID = regionIDNameString.substr(5, 2); // takes both the characters and concatenates as the state ID
            regionName = trim(regionIDNameString.substr(52, 71));
            CRTCBoundData rtcData = CRTCBoundData(stateID, regionName);
            mStateBoundsDataVector.push_back(rtcData);
         }
         else if (regionIDNameString[24] == 'P')
         {
            /* Parse and add places*/
            placeID = regionIDNameString.substr(5, 2) + regionIDNameString.substr(14, 5); // takes the characters specified and concatenates as place id
            regionName = trim(regionIDNameString.substr(52, 71));
            CRTCBoundData rtcData = CRTCBoundData(placeID, regionName);
            mPlaceBoundDataVector.push_back (rtcData);
         }
         else if (regionIDNameString[24] == 'C')
         {
            /* Parse and add counties*/
            countyID = regionIDNameString.substr(5, 5); // takes the characters specified and concatenates as countyid
            regionName = trim(regionIDNameString.substr(52, 71));
            stringToInt.str(countyID);
            stringToint >> regionID;
            mCountyBoundsDataVector.push_back(CRTCBoundData(countyID, regionName));
         }
         else if (regionIDNameString[24] == 'M')
         {
            /* Parse and add sub counties*/
            subCountyID = regionIDNameString.substr(5, 2) + regionIDNameString.substr(14, 5); // takes the characters specified and concatenates as subcountyid
            regionName = trim(regionIDNameString.substr(52, 71));
            stringToInt.str(subCountyID);
            stringToint >> regionID;
            mSubCountyBoundsDataVector.push_back(CRTCBoundData(subCountyID, regionName));
         }
      }
   }

   regionIdNameFileStream.close();
   /* Prompt user to load another RTC file. */
   userOption.clear();
   stringToint.clear();
   cout << "\nDo you want to open more RTC files? ('y' - yes , 'e' - exit, default no) :";
   getline(cin, userOption);
   if (userOption.compare("y") == 0)
   {
      parseRTCData();
   }
   else if (userOption.compare("e") == 0)
   {
      cout << "\nThank you so much have a great day!\n";
      exit(0);
   }
   else
   {
      cout << "\nContinuing to next operation.\n";
      return 0;
   }
   return 0;
}
int_least32_t CCensusFileManager::parseBoundData()
{
   string bndFileExtension(".BND");
   string userOption;
   string boundsFileName;
   std::stringstream stringToint;
   ifstream readBoundsFileStream;
   vector<string> boundsFiles=listFiles(censusFileDirectory,bndFileExtension);
   if(boundsFiles.size() <= 0)
   {
      cerr << "Cannot find RTC files in " << censusFileDirectory << std::endl;
      return -1;
   }
   unsigned int chosenOption =  boundsFiles.size() +1;

      /* Prompt user to look load Bounds Files.*/
      bool try_again=true;
      do
      {
         /*print menu*/
         cout<<"\nThere are "+ std::to_string(boundsFiles.size()) +" Bound files present\n";
         int i=1;
         for (string boundFile : boundsFiles)
         {
            cout << "\t"<<std::to_string(i) << ")" << boundFile << std::endl;
            i++;
         }
         cout << "\t"<< std::to_string(i)  << ")Exit";
         cout<<"\nPlease select an option to proceed:  ";
         getline(cin,userOption);
         stringToint.str(userOption);
         stringToint >> chosenOption;
         if(!stringToint.fail())
         {
            int option=chosenOption-1;
            if((option < boundsFiles.size()) && (option >= 0))
            {
               boundsFileName=censusFileDirectory+"/" + boundsFiles.at(option);
               try_again=false;
            }
            else if(chosenOption == (boundsFiles.size()+1))
            {
               cout << "Thank you for using the tool." << std::endl;
               return 0;
            }
            else
            {
               cerr << "Invalid input try again.\n" << std::endl;
            }
         }
         else
         {
            cerr << "Invalid input try again.\n" << std::endl;
         }
         chosenOption=0;
         userOption.clear();
         stringToint.clear();
      }
      while(try_again);

      readBoundsFileStream.open(boundsFileName);

      /* clear all data*/

      std::cout << "Adding Bounds Data to tables... \n";
      std::string boundFileLineString;
      string regionID,minLongitude,minLatitude,maxLongitude,maxLatitude;
      unsigned int regionIDInt;
      while(getline(readBoundsFileStream, boundFileLineString))
      {
         regionID = boundFileLineString.substr(0,7);
         minLongitude =boundFileLineString.substr(7,10);
         minLatitude = boundFileLineString.substr(17,9);
         maxLongitude = boundFileLineString.substr(26,10);
         maxLatitude = boundFileLineString.substr(36,9);

         stringToint.str(regionID);
         stringToint >> regionIDInt;
         for(auto&& stateData: mStateBoundsDataVector)
         {
            if(regionIDInt == stateData.getRegionID())
            {
               stateData.setMinLongitude(minLongitude);
               stateData.setMinLatitude(minLatitude);
               stateData.setMaxLatitude(maxLatitude);
               stateData.setMaxLongitude(maxLongitude);
            }
         }
         for(auto& countyData: mCountyBoundsDataVector)
         {
            if(regionIDInt == countyData.getRegionID())
            {
               countyData.setMinLongitude(minLongitude);
               countyData.setMinLatitude(minLatitude);
               countyData.setMaxLatitude(maxLatitude);
               countyData.setMaxLongitude(maxLongitude);
            }
         }
         for(auto& subCountyData: mSubCountyBoundsDataVector)
         {
            if(regionIDInt == subCountyData.getRegionID())
            {
               subCountyData.setMinLongitude(minLongitude);
               subCountyData.setMinLatitude(minLatitude);
               subCountyData.setMaxLatitude(maxLatitude);
               subCountyData.setMaxLongitude(maxLongitude);
            }
         }
         for(auto& placeData: mPlaceBoundDataVector)
         {
            if(regionIDInt == placeData.getRegionID())
            {
               placeData.setMinLongitude(minLongitude);
               placeData.setMinLatitude(minLatitude);
               placeData.setMaxLatitude(maxLatitude);
               placeData.setMaxLongitude(maxLongitude);
            }
         }

         /* Clear all parsed data for loading next line. */
         minLatitude.clear();
         minLongitude.clear();
         maxLatitude.clear();
         maxLongitude.clear();
         regionID.clear();
      }
      /* Close file stream. */
      readBoundsFileStream.close();
      /* Prompt for retry */
      userOption.clear();
      cout<<"\nDo you want to open more RTC files? ('y' - yes ,'r' - Try loading RTC,'e' - exit, default no) :";
      getline(cin,userOption);
      if(userOption.compare("y") == 0)
      {
         parseBoundData();
      }
      else if(userOption.compare("r") == 0)
      {
         parseRTCData();
      }
      else if(userOption.compare("e") == 0)
      {
         cout<<"\nThank you for using the tool. \n";
         exit(0);
      }
      else
      {
         return 0;
      }
   return 0;
}
