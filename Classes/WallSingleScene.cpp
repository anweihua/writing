#include "WallSingleScene.h"
#include "tinyxml.h" 
#include "PopLayer.h"
#include "tools/DataTool.h"
#include "LianxiScene.h"
#include "MainScene.h"
#include "MyToast.h"
#include "CeshiScene.h"

USING_NS_CC;

#define TAG_LAYER_EXIT 1001

//////////////////////////////////////////
CCScene* WallSingleScene::scene(string filename)
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	WallSingleScene *layer = WallSingleScene::create(filename);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

WallSingleScene* WallSingleScene::create(string wallxmlname){
	WallSingleScene* pret = new WallSingleScene();
	if (pret && pret->init(wallxmlname))
	{
		pret->autorelease();
		return pret;
	}else{
		delete pret;
		pret = NULL;
		return pret;
	}
}

// on "init" you need to initialize your instance
bool WallSingleScene::init(string xmlfilename)
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	wallXMLCurrent = xmlfilename;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	isMoved = false;
	touched = false;

	CCLog("WallSingleScene:init()");
	CCSprite* wall_tail = CCSprite::create("wall_tail.png");
	this->addChild(wall_tail,2);
	CCSize tailSize = wall_tail->getContentSize();
	wall_tail->setPosition(ccp(visibleSize.width/2,wall_tail->getContentSize().height/2));
	wall_tail->setScaleX(visibleSize.width/wall_tail->getContentSize().width);


	CCMenuItemImage* ceshi_button = CCMenuItemImage::create("ceshi_1.png",
		"ceshi_2.png",
		this,
		menu_selector(WallSingleScene::ceshi));
	CCMenu* menu = CCMenu::create(ceshi_button,NULL);
	this->addChild(menu,20);
	ceshi_button->setPosition(ccp(visibleSize.width - ceshi_button->getContentSize().width/2 ,ceshi_button->getContentSize().height/2));
	menu->setPosition(CCPointZero);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+wallXMLCurrent;
	unsigned long size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(myfilename.c_str(),"r",&size);
	TiXmlDocument* myDocument = new TiXmlDocument();
	myDocument->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string str_filename = "wall/" + wallXMLCurrent;
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(str_filename.c_str());
	TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());
	myDocument->LoadFile();
