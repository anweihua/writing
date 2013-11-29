#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
#pragma once
class PopLayer:public CCLayer
{
public:
	PopLayer(void);
	~PopLayer(void);

	virtual bool init();
	CREATE_FUNC(PopLayer);

	static PopLayer* create(const char* backgroundImage);

	virtual void registerWithTouchDispatcher();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	void setTitle(const char* title, int fontsize = 20);
	void setContentText(const char* text, int fontsize = 20,int padding = 50, int paddingTop = 100);

	void setCallBackFunc(CCObject* targer, SEL_CallFuncN callfun);

	bool addButton(const char* normalImage,const char* seletedImage ,const char* title, int tag=0);
	virtual void onEnter();
	virtual void onExit();
private:
	void buttonCallback(CCObject* pSender);
	int m_contentPadding;
	int m_contentPaddingTop;

	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;

	//内存管理相关
	CC_SYNTHESIZE_RETAIN(CCMenu*, m__pMenu,MenuButton);
	CC_SYNTHESIZE_RETAIN(CCSprite*, m__sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m__s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m__ltContentText, LabelContentText);
};

