//cpp dependencies
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

//ROOT dependencies
#include "TMath.h"

//Local dependencies
#include "include/checkMakeDir.h"
#include "include/lineUtils.h"

int runTxtMix(std::vector<std::string> fileNames)
{
  std::cout << "Running TxtMix..." << std::endl;

  unsigned int pos = 0;
  while(fileNames.size() > pos){
    bool keep = true;
    if(!checkFile(fileNames.at(pos))) keep = false;
    else if(fileNames.at(pos).find(".txt") == std::string::npos) keep = false;

    if(keep) ++pos;
    else fileNames.erase(fileNames.begin()+pos);
  }

  std::cout << "Processing " << fileNames.size() << " files..." << std::endl;
  for(unsigned int fI = 0; fI < fileNames.size(); ++fI){
    std::cout << " " << fI << "/" << fileNames.size() << ": " << fileNames.at(fI) << std::endl;
  }

  std::vector< std::map<std::string, std::vector<std::string> > > mapWordsToLines;
  std::vector<std::vector<std::string> > lines;
  lines.reserve(fileNames.size());

  for(unsigned int fI = 0; fI < fileNames.size(); ++fI){
    lines.push_back({});
    std::map<std::string, std::vector<std::string> > tempMapWordsToLines;
    
    std::ifstream file(fileNames.at(fI).c_str());
    std::string tempStr;

    while(std::getline(file, tempStr)){
      tempStr = removeSelectCharFromLine(tempStr);
      tempStr = lowerCaseAll(tempStr);
	
      lines.at(fI).push_back(tempStr);
      std::vector<std::string> words = returnWordsFromLine(tempStr);

      for(unsigned int wI = 0; wI < words.size(); ++wI){
	std::vector<std::string> tempVect = tempMapWordsToLines[words.at(wI)];
	tempVect.push_back(tempStr);
	tempMapWordsToLines[words.at(wI)] = tempVect;
      }
    }
    
    file.close();

    mapWordsToLines.push_back(tempMapWordsToLines);
  }

  for(unsigned int fI = 0; fI < fileNames.size(); ++fI){
    std::cout << "Lines in file \'" << fileNames.at(fI) << "\': " << std::endl;

    for(unsigned int lI = 0; lI < lines.at(fI).size(); ++lI){
      std::cout << " " << lI << "/" << lines.at(fI).size() << ": \'" << lines.at(fI).at(lI) << "\'" << std::endl;
    }
  }

  checkMakeDir("output");
  
  std::string outFileName = "output/outFile.txt";
  
  std::ofstream outFile(outFileName.c_str());
  std::vector<std::string> outLines;
  
  for(unsigned int fI = 0; fI < mapWordsToLines.size(); ++fI){
    std::map<std::string, std::vector<std::string> > map1 = mapWordsToLines.at(fI);
    
    for(unsigned int fI2 = 0; fI2 < mapWordsToLines.size(); ++fI2){
      if(fI == fI2) continue;

      std::map<std::string, std::vector<std::string> > map2 = mapWordsToLines.at(fI2);

      for(auto const &ent1 : map1){
	std::vector<std::string> lines2 = map2[ent1.first];
	if(lines2.size() == 0) continue;

	std::vector<std::string> lines1 = ent1.second;

	std::string word = " " +  ent1.first + " ";
	
	for(unsigned int lI1 = 0; lI1 < lines1.size(); ++lI1){
	  std::string line1 = lines1.at(lI1);
	  for(unsigned int lI2 = 0; lI2 < lines2.size(); ++lI2){
	    std::string line2 = lines2.at(lI2);

 	    line2.replace(0, line2.find(word), "");
	    line2 = line1.substr(0, line1.find(word)) + line2;

	    if(line2 == line1) continue;
	    if(line2 == lines2.at(lI2)) continue;

	    while(line2.find("  ") != std::string::npos){line2.replace(line2.find("  "), 2, " ");}
	    
	    bool isUnique = true;
	    for(unsigned int oI = 0; oI < outLines.size(); ++oI){
	      if(line2 == outLines.at(oI)){
		isUnique = false;
		break;
	      }
	    }
	 	   
	    if(!isUnique) continue;
	    std::vector<std::string> tempWords = returnWordsFromLine(line2);
	    outLines.push_back(line2);
	  }
	}
      }
    }
  }

  //Do a alphabetized sort of first words
  for(unsigned int i = 0; i < outLines.size(); ++i){

    for(unsigned int j = i+1; j < outLines.size(); ++j){

      std::vector<std::string> words1 = returnWordsFromLine(outLines.at(i));
      std::vector<std::string> words2 = returnWordsFromLine(outLines.at(j));

      //EDItING HERE
      unsigned int size = TMath::Min(words1.size(), words2.size());
      bool isFirstDefault = true;
      if(size != words1.size()) isFirstDefault = false;


      bool didSwap = false;
      for(unsigned int wI = 0; wI < size; ++wI){
	int sortVal = posWordSortLower(words2.at(wI), words1.at(wI));
	if(sortVal < 0) continue;

	didSwap = true;
	if(sortVal == 0){
	  std::string tempLine = outLines.at(j);	  
	  outLines.at(j) = outLines.at(i);	  
	  outLines.at(i) = tempLine;

	  break;
	}
	else if(sortVal == 1) break;
      }

      if(!didSwap && !isFirstDefault){
	std::string tempLine = outLines.at(j);	  
	outLines.at(j) = outLines.at(i);	  
	outLines.at(i) = tempLine;
      }
    }
  }


  for(unsigned int oI = 0; oI < outLines.size(); ++oI){
    outFile << outLines.at(oI) << std::endl;;
  }

  
  outFile.close();
  
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc != 2){
    std::cout << "Usage: ./bin/runTxtMix.exe <commaListOfFiles>" << std::endl;
    return 1;
  }

  std::string fileNameCommas = std::string(argv[1]) + ",";
  std::vector<std::string> fileNames;
  while(fileNameCommas.find(",,") != std::string::npos){fileNameCommas.replace(fileNameCommas.find(",,"), 2, ",");}
  while(fileNameCommas.find(",") != std::string::npos){
    fileNames.push_back(fileNameCommas.substr(0, fileNameCommas.find(",")));
    fileNameCommas.replace(0, fileNameCommas.find(",")+1, "");
  }
  if(fileNames.size() < 2){
    std::cout << "Given set of input files \'" << argv[1] << "\' has less than two valid files. Please give valid files in comma separated list" << std::endl;
    return 1;
  }

  int retVal = 0;
  retVal += runTxtMix(fileNames);
  return retVal;
}
