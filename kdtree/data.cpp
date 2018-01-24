
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: data.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 12/07/2017 04:36:12 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
  freopen("data.in", "w", stdout);
  srand(unsigned(time(NULL)));
  for(int ncase = 0; ncase < 1; ++ncase) {
    int n = abs(rand()) % 2000000 + 1;
    int m = abs(rand()) % 10000 + 1;
    printf("%d %d\n", n, m);
    for(int i = 0; i < n; ++i) {
      int x = abs(rand()) % 10000;
      int y = abs(rand()) % 10000;
      printf("%d %d\n", x, y);
    }
    for(int i = 0; i < m; ++i) {
      int x = abs(rand()) % 10000;
      int y = abs(rand()) % 10000;
      printf("%d %d\n", x, y);
    }
  }
  return 0;
}
