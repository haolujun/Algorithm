
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: data.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 11/28/2017 11:13:53 AM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
int main() {
  freopen("data.in", "w", stdout);
  srand(unsigned(time(NULL)));
  int n, m;
  for(int i = 0; i < 10000; ++i) {
    n = abs(rand()) % 500 + 1;
    m = abs(rand()) % 30 + 1;
    printf("%d %d", n, m);
    for(int j = 0; j < n; ++j) {
      printf(" %d", abs(rand()) % 100);
    }
    for(int j = 0; j < m; ++j) {
      printf(" %d", abs(rand()) % 100);
    }
    printf("\n");
  }
}

