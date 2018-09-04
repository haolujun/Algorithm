/**
 *    @file: main.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 09/03/2018 03:26:21 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stack>
#include <vector>
#include <math.h>
#include <sys/time.h>
#include <algorithm>
#include <iostream>
#include "Trie.h"
using namespace std;
int dp[50][50];

Trie trie;

int ed(std::string p, std::string t) {
  int dp[50][50];
  int n = p.size();
  int m = t.size();
  for(int i = 0; i <= n; ++i) dp[i][0] = i;
  for(int j = 0; j <= m; ++j) dp[0][j] = j;
  for(int i = 1; i <= n; ++i) {
    for(int j = 1; j <= m; ++j) {
      if(p[i - 1] == t[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = 1 + std::min(std::min(dp[i][j - 1], dp[i - 1][j]), dp[i - 1][j - 1]);
      }
    }
  }
  return dp[n][m];
}

int ed(std::string p, std::string t, int limit, int prev_r, int dp[][50]) {
  int n = p.size();
  int m = t.size();
  int l = std::max(n, m - limit);
  int r = std::min(n, m + limit);

  for (int i = prev_r + 1; i <= n; ++i) dp[i][0] = i;

  for(int i = prev_r + 1; i <= n; ++i) {
    for(int j = 1; j <= m; ++j) {
      if(p[i - 1] == t[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        dp[i][j] = 1 + std::min(std::min(dp[i][j - 1], dp[i - 1][j]), dp[i - 1][j - 1]);
      }
    }
  }
  return dp[n][m];
}

int cut(std::string p, std::string t, int dp[][50], int prev_r, int limit) {
  int n = p.size();
  int m = t.size();
  int l = max(1, m - limit);
  int r = std::min(n, m + limit);
  
  if (r > prev_r) {
    //编辑矩阵第0列前r行
    for(int i = prev_r + 1; i <= r; ++i) dp[i][0] = i;
    //编辑矩阵第r行
    for(int i = 1; i <= m - 1; ++i) {
      if (p[r - 1] == t[i - 1]) {
        dp[r][i] = dp[r - 1][i - 1];  
      } else {
        dp[r][i] = 1 + std::min(std::min(dp[r - 1][i], dp[r][i - 1]), dp[r - 1][i - 1]);
      }
    }
  }
  
  //编辑矩阵第m列
  int min_v = m;
  dp[0][m] = m;
  for(int j = 1; j <= r; ++j) {
    if (p[j - 1] ==  t[m - 1]) {
      dp[j][m] = dp[j - 1][m - 1];
    } else {
      dp[j][m] = 1 + std::min(std::min(dp[j - 1][m], dp[j][m - 1]), dp[j - 1][m - 1]);
    }
    if (j >= l && dp[j][m] < min_v) min_v = dp[j][m];
  }
  return min_v;
}

void search(Node *node, int dp[][50], int prev_r, std::string p, std::string t, int limit, std::vector<std::string> &match_words) {
  for(int i = 0; i < 26; ++i) {
    if (node->children[i] != NULL) {
      std::string tmp = t + node->children[i]->code;
      if(cut(p, tmp, dp, prev_r, limit) <= limit) {
        int r = std::min(p.size(), tmp.size() + limit);
        search(node->children[i], dp, r, p, tmp, limit, match_words);
      }
      if(node->children[i]->leaf == true && ed(p, tmp, limit, prev_r, dp) <= limit) {
        match_words.push_back(tmp);
      }
    }
  }
}


int main(int argc, char *argv[]) {
  int n, m;
  int limit = 6;
  char line[1024];
  struct timeval b, e;
  int choose = atoi(argv[1]);
  std::vector<std::string> words;

  freopen("words.txt", "r", stdin);
  freopen("out.txt"  , "w", stdout);
  
  scanf("%d %d", &n, &m);

  if(choose == 0) {
    for(int i = 0; i < n; ++i) {
      scanf("%s", line);
      std::string t(line);
      trie.insert(t);
    }
    
    gettimeofday(&b, NULL);
    for(int i = 0; i < m; ++i) {
      scanf("%s", line);
      std::string p(line);
      std::string t = "";
      words.clear();
      search(trie.root(), dp, 0, p, t, limit, words);
      for(int j = 0; j < words.size(); ++j) {
        cout << "Found " << words[j] << endl;
      }
    }
    gettimeofday(&e, NULL);
  } else {
    for(int i = 0; i < n; ++i) {
      scanf("%s", line);
      std::string t(line);
      words.push_back(t);
    }
    std::sort(words.begin(), words.end());
    gettimeofday(&b, NULL);
    for(int i = 0; i < m; ++i) {
      scanf("%s", line);
      std::string p(line);
      for(int j = 0; j < n; ++j) {
        if (j > 0 && words[j] == words[j - 1]) {
          continue;
        }
        if (ed(p, words[j]) <= limit) {
          cout << "Found " << words[j] << endl;
        }
      }
    }
    gettimeofday(&e, NULL);
  }
  printf("%d\n", (e.tv_sec - b.tv_sec) * 1000 + (e.tv_usec - b.tv_usec) / 1000);
  return 0;
}



