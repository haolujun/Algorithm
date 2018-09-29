
/***************************************************************************************
 *
 * Copyright (c) 2018 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: main.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 09/26/2018 09:35:41 AM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;

#define WIDE (256 * 256)

typedef struct Tuple {
  int first;
  int second;
  int x;
}Tuple;


typedef struct Word {
  int code;
  char c[3];
  int x;
}Word;

void radix_sort(Tuple *in_tuples, Tuple *out_tuples, int *s, int *t, int n, int &m) {
  int *c = new int[n];
  //第一趟排序
  for(int i = 0; i < m; ++i) c[i] = 0;
  for(int i = 0; i < n; ++i) c[in_tuples[i].second]++;
  for(int i = 1; i < m; ++i) c[i] += c[i - 1];
  for(int i = n - 1; i >= 0; --i) {
    s[--c[in_tuples[i].second]] = i;
  }
  

  //先备个份
  for(int i = 0; i < n; ++i) t[i] = s[i]; 
  
  //第二趟排序以第一趟排序结果继续排序
  for(int i = 0; i < m; ++i) c[i] = 0;
  for(int i = 0; i < n; ++i) c[in_tuples[t[i]].first]++;
  for(int i = 1; i < m; ++i) c[i] += c[i - 1];
  for(int i = n - 1; i >= 0; --i) {
    s[--c[in_tuples[t[i]].first]] = i;
  }
  

  //修正大小
  int p = 1;
  c[0] = 1;
  for(int i = 0; i < n; ++i) out_tuples[i] = in_tuples[t[s[i]]];
  
  for(int i = 1; i < n; ++i) {
    if(out_tuples[i].first == out_tuples[i - 1].first && out_tuples[i].second == out_tuples[i - 1].second) {
      c[i] = p;
    } else {
      c[i] = ++p;
    }
  }
  
  //这么做自己修改自己是不对的，修改t的过程又用到t
  //for(int i = 0; i < n; ++i) t[in_tuples[t[s[i]]].x] = c[i];
  //继续修正
  for(int i = 0; i < n; ++i) t[out_tuples[i].x] = c[i];
  //修正宽度
  m = p + 1;
  delete [] c;
}

void calc(std::string str, int *sa) {
  int n = str.size();
  Tuple *tuples = new Tuple[n];
  Tuple *res_tuples = new Tuple[n];
  int *s = new int[n]; //s[i]: 排第i的后缀的坐标
  int *t = new int[n]; //t[i]: 后缀suffix(i)排第几
  int *c = new int[WIDE];
  //单个字符基数排序
  for(int i = 0; i < WIDE; i++) c[i] = 0;
  for(int i = 0; i < n; ++i) c[str[i]]++;
  for(int i = 1; i < WIDE; ++i) {
    c[i] += c[i - 1];
  }
  
  for(int i = 0; i < n; ++i) {
    t[i] = --c[str[i]];
    s[c[str[i]]] = i;
  }

  for(int i = 0; i < n; ++i) {
    tuples[i].x = i;
  }

  //修正一下大小关系，相同大小修正成同一个数字
  int p = 1;
  t[s[0]] = 1;
  for(int i = 1; i < n; ++i) {
    if(str[s[i]] == str[s[i - 1]]) {
      t[s[i]] = p;
    } else {
      t[s[i]] = ++p;
    }
  }
  
  int m = p + 1;

  //循环log(n)次
  for(int step = 1; step < n; step = step << 1) {
    //设置每次迭代排序的两个数值
    for(int i = 0 ; i < n; ++i) {
      tuples[i].first = t[i];
    }
    
    for(int i = 0; i < n - step; ++i) { 
      tuples[i].second = t[i + step];
    }

    for(int i = n - step; i < n; ++i) {
      tuples[i].second = 0;
    }
    
    //接下来做两次基数排序 
    radix_sort(tuples, res_tuples, s, t, n, m);
  }
  
  for(int i = 0; i < n; ++i) sa[i] = res_tuples[i].x;
  
  delete [] t; 
  delete [] s;
  delete [] c;
  delete tuples;
  delete res_tuples;
  return;
}

void calc(std::vector<Word> &words, int *sa) {
  int n = words.size();
  Tuple *tuples = new Tuple[n];
  Tuple *res_tuples = new Tuple[n];
  int *s = new int[n]; //s[i]: 排第i的后缀的坐标
  int *t = new int[n]; //t[i]: 后缀suffix(i)排第几
  int *c = new int[WIDE];
  //单个字符基数排序
  for(int i = 0; i < WIDE; i++) c[i] = 0;
  for(int i = 0; i < n; ++i) c[words[i].code]++;
  for(int i = 1; i < WIDE; ++i) {
    c[i] += c[i - 1];
  }
  
  for(int i = 0; i < n; ++i) {
    t[i] = --c[words[i].code];
    s[c[words[i].code]] = i;
  }

  for(int i = 0; i < n; ++i) {
    tuples[i].x = i;
  }

  //修正一下大小关系，相同大小修正成同一个数字
  int p = 1;
  t[s[0]] = 1;
  for(int i = 1; i < n; ++i) {
    if(words[s[i]].code == words[s[i - 1]].code) {
      t[s[i]] = p;
    } else {
      t[s[i]] = ++p;
    }
  }
  
  int m = p + 1;

  //循环log(n)次
  for(int step = 1; step < n; step = step << 1) {
    //设置每次迭代排序的两个数值
    for(int i = 0 ; i < n; ++i) {
      tuples[i].first = t[i];
    }
    
    for(int i = 0; i < n - step; ++i) { 
      tuples[i].second = t[i + step];
    }

    for(int i = n - step; i < n; ++i) {
      tuples[i].second = 0;
    }
    
    //接下来做两次基数排序 
    radix_sort(tuples, res_tuples, s, t, n, m);
  }
  
  for(int i = 0; i < n; ++i) sa[i] = res_tuples[i].x;
  
  delete [] t; 
  delete [] s;
  delete [] c;
  delete tuples;
  delete res_tuples;
  return;
}

int main(int argc, char *argv[]) {
  string a = std::string(argv[1]);
  int *sa = new int[a.size()];
  calc(a, sa);
  for(int i = 0; i < a.size(); ++i) cout << sa[i] << " "; cout << endl;
  return 0;
}




