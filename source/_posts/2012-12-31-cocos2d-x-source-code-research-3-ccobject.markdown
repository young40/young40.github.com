---
layout: post
title: "2d-x深读3:CCObject"
date: 2012-12-31 17:21
comments: true
categories: [Cocos2d-x, Cocos2d-x 源代码研究]
---

CCObject是绝大部分cocos2d-x类的基类, 我们就从这里一步一步揭开cocos2d-x的奥秘.   

CCObject承担了两个重要的功能, 拷贝机制和内存管理.    

##拷贝机制
打开`CCObject.h` 首先看到的是类CCCopying, 而CCObject是从该类派生. 从这个意义上来讲CCCopying才是大部分类的基类.   
但CCCopying其实非常简单, 再加上没有其他类从CCCopying派生, 所以说CCObject才是大部分类的基类也是不错的. 我认为CCCopying仅仅是一个接口性质.   

CCCopying这个类非常简单, 只有一个成员函数copyWithZone(CCZone \*pZone); 简单到其实现只是为了报错. 尽管如此, 但其却承担了拷贝机制这一重要的功能.   

cocos2d-x和cocos2d-iphone是近亲, 所以cocos2d-x在API上会和cocos2d-iphone保持一致, 代码上也多有借鉴. CCObject明显就有很多NSObject的痕迹.
当然我对Objective-c并不熟悉, 这里都是些猜测罢了. copyWithZone或许就是借鉴了Obj-c.   

我们先来看下拷贝机制, 至于CCObject的代码倒不忙着看.   

<!--more-->
这两段代码来至于类CCArray, 我们可以看到调用了原对象的copy()来拷贝一份新的CCArray.  
而copy()则定义于CCObject中, 其工作就是调用copyWithZone.  

``` cpp CCArray
CCArray* CCArray::createWithArray(CCArray* otherArray)
{
    CCArray* pRet = (CCArray*)otherArray->copy();
    pRet->autorelease();
    return pRet;
}
/////----------////
CCObject* CCArray::copyWithZone(CCZone* pZone)
{
    CCAssert(pZone == NULL, "CCArray should not be inherited.");
    CCArray* pArray = new CCArray();
    pArray->initWithCapacity(this->data->num > 0 ? this->data->num : 1);

    CCObject* pObj = NULL;
    CCObject* pTmpObj = NULL;
    CCARRAY_FOREACH(this, pObj)
    {
        pTmpObj = pObj->copy();
        pArray->addObject(pTmpObj);
        pTmpObj->release();
    }
    return pArray;
}
```
这里面牵涉到两个概念, 深拷贝和浅拷贝. 不清楚的大家可以搜索下, 简而言之, 深拷贝才真正的完全拷贝. cocos2d-x实现的是深拷贝.   
我们在上面CCArray::copyWithZone中可以看到拷贝时对array的各个成员也执行了copy()的动作.   
这样才能防止如果obj2是obj1的浅拷贝, 很容易出现array的元素有可能被过早释放的情况.   

这里我们还可以一并说下CCZone这个类, 这个类也很简单, 只有一个构造函数和成员变量.   
``` cpp CCZone
class CC_DLL CCZone
{
public:
    CCZone(CCObject *pObject = NULL);

public:
    CCObject *m_pCopyObject;
};
```
我想CCZone存在的目的就是为了调用copyWithZone的时候对象传递方便.比如我们还可以看下CCSpeed的copyWithZone函数.   
``` cpp CCSpeed
CCObject *CCSpeed::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCSpeed* pRet = NULL;
    if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
    {
        pRet = (CCSpeed*)(pZone->m_pCopyObject);
    }
    else
    {
        pRet = new CCSpeed();
        pZone = pNewZone = new CCZone(pRet);
    }
    CCAction::copyWithZone(pZone);

    pRet->initWithAction( (CCActionInterval*)(m_pInnerAction->copy()->autorelease()) , m_fSpeed );
    
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}
```
我们可以看到构建了一个pZone, 然后调用父类的`CCAction::copyWithZone(pZone);`.   
完全是为了copyWithZone方便传递变量而创建的.   

拷贝机制看起来有点复杂, 但其实用起来很简单. 我觉得就两点, 1,使用copy()调用 2.各个类对自己的copyWithZone负责.

##内存管理
cocos2d-x的内存管理采用了引用计数的方法. 曾经看到过有人吐槽其内存管理在多线程下不好用.   

CCObject及其子类的对象在创建时, 引用计数默认为1, 每次retain后引用计数加1. 每次release后引用计数减1.
被自动管理的对象引用计数为0时, 会被自动释放.

老G总结的内存管理使用的几点原则(见参考1):   
- 原则1: 谁生成(new, copy)谁负责release.   
- 原则2: 谁retain, 谁负责release.    
- 原则3: 对于使用了autorelease的对象则不必管他.   

##几个常用的函数指针
当我看到下面这两句的时候, 我完全懵了.   
``` cpp
typedef void (CCObject::*SEL_SCHEDULE)(float);
#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
``` 
如果你也对函数指针不熟悉的话, 请翻下相关内容吧.
在这两句里面, 第一句其实是定义了一个返回类型为void的, 名字为SEL\_SCHEDULE的, 参数为float的函数指针.   
第二句其实是一个函数类型转换, 将(&_SELECTOR)强制转换成SEL\_SCHEDULE类型的函数指针.   

通常需要回调函数的时候, 就需要用到这些函数指针. 这里就不再赘述, 以后应该会提及相关知识.   

好, 我们接下来直接翻代码吧.  


- 版本:`cocos2d-2.1beta3-x-2.1.0`
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/cocoa/CCObject.h`
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/cocoa/CCObject.cpp`


{% include_code CCobject\.h lang:cpp  cocos2d-x-research/2.1.1/cocos2dx/cocoa/CCObject.h %}
{% include_code CCobject\.cpp lang:cpp  cocos2d-x-research/2.1.1/cocos2dx/cocoa/CCObject.cpp %}

- 参考1: http://4137613.blog.51cto.com/4127613/784134
