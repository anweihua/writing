#pragma once
 

#include <iostream>  
#include <stdio.h>  

//string��c++ ��ͷ�ļ������ڰ�����һ��string�࣬string s1���ǽ���һ��string��Ķ���   

#include <string>  

//cstring.h �Ƕ�Ӧ�ھ�C ͷ�ļ���std �汾  
#include <cstringt.h>  

#include <locale>  

using namespace std;  

class UTF8ToGBK  
{  
public:  
	UTF8ToGBK(void);  
	~UTF8ToGBK(void);  


	//��unicode�����stringת����wstring  
	static wstring stringToWstring(const string text);  

	//��utf8��ʽ����ת����gbk��vs2010��Ĭ�ϵı����ʽ  
	static string UTF8TOGBK(const string text);  

}; 
