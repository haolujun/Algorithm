
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: main.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 12/06/2017 05:19:57 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include "kdtree.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
void print(Point p) {
  std::cout << "<";
  std::cout << p.x[0];
  for(int i = 1; i < p.x.size(); ++i)
    std::cout << "," << p.x[i];
  std::cout << ">" << std::endl;
}

void test_kd() {
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
  int cost = 0, build_cost = 0;
  int n, m;
  while(scanf("%d %d", &n, &m) != EOF) {
    std::vector<Point> points;
    int x, y;
    for(int i = 0; i < n; ++i) {
      scanf("%d %d", &x, &y);
      Point p;
      p.x.push_back(x);
      p.x.push_back(y);
      points.push_back(p);
    }

    KDTree tree(2);
    struct timeval b, e;

    gettimeofday(&b, NULL);
    tree.build(points);
    gettimeofday(&e, NULL);
    build_cost += (e.tv_sec - b.tv_sec) * 1000 + (e.tv_usec - b.tv_usec) / 1000;
    for(int i = 0; i < m; ++i) {
      scanf("%d %d", &x, &y);
      Point p;
      p.x.push_back(x);
      p.x.push_back(y);
      gettimeofday(&b, NULL);
      Point r = tree.nearest(p);
      gettimeofday(&e, NULL);
      cost += (e.tv_sec - b.tv_sec) * 1000000 + (e.tv_usec - b.tv_usec);
      std::cout << distance(&r, &p) << std::endl;
    }
  }
  printf("build_cost=%dms cost=%dus\n", build_cost, cost);
}

Point nearest(std::vector<Point> &points, Point p) {
  
  double dist = distance(&points[0], &p);
  Point r = points[0];
  for(int i = 1; i < points.size(); ++i) {
    double tmp_dist = distance(&points[i], &p);
    if(tmp_dist < dist) {
      dist = tmp_dist;
      r = points[i];
    }
  }
  return r;
}

void test_baoli() {
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
  int cost = 0;
  int n, m;
  while(scanf("%d %d", &n, &m) != EOF) {
    std::vector<Point> points;
    int x, y;
    for(int i = 0; i < n; ++i) {
      scanf("%d %d", &x, &y);
      Point p;
      p.x.push_back(x);
      p.x.push_back(y);
      points.push_back(p);
    }
    for(int i = 0; i < m; ++i) {
      scanf("%d %d", &x, &y);
      Point p, r;
      p.x.push_back(x);
      p.x.push_back(y);
      struct timeval b, e;
      gettimeofday(&b, NULL);
      r = nearest(points, p);
      gettimeofday(&e, NULL);
      cost += (e.tv_sec - b.tv_sec) * 1000 + (e.tv_usec - b.tv_usec) / 1000;
      std::cout << distance(&p, &r) << std::endl;
    }
  }
  printf("cost=%dms\n", cost);

}

int main() {
  test_kd();
  return 0;
}

