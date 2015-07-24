---
layout: post
title: "2d-x深读1:CCPlatformConfig.h"
date: 2012-12-17 00:43
comments: true
categories: [Cocos2d-x, Cocos2d-x 源代码研究]
---

准备好好学习下Cocos2d-x(以下简称2dx), 我正在很努力地学习C++.

作为新手的我, 只能从最基本的代码读起. 就从`CCPlatformConfig.h`开始研究起吧, 这个文件没有include其他文件, 是最基本的文件.

研究成果见代码.

<!--more-->

- 版本:`cocos2d-2.1beta3-x-2.1.0`
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/platform`

``` cpp CCPlatformConfig.h
#ifndef __CC_PLATFORM_CONFIG_H__
#define __CC_PLATFORM_CONFIG_H__

/**
  Config of cocos2d-x project, per target platform.
  */

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which CC uses.
#define CC_PLATFORM_UNKNOWN            0
#define CC_PLATFORM_IOS                1
#define CC_PLATFORM_ANDROID            2
#define CC_PLATFORM_WIN32              3
#define CC_PLATFORM_MARMALADE          4
#define CC_PLATFORM_LINUX              5
#define CC_PLATFORM_BADA               6
#define CC_PLATFORM_BLACKBERRY         7
#define CC_PLATFORM_MAC                8

//默认是unknown的
// Determine target platform by compile environment macro.
#define CC_TARGET_PLATFORM             CC_PLATFORM_UNKNOWN

//CC_TARGET_OS_MAC等是从编译器的环境变量传入的. 参加各编译器或者IDE的预定义宏设置

// mac
#if defined(CC_TARGET_OS_MAC)
#undef  CC_TARGET_PLATFORM //先取消定义
#define CC_TARGET_PLATFORM         CC_PLATFORM_MAC //定义为MAC平台
#endif

// iphone
#if defined(CC_TARGET_OS_IPHONE)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_IOS
    #define CC_SUPPORT_PVRTC //支持prv压缩格式
#endif

// android
#if defined(ANDROID)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_ANDROID
#endif

// win32
#if defined(WIN32) && defined(_WINDOWS)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM         CC_PLATFORM_LINUX
#endif

// marmalade
#if defined(MARMALADE)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_MARMALADE
#endif

// bada
#if defined(SHP)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_BADA
#endif

// qnx
#if defined(__QNX__)
    #undef  CC_TARGET_PLATFORM
    #define CC_TARGET_PLATFORM     CC_PLATFORM_BLACKBERRY
#endif


//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

//不支持的平台,如果你在上面平台上遇到这个错误, 就可能是预定义变量, 或者编译器的环境变量设置错误
// check user set platform
#if ! CC_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma warning (disable:4127) //在win32平台不显示4127的warning, 这个具体用法可以参见
//http://blog.csdn.net/wowolook/article/details/8060334
#endif  // CC_PLATFORM_WIN32

#endif  // __CC_PLATFORM_CONFIG_H__
```
