#include "lianxi.h"
#include "UTF8ToGBK.h"
#include "WallScene.h"
#include "Tianzige.h"

#define tianzige_draw_tag 1001

lianxi::lianxi(){


}

lianxi::~lianxi(){

}

bool lianxi::init(){
    if (!CCLayerColor::initWithColor(ccc4(108,108,108,255)))
    {
	return false;
    }
    CCLog("lianxi init()");
    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    return true;
}

void lianxi::onEnter(){
    CCLog("onEnter linxian");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize(); 
    CCSize visiableSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSize originSize = CCDirector::sharedDirector()->getVisibleOrigin();

    //add wall-head
    CCSprite* wall_head = CCSprite::create("wall_head.png");
    this->addChild(wall_head,2);
    CCSize headSize = wall_head->getContentSize();
    wall_head->setPosition(ccp(visiableSize.width/2,visiableSize.height-wall_head->getContentSize().height/2));
    wall_head->setScaleX(visiableSize.width/wall_head->getContentSize().width);

    //add ceshi tail on the bottom
    CCSprite* wall_tail = CCSprite::create("wall_tail.png");
    this->addChild(wall_tail,2);
    CCSize tailSize = wall_tail->getContentSize();
    wall_tail->setPosition(ccp(visiableSize.width/2,wall_tail->getContentSize().height/2));
    wall_tail->setScaleX(visiableSize.width/wall_tail->getContentSize().width);

    //add tianzige
    CCSprite* tianzige = CCSprite::create("tianzige.png");
    this->addChild(tianzige,1);
    CCSize tianzigeSize = tianzige->getContentSize();
    tianzige->setPosition(ccp(visiableSize.width/2,visiableSize.height-headSize.height-tianzigeSize.height/2-50));
    

    CCSprite* tianzige_draw = CCSprite::create("tianzige.png");
    this->addChild(tianzige_draw,1,tianzige_draw_tag);
    CCSize tianzigeDrawSize = tianzige_draw->getContentSize();
    tianzige_draw->setPosition(ccp(visiableSize.width/2,tailSize.height+50+tianzigeDrawSize.height/2));
    
    CCSprite* bg = CCSprite::create("lianxi_background.png");
    this->addChild(bg,0);
    bg->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));
    bg->setScaleX(visiableSize.width/bg->getContentSize().width);
    bg->setScaleY((visiableSize.height-headSize.height-tailSize.height)/bg->getContentSize().height);

//     CCSprite* bg_top = CCSprite::create("lianxi_top.png");
//     this->addChild(bg_top,0);
//     bg_top->setPosition(ccp(visiableSize.width/2,-headSize.height+visiableSize.height-bg_top->getContentSize().height/2));
//     bg_top->setScaleX(visiableSize.width/bg_top->getContentSize().width);
//     float scale_y = visiableSize.height/(visiableSize.height-headSize.height*2);
//     bg_top->setScaleY(scale_y);
// 
//     CCSprite* bg_bottom = CCSprite::create("lianxi_bottom.png");
//     this->addChild(bg_bottom,0);
//     bg_bottom->setPosition(ccp(visiableSize.width/2,tailSize.height+bg_bottom->getContentSize().height/2));
//     bg_bottom->setScale(visiableSize.width/bg_bottom->getContentSize().width);
//     bg_bottom->setScaleY(scale_y);
// 
//     CCSprite* bg_separ = CCSprite::create("lianxi_separate.png");
//     this->addChild(bg_separ,0);
//     bg_separ->setPosition(ccp(visiableSize.width/2,visiableSize.height/2));
//     bg_separ->setScaleX(visiableSize.width/bg_separ->getContentSize().width);

    CCLabelTTF* back = CCLabelTTF::create("back","Arial",25);
    CCMenuItemLabel* menuLabel = CCMenuItemLabel::create(back,this,menu_selector(lianxi::menuBack));
    CCMenu* menu = CCMenu::create(menuLabel,NULL);
    this->addChild(menu,20);
    menuLabel->setPosition(ccp(back->getContentSize().width/2+100,back->getContentSize().height+30));
    menu->setPosition(CCPointZero);
    menu->registerWithTouchDispatcher();

    CCLog(hanzi.c_str());
    //luanma
    CCLabelTTF* pHanzi = CCLabelTTF::create(UTF8ToGBK::UTF8TOGBK(hanzi).c_str(),"Arial", 100);
    this->addChild(pHanzi,10);
    pHanzi->setPosition(tianzige->getPosition());

}


CCScene* lianxi::scene(string hanzi){
    CCScene *scene = CCScene::create();
    lianxi *layer = lianxi::create();
    layer->hanzi = hanzi;
    scene->addChild(layer);
    return scene;
}

bool  lianxi::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    prePoint = pTouch->getLocation();
    return true;
}
void  lianxi::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){

}
void  lianxi::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    
    CCSprite* tianzige = (CCSprite*)this->getChildByTag(tianzige_draw_tag);
    CCRect rect = tianzige->boundingBox();
    if (rect.containsPoint(location))
    {
// 	CCDrawNode * dot = CCDrawNode::create();
// 	dot->drawDot(location,5.0,ccc4f(188, 188, 188, 120));
// 	//this->addChild(dot,10);
// 	tianzige->addChild(dot,10);
	CCDrawNode* node = CCDrawNode::create();
	node->drawSegment(prePoint,location,1.0,ccc4f(180,180,180,100));
	prePoint = location;
    }
}

void lianxi::keyBackClicked(){
    CCLog("BBBB");
    CCDirector::sharedDirector()->replaceScene(WallScene::scene());
}

void lianxi::menuBack(CCObject* pSender){
    CCDirector::sharedDirector()->replaceScene(WallScene::scene());
}

void lianxi::draw(){
//     CCLog("Lianxi Draw()");
//     ccDrawLine(ccp(0,0),ccp(200,200));
}