#endif

	TiXmlElement* rootElement = myDocument->RootElement();  // Class
	TiXmlElement* metaElement = rootElement->FirstChildElement();  // meta   
	TiXmlElement* heightElement = metaElement->FirstChildElement(); //���meta��heightԪ��
	string mheight=heightElement->GetText();
	int height=atoi(mheight.c_str());
	TiXmlElement* widthElement = heightElement->NextSiblingElement(); //���meta��widthԪ��
	string mwidth=widthElement->GetText(); 
	int width=atoi(mwidth.c_str());

	rescale=(visibleSize.height - wall_tail->getContentSize().height)/(float)height;
	float width_rescale = visibleSize.width/(float)width;
	//////////////
	TiXmlElement* dataElement = metaElement->NextSiblingElement();  // data
	TiXmlElement* stoneElement = dataElement->FirstChildElement();  // stone

	hanzilist.clear();
	while (stoneElement) {
		CHanziManage temphanziManage;
		TiXmlElement* typeElement = stoneElement->FirstChildElement();  // type

		string flag1("wordbox");
		string flag2("nouse");
		string flag3(typeElement->GetText());
		if(flag3==flag1)
		{
			//wordbox
			TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
			TiXmlElement* yElement = xElement->NextSiblingElement();  // y
			TiXmlElement* wElement = yElement->NextSiblingElement();  // w
			TiXmlElement* hElement = wElement->NextSiblingElement();  // h
			TiXmlElement* imgElement = hElement->NextSiblingElement();  // img
			TiXmlElement* hanziElement = imgElement->NextSiblingElement();  // hanzi
			TiXmlElement* proficiencyElement = hanziElement->NextSiblingElement();  // proficiency

			string tempx=xElement->GetText();
			int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
			string tempy=yElement->GetText();
			int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
			string tempw=wElement->GetText();
			int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
			string temph=hElement->GetText();
			int h=atoi((temph.substr(0, temph.size()-2)).c_str());

			//����ϵ�ض�λ
			x=x+w/2;
			y=height-y-h/2;

			//���ţ�
			x*=width_rescale;
			y*=rescale;
			w*=width_rescale;
			h*=rescale;
			y += wall_tail->getContentSize().height;

			string tempfilename=imgElement->GetText();
			string temphanzi=hanziElement->GetText();
			//CCLog("temphanzi %s",temphanzi.c_str());
			//			string GBKhanzi = UTF8ToGBK::UTF8TOGBK(temphanzi);
			string temppro=proficiencyElement->GetText();

			//stone sprite
			//CCLog("tempfilename %s",tempfilename.c_str());
			CCSprite* pSprite1 = CCSprite::create(tempfilename.c_str());
			pSprite1->setScaleY(rescale);
			pSprite1->setScaleX(width_rescale);
			pSprite1->setPosition(ccp(origin.x+x, origin.y+y));
			this->addChild(pSprite1, 1);

			//�ı���
			hanzis.push_back(temphanzi);

			CCLabelTTF* pLabel = CCLabelTTF::create(temphanzi.c_str(), "XingShu", 100);
			pLabel->setPosition(ccp(origin.x + x, origin.y + y));
			this->addChild(pLabel, 2);

			//���ֹ���
			temphanziManage.character=hanziElement->GetText();
			temphanziManage.textbox=pLabel;
			temphanziManage.pos=CCPoint(x,y);
			temphanziManage.filename=tempfilename;
			temphanziManage.proficiency=atoi(temppro.c_str());

			vector<CHanziManage>::iterator it = hanzilist.end(); 
			hanzilist.insert(it,temphanziManage);
			stoneElement=stoneElement->NextSiblingElement();

		}
		if(flag3==flag2)
		{
			//nouse
			TiXmlElement* xElement = typeElement->NextSiblingElement();  // x
			TiXmlElement* yElement = xElement->NextSiblingElement();  // y
			TiXmlElement* wElement = yElement->NextSiblingElement();  // w
			TiXmlElement* hElement = wElement->NextSiblingElement();  // h
			TiXmlElement* imgElement = hElement->NextSiblingElement();  // img

			string tempx=xElement->GetText();
			int x=atoi((tempx.substr(0,tempx.size()-2)).c_str());
			string tempy=yElement->GetText();
			int y=atoi((tempy.substr(0, tempy.size()-2)).c_str());
			string tempw=wElement->GetText();
			int w=atoi((tempw.substr(0,tempw.size()-2)).c_str());
			string temph=hElement->GetText();
			int h=atoi((temph.substr(0, temph.size()-2)).c_str());

			//����ϵ�ض�λ
			x=x+w/2;
			y=height-y-h/2;

			//���ţ�
			x*=width_rescale;
			y*=rescale;
			w*=width_rescale;
			h*=rescale;
			y += wall_tail->getContentSize().height;

			//stone sprite
			string tempfilename=imgElement->GetText();
			CCSprite* pSprite2 = CCSprite::create(tempfilename.c_str());
			pSprite2->setScaleY(rescale);
			pSprite2->setScaleX(width_rescale);
			pSprite2->setPosition(ccp(origin.x+x, origin.y+y));
			this->addChild(pSprite2, 1);
			/////////////
			stoneElement=stoneElement->NextSiblingElement();
		}
	}

	////////////////////////////////////////////////////////////////////////////////
	CCSprite* pSprite = CCSprite::create("wall.JPG");
	CCSize spriteSize = pSprite->getContentSize();
	pSprite->setPosition(ccp(spriteSize.width/2,spriteSize.height/2));
	pSprite->setScale(15);
	this->addChild(pSprite, 0);

	//ע�ᴥ���¼�
	CCPoint changepoint=ccp(0,0);
	touched=false;
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);			//android back key
	//ԭ�����û������register�Ǹ���������Ҫ����������������֮һ
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	return true;
}

