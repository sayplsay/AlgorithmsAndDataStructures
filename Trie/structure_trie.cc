#include <iostream>
#include <cstring>
#include <algorithm>
#include "structure_trie.h"

using namespace std;

/*构造函数*/
Trie::Trie() 
{
  pRoot = new TrieNode();
}

/*析构函数*/
Trie::~Trie() 
{
  Destroy(pRoot);
}

/*插入*/
void Trie::Insert(const char* str) 
{
  int index;
  TrieNode* pLoc = pRoot;
  for(int i = 0; str[i]; i++) 
  {
    index = str[i] - 'a';
    if(index < 0 || index > MaxBranchNum) 
    {
      return;
    }
    if(NULL == pLoc->nextBranch[index])
    {
      pLoc->nextBranch[index] = new TrieNode();
    }
    pLoc = pLoc->nextBranch[index];
  }
  if(NULL != pLoc->word)
  {
    pLoc->count++;
    return;
  }
  else
  {
    pLoc->count++;
    pLoc->word = new char[strlen(str) + 1];
    strcpy(pLoc->word, str);
  }
}

/*搜索并返回个数*/
bool Trie::Search(const char* str, int& count)
{
  int index = -1;
  TrieNode* pLoc = pRoot;
  while(pLoc && *str)
  {
    index = *str - 'a';
    if(index < 0 || index > MaxBranchNum)
    {
      return false;
    }
    pLoc = pLoc->nextBranch[index];
    str++;
  }
  if(pLoc && pLoc->word)
  {
    count = pLoc->count;
    return true;
  }
  return false;
}

/*删除*/
bool Trie::Remove(const char* str)
{
  int index = -1;
  TrieNode* pLoc = pRoot;
  while(pLoc && *str)
  {
    index = *str - 'a';
    if(index < 0 || index > MaxBranchNum)
    {
      return false;
    }
    pLoc = pLoc->nextBranch[index];
    str++;
  }
  if(pLoc && pLoc->word)
  {
    delete[] pLoc->word;
    pLoc->word = NULL;
    pLoc->count = 0;
    return true;
  }
  return false;
}

/*打印所有结点*/
void Trie::PrintAll()  
{  
  Print(pRoot);  
}  

/*打印所有的以str开始的word*/
void Trie::PrintPre(const char* str)  
{  
  int index = -1;  
  TrieNode* pLoc = pRoot;  
  while(pLoc && *str)  
  {  
    index = *str - 'a';//如果区分大小写，可以扩展  
    if(index < 0 || index > MaxBranchNum)//不是一个单词，不执行插入  
    {  
      return;  
    }  
    pLoc = pLoc->nextBranch[index];  
    str++;  
  }  
  if (pLoc)//条件成立，找到该单词  
  {  
    Print(pLoc);  
  }  
} 

/*按照字典顺序输出以pRoot为根的所有的单词*/  
void Trie::Print(TrieNode* pRoot)  
{  
  if (NULL == pRoot)  
  {  
    return;  
  }  
  //输出单词  
  if (NULL != pRoot->word)  
  {  
    cout<<pRoot->word<<" "<<pRoot->count<<endl;  
  }  
  //递归处理分支  
  for (int i = 0;i < MaxBranchNum;i++)  
  {  
    Print(pRoot->nextBranch[i]);  
  }  
}  
  
/*销毁trie树*/  
void Trie::Destroy(TrieNode* pRoot)  
{  
  if (NULL == pRoot)  
  {  
    return;  
  }  
  for (int i = 0;i < MaxBranchNum;i++)  
  {  
    Destroy(pRoot->nextBranch[i]);  
  }  
  //销毁单词占得空间  
  if (NULL != pRoot->word)  
  {  
    delete []pRoot->word;     
    pRoot->word = NULL;  
  }  
  delete pRoot;//销毁结点  
  pRoot = NULL;  
}  
