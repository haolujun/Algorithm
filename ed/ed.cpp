
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: ed.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 10/12/2017 10:01:57 AM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;
struct timeval gb, ge;
int gcost = 0;
inline int loc(map<char, int*> find, int *len, char ch, int pos) {
  map<char, int*>::iterator it = find.find(ch);
  if(it == find.end()) {
    return -1;
  }
  int *loc = it->second;
  for(int i = 0; i < len[ch - 'a']; ++i) {
    if(loc[i] >= pos)
      return loc[i];
  }
  return -1;
}

inline int loc(int find[][200], int *len, int ch, int pos) {
  for(int i = 0; i < len[ch]; ++i) {
    if(find[ch][i] >= pos)  return find[ch][i];
  }
  return -1;
}

int dp(char *p, char *t, int k) {
  int c[200];
  int len_p = strlen(p);
  int len_t = strlen(t);
  int lact = k + 1;
  int min_v = len_p;
  for(int i = 0; i <= len_p; ++i) {
    c[i] = i;
  }
  
  for(int i = 1; i <= len_t; ++i) {
    int pc = 0, nc = 0;
    for(int j = 1; j <= lact; ++j) {
      if(p[j - 1] == t[i - 1]) {
        nc = pc;
      } else {
        if(pc < nc) nc = pc;
        if(c[j] < nc) nc = c[j];
        nc = nc + 1;
      }
      pc = c[j];
      c[j] = nc;
    }

    while(lact >= 0 && c[lact] > k) lact--;
    if(lact == len_p) {
      if(c[lact] < min_v) min_v = c[lact];
    } else {
      lact++;
    }
  }
  if(min_v <= k) return min_v;
  return -1;
}

int new_dp(char *p, char *t, int k) {

  int len_p = strlen(p);
  int len_t = strlen(t);
  int find[26][200];
  int len[26] = {0};
  memset(len, 0x00, sizeof(len)); 
  //生成loc表，用来快速查询
  for(int i = 0; i < len_p; ++i) {
    int ch = p[i] - 'a';
    find[ch][len[ch]++] = i + 1;
  }

  //===============================================
  int part[200];
  int pre_cn = 0, next_cn = 1, min_v = len_p, stop_cn = 1;
  part[0] = len_p;
  for(int i = 0; i < len_t; ++i) {
    //前一列partition数
    pre_cn = stop_cn;
    next_cn = 0;
    int l = part[0] + 1;
    int b = 1;
    int e = l;
    int tmp;
    int tmp_value = 0;
    int ch = t[i] - 'a';
    int pre_v = part[0];

    //前一列第0个partition长度肯定>=1
    if(len[ch] > 0 && (tmp = loc(find, len, ch, b)) != -1 && tmp <= e) {
      part[next_cn++] = tmp - 1;
    } else if(pre_cn >= 2 && part[1] - part[0] != 0){
      part[next_cn++] = part[0] + 1;
    } else {
      part[next_cn++] = part[0];
    }
    //每列第一个partition尾值
    tmp_value = part[0];
    //遍历前一列剩下的partition
    for(int j = 1; j < pre_cn && part[next_cn - 1] < len_p; ++j) {
      int x = part[j], y = pre_v;
      pre_v = part[j];
      l = x - y;
      if(l == 0) {
        part[next_cn++] = x + 1;
      } else {
        b = x - l + 2;
        e = x + 1;
        if(b <= len_p && len[ch] > 0 && (tmp = loc(find, len, ch, b)) != -1 && tmp <= e) {
          part[next_cn++] = tmp - 1;
        } else if(j + 1 < pre_cn && part[j + 1] - x != 0) {
          part[next_cn++] = x + 1;
        } else {
          part[next_cn++] = x;
        }
      }
      l = part[j] - part[j - 1];
      if(l == 0) {
        //新得到的partition长度为0，那么下一个partition的起始值比上一个partition尾值少1
        tmp_value -= 1;
      } else {
        tmp_value += l - 1;
      }

      l = part[next_cn - 1] - part[next_cn - 2];
      if(tmp_value - l + 1 <= k) {
        stop_cn = next_cn;
      }
    }
    
    if(part[next_cn - 1] != len_p) {
      part[next_cn++] = len_p;  
      tmp_value += len_p - part[next_cn - 2] - 1;
      if(tmp_value < min_v) {
        min_v = tmp_value;
      }
      
      if(next_cn >= 2 && tmp_value - (part[next_cn - 1] - part[next_cn - 2]) + 1 <= k) {
        stop_cn = next_cn;
      }
    } else {
      min_v = min_v < tmp_value ? min_v : tmp_value;
    }
  }
  if(min_v <= k) return min_v;
  return -1;
}

