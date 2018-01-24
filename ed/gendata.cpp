
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: gendata.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 10/12/2017 04:03:42 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main() {
  FILE *file = fopen("data.txt", "w");
  int n = 10000;
  srand(unsigned(time(NULL)));
  
  for(int i = 0; i < n; ++i) {
    int len_p = abs(rand()) % 50 + 50;
    int len_t = abs(rand()) % 100 + 100;
    for(int j = 0; j <= len_p; ++j) {
      fprintf(file, "%c", abs(rand()) % 26 + 'a');
    }
    fprintf(file, " ");
    
    for(int j = 0; j <= len_t; ++j) {
      fprintf(file, "%c", abs(rand()) % 26 + 'a');
    }

    int k = (2 * len_p / 3);
    fprintf(file, " %d", k);
    fprintf(file, "\n");
  }
  fclose(file);
}
