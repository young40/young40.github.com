---
layout: post
title: "基于Pomelo和Cocos2d-x的聊天室"
date: 2013-01-08 00:39
comments: true
categories: ["Cocos2d-x"]
---

据说聊天室和游戏系统有很多相似之处, 并且很多游戏本身就带着多人聊天系统. 所以Pomelo将聊天室作为了一个例子.    

开源是非常棒的思想和运动, 我简单提两点体会, 
1.发现了问题可以不用等官方响应,自己有能力就可以马上修复. 2.你可以贡献自己的代码,很多人都可以贡献代码,可以让项目快速发展.
当然一旦你的代码被接受并被很多人使用, 心中的成就感是不言而喻的.    

之所以要提到开源, 是因为今天我们要用到的东西基本上全部是开源的, 并且我们还要对其中的代码做一些修改才能正常工作. 再一次为开源鼓掌!     

这并不是一篇Pomelo,或者Cocos2d-x的入门说明, 需要你对这两者都有一定的了解. 
同时, 无论是Pomelo还是Cocos2d-x, 我都是新手, 如果文章中有错误的地方, 希望您能不吝指出.
您可以在文章下面留言, 或者在新浪微博上发微薄 @杨世玲 
(需要说明的一点是, 请勿私信提问非隐私性技术问题, 我是希望您的问题, 我的或者其他朋友的回复,能被更多朋友搜索到, 
我希望我们的交流, 能让更多朋友受到启发, 受益. 这也是对社区和世界和平的一点贡献吧.)    

<!--more-->

下面是我们需要使用到的工具及其版本:    

- 平台:OS X(10.8.2) Xcode(4.5.2)    
- Cocos2d-2.1beta3-x-2.1.0    
- Pomelo https://github.com/NetEase/pomelo
- socket.io-clientpp https://github.com/ebshimizu/socket.io-clientpp
- websocketpp https://github.com/zaphoyd/websocketpp
- Rapidjson https://code.google.com/p/rapidjson/
- chatofpomelo https://github.com/NetEase/chatofpomelo

查阅本文时请注意版本差异.        

### 环境配置
我们的目标是实现OS X, IOS, Android三个平台下的chatofpomelo的Cocos2d-x版本.   
所以需要安装各自平台的开发工具如Xcode,Eclipse,NDK等, 不是本文需要关心的内容. 相信大家也已经轻车熟路了.   
Pomelo的安装 `npm install pomelo -g`, 如有疑问请查看Pomelo的Wiki.   
websocketpp需要著名的C++库, boost. 我是使用了HomeBrew来安装.
其他安装方式请自省查阅相关文档, 有一点需要指出的是, 必须编译为带有x86(i386)支持lib文件. 
因为Cocos2d-x在OS X下是32位的.     
``` sh HomeBrew安装boost库
brew install boost --universal
```
上面这条命令中`--universal`即代表编译出来的boost的lib文件同时支持x86和x64.    

### 项目配置
Cocos2d-x跨平台无疑做的很好, 但是如果手动创建一个结构良好的跨平台项目,还是一件非常棘手的事.   
幸好我们可以偷懒, 打开Cocos2d-x的源代码, samples目录下面都是已经配置好的跨平台的项目文件.
合理利用, 将事半功倍.    

我们将samples目录下HelloCpp复制为ChatOfPomelo,打开找到proj.mac目录下面的`HelloCpp.xcodeproj`, 
可以将其重命名为`ChatOfPomelo-Mac.xcodeproj`, 以便于和接下来的ios项目有所区分.   

