---
layout: post
title: "一步一步CocosBuilder(4)[完结]"
date: 2012-12-29T10:54:00+08:00
comments: true
categories: [Cocos2d-x, CocosBuilder] 
draft: true
---

这一节也是很轻松, 主要是牵涉到Timeline的切换. 其他的都是简单的界面制作.   
所以CocosBuilder的威力也体现在这里, 很少的代码就能驱动界面了.   

我们这次先做`Particle System`, `Scroll View`, 最后以`Animations`收官.   

#####1
制作`Particle System`和`Scroll View`的界面很简单,我们也是以程序为重点, 所以不再赘述.   
只需要一句`this->openTest("ccb/Scroll.ccbi");`就可以调用新的界面了.   
其他诸如绑定之类相信大家已经是轻车熟路啦.    

<!--more-->

{% img /upload/2012-12/ccb-sbs-4-001.png %}
{% img /upload/2012-12/ccb-sbs-4-002.png %}


#####2
好吧, 我承认一点, 制作`Animations`的界面的时候, 我直接复制了例子中的节点.   
5个Timeline, 还是很复杂的, 应该是一个非常精细的工作. 向美术们致敬.   
``` cpp AnimationsLayer.h
class AnimationsLayer
: public cocos2d::CCLayer
, public cocos2d::extension::CCBSelectorResolver
{
public:
    AnimationsLayer();
    ~AnimationsLayer();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(AnimationsLayer, create);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);    
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);
    
    void onIdle(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onWave(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onJump(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onFunky(cocos2d::CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
private:
    cocos2d::extension::CCBAnimationManager *mAnimationManager;
};

class AnimationsLayerLoader : public cocos2d::extension::CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AnimationsLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(AnimationsLayer);
};
``` 
``` cpp AnimationsLayer.cpp
AnimationsLayer::AnimationsLayer()
: mAnimationManager(NULL)
{}

AnimationsLayer::~AnimationsLayer()
{
    CC_SAFE_RELEASE_NULL(mAnimationManager);
}

SEL_MenuHandler AnimationsLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler AnimationsLayer::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onIdle", AnimationsLayer::onIdle);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onJump", AnimationsLayer::onJump);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWave", AnimationsLayer::onWave);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onFunky", AnimationsLayer::onFunky);
    
    return NULL;
}

void AnimationsLayer::setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager)
{
    CC_SAFE_RELEASE_NULL(mAnimationManager);
    mAnimationManager = pAnimationManager;
    CC_SAFE_RETAIN(mAnimationManager);
}

void AnimationsLayer::onIdle(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Idle", 0.3f);
}

void AnimationsLayer::onJump(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Jump", 0.3f);
}

void AnimationsLayer::onWave(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Wave", 0.3f);
}

void AnimationsLayer::onFunky(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Funky", 0.3f);
}
```
整个代码还是很简单. 和以前的几个例子稍微有点不同的是在调用的时候, 需要把`CCBReader`里面的`AnimationManger`传入.   
好, 还是看下调用的代码吧.   
``` cpp MainScene.cpp
void MainScene::onAnimations(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    const char * pCCBFileName = "ccb/Animations.ccbi";
    const char * pCCNodeName = "AnimationsLayer";
    cocos2d::extension::CCNodeLoader * pCCNodeLoader = AnimationsLayerLoader::loader();
    
    CCScene *scene = CCScene::create();
    
    CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    lib->registerCCNodeLoader("HeaderLayer", HeaderLayerLoader::loader());
    
    if (pCCNodeName!=NULL && pCCNodeLoader!=NULL)
    {
        lib->registerCCNodeLoader(pCCNodeName, pCCNodeLoader);
    }
    
    CCBReader *reader = new CCBReader(lib);
    CCNode *node = reader->readNodeGraphFromFile(pCCBFileName, this);
    reader->autorelease();
    
    if (node != NULL)
    {
        //获取AnimationManger 并传给AnimationsLayer
        ((AnimationsLayer*)node) ->setAnimationManager(reader->getAnimationManager());
        
        scene->addChild(node);
    }
    
    this->mTitleLabelTTF->setString(pCCBFileName);
    
    CCDirector::sharedDirector()->pushScene(scene);
}
```

{% img /upload/2012-12/ccb-sbs-4-003.png %}


###后记
看到微博上@做游戏的老G 吐槽说CocosBuilder如何不好用, 学习难度比较大. 我也是很认可这一点.   
但是只要是坚持把TestCpp例子完全读完了, 相信掌握CocosBuilder也是很快能够做到的.   
既然你看完了这个教程. 相信你也已经能够很好地驾驭CocosBuilder. 关键还在实践呦!    

微博上@Ghost_233 说了一个很好的思路, 直接从ccb文件中生成对应的C++类, 而且他还做了一些实践.   
我暂时还没有研究他的代码, 不过肯定是可行. 这是一个很赞的思路.   
希望大家能动手, 最好合作出来一个较好的工具来, 我想, 是不是集成到CocosBuilder会更好?   

谢谢大家耐心看完我的文章.   








<hr />
代码打包下载:<a href="http://pan.baidu.com/share/link?shareid=158075&uk=2281999082" target="_blank">下载点</a> 

- 本文是系列文章, 请查看其他文章:
- [一步一步CocosBuilder(1)](http://young40.github.com/blog/2012/12/21/cocosbuilder-step-by-step-part-one/)
- [一步一步CocosBuilder(2)](http://young40.github.com/blog/2012/12/25/cocosbuilder-step-by-step-part-two/)
- [一步一步CocosBuilder(3)](http://young40.github.com/blog/2012/12/28/cocosbuilder-step-by-step-part-three/)
- [一步一步CocosBuilder(4)完结](http://young40.github.com/blog/2012/12/29/cocosbuilder-step-by-step-part-four/)

谢谢您耐心看完此文, 希望能对您有些帮助.如果有什么问题, 意见, 建议, 拍砖, 欢迎留言或者联系我.

新浪微博: [@杨世玲](http://weibo.com/young40) [http://weibo.com/young40](http://weibo.com/young40)

































































