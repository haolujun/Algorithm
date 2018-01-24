#include <stdio.h>
#include <string.h>

#define POS(i, j, k, la, lb, lc) ((i) * (lb)* (lc) + (j) * (lc) + (k))
#define max(a, b) ((a) > (b) ? (a) : (b))

int cover(char *a, char *b, int nstr){
 
  if(a == NULL || b == NULL || nstr == 0) {
    return 0;
  }
 
  int la = strlen(a);
  int lb = strlen(b);
 
  if(la == 0 || lb == 0) {
    return 0;
  }
 
  //动规数组 
  int *dp = new int[la * lb * (nstr + 1)];
  //记录全局最优解 
  int *maxcover = new int[la * lb * (nstr + 1)];
 
  int *method = new int[la * lb * (nstr + 1)];
 
  for(int i = 0; i < la * lb * (nstr + 1); ++i) 
    *(method + i) = -1;
 
  if(dp == NULL || maxcover == NULL){
    if(dp != NULL) delete[] dp;
    if(maxcover != NULL) delete[] maxcover;
    return 0;
  }
 
  //动规方程 
  int result = 0;
  for(int i = 0; i < la * lb * (nstr + 1); ++i){
    dp[i] = maxcover[i] = 0;
  }
 
  //dp[i][0][1] = a[i] == b[0] ? 1 : -1;
  for(int i = 0; i < la; ++i){
    int x = POS(i, 0, 1, la, lb, nstr + 1);
    int y = POS(i - 1, 0, 1, la, lb, nstr + 1);
    if(a[i] == b[0]){
      result = dp[x] = maxcover[x] = 1;
      method[x] = 0;
    } else {
      maxcover[x] = (i > 0 ? maxcover[y] : 0);
    }
  }
 
  //dp[0][j][1] = a[0] == b[j] ? 1 : -1;
  for(int j = 0; j < lb; ++j){
    int x = POS(0, j, 1, la, lb, nstr + 1);
    int y = POS(0, j - 1, 1, la, lb, nstr + 1);
    if(a[0] == b[j]){
      result = dp[x] = maxcover[x] = 1;
      method[x] = 0;
    } else {
      maxcover[x] = (j > 0 ? maxcover[y] : 0);
    }
  }
 
  for(int i = 1; i < la; ++i){
    for(int j = 1; j < lb; ++j){
      for(int k = 1; k <= nstr; ++k){
        int x = POS(i, j, k, la, lb, nstr + 1);
        int y = POS(i, j - 1, k, la, lb, nstr + 1);
        int z = POS(i - 1, j, k, la, lb, nstr + 1);
        maxcover[x] = max(maxcover[x], maxcover[y]);
        maxcover[x] = max(maxcover[x], maxcover[z]);
        if(a[i] != b[j]){
          continue;
        }
        x = POS(i - 1, j - 1, k - 1, la, lb, nstr + 1);
        y = POS(i - 1, j - 1, k, la, lb, nstr + 1); 
        z = POS(i, j, k, la, lb, nstr + 1);
        //a[i],b[j]单独划分成一个子串 
        if(maxcover[x] + 1 > dp[z]) {
          dp[z] = maxcover[x] + 1;
          method[z] = 0;
        }
        //dp[z] = max(dp[z], maxcover[x] + 1);
        //a[i],b[j]和前面的字符连接成一个子串 
        //dp[z] = max(dp[z], dp[y] + 1);
        if(dp[y] + 1 >= dp[z]) {
          dp[z] = dp[y] + 1;
          method[z] = 1;
        }
        //更新全局最大值 
        maxcover[z] = max(maxcover[z], dp[z]);
        result = max(dp[z], result);
      }
    }
  }
 
  int i = 0, j = 0, k = 0;
  for(i = 0; i < la; ++i) {
    for(j = 0; j < lb; ++j) {
      for(k = 0; k < nstr + 1; ++k) {
        int x = POS(i, j, k, la, lb, nstr + 1);
        if(dp[x] == result)
          break;
      }
      if(k != nstr + 1)
        break;
    }
    if(j != lb)
      break;
  }
  
  while(i >= 0 && j >= 0 && k >= 1) {
    int z = POS(i, j, k, la, lb, nstr + 1);
    if(method[z] == -1) {
      int x = POS(i - 1, j, k, la, lb, nstr + 1);
      int y = POS(i, j - 1, k, la, lb, nstr + 1);
      if(maxcover[z] == maxcover[x])
        i--;
      else if(maxcover[z] == maxcover[y])
        j--;
    } else if(method[z] == 0) {
      printf("single char: <%d, %d, %d> %d %c\n", i, j, k, dp[z], a[i]);
      i--;j--;k--;
    } else {
      printf("contine with before: <%d, %d, %d> %d %c\n", i, j, k, dp[z], a[i]);
      i--;j--;
    }
  }
  delete[] dp;
  delete[] maxcover;
  return result;
}


int main(){
  char a[100], b[100];
  int nstr = 5;
  int dp[100][100][4];
  while(scanf("%s %s", a, b) != EOF){
    int ret = cover(a, b, nstr);
    printf("%d\n", ret);
  }
  return 0;
} 
