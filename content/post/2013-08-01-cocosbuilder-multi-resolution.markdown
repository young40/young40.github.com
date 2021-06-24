---
layout: post
title: "CocosBuilder 多分辨率"
date: 2013-08-01T22:10:00+08:00
comments: true
categories: [Cocos2d-x, CocosBuilder]
draft: true

---

最近两个项目大量使用了CocosBuilder, 对于开发效率提高是巨大的. 一直计划写一篇博客谈谈CocosBuilder的多分辨率问题, 懒病加上一些疙疙瘩瘩的小事情, 拖延了一个多月, 才终于下定决心要尽快完成这篇博客.

在这篇文章中, 假定你比较熟悉CocosBuilder, 如果您还不是很熟悉CocosBuilder, 推荐您看下我的另外一篇文章. [从零开始学习CocosBuilder](http://www.ityran.com/archives/2614) 我还是小菜鸟一个, 难免会有很多错误, 还望各位看客不吝赐教. 

版本:

- Cocos2d-x 2.1.4 http://cocos2d-x.googlecode.com/files/cocos2d-x-2.1.4.zip
- CocosBuilder 3.0alpha5 http://cocosbuilder.googlecode.com/files/CocosBuilder-3.0-alpha5.zip

最近几次Cocos2d-x和CocosBuilder(以下简写为2dx和ccb)都是联合发布的, 使用联合发布的版本会减少不兼容的可能.

<!--more-->

##基本概念

我觉得在多分辨率中重要的概念就是`位置`, `大小`, `节点关系`, 可以通过位置, 大小, 节点关系, 锚点等的合理组合, 最终达到理想的界面. 当然, 这需要ccb的使用者更多的思考. ^\_-

###位置
{% img /upload/2013-08/ccb-multi-r-01.png %}

我们可以看到ccb中位置共有6种设置. 其中前四种设置方式代表了当前节点相对于其父节点的一个顶点的绝对位置.

{% img /upload/2013-08/ccb-multi-r-02.png %}

如上图所示, 我们把`Click me` 按钮, 放在了距离其父节点右上角各50px.
这种位置的设置方式, 一般在节点需要放在父节点一个角上时很有用.

{% img /upload/2013-08/ccb-multi-r-03.png %}

第5种设置方式表示当前节点的坐标由其父节点大小(宽width, 高height)的百分比组成, 其中父节点的左下角为原点.
如上图所示一个节点的父节点大小为200px\*100px, 当前节点设置为(70%, 50%),那么当前节点距离其父节点左下角就应该是(140, 50).
百分比设置是位置设置中最常用的方式, 只有当百分比设置不适合时才去考虑其他设置方式.


{% img /upload/2013-08/ccb-multi-r-04.png %}

第6种设置方式其实就是第1种设置方式的x,y值分别乘以一个全局缩放系数. 我们接下来就讲这个全局缩放系数, 这里先假设iphone下为1, iphone retina下为2.
如上图所示, 当前节点在iphone下距离其父节点左下角(50\*1, 50\*1), 在iphone retina下则就是(50\*2, 50\*2).

###全局缩放系数
如果我们需要在屏幕上放一个CCLabelTTF, 字体大小设置为48, 分别放在480\*320分辨率的iphone上和960\*640分辨率的iphone retina上, 那么在iphone retina上显示会显得特别小.
这个时候我们就需要为游戏设置一个全局缩放系数, iphone下为1, iphone retina下为2, 那么ccb中设置字体大小为48, 在游戏中iphone上字体大小为48\*1=48, iphone retina中字体大小为48\*2=96.

你可以在设置位置, 内容大小, 缩放, 字体大小的时候, 找到这个选项. 都是一样的道理, 对应的数据都需要乘以全局缩放系数.

默认iphone(480\*320)的全局缩放系数是1, iPad/iphone HD是2, iPad HD就是4了.
当然你可以自己设置一套全局缩放系数.

###内容大小
{% img /upload/2013-08/ccb-multi-r-05.png %}

只有如CCLayer, CCNode等几个节点可以设置内容大小. 可以有6个选项.

- 1. 绝对大小
- 2. 百分比相对大小, 相对于父节点的大小
- 3. 相对父节点大小, 宽高设置为(100, 100), 那么该节点比其父节点宽高各小100px. 如果设置为(0, 0), 那么该节点就和父节点一样大小.  
- 4. 宽度百分比, 高度绝对大小.
- 5. 宽度绝对大小, 高度百分比.
- 6. 绝对大小, 会被乘以全局缩放系数.

##基本操作

###多分辨率的文档

{% img /upload/2013-08/ccb-multi-r-06.png %}

从菜单中选择`Edit Resolutions...`, 打开编辑多分辨率对话框.

{% img /upload/2013-08/ccb-multi-r-07.png %}

我们可以看到当前为iphone设置了(480\*320)的分辨率, `Scale`这个选项就是我们上面提到的`全局缩放系数`, 值为1.
为iPad设置了(1024\*768)的分辨率, 全局缩放系数为2.

我们可以点击`Add Predefined Resolution`, 来添加ccb预定义的分辨率. 比如添加一个iphone5的分辨率.
哦, 什么, 分辨率是568\*320???? `需要注意的是，CocosBuilder的工作是基于点（Point）的，而不是像素的。`.

####Resolution Extension和resources-auto(插播)
ccb中有一个resources-auto的重要概念, 你可以只提供一套资源, ccb通过全局缩放系数, 来为你自动生成不同分辨率的图片. 
假设你为iPad retina(全局缩放系数为4)设计了一张100\*100的图片.那么iphone会得到一张25\*25的图片. iPad/iphone HD会得到一张50\*50的图片.

说到资源解决方案, 首先要决定你想要支持的最大分辨率. 假设需要完美支持iPad HD, 就按2048\*1536来做资源, 此时全局缩放系数为4. 
把图片资源放置到resources-auto目录下, 那么ccb会帮我们自动缩放出其他需要的分辨率.
嗯, 还需要像下图一样设置, 告诉ccb需要从4x来设置. 从菜单的`Publish Settings...`打开.

{% img /upload/2013-08/ccb-multi-r-08.png %}

如果是和背景无关的图片, 一般来讲自动缩放就可以胜任了. 

如果需要在不同的分辨率下使用不同的图片, 比如一张背景图, iphone上使用960\*640的图片, iPad上使用一张1024\*768的图片.
那么可以在资源目录创建resources-iphone, resources-ipad目录, 然后在resources-iphone放置一张960\*640分辨率的图片demo.png, 在resources-ipad目录下放置一张1024\*768分辨率的图片demo.png. 要注意需要保持其文件名相同.

我们在ccb的工程目录下仍然只看到了一份demo.png, 不要着急, ccb会根据我们选用的不同的分辨率来给我们呈现不同的图片.

ok, 插播结束. 继续说我们的多分辨率文档.

其实, 我们可以给文档添加一个iphone hd, 以及正常iphone5尺寸的文档. 比较遗憾的是ccb不支持编辑`Resolution Extension`的列表. 
而ipad和iphonehd的设置是在一起的, 并且也没有iphone5的Resolution可以设置.
所以我希望你没有需要必须要区分iphonehd/iphone5的需求, 在ccb里面是无法区分的. 令人欣慰的是, 我们还是可以在游戏里面区分他们.-\_=

给文档添加的分辨率是可以自定义的, 名字, 宽, 高, 全局缩放系数. 所以我们还是可以创建全尺寸的iphonehd/iphone5的分辨率.


{% img /upload/2013-08/ccb-multi-r-09.png %}

然后, 我们可以在ccb的菜单中找到切换多分辨率的选项, 这样就可以很方便的预览在各个分辨率下的效果.
注意他们的快捷键哦, 很有用哦.推荐在使用的时候, 经常在几个分辨率下切换查看效果是否正确理想.


{% img /upload/2013-08/ccb-multi-r-10.png %}

##做一个ccb工程
是时候做一个ccb的工程了, 不过我有点羞涩要给你展示我做的工程. 因为它实在太丑了, 还是来一张截图吧, 希望没有吓到你.-\_= 


{% img /upload/2013-08/ccb-multi-r-11.png %}


##代码一下
由于上面提到的一些限制, 只能基本上做到在ccb中做的, 就是我们在游戏中看到的.

ccb自带的帮助中`Setting scale and design size`, 需要做一些改进. 下面是我的代码.




``` cpp AppDelegate.cpp 

 //set the correct resource for CocosBuilder
    {
        CCSize sizeIphone = CCSizeMake(480, 320);
        CCSize sizeIphoneHD = CCSizeMake(960, 640);
        CCSize sizeIphone5 = CCSizeMake(1136, 640);
        CCSize sizeIpad = CCSizeMake(1024, 768);
        CCSize sizeIpadHD = CCSizeMake(2048, 1536);
        
        CCSize designSize = sizeIphoneHD;
        CCSize resourceSize = sizeIphoneHD;
        CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
        
        std::vector<std::string> searchPaths;
        std::vector<std::string> resDirOrders;
        
        TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
        if (platform == kTargetIphone || platform == kTargetIpad)
        {
            CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
            
            if (screenSize.width > 768)
            {
                resourceSize = sizeIpadHD;
                designSize = sizeIpadHD;
                resDirOrders.push_back("resources-ipadhd");
                
                CCBReader::setResolutionScale(4.0f);
            } else if (screenSize.width > 640) {
                resourceSize = sizeIpad;
                designSize = resourceSize;
                resDirOrders.push_back("resources-ipad");
                resDirOrders.push_back("resources-iphonehd");
                
                CCBReader::setResolutionScale(2.0f);
            } else if (screenSize.width > 480) {
                if (screenSize.height > 960) {
                    designSize = sizeIphone5;
                }
                else {
                    designSize = sizeIphoneHD;
                }
                resourceSize = sizeIphoneHD;
                
                CCBReader::setResolutionScale(2.0f);
                
                resDirOrders.push_back("resources-iphonehd");
            } else {
                CCBReader::setResolutionScale(1.0f);

                designSize = sizeIphone;
                resourceSize = sizeIphone;
                
                resDirOrders.push_back("resources-iphone");
            }
            
            CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(resDirOrders);
        }
        
        pDirector->setContentScaleFactor(resourceSize.width / designSize.width);
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(
                                                               designSize.width, designSize.height,
                                                               kResolutionExactFit);
    }
    
    CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    CCBReader *reader = new CCBReader(lib);
    
    CCScene *scene = reader->createSceneWithNodeGraphFromFile("MainScene.ccbi");
    
    pDirector->runWithScene(scene);
```
这里只支持了iOS, 如果需要支持Android的话, 可以挑选几个需要的分辨率, 邻近的分辨率坐下拉伸等策略.

运行下代码, 可以看下效果. 收工.


工程下载: http://pan.baidu.com/share/link?shareid=3612867199&uk=2281999082

推荐看下bilt的两篇博客: http://blog.justbilt.com/569/ http://blog.justbilt.com/579/ 里面还罗列了数片前辈们关于多分辨率的文章, 我学习的过程中也受益匪浅. 
一并致谢. 







