在ChatOfPomelo下创建lib目录, 下载socket.io-clientpp和websocketpp并解压到lib目录下面. 
并将socket.io-clientpp/lib/rapidjson 复制到lib目录下面.    
目录结构应该是这样的.
``` sh 目录
ChatOfPomelo/
├── Classes
├── Resources
├── lib
│   ├── rapidjson
│   ├── socket.io-clientpp
│   └── websocketpp
├── proj.android
├── proj.blackberry
├── proj.ios
├── proj.linux
├── proj.mac
├── proj.marmalade
└── proj.win32
```
我们打开proj.mac下的项目文件, 在Xcode中打开项目设置页, 点开Targets的设置.
在Build Settings下面找到`Search Paths`, 将`Always Search User Paths`设置为`Yes`.   
给`Header Search Paths` 依次添加下面路径:
```
/usr/local/include_code
"$(SRCROOT)/../lib/socket.io-clientpp/src"
"$(SRCROOT)/../lib/websocketpp/src"
"$(SRCROOT)/../lib/rapidjson/include"
```
如下图所示:
{%img /upload/2013-01/chat-of-pomelo-cocos2d-x-001.png %}

给`Library Search Paths`添加路径`/usr/local/lib`. 如下图:
{%img /upload/2013-01/chat-of-pomelo-cocos2d-x-002.png %}

其中`/usr/local/include`和`/usr/local/lib`是boost的所在位置(HomeBrew的默认安装位置).
如果你的boost不在上述位置, 请自行更改.

接下来将`websocketpp`的工程文件添加进来.参见下图:      
{%img /upload/2013-01/chat-of-pomelo-cocos2d-x-003.png %}

然后打开工程的设置, 在Targets的Build Phases页面,找到`Link Binary With Libraries`,
添加`libwebsocketpp.a`, 以及boost的库文件(在/usr/local/lib中找到).       
``` sh boost 需要添加的库文件列表
libboost_date_time-mt.a
libboost_graph-mt.a
libboost_random-mt.a
libboost_regex-mt.a
libboost_system-mt.a
libboost_thread-mt.a
libboost_timer-mt.a
```
添加的方法参见下图:    
{%img /upload/2013-01/chat-of-pomelo-cocos2d-x-004.png %}

##文件Hack
我们在`HelloWorldScene.cpp`中添加一句`#include <socket_io_client.hpp>`.   
然后试图运行项目的话, 会看到几处报错(有可能你看到这篇文章的时候相关库文件已经更新了, 没有报错的话, 直接跳过这段即可.)   
报错的是`socket_io_client.hpp`中的`std::function`以及`std::unique_ptr`等.

这是因为这些语句是C++11的新语法. 而如果在Xcode中启用C++11的支持,则Cocos2d-x无法通过编译.
原因有人说是Cocos2d-x不支持C++11, 有人说是OS X自带的libc++库太旧的原因, Xcode4.6中会更新该库.
我是小菜鸟一只, 也分不清谁说的对.    

但我们可以通过修改这些代码, 采用boost的API来完成相应的工作.
而websocketpp这个项目又大量采用了boost, 所以修改为boost的API还是挺划算的.   

找到`socket_io_client.hpp`:  
``` cpp socket_io_client.hpp
//typedef std::function<void (socketio_events&, const Value&)> eventFunc;
//修改为
typedef boost::function<void (socketio_events&, const Value&)> eventFunc;

//std::unique_ptr<boost::asio::deadline_timer> m_heartbeatTimer;
//修改为
boost::shared_ptr<boost::asio::deadline_timer> m_heartbeatTimer;
```

找到`socket_io_client.cpp`:
``` cpp socket_io_client.cpp
//m_heartbeatTimer = std::unique_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(con->get_io_service(), boost::posix_time::seconds(0)));
//修改为
m_heartbeatTimer = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer(con->get_io_service(), boost::posix_time::seconds(0)));
```
打开`lib/rapidjson/include/rapidjson/document.h`:
``` cp document.h
//if (reader.Parse<parseFlags>(is, *this)) {
修改为
if (reader.template Parse<parseFlags>(is, *this)) {"
//这个是jsoncpp的一个bug, 新版本里面已经修复, 但socket_io_client绑定了旧版本, 暂未升级. 
```

这是整个项目应该就可以正常编译运行了. 如果没能正常运行, 你可能在某一步出错了, 
可以在下面评论分享下你的问题, 如果已经找到解决办法, 也要把解决方法也贴出来, 分享给大家哦, 亲!

//将socket_io_client, websocketpp的cpp文件引入到项目.
