int new_column_partition(char *p, char *t) {
  int len_p = strlen(p);
  int len_t = strlen(t);
  int find[26][200];
  int len[26] = {0};
  int part[200];  
  //生成loc表，用来快速查询
  for(int i = 0; i < len_p; ++i) {
    find[p[i] - 'a'][len[p[i] - 'a']++] = i + 1;
  }
  
  int pre_cn = 0, next_cn = 1, min_v = len_p;
  part[0] = len_p;
  
  for(int i = 0; i < len_t; ++i) {
    //前一列partition数
    pre_cn = next_cn;
    next_cn = 0;
    int l = part[0] + 1;
    int b = 1;
    int e = l;
    int tmp;
    int tmp_value = 0;
    int pre_v = part[0];
    //前一列第0个partition长度肯定>=1
    if(len[t[i] - 'a'] >0 && (tmp = loc(find, len, t[i] - 'a', b)) != -1 && tmp <= e) {
      part[next_cn++] = tmp - 1;
    } else if(pre_cn >= 2 && part[1] - part[0] != 0){
      part[next_cn++] = part[0] + 1;
    } else {
      part[next_cn++] = part[0];
    }
    //每列第一个partition尾值
    tmp_value = part[0];

    //遍历前一列剩下的partition
    for(int j = 1; j < pre_cn && part[next_cn - 1] < len_p; ++j) {
      int x = part[j], y = pre_v;
      pre_v = part[j];
      l = x - y;
      if(l == 0) {
        part[next_cn++] = x + 1;
      } else {
        b = x - l + 2;
        e = x + 1;
        if(b <= len_p && len[t[i] - 'a'] > 0 && (tmp = loc(find, len, t[i] - 'a', b)) != -1 && tmp <= e) {
          part[next_cn++] = tmp - 1;
        } else if(j + 1 < pre_cn && part[j + 1] - x != 0) {
          part[next_cn++] = x + 1;
        } else {
          part[next_cn++] = x;
        }
      }
      l = part[j] - part[j - 1];
      if(l == 0) {
        //新得到的partition长度为0，那么下一个partition的起始值比上一个partition尾值少1
        tmp_value -= 1;
      } else {
        tmp_value += l - 1;
      }
    }
    
    if(part[next_cn - 1] != len_p) {
      part[next_cn++] = len_p;  
      tmp_value += len_p - part[next_cn - 2] - 1;
      if(tmp_value < min_v) {
        min_v = tmp_value;
      }
    } else {
      min_v = min_v < tmp_value ? min_v : tmp_value;
    }
  }
  return min_v;
}



