---
layout: post
title: "一步一步CocosBuilder(2)"
date: 2012-12-25 21:15
comments: true
categories: [Cocos2d-x, CocosBuilder] 
---

上一篇我们建立了基本的工作环境, 这一次我们将基本实现例子中的HelloCocosBuilder.


###注意
这两天纠结了一个问题. 我发现按照上次的配置来实现例子, 图片的大小怎么都和例子中的搭配不上. 后来动用CCB的源代码才看出端倪.
CCB v3.0加入了一个`resources-auto`的目录, 在发布的时候, 会自动缩放图片到对应的分辨率去. 因为图片的分辨率不合适(初步目测), 我们暂时不使用这功能. 请大家将上一篇文章
中的`resources-auto`文件夹下图片移到父目录`ccb-source`下面. 这个也解释了为什么`resources-auto`有非图片文件会导致CCB崩溃的原因.

<!--more-->

#####1
在CCB中打开MainScene.ccb,按`delete`键删除已经存在的几个Sprite.
#####2
添加一个`CCLayerGradient`到屏幕, 如果有问题的话, 可以参见下文档. 选中该节点, 我们可以在右侧看到他的属性, 常规的属性有比如位置, 大小, 锚点, 缩放, 旋转, 标签.
我们现在将其设置为大小为100%x100%的大小, 即满屏.
#####3
从左侧的文件中将`burst.png`拖入屏幕, 也可以使用工具栏添加然后再选择属性`Sprite frame`.   
可以在节点树上双击该节点给其重命名, 我们将该节点命名为`CCSprite-burst`, 便于识别.  
同样我们将其中心的设置到屏幕中心50%x50%, 缩放到2.5倍.   
然后将`Blend src`, `Blend dst` 都设置为`One`, 好吧, 我暂时还不知道这个Blend是什么东西. 有知道的朋友不妨在评论中告诉我下, 或者参考资料.   
大概是透明之类的东西吧.   
#####4
然后我们再添加一个`CCParticleSystemQuad`粒子系统, 就是工具栏的那个Fx按钮.   
将其位置设为最下居中, 在最下边`Particle Texture`设置为`ccbParticleSnow.png`效果,粒子系统还是很复杂的,估计能单独讲很久了. 这里就不再展开了.   
#####5
将`logo-icon.png`拖入屏幕, 放在左上角合适位置, 并将其缩放到0.5. 节点重命名为`CCSprite-logo-icon`.   
接下来就是动画的制作了.选中节点, 可以在菜单栏中`Animation => Insert KeyFrame`找到插入Framekey的各种快捷键.或许我们需要记熟他们.   
首先让我们给`Timeline`设置下时间.双击时间可以编辑, 我们输入00:03:00, 就是3秒钟. 后面最后一位是帧数, 每秒30帧.   
按下小三角可以展开当前节点的各种KeyFrame.    
拖动时间轴游标到1秒10帧,按下键盘`S`, 可以插入一个Scale类型的KeyFrame, 这时将当前节点Scale属性改为0.   
然后继续拖动时间轴游标到2秒10帧, 再插入一个Scale KeyFrame, 并将Scale属性改为0.5. 这时点击播放按钮, 可以看到logo会蹦出来.   
然后在紫色条上点击右键, 可以看到动作菜单, 我们选择`Bounce Out`, 这个时候再点击播放, 就可以看到logo蹦出的时候会有一个弹震的效果.   
再来说明两点, 紫色条设置不同的动作, 可以看到上面会有一点阴影变化, 可以帮助我们快速确认设置的是什么动作.   
如果不小心加多了KeyFrame, 那么可以点击选择KeyFrame, 然后点击`delete`键删除.   
还有需要注意的是, 如果游标不在KeyFrame的点上, 那么此时无法编辑该节点属性, 移动到相应的KeyFrame点即可.   
#####6
然后依法炮制, 将`logo.png`加入, 你可以给其设置想要的动画效果.
#####7
我们再添加一个`CCLabelBMFont`, 字体设置为`markerfelt24shadow.fnt`.Label的内容设置为"Hello Cocos2d-x!!!", 然后调整下位置, 换个喜欢的颜色.
#####8
我们再添加一个`CCControlButton`, Title设置为`Menu & Item`.    
然后再分别设置State Normal, State Highlighted的Sprite frame为`btn-test-0.png` &nbsp; `btn-test-1.png`.   
然后再设置下弹出效果.   
将这个节点再复制5份, 对齐. 再分别设置下Title和弹出效果.设置对齐的时候可能会比较麻烦. 可以从标尺那里拉出几条线, 用于对齐.将该对齐线拉出显示区域即可删除.   
{% img /upload/2012-12/ccb-sbs-2-001.png %}
#####9
最后将左右的两片叶子加入屏幕, 设置个动画即可.
然后将CCB项目发布一下, 在Xcode里面将项目运行一下. 就可以看到效果了.

