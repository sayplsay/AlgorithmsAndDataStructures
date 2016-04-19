#ifndef _STRUCTURE_TRIE_H
#define _STRUCTURE_TRIE_H
#include <string.h>
/* 每个节点最大的分支个数*/
const int MaxBranchNum = 26;

/*定义trie树的结点*/
class TrieNode {
public:
    char* word; //存储的单词
    int count;  //出现的次数
    TrieNode* nextBranch[MaxBranchNum]; // 指向分支的指针
public:
    TrieNode(): word(NULL), count(0) {
      memset(nextBranch, NULL, sizeof(TrieNode*) * MaxBranchNum);
    }
};

class Trie {
public:
    Trie();
    ~Trie();
    void Insert(const char* str); // 插入字符串
    bool Search(const char* str, int& count); // 查找字符串str，并返回出现的次数
    bool Remove(const char* str); // 删除字符串
    void PrintAll(); // 打印所有结点
    void PrintPre(const char* str); // 打印以str为前缀的单词
private:
    TrieNode* pRoot; // 根结点
private:
    void Destroy(TrieNode* pRoot);
    void Print(TrieNode* pRoot);
};


#endif