void WallSingleScene::onEnter(){
	CCLayer::onEnter();

}

void WallSingleScene::onExit(){
	CCLayer::onExit();
}

// bool  WallScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
// {
// 	popup();
// 	touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
// 	touched=true;
// 	beginTime = millisecondNow();
// 	return true;
// }
// void  WallScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
// {
// 
// 	long endTime = millisecondNow();
// 	if (endTime-beginTime < 250 && isMoved==false)
// 	{
// 		//single click
// 		CCPoint touchpoint = pTouch->getLocation();
// 		//CCLog("Touchpoint %f %f",touchpoint.x,touchpoint.y);
// 		for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
// 		{
// 			CCPoint hanziPos = iter->pos;
// 			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
// 			CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
// 			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
// 			CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);
// 
// 			if (rect.containsPoint(touchpoint))
// 			{
// 				CCLog(iter->character.c_str());
// 				this->singleClick(iter->character);
// 				return;
// 			}
// 		}
// 	}
// 
// 	if (endTime-beginTime > 3000)
// 	{
// 		popup();
// 	}
// 	touched=false;
// 	isMoved = false;
// }
// void  WallScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
// {
// 	isMoved = true;
// 	CCPoint newpos=ccp(pTouch->getLocation().x , pTouch->getLocation().y);
// 	CCPoint temppoint=ccp(newpos.x-touchbeginpoint.x, newpos.y-touchbeginpoint.y);
// 	changepoint =ccp(changepoint.x+temppoint.x, changepoint.y+temppoint.y);
// 	//CCLog("%f %f",changepoint.x,changepoint.y);
// 	////////////////
// 	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
// 
// 	if(changepoint.y!=0)
// 		changepoint.y=0;
// 
// 	if(changepoint.x>=0)
// 		changepoint.x=0;
// 
// 	if (changepoint.x<=-3000*rescale+visibleSize.width)
// 		changepoint.x=-3000*rescale+visibleSize.width;
// 	////////////////
// 	this->setPosition(changepoint);
// 	touchbeginpoint = newpos;
// }

void WallSingleScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCLog("Touches Began~~~");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();

	touchbeginpoint = ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	prePoint = touchbeginpoint;
	touched=true;

	beginTime = millisecondNow();
	//��ʱ��,ֱ��ʹ��scheduleUpdate��Ч
	//this->scheduleUpdate();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this,1.5f,false);
	for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
	{
		CCPoint hanziPos = iter->pos;
		//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
		CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
		//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
		CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);

		if (rect.containsPoint(touchbeginpoint))
		{
			selectedHanzi = iter->character;
			selectedCHanziManageIter = iter;
		}
	}

}
void WallSingleScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	CCLog("ccTouchesMoved");
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	if (ccpDistance(prePoint,pTouch->getLocation()) > 50)
	{
		isMoved = true;
	}
	CCPoint newpos=ccp(pTouch->getLocation().x , pTouch->getLocation().y);
	CCPoint temppoint=ccp(newpos.x-touchbeginpoint.x, newpos.y-touchbeginpoint.y);
	changepoint =ccp(changepoint.x+temppoint.x, changepoint.y+temppoint.y);
	//CCLog("%f %f",changepoint.x,changepoint.y);
	////////////////
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	if(changepoint.y!=0)
		changepoint.y=0;

	if(changepoint.x>=0)
		changepoint.x=0;

	if (changepoint.x<=-900*rescale+visibleSize.width)
		changepoint.x=-900*rescale+visibleSize.width;
	////////////////
	//this->setPosition(changepoint);
	touchbeginpoint = newpos;
}

void WallSingleScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent){
	CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
	long endTime = millisecondNow();
	float length = ccpDistance(prePoint,pTouch->getLocation());
// 	CCLog("length:%f  %f",length,endTime-beginTime);

	if (endTime-beginTime < 1000 && length <= 50)
	{
		//single click
		CCPoint touchpoint = pTouch->getLocation();
		//CCLog("Touchpoint %f %f",touchpoint.x,touchpoint.y);
		for (vector<CHanziManage>::iterator iter = hanzilist.begin();iter!=hanzilist.end();++iter)
		{
			CCPoint hanziPos = iter->pos;
			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
			CCPoint realPos = ccp(hanziPos.x+changepoint.x,hanziPos.y+changepoint.y);
			//CCLog("hanziPos %f %f",hanziPos.x,hanziPos.y);
			CCRect rect = CCRectMake(realPos.x-100,realPos.y-100,200,200);
			//������Χ200�����ڣ��ж�Ϊ����
			if (rect.containsPoint(touchpoint))
			{
				CCLog(iter->character.c_str());
				this->singleClick(iter->character);
				return;
			}
		}
	}

	touched=false;
	isMoved = false;
	selectedHanzi = "";

	//�����ʱ��
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this);

}

void WallSingleScene::registerWithTouchDispatcher(){
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,1);
}

void WallSingleScene::update(float delta){
	CCLog("update");
}



void WallSingleScene::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
	////
	//this->setTouchEnabled(false);
	//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	////
	// #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// 	exit(0);
	// #endif
}

bool WallSingleScene::isInSprite(CCTouch* pTouch){
	// ���ص�ǰ����λ����OpenGL���� 
	CCPoint touchPoint=pTouch->getLocation();
	// ����������ת��Ϊ��ǰ��View�ı�������ϵ

	CCPoint reallyPoint=this->getParent()->convertToNodeSpace(touchPoint);
	// ��ȡ��ǰ���ڸ�view������ϵ

	CCRect rect=this->boundingBox();
	// CCnode->convertToNodeSpace ����  convertToWorldSpace �ǻ��ڵ�ǰNode��  �뵱ǰNode���

	if(rect.containsPoint(reallyPoint)){
		return true;
	}
	return false;
}

/************************************************************************/
/* string hanzi ����lianxi������д                                                                      */
/************************************************************************/
void WallSingleScene::singleClick(string hanzi){
	//���schedule,��Ȼ���ܳ��ֲ���Ԥ�����⡣
	
	if (hanzi != "a")
	{
		this->unscheduleAllSelectors();
		CCDirector::sharedDirector()->getTouchDispatcher()->removeAllDelegates();
		CCDirector::sharedDirector()->pushScene(LianxiScene::create(hanzis,hanzi));
	}

}

void WallSingleScene::popup(string hanzi){
// 	CCLog("popup wall");
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	popL = PopLayer::create(hanzi,"pop/background.png");
	popL->setContentSize(CCSizeMake(winSize.width*0.75,winSize.height*0.75));
	popL->setTitle("test");
	popL->setEditBox();
	popL->setCallBackFunc(this,callfuncN_selector(WallSingleScene::buttonCallBack));
	popL->addButton("sure_up.png","sure_down.png","Y",0);
	popL->addButton("cancer_up.png","cancer_down.png","N",1);
	CCDirector::sharedDirector()->getRunningScene()->addChild(popL,100);
	//�����ʱ��
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this);
}

void WallSingleScene::buttonCallBack(CCNode* pNode){
	CCLog("button call back. tag: %d", pNode->getTag());
	if (pNode->getTag() == 0)
	{
		//�����Ի���ȷ�ϣ�������д����Ӧλ��
		//const char* h = popL->getHanzi();
		for (vector<CHanziManage>::iterator iter = hanzilist.begin(); iter != hanzilist.end(); ++iter)
		{
			if (iter == selectedCHanziManageIter)
			{
				// ���Ķ��ĺ���д�뵽xml�ļ���saveToFile()
				const char* h = popL->getHanzi();
				//				string dst(h,strlen(h)+1);
				saveToFile(iter->character, h);

				iter->character = string(popL->getHanzi());
				CCLabelTTF* t = (CCLabelTTF*)iter->textbox;
				t->setString(popL->getHanzi());
			}
		}


	}else
	{
		//�����Ի���ȡ����ʲô������
	}
}