int column_partition(char *p, char *t) {
  map<char, vector<int> > pos;
  map<char, vector<int> > loc;

  //vector<vector<int> > pos;
  //vector<vector<int> > loc;
  
  //pos.reserve(26);
  //loc.reserve(26);
  
  //生成loc表，用来快速查询
  int len = strlen(p);
  for(int i = 0; i < len; ++i) {
    pos[p[i]].push_back(i + 1);
    //pos[p[i] - 'a'].push_back(i + 1);
  }
  
  
  map<char, vector<int> >::iterator it;
  
  for(it = pos.begin(); it != pos.end(); ++it) {
    loc[it->first].reserve(len + 1);
    int size = it->second.size();
    int pre = -1, next = -1;
    for(int i = 0; i < size; ++i) {
      pre = next + 1;
      next = it->second[i];
      for(int j = pre; j <= next; ++j) {
        loc[it->first][j] = next;  
      }
    }
    
    if(next < len) {
      for(int i = next + 1; i <= len; ++i) {
        loc[it->first][i] = -1;
      }
    }
  } 
  /* 
  cout << "  ";  
  for(int i = 0; i <= len; ++i) {
    cout << setw(2) << i << " ";
  }
  cout << endl;

  for(it = loc.begin(); it != loc.end(); ++it) {
    cout << it->first << ":";
    for(int i = 0; i <= len; ++i) {
      cout << setw(2) << it->second[i] << " ";
    }
    cout << endl;
  }*/
  
  //===============================================
  gettimeofday(&gb, NULL);
  int len_p = strlen(p);
  int len_t = strlen(t);
  int *part[2];
  part[0] = (int *)malloc(sizeof(int) * len_p + 1);
  part[1] = (int *)malloc(sizeof(int) * len_p + 1);

  int pre_cn = 0, next_cn = 1, min_v = len_p;
  part[0][0] = len_p;
  
  for(int i = 0; i < len_t; ++i) {
    //前一列partition数
    pre_cn = next_cn;
    next_cn = 0;
    int pre = i % 2, next = (i + 1) % 2;
    int l = part[pre][0] + 1;
    int b = 1;
    int e = l;
    int tmp;
    int tmp_value = 0;
    map<char, vector<int> >::iterator it;
    //前一列第0个partition长度肯定>=1
    if((it = loc.find(t[i])) != loc.end() && (tmp = it->second[b]) != -1 && tmp <= e) {
      part[next][next_cn++] = tmp - 1;
    } else if(pre_cn >= 2 && part[pre][1] - part[pre][0] != 0){
      part[next][next_cn++] = part[pre][0] + 1;
    } else {
      part[next][next_cn++] = part[pre][0];
    }
    //每列第一个partition尾值
    tmp_value = part[next][0];

    //遍历前一列剩下的partition
    for(int j = 1; j < pre_cn && part[next][next_cn - 1] < len_p; ++j) {
      int x = part[pre][j], y = part[pre][j - 1];
      l = x - y;
      if(l == 0) {
        part[next][next_cn++] = x + 1;
      } else {
        b = x - l + 2;
        e = x + 1;
        if((it = loc.find(t[i])) != loc.end() && b <= len_p && (tmp = it->second[b]) != -1 && tmp <= e) {
          part[next][next_cn++] = tmp - 1;
        } else if(j + 1 < pre_cn && part[pre][j + 1] - x != 0) {
          part[next][next_cn++] = x + 1;
        } else {
          part[next][next_cn++] = x;
        }
      }
      l = part[next][j] - part[next][j - 1];
      if(l == 0) {
        //新得到的partition长度为0，那么下一个partition的起始值比上一个partition尾值少1
        tmp_value -= 1;
      } else {
        tmp_value += l - 1;
      }
    }
    
    if(part[next][next_cn - 1] != len_p) {
      part[next][next_cn++] = len_p;  
      tmp_value += len_p - part[next][next_cn - 2] - 1;
      if(tmp_value < min_v) {
        min_v = tmp_value;
      }
    } else {
      min_v = min_v < tmp_value ? min_v : tmp_value;
    }
  }
   
  /* 
  for(int i = 0; i <= len_t; ++i) {
    int size = partition[i].size();
    cout << "Column " << i << endl;
    for(int j = 0; j < size; ++j) {
      cout << "Part[" << j << "] = " << partition[i][j] << " value:" << value[i][j] << endl;
    }
  }
  */
  /*   
  for(int i = 0; i <= len_t; ++i) {
    if(value[i][value[i].size() - 1] < min_v) {
      min_v = value[i][value[i].size() - 1];
    }
  }*/
  free(part[0]);
  free(part[1]);
  gettimeofday(&ge, NULL);
  int c = ge.tv_sec * 1000 + ge.tv_usec / 1000 - gb.tv_sec * 1000 - gb.tv_usec / 1000;
  gcost += c;
  return min_v;
}


