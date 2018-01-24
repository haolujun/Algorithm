
/***************************************************************************************
 *
 * Copyright (c) 2017 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: kdtree.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 12/06/2017 05:00:57 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#include "kdtree.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <math.h>

double distance(const Point *a, const Point *b) {
  double c = 0.0;
  for(int i = 0; i < a->x.size(); ++i) {
    c = c + (a->x[i] - b->x[i]) * (a->x[i] - b->x[i]);
  }
  return sqrt(c);
}

void KDTree::_release(Node *root) {
  if(root == NULL) {
    return;
  }

  if(root->l != NULL) {
    _release(root->l);
    root->l = NULL;
  }
  
  if(root->r != NULL) {
    _release(root->r);
    root->r = NULL;
  }

  delete root;
}

void KDTree::fast_build(std::vector<Point> &points) {
  _root = new Node();
  _fast_build(points, 0, points.size(), _root, 0);
}

void KDTree::_fast_build(std::vector<Point> &points, int start, int end, Node *node, int depth) {
  if(start >= end) {
    return;
  }
  int mid = (end - start) / 2;
  std::nth_element(points.begin() + start, points.begin() + start + mid, points.begin() + end, compare[depth % _dim]);
  
  node->l = node->r = NULL;
  node->p = points[start + mid];
  node->dim = depth % _dim;
  if(start + mid - 1 >= start) {
    node->l = new Node();
    _build(points, start, start + mid, node->l, depth + 1);
  }

  if(start + mid + 1 < end) {
    node->r = new Node();
    _build(points, start + mid + 1, end, node->r, depth + 1);
  }
}

void KDTree::build(std::vector<Point> &points) {
  _root = new Node();
  _build(points, 0, points.size(), _root, 0);
}

void KDTree::_build(std::vector<Point> &points, int start, int end, Node *node, int depth) {
  if(start >= end) {
    return;
  }
  std::sort(points.begin() + start, points.begin() + end, compare[depth % _dim]);
  int mid = (end - start) / 2;
  node->l = node->r = NULL;
  node->p = points[start + mid];
  node->dim = depth % _dim;
  if(start + mid - 1 >= start) {
    node->l = new Node();
    _build(points, start, start + mid, node->l, depth + 1);
  }

  if(start + mid + 1 < end) {
    node->r = new Node();
    _build(points, start + mid + 1, end, node->r, depth + 1);
  }
}

void KDTree::display() {
  std::string path("#");
  _display(_root, path);
}

void KDTree::_display(Node *root, std::string path) {
  if(root == NULL) {
    return;
  }
  std::cout << path << " Point:";
  for(int i = 0; i < root->p.x.size(); ++i)
    std::cout << root->p.x[i] << " ";
  std::cout << std::endl;

  _display(root->l, path + 'L');
  _display(root->r, path + 'R');
}

Point KDTree::nearest(const Point &point) {
  std::stack<StackItem> path;
  Point *near = &_root->p;
  Node *node = _root;
  while(node != NULL) {
    near = &node->p;
    path.push(StackItem(node, 1));
    if(point.x[node->dim] < node->p.x[node->dim]) {
      node = node->l;
    } else {
      node = node->r;
    }
  }
  
  double range = distance(&point, near);
  while(path.size() > 0) {
    StackItem stack_item = path.top();
    node = stack_item.node;
    path.pop();

    if(stack_item.node == NULL) {
      continue;
    }
    
    double tmp_dist = distance(&node->p, &point);
    
    if(tmp_dist < range) {
      range = tmp_dist;
      near = &node->p;
    }
    
    if(node->l == NULL && node->r == NULL) {
      continue;
    }
  
    double d = node->p.x[node->dim] - point.x[node->dim];

    if(fabs(d) >= range) { //node某一侧节点可以省略不搜索
      if(stack_item.ancestor == 1) {
        continue; 
      } else {
        if(d >= 0.0) {
          path.push(StackItem(node->l, 0));
        } else {
          path.push(StackItem(node->r, 0));
        }
      }
    } else {               //node两侧都存在可能节点
      if(stack_item.ancestor == 1) {
        if(d > 0.0) {      //不要写成d>=0.0，与最开始递归查找叶节点的比较大小要对应上
          path.push(StackItem(node->r, 0));
        } else {
          path.push(StackItem(node->l, 0));
        }
      } else {
        path.push(StackItem(node->r, 0));
        path.push(StackItem(node->l, 0));
      }
    }
  }
  return *near;
}

void KDTree::range_find(const Point &point, double range, std::vector<Point> &points) {
  std::stack<Node *> path;
  path.push(_root);
  
  while(path.size() > 0) {
    Node *node = path.top();
    path.pop();
    if(node == NULL) {
      continue;
    }

    double tmp_dist = distance(&node->p, &point);
    if(tmp_dist <= range) {
      points.push_back(node->p);
    }
    
    if(node->l == NULL && node->r == NULL) {
      continue;
    }
  
    double d = node->p.x[node->dim] - point.x[node->dim];

    if(fabs(d) > range) { //node某一侧节点可以省略不搜索
      if(d < 0.0) {       //可能的节点在node右子树
        path.push(node->r);
      } else {            //可能的节点在node左子树
        path.push(node->l);
      }
    } else {              //node两侧都存在可能节点
      path.push(node->r);
      path.push(node->l);
    }
  }
}
