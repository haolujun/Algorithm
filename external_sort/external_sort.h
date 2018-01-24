
/***************************************************************************************
 *
 * Copyright (c) 2014 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: external_sort.h
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 01/08/2018 02:58:42 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#ifndef __EXTERNAL_SORT__
#define __EXTERNAL_SORT__

#include "file_storage.h"
namespace afanti {
namespace index {
template<class ELEMENT>
class ExternalSort {
  public:
    
    ExternalSort(std::string dir, std::string w_file_prefix, int batch_count) {
      _dir = dir;
      _w_file_prefix = w_file_prefix;
      _batch_count = batch_count;
      _file_storage = new FileStorage<ELEMENT>(_dir, _w_file_prefix, batch_count);
    }
    
    ~ExternalSort() {
      delete _file_storage;
    }

    void push(ELEMENT &ele) {
      _file_storage->sorted_write(ele);
    }
   
    void flush() {
      _file_storage->sorted_flush();
    } 

    int merge_sort(
      FileStorage<ELEMENT> *x, 
      FileStorage<ELEMENT> *y, 
      FileStorage<ELEMENT> *z
    ) {
      //告诉编译器这是个数据类型，否则有些sb编译器是推断不出来的
      typename FileStorage<ELEMENT>::Iterator x_b = x->begin();
      typename FileStorage<ELEMENT>::Iterator x_e = x->end();
      typename FileStorage<ELEMENT>::Iterator y_b = y->begin();
      typename FileStorage<ELEMENT>::Iterator y_e = y->end();

      while(x_b != x_e && y_b != y_e) {
        if(*x_b < *y_b) {
          z->write(*x_b);
          ++x_b;
        } else if(*x_b > *y_b) {
          z->write(*y_b);
          ++y_b;
        } else {
          z->write(*x_b);
          z->write(*y_b);
          ++x_b;
          ++y_b;
        }
      }
      
      while(x_b != x_e) {
        z->write(*x_b);
        ++x_b;
      }

      while(y_b != y_e) {
        z->write(*y_b);
        ++y_b;
      }

      z->flush();
      return 0;
    }
    
    int sort() {
      int step = 1;
      char prefix[64] = {0};
      std::vector<FileStorage<ELEMENT>*> storages;
      std::vector<FileStorage<ELEMENT>*> new_storages; 
      std::vector<std::string> filenames = _file_storage->get_files();
      
      for(int i = 0; i < filenames.size(); ++i) {
        FileStorage<ELEMENT> *storage = new FileStorage<ELEMENT>(filenames[i]);
        storages.push_back(storage);
      }
      
      while(storages.size() > 1) {
        for(int i = 0; i < storages.size(); i += 2) {
          if(i + 1 >= storages.size()) {
            FileStorage<ELEMENT> *storage = new FileStorage<ELEMENT>(storages[i]);
            new_storages.push_back(storage);
            break;
          }
          
          memset(prefix, 0x00, sizeof(prefix));
          snprintf(prefix, 64, "%s_%d_%d", _w_file_prefix.c_str(), step, i);
          
          FileStorage<ELEMENT> *storage = new FileStorage<ELEMENT>(
                                                _dir.c_str(),
                                                std::string(prefix), 
                                                _batch_count
                                              );
          
          merge_sort(storages[i], storages[i + 1], storage);
          new_storages.push_back(storage);
          
          storages[i]->delete_files();
          storages[i + 1]->delete_files();
        }

        storages.swap(new_storages);

        for(int i = 0; i < new_storages.size(); ++i) {
          delete new_storages[i];
        }

        new_storages.clear();
        step++;
      }
    }

  private:
    std::string _dir;
    std::string _w_file_prefix;
    int _batch_count;                    //每个文件多少行
    FileStorage<ELEMENT> *_file_storage; //写文件工具
};

}}
#endif
