---
layout: post
title: "一步一步CocosBuilder(3)"
date: 2012-12-28 00:54
comments: true
categories: [Cocos2d-x, CocosBuilder] 
---

今天要说的内容比较轻松, 重要的知识点不多.   
目标是实现`Menus & Items`, `Sprites & 9 Slice`, `Buttons & Labels`.   

#####1
首先我们在CocosBuilder中建立一个新的`Header.ccb`.具体请参见CocosBuilder的项目.     
在创建的时候,勾选`Full Sceen`, 并且将分辨率的`Height`都修改为40.   
这个文件很简单, 根节点下只包含一个`CCLayerColor`,根节点的`Custom class`设置为`HeaderLayer`.   
`CCLayerColor`下面包含一个`CCLabelTTF`和`CCMenu`.   
`CCLabelTTF`的变量绑定为类型`Owner var`, 值为`mTitleLabelTTF`.   
`CCMenu`包含一个`CCMenuItemImage`. `CCMenuItemImage`的`Selector`属性设置为`onBackClicked`.      
这个ccb作为一个公用的元素被其他ccb文件所引用. 是属于模块化的一个例子.   
然后我们在Xcode中新建一个`HeaderLayer`类, 并同时声明一个`HeaderLayerLoader`类.   

<!--more-->

``` cpp HeaderLayer.h
class HeaderLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(HeaderLayer, create);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

    void onBackClicked(CCObject *pSender);

};

class HeaderLayerLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(HeaderLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(HeaderLayer);
};
```
``` cpp HeaderLayer.cpp
SEL_MenuHandler HeaderLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBackClicked", HeaderLayer::onBackClicked);
    
    return NULL;
}

SEL_CCControlHandler HeaderLayer::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}

void HeaderLayer::onBackClicked(cocos2d::CCObject *pSender)
{
    CCDirector::sharedDirector()->popScene();
}
```
#####2
在CocosBuilder中新建一个`Menus.ccb`, 并将根节点`Custom class`设置为`MenuLayer`.   
再加入几个`CCMenuItemImage`, Selector分别设置为`onPressA`, `onPressB`, `onPressC`.   
再加入一个`CCLabelBMFont`, 绑定`Doc root var`, 值为`mMessageLabelBMFont`.   
在Xcode中新建一个类`MenuLayer`, 并且在`MenuLayer.h`中同时声明一个`MenuLayerLoader`类.   
``` cpp MenuLayer.h
class MenuLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
{
public:
    MenuLayer();
    ~MenuLayer();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MenuLayer, create);

    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

    void onPressA(cocos2d::CCObject *pSender);
    void onPressB(cocos2d::CCObject *pSender);
    void onPressC(cocos2d::CCObject *pSender);

private:
    cocos2d::CCLabelBMFont *mMessageLabelBMFont;
};

class MenuLayerLoader : public cocos2d::extension::CCNodeLoaderLibrary
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MenuLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MenuLayer);    
};
```
``` cpp MenuLayer.cpp
MenuLayer::MenuLayer()
: mMessageLabelBMFont(NULL)
{}

MenuLayer::~MenuLayer()
{
    CC_SAFE_DELETE(mMessageLabelBMFont);
}

bool MenuLayer::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMessageLabelBMFont", CCLabelBMFont*, this->mMessageLabelBMFont);
    return false;
}

SEL_MenuHandler MenuLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPressA", MenuLayer::onPressA);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPressB", MenuLayer::onPressB);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPressC_NO_Match", MenuLayer::onPressC);
   //注意这里, 我们给第三个绑定了一个不存在的SelectorName. 
    return NULL;
}

SEL_CCControlHandler MenuLayer::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}

void MenuLayer::onPressA(cocos2d::CCObject *pSender)
{
    this->mMessageLabelBMFont->setString("A pressed.");
}

void MenuLayer::onPressB(cocos2d::CCObject *pSender)
{
    this->mMessageLabelBMFont->setString("B pressed.");
}

void MenuLayer::onPressC(cocos2d::CCObject *pSender)
{
    this->mMessageLabelBMFont->setString("C pressed.");
}
```
#####3
或许有点着急了吧, 我们如何让这个运行起来呢?    
首先给`MainScene`增加一个新函数的声明及定义, 还需要增加一个变量用于和`Header.ccb`中的`mTitleLabelTTF`绑定.   
``` cpp MainScene.h
public:
    void openTest(const char * pCCBFileName, const char * pCCNodeName = NULL, cocos2d::extension::CCNodeLoader * pCCNodeLoader = NULL);
private:
    cocos2d::CCLabelTTF *mTitleLabelTTF;
```
``` cpp MainScene.cpp
void MainScene::openTest(const char * pCCBFileName, const char * pCCNodeName, cocos2d::extension::CCNodeLoader * pCCNodeLoader)
{
    CCScene *scene = CCScene::create();
    
    CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    lib->registerCCNodeLoader("HeaderLayer", HeaderLayerLoader::loader());
    lib->registerCCNodeLoader(pCCNodeName, pCCNodeLoader);
    
    CCBReader *reader = new CCBReader(lib);
    CCNode *node = reader->readNodeGraphFromFile(pCCBFileName, this);
    reader->autorelease();
    
    if (node != NULL)
    {
        scene->addChild(node);
    }
    
    this->mTitleLabelTTF->setString(pCCBFileName);
    
    CCDirector::sharedDirector()->pushScene(scene);
}

bool MainScene::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLabelText", CCLabelBMFont*, this->mLabelText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTitleLabelTTF", CCLabelTTF*, this->mTitleLabelTTF);
    
    return false;
}
void MainScene::onButtonTest(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->openTest("ccb/Menus.ccbi", "MenuLayer", MenuLayerLoader::loader());
}
```
#####4
`Sprites & 9 Slice` 这个比较简单, 只是靠CocosBuilder就能完成任务了.   

{% img /upload/2012-12/ccb-sbs-3-001.png %}

我拼的不太好, 嘿嘿. 主要是着重代码, 并非设计, 漂亮的事情交给美术妹纸吧.   

#####5
`Buttons`这块主要是研究了`CCControlEvent`的各种情况. 我们只贴出核心代码.   
``` cpp ButtonsLayer.cpp
void ButtonsLayer::onBtnClicked(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    switch (pCCControlEvent) {
        case CCControlEventTouchCancel:
            this->mEventLabelBMFont->setString("Cancel");
            break;
        case CCControlEventTouchDown:
            this->mEventLabelBMFont->setString("Touch Down");
            break;
        case CCControlEventTouchDragEnter:
            this->mEventLabelBMFont->setString("Drag Enter");
            break;
        case CCControlEventTouchDragExit:
            this->mEventLabelBMFont->setString("Drag Exit");
            break;
        case CCControlEventTouchDragInside:
            this->mEventLabelBMFont->setString("Drag Inside");
            break;
        case CCControlEventTouchDragOutside:
            this->mEventLabelBMFont->setString("Drag OutSide");
            break;
        case CCControlEventTouchUpInside:
            this->mEventLabelBMFont->setString("Up Inside");
            break;
        case CCControlEventTouchUpOutside:
            this->mEventLabelBMFont->setString("Up Outside");
            break;
        default://所有的定义都在上面了,TestCPP里面有句经典的点评, 想看的可以去翻一下.
            break;
    }
}
```

{% img /upload/2012-12/ccb-sbs-3-002.png %}




























{% render_partial _posts/cocosbuilder-step-by-step-footer.markdown %}








































