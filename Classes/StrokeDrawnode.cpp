#include "StrokeDrawnode.h"

StrokeDrawnode::StrokeDrawnode()
{
}

//************************************
// Method:    StrokeDrawnode
// FullName:  StrokeDrawnode::StrokeDrawnode
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: Stroke stro 传入一笔，构造一笔Node
//************************************
StrokeDrawnode::StrokeDrawnode(Stroke stro){
	this->stroke = stro;
}

void StrokeDrawnode::onEnter(){
	CCNode::onEnter();
}

void StrokeDrawnode::onExit(){
	CCNode::onExit();
}

StrokeDrawnode* StrokeDrawnode::create(Stroke stro){
	StrokeDrawnode* pRet = new StrokeDrawnode(stro);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

StrokeDrawnode::~StrokeDrawnode()
{

}

void StrokeDrawnode::draw(){
	//CCLog("StrokeDrawnode::draw");
	//在这里设定笔画的粗细和颜色。 具体要查看
	glLineWidth(6.0f);					//笔画粗细
	ccDrawColor4F(0,0,0,1);				//笔画颜色
// 	glEnable(GL_LINE_SMOOTH);
	CCPoint pre = stroke.pointList[0];					//直接拿了pointList第一个值
	for (int i = 1; i< stroke.pointCount; i++)
	{
		ccDrawLine(pre,stroke.pointList[i]);
		pre = stroke.pointList[i];
	}
// 	glDisable(GL_LINE_SMOOTH);
}

void StrokeDrawnode::addPoint(CCPoint point){
	this->stroke.addPoint(point);
}