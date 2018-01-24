
/***************************************************************************************
 *
 * Copyright (c) 2014 Lejent, Inc. All Rights Reserved
 *
 **************************************************************************************/
/**
 *    @file: file_storage.h
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 01/08/2018 10:23:06 AM
 * @version: 1.0 
 *   @brief: 
 *  
 **/

#ifndef __FILE_STORAGE__
#define __FILE_STORAGE__
#include <map>
#include <set>
#include <string>
#include <vector>
#include <deque>
#include <cstring>
#include <algorithm>
#include <cstdio>

namespace afanti {

namespace index {

template<class ELEMENT>
class FileStorage {
  public:
    class Iterator{
      private:
        int _file_index;
        long long _line_no;
        int _cache_size;
        FILE *_cur_file;
        char _line[1024];
        ELEMENT _ele;
        std::vector<std::string> _files;
        std::deque<ELEMENT> _cache;          
      
      public:
        Iterator(std::vector<std::string> &files, int line_no) {
          this->_files = files;
          this->_file_index = 0;
          this->_cur_file = NULL;
          this->_line_no = line_no;
          this->_cache_size = 100;
          _fill();

          if(_cache.size() > 0) {
            _ele = *(_cache.begin());
            _cache.pop_front();
          }
        }

        ~Iterator() {
          if(_cur_file != NULL) {
            fclose(_cur_file);
            _cur_file = NULL;
          }
        }

        ELEMENT& operator*(){
          return _ele;
        }

        Iterator& operator++(){
          if(_cache.size() != 0) {
            _ele = *(_cache.begin());
            _cache.pop_front();
            _line_no++;
            return *this;
          }
          
          _fill();
            
          if(_cache.size() != 0) {
            _ele = *(_cache.begin());
            _cache.pop_front();
            _line_no++;
            return *this;
          }

          _line_no = -1;

          return *this;
        }

        bool operator!=(Iterator &it) {
          if(this->_line_no != it._line_no) {
            return true;
          }
          return false;
        }

        bool operator==(Iterator &it) {
          return this->_line_no == it._line_no;
        }
     
      private:
        void _fill() {
          //已经读完所有文件
          if(_file_index >= _files.size()) {
            return;
          }
          
          //还没有打开任何文件
          if(_cur_file == NULL) {
            _cur_file = fopen(_files[_file_index].c_str(), "r");  
          }

          while(_cache.size() < _cache_size) {
            void *ret = fgets(_line, 1024, _cur_file);
            if(ret != NULL) {
              ELEMENT ele;
              ele.parse(_line);
              _cache.push_back(ele);
            } else {
              //到了文件结尾，换下一个文件
              fclose(_cur_file);
              _cur_file = NULL;
              _file_index++;
              
              if(_file_index >= _files.size()) {
                return;
              }
              _cur_file = fopen(_files[_file_index].c_str(), "r");
            }
          }
        }
    };
    
    FileStorage(std::string dir, std::string w_file_prefix, int batch_count){
      _file_no = 0;
      _batch_count = batch_count;
      _w_prefix = w_file_prefix;
      _dir = dir;
    }

    FileStorage(std::string r_filename){
      _file_no = 0;
      _filenames.insert(r_filename);
    }
    
    FileStorage(FileStorage *storage) {
      _file_no = storage->_file_no;
      _batch_count = storage->_batch_count;
      _w_prefix = storage->_w_prefix;
      _filenames = storage->_filenames;
      _elements = storage->_elements;
      _dir = storage->_dir;
    }

   
    ~FileStorage() {
    }
    
    Iterator begin() {
      std::vector<std::string> files;
      files = this->get_files();
      return Iterator(files, 0);
    }

    Iterator end() {
      std::vector<std::string> files;
      files = this->get_files();
      return Iterator(files, -1); 
    }

    int write(ELEMENT &ele) {
      _elements.push_back(ele); 
      if(_elements.size() >= _batch_count) {
        flush();  
      }
    }
   
    int sorted_write(ELEMENT &ele) {
      _elements.push_back(ele);
      if(_elements.size() >= _batch_count) {
        sorted_flush();
      }
    }

    template<class COMPARE>
    int write(ELEMENT &ele, COMPARE compare) {
      _elements.push_back(ele);
      if(_elements.size() >= _batch_count) {
        flush(compare);
      }
      return 0;
    }

    template<class COMPARE>
    int flush(COMPARE compare) {
      std::sort(_elements.begin(), _elements.end(), compare);
      flush();
      return 0;
    }

    int sorted_flush() {
      std::sort(_elements.begin(), _elements.end());
      flush();
    }

    int flush() {
      if(_elements.size() == 0) {
        return 0;
      }

      char filename[64] = {0};
      snprintf(filename, 64, "%s/%s_%d", _dir.c_str(), _w_prefix.c_str(), _file_no);
      FILE *wfile = fopen(filename, "a+");
    
      if(_filenames.find(std::string(filename)) == _filenames.end()) {
        _filenames.insert(std::string(filename));
      }

      for(int i = 0; i < _elements.size(); ++i) {
        std::string str = _elements[i].to_string();
        fwrite(str.c_str(), str.size(), 1, wfile);    
      }
      fclose(wfile);
      wfile = NULL;
      _file_no++;
      _elements.clear();
      return 0;
    }

    std::vector<std::string> get_files(){
      std::vector<std::string> fnames;
      for(std::set<std::string>::iterator it = _filenames.begin();
          it != _filenames.end(); ++it) {
        fnames.push_back(*it); 
      }
      return fnames;
    }

    void delete_files() {
      for(std::set<std::string>::iterator it = _filenames.begin();
          it != _filenames.end(); ++it) {
        remove(it->c_str()); 
      }
    }

  private:
    
    int _batch_count;                     //批量写入时，每次写入文件数据大小
    int _file_no;                         //当前文件编号(0, 1, 2,......)
    std::string _dir;                     //写文件目录
    std::string _w_prefix;                //写文件时的前缀
    std::set<std::string> _filenames;     //文件存储产生的所有文件
    std::vector<ELEMENT> _elements;       //元素内存缓存
};

}}
#endif
