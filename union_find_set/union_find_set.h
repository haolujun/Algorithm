
/***************************************************************************************
 *
 * Copyright (c) 2014 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: union_find_set.h
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 01/30/2018 10:24:11 AM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#ifndef __UNION_FIND_SET_H__
#define __UNION_FIND_SET_H__

#include <assert.h>
class UnionFindSet{
  public:
    UnionFindSet(int size) {
      _size = size;
      _pre = new int[_size + 1];
      if(_pre == 0x00) {
        assert(0);
      }

      for(int i = 0; i <= _size; ++i) {
        _pre[i] = i;
      }
    }

    ~UnionFindSet() {
      if(_pre != 0x00) {
        delete[] _pre;
        _pre = 0x00;
      }
    }

    int find(int x) {
      int r = x;
      while(_pre[r] != r) {
        r = _pre[r];
      }
      
      while(_pre[x] != x) {
        int tmp = _pre[x];
        _pre[x] = r;
        x = tmp;
      }
      return r;
    }
    
    void join(int x, int y) {
      x = find(x);
      y = find(y);
      if(x != y) {
        _pre[x] = y;
      }
    }

  private:
    int _size;
    int *_pre;
};

#endif

