#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include "structure_trie.h"

using namespace std;

int main()  
{  
    Trie t;  
    string str;  
    int count = -1;  
    ifstream in("./word.txt", ios::in | ios::binary);  
    //把单词输入字典树  
    while(in >> str)  
    {  
        //transform(str.begin(),str.end(),str.begin(),tolower);//大写变小写  
        t.Insert(str.c_str());  
    }  
    //查找  
    bool isFind = t.Search("the",count);  
    if (isFind)  
    {  
        cout<<"存在the,出现次数："<<count<<endl;  
    }  
    else  
    {  
        cout<<"不存在the!"<<endl;  
    }  
    //输出  
    t.PrintAll();  
    //删除  
    bool isDel = t.Remove("the");  
    if (isDel)  
    {  
        cout<<"删除成功!"<<endl;  
    }  
    else  
    {  
        cout<<"删除失败!"<<endl;  
    }  
    //输出以w开头的单词  
    t.PrintPre("w");  
    cout<<endl;  
}  
