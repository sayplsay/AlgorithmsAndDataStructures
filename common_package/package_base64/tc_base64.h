#ifndef __TC_BASE64_H
#define __TC_BASE64_H

#include <string>

using namespace std;

namespace taf
{
/////////////////////////////////////////////////
/** 
* @file tc_base64.h 
* @brief  base64�������. 
*  
*/

/////////////////////////////////////////////////

/**
* @brief �����ṩ��׼��Base64�ı������ 
*/
class TC_Base64
{
public:
    /**
	* @brief  ���ַ�������base64����. 
	*  
    * @param data         ��Ҫ���������
	* @param bChangeLine  �Ƿ���Ҫ�����ձ������ݼ��뻻�з� ��
	*   				  (RFC�н���ÿ76���ַ������س����У�Ĭ��Ϊ����ӻ���
    * @return string      ����������
    */
    static string encode(const string &data, bool bChangeLine = false);
    
    /**
	* @brief  ���ַ�������base64����. 
	*  
    * @param data     ��Ҫ���������
    * @return string  ����������
    */    
    static string decode(const string &data);

    /**
	* @brief  ���ַ�������base64���� . 
	*  
    * @param pSrc        ��Ҫ���������
    * @param nSrcLen     ��Ҫ��������ݳ���
    * @param pDst        ����������      
	* @param bChangeLine �Ƿ���Ҫ�����ձ������ݼ��뻻�з��� 
	*   				 RFC�н���ÿ76���ַ������س����У�Ĭ��Ϊ����ӻ���
    * @return            �������ַ����ĳ���
    */
    static int encode(const unsigned char* pSrc, int nSrcLen, char* pDst, bool bChangeLine = false);
    
    /**
	* @brief  ���ַ�������base64����. 
	*  
    * @param pSrc    ��Ҫ���������    
    * @param nSrcLe  ��Ҫ��������ݳ���
    * @param pDst   ����������
    * @return       �������ַ����ĳ���
    */    
    static int decode(const char* pSrc, int nSrcLen, unsigned char* pDst);
    
protected:

    /**
    * base64�����
    */
    static const char EnBase64Tab[];
    /**
    * base64�����
    */
    static const char DeBase64Tab[];
};

}
#endif