#####10
我们接下来研究文件和类的绑定, 控件和变量的绑定, 以及事件和函数的绑定.   
我们在CCB中将根节点的`Custom class`设置为`MainScene`.  
我们在Xcode中打开`MainScene.h`, 给`MainScene`增加一个新的函数`CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainScene, create);`.   
最后的代码应该如下:
``` cpp MainScene
class MainScene
: public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainScene, create);
};
```
然后在`MainScene.h`中添加一个新类`MainSceneLayerLoader`代码如下:
``` cpp MainSceneLayerLoader
class MainSceneLayerLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainSceneLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainScene);
};
```
如果需要探究的话, 可以点开相应的函数定义. 也都是些简单的工作.   
吐槽下, 这些名字真的是太难记. 可能需要多熟悉下, 搞清楚其原理了才好.   

然后我们在`MainScene.cpp`中添加一行注册将ccb文件和类绑定起来.
``` cpp MainScene::scene()
CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
lib->registerCCNodeLoader("MainScene", MainSceneLayerLoader::loader());
```
然后运行下, 确保没有错误.   
这几行代码, 就是ccb文件和类绑定的最简模式了.   

#####11
接着我们研究下控件和变量的绑定.   
我们先在CCB中给`CCLabelBMFont`添加一个`Doc root var`类型的绑定`mLabelText`.   
一个需要注意的地方就是CCB v3.0alpha似乎无法将`jsControlled`设置为`false`, 所以可能需要手工打开`MainScene.ccb`将其改为`false`.   
``` xml MainScene.ccb
	<key>jsControlled</key>
	<false/>
```
然后在Xcode中给`MainScene`增加一个从`CCBMemberVariableAssigner`的继承并实现其虚函数. 并且添加相应的变量.     
``` cpp MainScene
class MainScene
: public cocos2d::CCLayer
, public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    MainScene();
    ~MainScene();
    
    static cocos2d::CCScene* scene();
        
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MainScene, create);
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, cocos2d::CCNode* pNode);
    
private:
    cocos2d::CCLabelBMFont *mLabelText;
};
```
在`MainScene.cpp`中需要添加的是:
``` cpp MainScene.cpp
MainScene::MainScene()//构造函数
: mLabelText(NULL)
{}

MainScene::~MainScene()//析构函数中释放内存不能忘
{
    CC_SAFE_DELETE(mLabelText);
}

bool MainScene::onAssignCCBMemberVariable(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::CCNode *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLabelText", CCLabelBMFont*, this->mLabelText);
    
    return true;
}
```
需要注意的是, 由于绑定较晚, 所以无法在init函数中使用变量`mLabelText`.   
#####12
接下来我们研究事件和函数的绑定.   
在CCB中找到`Menu & Item`这个`CCControlButton`, 并设置其Selector为`onButtonTest`.Target设置为`Document root`.   
这里需要注意的是, 设置值的时候,cocos2d-iphone似乎比较喜欢设置成`onButtonTest:`即多个`:`的形式. 我们在cocos2d-x中保持一致即可.不必加`:`.    
然后打开`MainScene.h`给`MainScene`再增加一个继承.
``` cpp MainScene
class MainScene
: public cocos2d::CCLayer
, public cocos2d::extension::CCBMemberVariableAssigner
, public cocos2d::extension::CCBSelectorResolver
```
还有需要实现的函数
``` cpp MainScene
virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName);
    
void onButtonTest(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
```
接下来是增加的函数的实现
``` cpp MainScene.cpp
SEL_MenuHandler MainScene::onResolveCCBCCMenuItemSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler MainScene::onResolveCCBCCControlSelector(cocos2d::CCObject *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onButtonTest", MainScene::onButtonTest);

    return NULL;
}

void MainScene::onButtonTest(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    this->mLabelText->setString("Hello CocosBuilder!!!");
}
```
好了, 现在运行一下, 点击按钮, 就会看到`Hello Cocos2d-x!!!`变成了`Hello CocosBuilder!!!`了.   
#####13
刚才我们提到变量绑定较init函数(更别提运行更早的构造函数了)更晚. 那么如何运行后就控制这些控件呢?    
方法当然是有, 我们需要增加一个`CCNodeLoaderListener`, 并实现其接口.新增代码如下:
``` cpp MainScene
, public cocos2d::extension::CCBSelectorResolver
, public cocos2d::extension::CCNodeLoaderListener
{
    ///......代码省略鸟
    virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
    ///......代码省略鸟
}
```
``` cpp MainScene.cpp
void MainScene::onNodeLoaded(cocos2d::CCNode *pNode, cocos2d::extension::CCNodeLoader *pNodeLoader)
{
    this->mLabelText->setString("All Loaded");
}
```
好啦, 运行就能看到`All Loaded`啦.   

{% img /upload/2012-12/ccb-sbs-2-002.png %}









{% render_partial _posts/cocosbuilder-step-by-step-footer.markdown %}








































