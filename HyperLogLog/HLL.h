
/***************************************************************************************
 *
 * Copyright (c) 2014 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: HLL.h
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 03/07/2018 01:40:46 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#ifndef __HLL_H__
#define __HLL_H__

class HLL{
  public:
    
    HLL(int register_bits = 6);
    
    ~HLL();
  
    void put(unsigned int hash_value);

    unsigned int distinct_count();

    void reset();

    
  private:
    
    int _count_of_zero_registers();
    int _run_of_zeros(unsigned int h, int b);
    int _get_register_index(unsigned int h, int b);

    int _register_bits;
    int *_registers;
};


#endif


