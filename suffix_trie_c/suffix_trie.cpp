#include <string.h>
#include "suffix_trie.h"
  
SuffixTrie::SuffixTrie() : pRoot(NULL), constant_str("") {}  
  
SuffixTrie::~SuffixTrie() {  
  if(NULL != pRoot) {
    DestoryTree();  
  }
  pRoot = NULL;  
}  
  
//以contex字符串建立节点并插入到parent的子节点  
void SuffixTrie::add_to_tree(TrieNode *parent, std::string contex) {  
  TrieNode* pSub = new TrieNode();  
  pSub->text = contex; 
  if (NULL == parent->lchild) {  
    parent->lchild = pSub;  
  }  
  else {  
    pSub->sibling = parent->lchild;  
    parent->lchild = pSub;  
  }  
}  
  
void SuffixTrie::initialise() {  
  int a[128] = {0};
  pRoot = new TrieNode();
  pRoot->text = "root";
  for (int i = 0; i < constant_str.size(); ++i) {
    if (0 == a[constant_str[i]]) {
      add_to_tree(pRoot, constant_str.substr(i));
      a[constant_str[i]] = 1;
    }
  }
}


//查找并更正后缀surffix  
void SuffixTrie::fix_surffix(TrieNode *parent, std::string surffix) {  
  TrieNode *pNode = parent->lchild;
  if (NULL == pNode) {
    return;
  }  
  
  //找出可能包含此后缀的分支
  while(pNode) { 
    if(surffix[0] == pNode->text[0]) {
      break;
    }
    pNode = pNode->sibling;
  }
  
  //情况二：没有分支，直接添加，后缀surffix处理结束
  if (NULL == pNode) {
    add_to_tree(parent, surffix);
    return;
  }

  //找出了所属的分支
  int i = 1;
  int surffix_len = surffix.size();
  int branch_len = pNode->text.size();
  while(1) {
    //情况一：发现后缀已经包含在其中了，增加一个节点
    if (surffix_len < i+1) {
      if (i < branch_len) {
        TrieNode *pOld = pNode->lchild;
        add_to_tree(pNode, pNode->text.substr(i));
        pNode->lchild->sibling = NULL;
        pNode->lchild->lchild = pOld;
        pNode->text= pNode->text.substr(0,i);
      }
      return;
    }
    
    //一个分直节点已经检查完，用后缀剩下的子串递归检查子节点中下一个可能分支
    if (branch_len < i+1) {
      fix_surffix(pNode, surffix.substr(i));
      return;
    }
    
    if (surffix[i] != pNode->text[i]) {
      // 保存当前节点的孩子
      TrieNode *pOld = pNode->lchild;
      //当前节点字符串中断点及以后的子串作为其子节点
      add_to_tree(pNode, pNode->text.substr(i));
      //当前节点的原有子节点作为刚添加的节点的子节点
      pNode->lchild->sibling = NULL;
      pNode->lchild->lchild = pOld;
      //当前后缀剩下的子串作为其子节点
      add_to_tree(pNode, surffix.substr(i));
      //当前节点去掉其中断点及以后的子串
      pNode->text= pNode->text.substr(0,i);
      return;
    }
    i++;
  }
}

void SuffixTrie::scan_all_suffix() {  
  for(int i = 1; i < constant_str.size(); ++i) {   // 第一个前缀不需要检查
    fix_surffix(this->pRoot, constant_str.substr(i));  
  }  
}  
  
bool SuffixTrie::CreateTree(std::string& constant_str) {  
  if(NULL != pRoot || 0 == constant_str.size()) {
    return false;  
  }
  
  this->constant_str = constant_str;  
  initialise(); // 插入所有  
  scan_all_suffix();
  return true;  
} 

//释放掉树中各节点占用的空间  
void SuffixTrie::cyl_clear(TrieNode *parent) {  
  if (NULL == parent) {
    return;
  } 
  TrieNode* pSibling = parent->sibling;
  TrieNode* pTemp = NULL;
  while (NULL != pSibling) {
    if (NULL != pSibling->lchild) {
      cyl_clear(pSibling->lchild);
    }
    pTemp= pSibling;
    pSibling = pSibling->sibling;
    delete pTemp;
    pTemp = NULL;
  }  

  if (NULL != parent->lchild){
    cyl_clear(parent->lchild); 
  }
  delete parent;
  parent = NULL;
}  
  
void SuffixTrie::DestoryTree() {  
  cyl_clear(pRoot); 
  pRoot = NULL; 
}  
  
void SuffixTrie::out(TrieNode *parent, int whitspace)  
{  
  if (NULL == parent) {
    return;  
  }
  std::cout << parent->text << std::endl; 
  parent = parent->lchild;  
  while (NULL != parent) { 
    for(int i=0; i < whitspace; i++) {
      std::cout <<"   ";
    }  
    if (parent->sibling) {  
      std::cout <<"├─";  
    }
    else {  
      std::cout <<"└─";  
    }
    out(parent, whitspace+1);  
    parent = parent->sibling;  
  }  
}  
  
void SuffixTrie::Show_Out() {  
    out(this->pRoot, 0);  
} 

/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
