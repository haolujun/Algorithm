
/***************************************************************************************
 *
 * Copyright (c) 2018 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: main.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 03/07/2018 02:30:33 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include <map>
#include <string>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "HLL.h"

unsigned int BKDRHash(const char *str)  
{  
    register unsigned int hash = 0;  
    while (unsigned int ch = (unsigned int)*str++)  
    {         
        hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313..  
        // 有人说将乘法分解为位运算及加减法可以提高效率，如将上式表达为：hash = hash << 7 + hash << 1 + hash + ch;  
        // 但其实在Intel平台上，CPU内部对二者的处理效率都是差不多的，  
        // 我分别进行了100亿次的上述两种运算，发现二者时间差距基本为0（如果是Debug版，分解成位运算后的耗时还要高1/3）；  
        // 在ARM这类RISC系统上没有测试过，由于ARM内部使用Booth's Algorithm来模拟32位整数乘法运算，它的效率与乘数有关：  
        // 当乘数8-31位都为1或0时，需要1个时钟周期  
        // 当乘数16-31位都为1或0时，需要2个时钟周期  
        // 当乘数24-31位都为1或0时，需要3个时钟周期  
        // 否则，需要4个时钟周期  
        // 因此，虽然我没有实际测试，但是我依然认为二者效率上差别不大          
    }  
    return hash;  
} 

int main() {
  int count = 0;
  int n = 1000000;
  HLL hll(6);
  std::map<std::string, int> _strs;
  srand(unsigned(time(NULL)));
  
  for(int i = 0; i < n; ++i) {
    int len = abs(rand()) % 6 + 1;
    std::string a = "";
    for(int j = 0; j < len; ++j) {
      char ch = abs(rand()) % 26 + 'a';
      a = a + ch;
    }

    if(_strs.find(a) == _strs.end()) {
      _strs[a] = 1;
      count++;
    }

    unsigned int hash = BKDRHash(a.c_str());
    hll.put(hash);
  }

  printf("Real distinct = %d HLL.distinct = %u\n", count, hll.distinct_count());
  
  return 0;
}

