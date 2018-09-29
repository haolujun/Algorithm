
/***************************************************************************************
 *
 * Copyright (c) 2018 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: extract_word.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 09/28/2018 09:12:51 AM
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
#include <math.h>
using namespace std;

typedef struct Weight{
  Weight() {
    p = 0.0; p1 = 0.0; p2 = 0.0;
    left_count = 0;
    right_count = 0;
  }
  double p;  //出现的概率
  double p1; //内聚度
  double p2; //左临或又临熵
  std::map<std::string, int> left;
  std::map<std::string, int> right;
  int left_count;
  int right_count;
}Weight;

int main() {
  std::map<string, int> count;
  std::map<string, Weight> p;
  FILE *file = fopen("text.txt.gbk", "rb");
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  char *buf = new char[size];
  char *hanzi_buf = new char[size];
  fseek(file, 0, SEEK_SET);
  fgets(buf, size, file);
  fclose(file);
  printf("file size: %d\n", size); 
  int j = 0;
  for(int i = 0; i < size; ++i) {
    if (buf[i] < 0 && i + 1 < size) {
      hanzi_buf[j++] = buf[i];
      hanzi_buf[j++] = buf[i + 1];
      i = i + 1;
    }
  }

  hanzi_buf[j] = 0;

  for(int i = 0; i < j; i += 2) {
    if (i + 1 < j) {
      count[std::string(hanzi_buf + i, 2)]++;
    }

    if (i + 3 < j) {
      count[std::string(hanzi_buf + i, 4)]++;
    }
      
    if (i + 5 < j) {
      count[std::string(hanzi_buf + i, 6)]++;
    }
    
    if (i + 7 < j) {
      count[std::string(hanzi_buf + i, 8)]++;
    }
  }

  for(map<string, int>::iterator it = count.begin(); it != count.end(); ++it) {
    Weight w;
    w.p = (double)it->second / (double)j;
    p[it->first] = w;
  }
  
    
  for(map<string, Weight>::iterator it = p.begin(); it != p.end(); ++it) {
    if (it->first.size() == 2) {
      continue;
    }
  
    double co_p = 0.0;
     
    int w_size = it->first.size();
    for(int i = 2; i <= w_size - 2; i += 2) {
      std::string w1(it->first.c_str(), i);
      std::string w2(it->first.c_str() + i, w_size - i);
      if (p[w1].p * p[w2].p > co_p) { 
        co_p = p[w1].p * p[w2].p;
      }
    }
    
    it->second.p1 = it->second.p / co_p;
  }
  
  
  //计算左临字
  for(int i = 2; i < j; i += 2) {
    if (i + 4 < j) {
      std::string w(hanzi_buf + i, 4);
      std::string left(hanzi_buf + i - 2, 2);
      p[w].left[left]++;
      p[w].left_count++;
    }
    if (i + 6 < j) {
      std::string w(hanzi_buf + i, 6);
      std::string left(hanzi_buf + i - 2, 2);
      p[w].left[left]++;
      p[w].left_count++;
    }
    if (i + 8 < j) {
      std::string w(hanzi_buf + i, 8);
      std::string left(hanzi_buf + i - 2, 2);
      p[w].left[left]++;
      p[w].left_count++;
    }
  }
  
  //计算右临字
  for(int i = 0; i < j - 2; i += 2) {
    if (i + 4 < j - 2) {
      std::string w(hanzi_buf + i, 4);
      std::string right(hanzi_buf + i + 4, 2);
      p[w].right[right]++;
      p[w].right_count++;
    }
       
    if (i + 6 < j - 2) {
      std::string w(hanzi_buf + i, 6);
      std::string right(hanzi_buf + i + 6, 2);
      p[w].right[right]++;
      p[w].right_count++;
    }
       
    if (i + 8 < j - 2) {
      std::string w(hanzi_buf + i, 8);
      std::string right(hanzi_buf + i + 8, 2);
      p[w].right[right]++;
      p[w].right_count++;
    }
  }

  for(std::map<string, Weight>::iterator it = p.begin(); it != p.end(); ++it) {
    Weight *weight = &it->second;
    double lw = 0.0, rw = 0.0;
    for(std::map<string, int>::iterator it1 = weight->left.begin(); it1 != weight->left.end(); ++it1) {
      lw += -(((double)it1->second) / ((double)weight->left_count)) * log(((double)it1->second) / ((double)weight->left_count));
    }
    
    for(std::map<string, int>::iterator it1 = weight->right.begin(); it1 != weight->right.end(); ++it1) {
      rw += -(((double)it1->second) / ((double)weight->right_count))* log(((double)it1->second) / ((double)weight->right_count));
    }
    
    it->second.p2 = lw < rw ? lw : rw;
  }
  
  //剑来的参数
  //double param1 = 200.0;
  //double param2 = 1.5;
  //悲伤逆流成河的参数
  double param1 = 150.0;
  double param2 = 1.0;
  FILE *wfile = fopen("words.txt", "wb");

  for(std::map<string, Weight>::iterator it = p.begin(); it != p.end(); ++it) {
    Weight w = it->second;
    if (w.p1 >= param1 && w.p2 >= param2) {
      fprintf(wfile, "%s %.10lf %lf %lf\r\n", it->first.c_str(), w.p, w.p1, w.p2);
      for(std::map<string, int>::iterator it1 = it->second.left.begin(); it1 != it->second.left.end(); ++it1) {
        fprintf(wfile, "%s:%d ", it1->first.c_str(), it1->second);
      }
      fprintf(wfile, "\r\n");
      
      for(std::map<string, int>::iterator it1 = it->second.right.begin(); it1 != it->second.right.end(); ++it1) {
        fprintf(wfile, "%s:%d ", it1->first.c_str(), it1->second);
      }
      fprintf(wfile, "\r\n");
    }
  }
  
  fclose(wfile);
  delete [] buf;
  delete [] hanzi_buf;
  
  return 0;
}

