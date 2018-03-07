
/***************************************************************************************
 *
 * Copyright (c) 2018 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: HLL.cpp
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 03/07/2018 01:40:39 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#include "HLL.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

HLL::HLL(int register_bits) {
  _register_bits = register_bits;
  int size = pow(2, _register_bits);
  _registers = (int *)malloc(size * sizeof(int));
  for(int i = 0; i < size; ++i) {
    _registers[i] = 0;
  }
}

HLL::~HLL(){
  if(_registers != NULL) {
    free(_registers);
    _registers = NULL;
  }
}

void HLL::put(unsigned int hash_value) {
  int register_index = _get_register_index(hash_value, _register_bits);
  int run_length = _run_of_zeros(hash_value, _register_bits);
  _registers[register_index] = _registers[register_index] > run_length ? 
                               _registers[register_index] : run_length ;
}

int HLL::_get_register_index(unsigned int h, int b) {
  unsigned int mask = pow(2, b) - 1;
  return h & mask;
}

int HLL::_run_of_zeros(unsigned int h, int b) {
  int run = 0;
  for(int i = b; i < 32; ++i) {
    run++;
    if (((h >> i) & 0x01) == 1) {
      break;
    }
  }
  return run;
}

int HLL::_count_of_zero_registers() {
  int m = pow(2, _register_bits);
  int zero_count = 0;
  for(int i = 0; i < m; ++i) {
    if(_registers[i] == 0) {
      zero_count++;
    }
  }
  return zero_count;
}

unsigned int HLL::distinct_count() {
  int m = pow(2, _register_bits);
  double alpha = 0.0;
  if(m == 16) {
    alpha = 0.673;  
  } else if(m == 32) {
    alpha = 0.697;
  } else if(m == 64) {
    alpha = 0.709;
  } else {
    alpha = 0.7213 / (1 + 1.079 / m);
  }
  
  double sum = 0.0;
  for(int i = 0; i < m; ++i) {
    sum = sum + pow(2.0, -_registers[i]);
  }
  
  double dv_est = alpha * m * m * 1.0 / sum;
  double dv = 0.0;
  if (dv_est < 5.0 / 2.0 * m) {
    int v = _count_of_zero_registers();
    if(v == 0) {
      dv = dv_est;
    } else {
      dv = m * log((double)m / (double)v);
    }
  }

  if (dv_est <=  1.0 / 30 * pow(2, 32)) {
    dv = dv_est;
  } else {
    dv = -pow(2, 32) * log(1 - dv_est / pow(2, 32));
  }
  return (unsigned int)dv;
}

