/**
 *    @file: Trie.h
 *  @author: zhangxingbiao(xingbiao.zhang@lejent.com)
 *    @date: 09/03/2018 03:26:45 PM
 * @version: 1.0 
 *   @brief: 
 *  
 **/
#ifndef __TRIE_H__
#define __TRIE_H__
#include <string>

typedef struct Node {
  char code;
  bool leaf;
  Node *children[26];
  Node () {
    for(int i = 0; i < 26; ++i) {
      children[i] = NULL;
    }
    leaf = false;
  }
}Node;

class Trie {

  public:
    
    Trie () {
      _root = new Node();
    }

    void insert(std::string &word) {
      Node *node = _root;
      for(int i = 0; i < word.size(); ++i) {
        int index = word[i] - 'a';
        if (node->children[index] == NULL) {
          node->children[index] = new Node();
          node->children[index]->code = word[i];
          node = node->children[index];
        } else {
          node = node->children[index];
        }
      }
      node->leaf = true;
    }

    Node* root() {
      return _root;
    }

  private:
    Node *_root;

};


#endif

