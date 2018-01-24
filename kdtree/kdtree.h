
/***************************************************************************************
 *
 * Copyright (c) 2014 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: kdtree.h
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 12/06/2017 03:36:54 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#ifndef __KD_TREE_H__
#define __KD_TREE_H__
#include <vector>
#include <string>
#include <assert.h>

typedef struct {
  std::vector<double> x;
}Point;

typedef struct Node{
  Node *l;
  Node *r;
  Point p;
  int dim;
} Node;

class StackItem {
  public:
    StackItem(Node *node, int ancestor) {
      this->node = node;
      this->ancestor = ancestor;
    }
    Node *node;
    int ancestor;
};

struct Compare {
  bool operator() (Point a, Point b) {
    return a.x[dim] < b.x[dim];
  }
  int dim;
};

double distance(const Point *a, const Point *b);

class KDTree {
  public:
    KDTree(int dim) {
      _dim = dim;
      _root = NULL;
      compare = new Compare[dim];
      for(int i = 0; i < dim; ++i) {
        compare[i].dim = i;
      }
    }
    
    ~KDTree() {
      _release(_root);
      delete[] compare;
    }

    //普通排序建立算法
    void build(std::vector<Point> &points);
    //nth建立算法
    void fast_build(std::vector<Point> &points);
    //找距离最近的点
    Point nearest(const Point &point);
    //找一个范围内的点
    void range_find(const Point &point, double range, std::vector<Point> &points);
    //打印这棵树
    void display();
  private:
    void _display(Node *root, std::string path);
    void _build(std::vector<Point> &point, int start, int end, Node *root, int depth);
    void _fast_build(std::vector<Point> &point, int start, int end, Node *root, int depth);
    void _release(Node *root);
    int _dim;     //维度
    Node *_root;  //根节点
    Compare* compare;
};

#endif

