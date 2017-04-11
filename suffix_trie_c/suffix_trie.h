#ifndef _SUFFIX_TRIE_H  
#define _SUFFIX_TRIE_H 
  
#include <iostream>  
#include <string> 

class TrieNode {
public:
  TrieNode* lchild;
  TrieNode* sibling;
  std::string text;
public:
  TrieNode(): lchild(NULL), sibling(NULL){}   
};

class SuffixTrie { 
public:  
    SuffixTrie();  
    ~SuffixTrie();  
    bool CreateTree(std::string& constant_str);  
    void DestoryTree();  
    void Show_Out();  
protected:  
    void out(TrieNode *parent, int whitspace);  
    void initialise();  
    void scan_all_suffix();  
    void fix_surffix(TrieNode *parent, std::string surffix);  
    void add_to_tree(TrieNode *parent, std::string contex);  
    void cyl_clear(TrieNode *parent);  
private:  
    TrieNode* pRoot;  
    std::string constant_str;  
};  
  
#endif  
  
/* 
    根据后缀树生成步骤经验总结一个新的创建方法： 
 
    step 1 : 
    initialise:  扫描给定字符串的每一个字符，如果该字符和根节点的每一个儿子首字母都不同， 
                   则将以此字符为首的整个后缀插入到根节点的儿子中； 
    step 2： 
    scan_all_suffix:       
             然后从给定字符串的最长后缀到最短后缀依次按trie树的方式在初步建立的树中查找， 
             肯定会出现以下3种情况之一： 
             情况一：如果查找成功，则当前后缀处理完成，继续查找下一个后缀； 
             情况二：如果对比完一个节点都符合，但在其子节点中找不到下一个分支，查找失败， 
                          则直接用当前后缀剩下的字符串建立新节点，并作为当前节点的一个子节点； 
             情况三：否者，一定会在一个节点字符串的第 k 个字符处出现查找失败，假设此时已经 
                          查找到当前后缀的第m个字符处,这时就可以分割该节点， 
                          1. 以当前查找节点从 k 开始的后缀建立新节点，并作为当前查找结点一个子节点； 
                          2. 当前扫描的后缀从 m 开始的后缀建立新节点，作为其另一个子节点； 
                          3.将当前节点从k开始的后缀去掉； 
 
     当运行完上面两个循环后，一颗后缀树就建立完成了。 
*/  

/* vim: set expandtab nu smartindent ts=2 sw=2 sts=2: */
