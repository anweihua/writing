//
//  TDInvFileUtils.h
//  MyCocoa2DTest
//
//  Created by Τ ��ȫ on 13-2-27.
//
//

#ifndef __MyCocoa2DTest__TDInvFileUtils__
#define __MyCocoa2DTest__TDInvFileUtils__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

/** ��������ļ�����Ͷ�ȡ
 */

class TDInvFileUtils {
public:
    /** ��ȡ�����ļ����������� */
    static string getFileByName(string pFileName);
    
    /** �������ݵ��ļ� */
    static bool saveFile(char* pContent,string pFileName);
    
};

#endif /* defined(__MyCocoa2DTest__TDInvFileUtils__) */