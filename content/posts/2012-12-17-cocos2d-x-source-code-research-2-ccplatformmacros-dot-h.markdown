---
layout: post
title: "2d-x深读2:CCPlatformMacros.h"
date: 2012-12-17T01:49:00+08:00
comments: true
categories: [Cocos2d-x, Cocos2d-x 源代码研究]
draft: true
---


- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/include/ccConfig.h`
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/platform/mac/CCPlatformDefine.h` //注意我是以mac平台研究的, 其他平台略有不同
- 路径:`cocos2d-2.1beta3-x-2.1.0/cocos2dx/platform/CCPlatformMacros.h`
<!--more-->
``` cpp ccConfig.h
#include "platform/CCPlatformConfig.h"

//设置为1后会include CCPhysicsScript 和 CCPhysicsDebugNode 添加到物理引擎支持
//需要保证Chipmunk在搜索路径中
#ifndef CC_ENABLE_CHIPMUNK_INTEGRATION
#define CC_ENABLE_CHIPMUNK_INTEGRATION 0
#endif

//设置为1后会include CCPhysicsScript 添加到Box2D的支持
//同样需要保证Box2D会在搜索路径中
#ifndef CC_ENABLE_BOX2D_INTEGRATION
#define CC_ENABLE_BOX2D_INTEGRATION 0
#endif

/** 
 设置为1后,2d学会维护一个OpenGL状态缓存来避免不必要的切换
 为了使用这个功能,需要用下列函数来替换GL的对应函数
    - ccGLUseProgram() instead of glUseProgram()
    - ccGLDeleteProgram() instead of glDeleteProgram()
    - ccGLBlendFunc() instead of glBlendFunc()
如果这个功能被禁止, 那么这些函数会直接调用对应的GL函数, 当然就没有缓存了
打开这个功能能提高速度
如果你的代码是从GL ES 1.1升级过来的, 保持这个功能被禁用. 如果各种功能正常的话, 你可以启用这个功能
#ifndef CC_ENABLE_GL_STATE_CACHE
#define CC_ENABLE_GL_STATE_CACHE 1
#endif

/**
启用后,纹理的坐标会用这个公式来计算
- texCoord.left = (rect.origin.x*2+1) / (texture.wide*2);
- texCoord.right = texCoord.left + (rect.size.width*2-2)/(texture.wide*2);

The same for bottom and top.

This formula prevents artifacts by using 99% of the texture.
The "correct" way to prevent artifacts is by using the spritesheet-artifact-fixer.py or a similar tool.

受影响的类:
- CCSprite / CCSpriteBatchNode and subclasses: CCLabelBMFont, CCTMXTiledMap
- CCLabelAtlas
- CCQuadParticleSystem
- CCTileMap
*/
#ifndef CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
#define CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL 0
#endif

//默认0.5秒, 更新下FPS的数据. 数字越大FPS数据越可靠.
#ifndef CC_DIRECTOR_STATS_INTERVAL
#define CC_DIRECTOR_STATS_INTERVAL (0.5f)
#endif

//FPS数据显示位置,默认左下
#ifndef CC_DIRECTOR_FPS_POSITION
#define CC_DIRECTOR_FPS_POSITION ccp(0,0)
#endif

/** @def CC_DIRECTOR_DISPATCH_FAST_EVENTS
 If enabled, and only when it is used with CCFastDirector, the main loop will wait 0.04 seconds to
 dispatch all the events, even if there are not events to dispatch.
 If your game uses lot's of events (eg: touches) it might be a good idea to enable this feature.
 Otherwise, it is safe to leave it disabled.
 
 To enable set it to 1. Disabled by default.
 
 @warning This feature is experimental
 */
#ifndef CC_DIRECTOR_DISPATCH_FAST_EVENTS
 #define CC_DIRECTOR_DISPATCH_FAST_EVENTS 0
#endif

/** @def CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
If enabled, cocos2d-mac will run on the Display Link thread. If disabled cocos2d-mac will run in its own thread.

If enabled, the images will be drawn at the "correct" time, but the events might not be very responsive.
If disabled, some frames might be skipped, but the events will be dispatched as they arrived.

To enable set it to a 1, to disable it set to 0. Enabled by default.

Only valid for cocos2d-mac. Not supported on cocos2d-ios.

*/
#ifndef CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
#define CC_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD 1
#endif

/** @def CC_NODE_RENDER_SUBPIXEL
 If enabled, the CCNode objects (CCSprite, CCLabel,etc) will be able to render in subpixels.
 If disabled, integer pixels will be used.
 
 To enable set it to 1. Enabled by default.
 */
#ifndef CC_NODE_RENDER_SUBPIXEL
#define CC_NODE_RENDER_SUBPIXEL 1
#endif

/** @def CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
 If enabled, the CCSprite objects rendered with CCSpriteBatchNode will be able to render in subpixels.
 If disabled, integer pixels will be used.
 
 To enable set it to 1. Enabled by default.
 */
#ifndef CC_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define CC_SPRITEBATCHNODE_RENDER_SUBPIXEL    1
#endif

/** @def CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
 Use GL_TRIANGLE_STRIP instead of GL_TRIANGLES when rendering the texture atlas.
 It seems it is the recommend way, but it is much slower, so, enable it at your own risk
 
 To enable set it to a value different than 0. Disabled by default.

 */
#ifndef CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
#define CC_TEXTURE_ATLAS_USE_TRIANGLE_STRIP 0
#endif

//VAO定点数组对象
/** @def CC_TEXTURE_ATLAS_USE_VAO
 By default, CCTextureAtlas (used by many cocos2d classes) will use VAO (Vertex Array Objects).
 Apple recommends its usage but they might consume a lot of memory, specially if you use many of them.
 So for certain cases, where you might need hundreds of VAO objects, it might be a good idea to disable it.
 
 To disable it set it to 0. Enabled by default.
 
 */
#ifndef CC_TEXTURE_ATLAS_USE_VAO
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        #define CC_TEXTURE_ATLAS_USE_VAO 1
    #else
        /* Some Windows display adapter driver cannot support VAO. */
        /* Some android devices cannot support VAO very well, so we disable it by default for android platform. */
        /* Blackberry also doesn't support this feature. */
		#define CC_TEXTURE_ATLAS_USE_VAO 0
    #endif
#endif


/** @def CC_USE_LA88_LABELS
 If enabled, it will use LA88 (Luminance Alpha 16-bit textures) for CCLabelTTF objects.
 If it is disabled, it will use A8 (Alpha 8-bit textures).
 LA88 textures are 6% faster than A8 textures, but they will consume 2x memory.

 This feature is enabled by default.

 @since v0.99.5
 */
#ifndef CC_USE_LA88_LABELS
#define CC_USE_LA88_LABELS 1
#endif

/**
  启用后,所有CCSprite的子类都会花上一个边框,方便调试
 0 -- disabled
 1 -- draw bounding box
 2 -- draw texture box
*/
#ifndef CC_SPRITE_DEBUG_DRAW
#define CC_SPRITE_DEBUG_DRAW 0
#endif

//CCSpriteBatchNode的调试用加边框
#ifndef CC_SPRITEBATCHNODE_DEBUG_DRAW
#define CC_SPRITEBATCHNODE_DEBUG_DRAW 0
#endif

//CCLabelBMFont调试加边框
#ifndef CC_LABELBMFONT_DEBUG_DRAW
#define CC_LABELBMFONT_DEBUG_DRAW 0
#endif

//LabeltAtlas调试加边框
#ifndef CC_LABELATLAS_DEBUG_DRAW
#define CC_LABELATLAS_DEBUG_DRAW 0
#endif

/** @def CC_ENABLE_PROFILERS
 If enabled, will activate various profilers within cocos2d. This statistical data will be output to the console
 once per second showing average time (in milliseconds) required to execute the specific routine(s).
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default.
 */
#ifndef CC_ENABLE_PROFILERS
#define CC_ENABLE_PROFILERS 0
#endif

/** Enable Lua engine debug log */
#ifndef CC_LUA_ENGINE_DEBUG
#define CC_LUA_ENGINE_DEBUG 0
#endif
```

