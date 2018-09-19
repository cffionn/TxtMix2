//cpp dependencies
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//ROOT dependencies
#include "TRandom3.h"

//Local dependencies
#include "include/checkMakeDir.h"
#include "include/lineUtils.h"

int simpleConstructedPoem(const std::string inFileName)
{
  if(!checkFile(inFileName) || inFileName.find(".txt") == std::string::npos){
    std::cout << "Given inFileName \'" << inFileName << "\' is invalid. return 1" << std::endl;
    return 1;
  }

  std::ifstream file(inFileName);
  std::string tempStr;

  std::vector<std::string> lines;
  std::vector<std::string> firstWords;
  std::vector<std::string> lastWords;
  
  while(std::getline(file,tempStr)){
    if(tempStr.size() == 0) continue;
    lines.push_back(tempStr);
    std::vector<std::string> words = returnWordsFromLine(tempStr);
    firstWords.push_back(words.at(0));
    lastWords.push_back(words.at(words.size()-1));
  }
  
  file.close();

  const Int_t nRoundsMax = 10000;
  Int_t nRound = 0;
  const unsigned int nLines = 5;
  TRandom3* randGen_p = new TRandom3(0);
  std::vector<std::string> poem;
  std::vector<std::string> poemFirst;
  std::vector<std::string> poemLast;

  while(poem.size() < nLines && nRound < nRoundsMax){
    ++nRound;
    unsigned int pos = randGen_p->Uniform(0, lines.size());
    if(pos > lines.size()) continue;

    std::vector<std::string> words = returnWordsFromLine(lines.at(pos));

    bool goodLine = true;
    for(unsigned int pI = 0; pI < poem.size(); ++pI){
      if(words.at(0) == poemFirst.at(pI)){
	goodLine = false;
	break;
      }
      if(words.at(words.size()-1) == poemLast.at(pI)){
	goodLine = false;
	break;
      }
    }

    if(!goodLine) continue;

    poem.push_back(lines.at(pos));
    poemFirst.push_back(words.at(0));
    poemLast.push_back(words.at(words.size()-1));
  }
  
  std::cout << std::endl;
  for(unsigned int pI = 0; pI < poem.size(); ++ pI){
    std::cout << poem.at(pI) << std::endl;
  }
  std::cout << std::endl;
  
  delete randGen_p;
  
  return 0;
}

int main(int argc, char* argv[])
{
  if(argc != 2){
    std::cout << "Usage: ./bin/simpleConstructedPoem.exe <inFileName>" << std::endl;
    return 1;
  }

  int retVal = 0;
  retVal += simpleConstructedPoem(argv[1]);
  return retVal;
}
