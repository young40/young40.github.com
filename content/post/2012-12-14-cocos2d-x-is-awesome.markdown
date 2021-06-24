---
layout: post
title: "Cocos2d-x 真是太棒了"
date: 2012-12-14T03:07:00+08:00
comments: true
categories: [杂谈]
draft: true
---

大概从7月20号左右, 开始决定转向游戏, 首先接触了cocos2d-iphone, 学习了几天. 然后简单了解下就转向了cocos2d-x的学习.

2d-x最令人开心的就是跨平台特性, 经过这段时间的尝试, 感觉跨平台特性确实很强大. 我一般都是在Xcode写代码, 以os x平台做调试, 然后ios平台就不用提了, 直接编译即可.
Win和Android下面一般都是直接编译即可. 当然, 因为我对C++也是刚刚开始学习, 所以代码写的不标准, 那么在VS下面会无法通过编译. 主要是体现在一些需要将函数作为参数传入的地方,
比较会不容易弄清楚回调函数的参数. Xcode我通通将`CCObject *sender`作为参数, 而VS要求就非常严格了.

<!--more-->

在Win上, 我使用VS2012编译, 能很好地支持我现在使用的C++11的一些特性, 比如类似于:
```
vector<int> v(1);
for (auto i : v)
{
}
```
而在Android上, 因为一直没有更新NDK, 版本是r8b, 一直没有办法很好地编译这段代码. 更新为r8c之后,可以编译了.太好了, 我还是很喜欢C++11的这些新的语法糖.

默认是没有启用C++11特性的, 可以在`Android.mk`中, 添加`LOCAL_CPPFLAGS += -std=c++0x`来启用这个特性.


作为练习, 我将潘潘的j2me的坦克游戏简单移植到ios上, 简单实现以后, 我转向山寨@胡铠鸥 的PopCube(<a target="_blank" href="https://itunes.apple.com/cn/app/popcube!/id562905043?l=en&mt=8">原版下载</a>)了. 山寨的很彻底, 直接将图片,声音等资源从原版的ipa里面提取出来, 或许游戏资源的保护也应该做的更好一点.

接下来要转向继续开发坦克了, 希望一切顺利.