int ed_distance(char *p, char *t) {
  int len_p = strlen(p);
  int len_t = strlen(t);
  int d[500][500];

  for(int i = 0; i <= len_p; ++i) {
    d[i][0] = i;
  }

  for(int j = 0; j <= len_t; ++j) {
    d[0][j] = 0;
  }

  for(int i = 1; i <= len_p; ++i) {
    for(int j = 1; j <= len_t; ++j) {
      if(p[i - 1] == t[j - 1]) {
        d[i][j] = d[i - 1][j - 1];
      } else {
        d[i][j] = min(min(d[i - 1][j], d[i][j - 1]), d[i - 1][j - 1]) + 1;
      }
    }
  }
  
  /*   
  for(int i = 0; i <= len_p; ++i) {
    for(int j = 0; j <= len_t; ++j) {
      cout << d[i][j] << " ";
    }
    cout << endl;
  }
  */
  int min_v = len_p;
  for(int j = 0; j <= len_t; ++j) {
    if(d[len_p][j] < min_v) {
      min_v = d[len_p][j];
    } 
  }
  return min_v;
}

int main(int argc, char *argv[]) {
  
  FILE *file = fopen("data.txt", "r");
  
  FILE *fout = fopen(argv[1], "w");

  char line[1024] = {0};
  struct timeval b, e;
  int cost = 0;
  while(fgets(line, 1023, file) != NULL) {
    int len = strlen(line);
    line[len - 1] = 0;
    len = len - 1;
    int split[2] = {0, 0};
    int cnt = 0;
    for(int i = 0; i < len; ++i) {
      if(line[i] == ' ') {
        split[cnt++] = i;
        line[i] = '\0';
      }
    }
    //printf("%s %s\n", line, line + split + 1);
    if(atoi(argv[1]) == 1) {
      gettimeofday(&b, NULL);
      int v = dp(line, line + split[0] + 1, atoi(line + split[1] + 1));
      gettimeofday(&e, NULL);
      cost += e.tv_sec * 1000 + e.tv_usec / 1000 - b.tv_sec * 1000 - b.tv_usec / 1000;
      fprintf(fout, "%d\n", v);
    } else {
      gettimeofday(&b, NULL);
      int v = new_dp(line, line + split[0] + 1, atoi(line + split[1] + 1));
      gettimeofday(&e, NULL);
      cost += e.tv_sec * 1000 + e.tv_usec / 1000 - b.tv_sec * 1000 - b.tv_usec / 1000;
      fprintf(fout, "%d\n", v); 
    }
  }
  if(atoi(argv[1]) == 1) {
    cout << "Cost " << cost << "ms" << endl;
  } else {
    cout << "Cost " << cost << "ms" << endl;
  }
  fclose(file);
  fclose(fout);

  //column_partition(p, t);
  //cout << ed_distance(p, t) << endl;
  return 0;
}



/*   
int main(int argc, char *argv[]) {
  
  FILE *file = fopen("data.txt", "r");
  
  FILE *fout = fopen(argv[1], "w");

  char line[1024] = {0};
  struct timeval b, e;
  int cost = 0;
  while(fgets(line, 1023, file) != NULL) {
    int len = strlen(line);
    line[len - 1] = 0;
    len = len - 1;
    int split = 0;
    for(int i = 0; i < len; ++i) {
      if(line[i] == ' ') {
        split = i;
        break;
      }
    }
    line[split] = '\0';
    //printf("%s %s\n", line, line + split + 1);
    if(atoi(argv[1]) == 1) {
      gettimeofday(&b, NULL);
      int v = ed_distance(line, line + split + 1);
      gettimeofday(&e, NULL);
      cost += e.tv_sec * 1000 + e.tv_usec / 1000 - b.tv_sec * 1000 - b.tv_usec / 1000;
      fprintf(fout, "%d\n", v);
    } else {
      //gettimeofday(&b, NULL);
      int v = new_column_partition(line, line + split + 1);
      //gettimeofday(&e, NULL);
      //cost += e.tv_sec * 1000 + e.tv_usec / 1000 - b.tv_sec * 1000 - b.tv_usec / 1000;
      fprintf(fout, "%d\n", v); 
    }
  }
  if(atoi(argv[1]) == 1) {
    cout << "Cost " << cost << "ms" << endl;
  } else {
    cout << "Cost " << gcost << "ms" << endl;
  }
  fclose(file);
  fclose(fout);

  //column_partition(p, t);
  //cout << ed_distance(p, t) << endl;
  return 0;
}
*/