``` cpp mac/CCPlatformDefine.h
#include <assert.h> //引入assert

#define CC_DLL //CC_DLL只在win32平台下有效

#if CC_DISABLE_ASSERT > 0
#define CC_ASSERT(cond) //禁用了assert
#else
#define CC_ASSERT(cond) assert(cond) //启用了assert
#endif

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam

//定义空指针
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
```
我们可以看到win32中不同的部分比如`CC_DLL`
``` cpp win32/CCPlatformDefine.h
#if defined(_USRDLL)
    #define CC_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define CC_DLL     __declspec(dllimport)
#endif
```

Linux部分也是`CC_DLL`不同
``` cpp linux/CCPlatformDefine.h
#if defined(_USRDLL)
#define CC_DLL __attribute__ ((visibility ("default")))
#else         /* use a DLL library */
#define CC_DLL __attribute__ ((visibility ("default")))
#endif
```

Android中比较不同的部分是`CC_ASSERT`
``` cpp android/CCPlatformDefine.h
#define CC_ASSERT(cond) \
if (! (cond)) \
{ \
    char content[256]; \
    sprintf(content, "%s function:%s line:%d", __FILE__, __FUNCTION__, __LINE__);  \
    CCMessageBox(content, "Assert error"); \
}
```


我们最后再来看下`CCPlatformMacros.h`
``` cpp CCPlatformMacros.h
#include "ccConfig.h"
#include "CCPlatformConfig.h"
#include "CCPlatformDefine.h"

//CREATE_FUNC是2d-x的一个核心功能,一般采用例如`MySpriter *my = MySpriter::create()`的形式调用.
//这里的create()成员函数即是由CREATE_FUNC创建, 来达到让2d-x来管理内存分配的功能
#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}
//早期版本还有一个NODE_FUNC完成和CREATE_FUNC一样的功能,已经废弃,不再讨论

//CC_ENABLE_CACHE_TEXTURE_DATA 只在Android上启用, 会缓存纹理
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define CC_ENABLE_CACHE_TEXTURE_DATA       1
#else
    #define CC_ENABLE_CACHE_TEXTURE_DATA       0
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//重新绑定indices, 避免在android和win32下glDrawElements函数crash的bug
    #define CC_REBIND_INDICES_BUFFER  1
#else
    #define CC_REBIND_INDICES_BUFFER  0
#endif

//常用宏定义
#ifdef __cplusplus //namespace只在C++中起作用
    #define NS_CC_BEGIN                     namespace cocos2d {
    #define NS_CC_END                       }
    #define USING_NS_CC                     using namespace cocos2d  //应该尽量使用USING_NS_CC, 兼容性更好
#else
    #define NS_CC_BEGIN 
    #define NS_CC_END 
    #define USING_NS_CC 
#endif 

//成员函数快捷声明/定义宏
//多使用这些宏, 可以方便地定义出合乎面向对象原则的成员变量.
//需要注意的是这些调用了这些宏之后, 应该养成重新写public, private等标号的习惯. 因为之后的都会变成public
//定义的成员变量都是protected的
//只读的成员变量
#define CC_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);

//getter是为引用
#define CC_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);

//可读可写成员变量
#define CC_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(varType var);

#define CC_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(const varType& var);

//以上的getter,setter只有声明, 下面的这些带有定义
#define CC_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }

#define CC_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

//setter中会retain一次var, 防止被gc回收.
#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
    if (varName != var) \
    { \
        CC_SAFE_RETAIN(var); \
        CC_SAFE_RELEASE(varName); \
        varName = var; \
    } \
} 

#define CC_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define CC_SAFE_FREE(p)                do { if(p) { free(p); (p) = 0; } } while(0)
#define CC_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define CC_SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = 0; } } while(0)
#define CC_SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)
#define CC_BREAK_IF(cond)            if(cond) break

#define __CCLOGWITHFUNCTION(s, ...) \
    CCLog("%s : %s",__FUNCTION__, CCString::createWithFormat(s, ##__VA_ARGS__)->getCString())

//这个宏定义要注意大小写, 会很容易和cocos2d::CCLog弄错, 因为代码提示的问题.
// cocos2d debug
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
#define CCLOG(...)       do {} while (0)
#define CCLOGINFO(...)   do {} while (0)
#define CCLOGERROR(...)  do {} while (0)
#define CCLOGWARN(...)   do {} while (0)

#elif COCOS2D_DEBUG == 1
#define CCLOG(format, ...)      cocos2d::CCLog(format, ##__VA_ARGS__)
#define CCLOGERROR(format,...)  cocos2d::CCLog(format, ##__VA_ARGS__)
#define CCLOGINFO(format,...)   do {} while (0)
#define CCLOGWARN(...) __CCLOGWITHFUNCTION(__VA_ARGS__)

#elif COCOS2D_DEBUG > 1
#define CCLOG(format, ...)      cocos2d::CCLog(format, ##__VA_ARGS__)
#define CCLOGERROR(format,...)  cocos2d::CCLog(format, ##__VA_ARGS__)
#define CCLOGINFO(format,...)   cocos2d::CCLog(format, ##__VA_ARGS__)
#define CCLOGWARN(...) __CCLOGWITHFUNCTION(__VA_ARGS__)
#endif // COCOS2D_DEBUG

// Lua engine debug
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0 || CC_LUA_ENGINE_DEBUG == 0
#define LUALOG(...)
#else
#define LUALOG(format, ...)     cocos2d::CCLog(format, ##__VA_ARGS__)
#endif // Lua engine debug

//定义废弃(deprecated)属性
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define CC_DEPRECATED_ATTRIBUTE
#endif 
```
