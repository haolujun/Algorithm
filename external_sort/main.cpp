
/***************************************************************************************
 *
 * Copyright (c) 2018 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: main.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 01/09/2018 02:49:25 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "external_sort.h"

using namespace std;


class DocWordInfo {
  public:
    std::string word;
    std::string type;
    int tf;
  
    std::string to_string() {
      char buf[256] = {0};
      snprintf(buf, 256, "%s\t%s\t%d\n", word.c_str(), type.c_str(), tf);
      return std::string(buf);
    }

    void parse(char *line) {
      int word_end = -1;
      int type_end = -1;
      int tf_end = -1;
      int pos = 0;
      
      while(line[pos] != '\0') {
        if(line[pos] == '\t') {
          break;
        }
        pos++;
      }
    
      word_end = pos;
      line[pos++] = 0;
      word = std::string(line);

      while(line[pos] != '\0') {
        if(line[pos] == '\t') {
          break;
        }
        pos++;
      }
      
      type_end = pos;
      line[pos++] = 0;
      type = std::string(line + word_end + 1);

      while(line[pos] != '\0' && line[pos] != '\n') {
        pos++;
      }

      line[pos] = 0;
      tf = atoi(line + type_end + 1);
    }
    
    friend bool operator<(DocWordInfo a, DocWordInfo b);
    
    friend bool operator>(DocWordInfo a, DocWordInfo b);
};

//stl::algorithm用的是友元重载运算符
bool operator<(DocWordInfo a, DocWordInfo b) {
  if(a.word != b.word) {
    return a.word < b.word;
  }
  return a.tf < b.tf;
}

bool operator>(DocWordInfo a, DocWordInfo b) {
  if(a.word != b.word) {
    return a.word > b.word;
  }
  return a.tf > b.tf;
}

int main() {
  afanti::index::ExternalSort<DocWordInfo> esort("./result", "SORTFILE", 3);
  srand(unsigned(time(NULL)));
  char str[20];
  for(int i = 0; i < 10; ++i) {
    DocWordInfo dwi;
    int len = abs(rand()) % 10 + 1;
    dwi.word = "";
    for(int j = 0; j < len; ++j)
      str[j] = (abs(rand()) % 26 + 'a');
    str[len] = 0;
    dwi.word = std::string(str);
    dwi.type = "x";
    dwi.tf = abs(rand()) % 100;
    esort.push(dwi);
  }
  esort.flush();
  esort.sort(); 
  return 0;
}
