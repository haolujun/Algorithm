
/***************************************************************************************
 *
 * Copyright (c) 2018 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: main.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 01/30/2018 10:34:10 AM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include "union_find_set.h"
#include <stdio.h>

int main() {
  UnionFindSet ufset(10);
  ufset.join(1, 2);
  ufset.join(4, 3);
  printf("%d %d\n", ufset.find(3), ufset.find(4));
  printf("%d %d\n", ufset.find(1), ufset.find(2));
  ufset.join(1, 3);
  printf("%d %d\n", ufset.find(1), ufset.find(4));
  return 0;
}

