---
layout: post
title: "一步一步CocosBuilder(1)"
date: 2012-12-21 02:46
comments: true
categories: [Cocos2d-x, CocosBuilder] 
---

CocosBuilder是Cocos2d系列的配套开源工具,最新的版本是3.0a, 也是我们这个系列文章所采用的版本.
CCB(CocosBuilder简称)目前只有OS X版本, 没有Windows的版本, 而且源代码也是用Objective-c, 估计移植难度会比较大.
我想这个也是其不够流行的原因吧.

如果你是CCB的熟客,那么你一定会记得CCB v2.x以前版本附带的例子. 而CCB v3.0采用了一个叫做CocosDragonJS的例子, 主要是展示CCB的Javascript的调用能力.
我虽然很喜欢JS, 但是正在学习2d-x, 又很想学好, 所以还是打好基础, 学好原滋原味的2d-x才好, 以后换用无论是Html5, 还是JS, Lua, 心里才会觉得踏实.
我无论C++,还是2d-x都是新手,又是第一次想正儿八经的写博客文章, 难免有很多错误, 还望各位看客踊跃拍砖.

这次的目的是基本实现原来v2.0中的例子.
我采用的2d-x的版本是:cocos2d-2.1beta3-x-2.1.0, 以OS X版本为主, 以sample目录下的HelloCpp为基础改进. CCB的版本是v3.0alpha0.

<!--more-->

本文的目标是基本实现将CCB整合进Mac OS X版本的项目中.

首先将2d-x源代码中sample目录下的HelloCpp目录重命名为TestCocosBuilder.并使用CocosBuilder创建新项目文件在其Resources文件夹中.
还需要将项目需要的图片资源放入Resources目录中. 具体可以参见我打包后的整体项目文件.还可以参见文末的目录树说明.


贴下Xcode项目的目录,如果添加extensions应该逐个添加,否则可能会遇到编译错误,有可能会把其他平台文件错误添加进来.

{% img /upload/2012-12/ccb-sbs-1-001.png %}

做完这些准备工作后, 我们接下来按步来讲解. 

1.首先编辑下`AppMacros.h`, 修改以下代码
``` cpp AppMacros.h
static Resource smallResource  =  { cocos2d::CCSizeMake(480, 320),   "resources-iphone" };
static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "resources-ipad"   };
static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "resources-ipadhd" };
```
这一段主要是修改`HelloCpp`默认的资源目录, 配合CCB的发布路径.同时可以将原来几个目录中文件分别移入对应目录.
2.添加一个新类`MainScene`如下
``` cpp MainScene.h
#ifndef __HelloCpp__MainScene__
#define __HelloCpp__MainScene__

#include "cocos2d.h"
#include "cocos-ext.h"

class MainScene
: public cocos2d::CCLayer //从CCLayer派生
{
public:
    static cocos2d::CCScene* scene();
};

#endif /* defined(__HelloCpp__MainScene__) */
```

``` cpp MainScene.cpp
#include "MainScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

CCScene* MainScene::scene()
{
    CCScene *scene = CCScene::create();
    
    CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); //生成一个默认的Node Loader
    
    CCBReader *reader = new CCBReader(lib); //用node load lib 初始化一个ccb reader
    
    CCNode *node = reader->readNodeGraphFromFile("MainScene.ccbi", scene); //从ccbi文件中加载node
    
    reader->release(); //注意手动释放内存
    
    if (node!=NULL)
    {
        scene->addChild(node); //将node 添加到scene中
    }
    
    return scene;
}
```
3.修改`AppDelegate.cpp`中默认的启动scene
``` cpp 
CCScene *pScene = MainScene::scene(); //HelloWorld::scene();
```
4.注意CCB中发布选项的设置, 并发布一次.
{% img /upload/2012-12/ccb-sbs-1-002.png %}

5.如果没有错误的话, 你应该可以看到运行的效果.
{% img /upload/2012-12/ccb-sbs-1-003.png %}


Update:
@王哲Walzer 说,CCB大部分是美术在用, 在美术圈讨论, 所以程序圈较少看到人在讨论, 其实用的公司挺多的.
http://weibo.com/1702027805/zay18wLT5

``` sh 目录结构
├── samples #目录结构和2d-x源代码保持一致,相信大家对2d-x源代码很熟悉了.
│   └── CocosBuilderTest #项目所在目录
│       ├── Classes #代码所在目录
│       ├── Resources #资源目录
│       │   ├── ccb #ccb发布后的目录
│       │   │   ├── HelloCocosBuilder.ccbi
│       │   │   ├── ...
│       │   │   └── TestSprites.ccbi
│       │   ├── ccbResources #发布后的默认ccb资源
│       │   ├── fonts
│       │   ├── resources-ipad #注意这几个目录, 稍后我们会提及
│       │   ├── resources-ipadhd
│       │   ├── resources-iphone
│       │   ├── resources-iphonehd
│       ├── ccb-source #ccb项目目录
│       │   ├── ccb
│       │   │   ├── HelloCocosBuilder.ccb #这几个是原CocosBuilder例子中的ccb, 直接复制过来了.
│       │   │   ├── ...
│       │   │   └── TestSprites.ccbi
│       │   ├── ccbResources
│       │   ├── resources-auto #CocosBuilderv3.0的资源目录, 似乎只能存放图片
│       │   │   ├── btn-a-0.png #否则会引起发布是造成CocosBuilder崩溃
│       │   │   ├── ...
│       │   │   └── scale-9-demo.png
│       │   ├── MainScene.ccb #默认生成的ccb, 我们这一次不对其更改
│       │   ├── MainScene.js
│       │   ├── animated-grossini.plist
│       │   ├── animated-grossini.png
│       │   ├── ccb-source.ccbproj
│       │   ├── comic\ andy.ttf
│       │   ├── grossini-generic.plist
│       │   ├── grossini-generic.png
│       │   ├── markerfelt24shadow.fnt
│       │   └── markerfelt24shadow.png
│       └── proj.mac
│           ├── HelloCpp.xcodeproj #项目文件, 暂时只支持mac os x
├── cocos2dx #2d-x文件
├── extensions #2d-x的扩展
│   ├── CCBReader #这个是解析CocosBuilder的主要CCBReader
│   │   ├── CCBAnimationManager.cpp
│   │   ├── CCBAnimationManager.h
│   │   ├── ...
│   │   └── CCSpriteLoader.h
│   ├── GUI
│   ├── network
│   ├── physics_nodes
│   ├── ExtensionMacros.h
│   └── cocos-ext.h
```


<hr />
代码打包下载:<a href="http://pan.baidu.com/share/link?shareid=158075&uk=2281999082" target="_blank">下载点</a> 

- 本文是系列文章, 请查看其他文章:
- [一步一步CocosBuilder(1)](http://young40.github.com/blog/2012/12/21/cocosbuilder-step-by-step-part-one/)
- [一步一步CocosBuilder(2)](http://young40.github.com/blog/2012/12/25/cocosbuilder-step-by-step-part-two/)
- [一步一步CocosBuilder(3)](http://young40.github.com/blog/2012/12/28/cocosbuilder-step-by-step-part-three/)
- [一步一步CocosBuilder(4)完结](http://young40.github.com/blog/2012/12/29/cocosbuilder-step-by-step-part-four/)

谢谢您耐心看完此文, 希望能对您有些帮助.如果有什么问题, 意见, 建议, 拍砖, 欢迎留言或者联系我.

新浪微博: [@杨世玲](http://weibo.com/young40) [http://weibo.com/young40](http://weibo.com/young40)








