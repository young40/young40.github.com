---
layout: post
title: "2d-x深读4:CCAutoReleasePool"
date: 2013-01-03T15:09:00+08:00
comments: true
categories: [Cocos2d-x, Cocos2d-x 源代码研究]
draft: true
---

`CCAutoReleasePool.h`主要定义了两个类`CCAutoReleasePool`和`CCPoolManager`.    

其实因为cocos2d-x自动管理内存的原因, 所以, 我们平时很少需要直接使用这两个类.    
为了探究其中的自动管理内存的机制, 我们直接分析其代码.   

总体上来讲, 内存管理这块的机制难度或许比较大. 我研究了一天, 还是有诸多疑问.    
暂时先放一放, 待我功力进化后再来继续研究这块.    
<!--more-->

- 版本:`cocos2d-2.1beta3-x-2.1.0`
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/cocoa/CCAutoReleasePool.h`
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/cocoa/CCAutoReleasePool.cpp`


{% include_code CCObject.h %}
{% include_code CCAutoReleasePool\.h lang:cpp  cocos2d-x-research/2.1.1/cocos2dx/cocoa/CCAutoReleasePool.h %}
{% include_code CCAutoReleasePool\.cpp lang:cpp  cocos2d-x-research/2.1.1/cocos2dx/cocoa/CCAutoReleasePool.cpp %}

- 参考1: http://4137613.blog.51cto.com/4127613/784134
