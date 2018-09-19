#ifndef LINEUTILS_H
#define LINEUTILS_H

//cpp dependencies
#include <string>
#include <vector>

//ROOT dependencies
#include "TMath.h"

//local dependencies
#include "include/stringUtil.h"

const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string lower = "abcdefghijklmnopqrstuvwxyz";

std::string lowerCaseAll(std::string line)
{
  for(unsigned int lI = 0; lI < line.size(); ++lI){
    if(upper.find(line.substr(lI,1)) != std::string::npos){
      line.replace(lI, 1, lower.substr(upper.find(line.substr(lI,1)), 1));
    }
  }

  return line;
}


std::string removeAllNonAlphaFromLine(std::string line)
{
  unsigned int pos = 0;

  while(pos < line.size()){
    if(upper.find(line.substr(pos,1)) != std::string::npos) ++pos;
    else if(lower.find(line.substr(pos,1)) != std::string::npos) ++pos;
    else line.replace(pos, 1, "");
  }
  
  return line;
}

std::string removeSelectCharFromLine(std::string line)
{
  const std::string charsToRemove = ".,!?-";
  for(unsigned int cI = 0; cI < charsToRemove.size(); ++cI){
    std::string cha = charsToRemove.substr(cI,1);
    while(line.find(cha) != std::string::npos){line.replace(line.find(cha), 1, " ");}
  }

  return line;
}

std::vector<std::string> returnWordsFromLine(std::string line)
{
  std::vector<std::string> words;
  line = line + " ";

  line = removeSelectCharFromLine(line);

  while(line.find("  ") != std::string::npos){line.replace(line.find("  "), 2, " ");}
  while(line.substr(0,1).find(" ") != std::string::npos){line.replace(0,1,"");}
  while(line.size() != 0){
    std::string word = line.substr(0, line.find(" "));
    words.push_back(word);
    line.replace(0, line.find(" ") + 1, "");
  }

  return words;
}

int posWordSortLower(std::string word1, std::string word2)
{
  word1 = removeAllNonAlphaFromLine(word1);
  word2 = removeAllNonAlphaFromLine(word2);

  word1 = lowerCaseAll(word1); 
  word2 = lowerCaseAll(word2);

  if(isStrSame(word1, word2)) return -1;
  
  unsigned int size = TMath::Min(word1.size(), word2.size());
  bool defaultIsFirst = true;
  if(word2.size() == size) defaultIsFirst = false;
  
  int lowerWord = -1;//val -1, 0, 1
  for(unsigned int i = 0; i < size; ++i){
    std::string let1 = word1.substr(i,1);
    std::string let2 = word2.substr(i,1);

    int pos1 = lower.find(let1);
    int pos2 = lower.find(let2);

    if(pos1 < pos2){
      lowerWord = 0;
      break;
    }
    else if(pos1 > pos2){
      lowerWord = 1;
      break;
    }
  }
  
  
  if(lowerWord < 0){
    if(defaultIsFirst) return 0;
    else return 1;
  }
  return lowerWord;
}

#endif
