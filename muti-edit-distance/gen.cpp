/**
 *    @file: gen.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 09/03/2018 05:22:40 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <string>
using namespace std;
int main() {
  freopen("words.txt", "w", stdout);
  map<string, int> words;
  srand(unsigned(time(NULL)));
  int n = abs(rand()) % 10000 + 100000;
  int m = abs(rand()) % 100 + 100;
  printf("%d %d\n", n, m);
  for(int i = 0; i < n; ++i) {
    int m = abs(rand()) % 5 + 5;
    int stop = 0;
    while(!stop) {
      std::string p = "";
      for(int j = 0; j < m; ++j) {
        char ch = 'a' + abs(rand()) % 5;
        p = p + ch;
      }
      if (words.find(p) == words.end()) {
        words[p] = 1;
      }
      printf("%s\n", p.c_str());
      stop = 1;
    }
  }
  
  for(int i = 0; i < m; ++i) {
    int m = abs(rand()) % 5 + 5;
    for(int j = 0; j < m; ++j) {
      printf("%c", 'a' + abs(rand()) % 5);
    }
    printf("\n");
  }
  return 0;
}