void WallSingleScene::longPressUpdate(float fDelta){
	CCLog("Update %f",fDelta);

	if (isMoved == false && selectedHanzi.length() > 0)
	{
		popup(selectedHanzi);
	}
	//�����ʱ��
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(WallSingleScene::longPressUpdate),this);
}

/**
	src ԭ��
	dst Ŀ����
	��ԭ���滻��Ŀ���֣��滻xml�ļ��в��ҵ��ĵ�һ����
*/
void WallSingleScene::saveToFile(string src,const char* dst){
	int i = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string wallpath = "wall/" + wallXMLCurrent;
	string myfilename=CCFileUtils::sharedFileUtils()->fullPathForFilename(wallpath.c_str());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string myfilename = CCFileUtils::sharedFileUtils()->getWritablePath()+wallXMLCurrent;
#endif
	TiXmlDocument* myDocument = new TiXmlDocument(myfilename.c_str());
	myDocument->LoadFile();

	TiXmlElement* rootEle = myDocument->RootElement();
	TiXmlElement* meta = rootEle->FirstChildElement();
	TiXmlElement* data = meta->NextSiblingElement();
	TiXmlElement* stone = data->FirstChildElement();
	while (stone)
	{

		TiXmlElement* type = stone->FirstChildElement();
		string text(type->GetText());
		string wordbox("wordbox");
		if (text == wordbox)
		{
			TiXmlElement* img = stone->FirstChildElement("img");
			TiXmlElement* hanzi = stone->FirstChildElement("hanzi");
			string text(hanzi->GetText());
			if (text == src)
			{
				TiXmlElement* new_hanzi = new TiXmlElement("hanzi");
				// 				string dst_str = GBKToUTF8(dst);
				// 				string dst_ss = DataTool::GB2312ToUTF8(dst);
				// 				CCLog("DST XXXX%s XXXX%s",dst.c_str(),dst_str.c_str());
				// 				const char* temp = dst.c_str();
				TiXmlText* newText = new TiXmlText(dst);
				new_hanzi->InsertEndChild(*newText);
				stone->RemoveChild(hanzi);
				stone->InsertAfterChild(img,*new_hanzi);
				break;
			}
		}
		stone = stone->NextSiblingElement();
	}
	bool ret = myDocument->SaveFile(myfilename.c_str());
}




void WallSingleScene::keyBackClicked(){
	CCLog("WallSingleScene::keyBackClicked");
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByTag(TAG_LAYER_EXIT) == NULL) {
		CCLog("WallSingleScene::NULL");
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		PopLayer* exitDialog = PopLayer::create("pop/background.png");
		exitDialog->setContentSize(CCSizeMake(winSize.width*0.8,winSize.height*0.5));
		exitDialog->setTitle("back",50);
		exitDialog->setContentText("back",60,100,150);
		exitDialog->setCallBackFunc(this,callfuncN_selector(WallSingleScene::backtoMainScene));
		exitDialog->addButton("sure_up.png","sure_down.png","Y",0);
		exitDialog->addButton("cancer_up.png","cancer_down.png","N",1);
		CCDirector::sharedDirector()->getRunningScene()->addChild(exitDialog,100,TAG_LAYER_EXIT);
	}
}

void WallSingleScene::backtoMainScene(CCNode* pNode){
	if (pNode->getTag() == 0)
	{
		CCDirector::sharedDirector()->replaceScene(MainScene::scene());
	}else{
		CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(TAG_LAYER_EXIT);
	}
}

void WallSingleScene::ceshi(CCObject* pSender){
	CCLog("WallSingleScene::ceshi clicked");
// 	MyToast::showToast(this,DataTool::getChinese("stroke_wrong"),2);

	CeshiScene* scene = CeshiScene::create(wallXMLCurrent,hanzis);
	CCDirector::sharedDirector()->replaceScene(scene);
}