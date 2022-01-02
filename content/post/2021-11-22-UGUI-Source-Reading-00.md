+++
title = "UGUI源码分析(零): 环境搭建"
date = 2021-11-22T16:36:17+08:00
lastmod = 2022-01-03T00:54:09+08:00
tags = ["Unity", "UGUI"]
categories = ["UGUI源码分析"]
draft = false
+++

有道是, 工欲善其事, 必先利其器. 想研究代码, 最好不过的方式就是边修改边运行看看效果, 研究UGUI自然也不例外.
我们先看下如何搭建一个可以修改调试UGUI环境.


## 版本的选择 {#版本的选择}

Unity在2019.1及之前版本, 以dll的形式提供UGUI, 并在GitHub提供了UGUI的C#源码下载(地址: <https://github.com/Unity-Technologies/uGUI>).

2019.2及以后版本, 使用PackageManager的形式提供UGUI及其源码, 但还是不方便进行修改和调试.

在当前最新的2021.2.3f1版本, UGUI的部分Text相关控件被标记为Legacy, 取而代之的是由TextMeshPro提供相关控件. TextMeshPro的优点不再赘述,
鉴于此情况, 研究UGUI, 也要把TextMeshPro一并研究.

我选择了当前最新的2021.2.3f1版本(macOS), 相信环境搭建也适用2019.2以后版本. 至于2019.1及以前版本, 网上有很多教程, 大家可以参考一下.


## 操作步骤 {#操作步骤}

-   1. 新建项目

使用2021.2.3f1版本新建项目, 建议用git将项目托管起来, 方便查看及保存修改记录.

-   2. 导入TextMeshPro插件资源

从菜单 `Window => TextMeshPro => Import TMP Essential Resources` 导入TextMeshPro插件资源.

-   3. 复制插件

我们需要将UGUI和TextMeshPro插件先保存起来. 正常打开上一步中新建的项目, 将项目目录下Library/PackageCache下
`com.unity.ugui@1.0.0`, `com.unity.textmeshpro@3.0.6`, `com.unity.visualscripting@1.7.5` 目录, 复制到项目Assets目录.
可以将目录重命名为更简单的名字, 方便后序使用.

-   4. 删除插件引用

使用文本编辑器打开 `Packages/manifest.json` 删除如下插件:

`"com.unity.textmeshpro": "3.0.6"`

`"com.unity.ugui": "1.0.0"`

`"com.unity.visualscripting": "1.7.5"`.

可以将Unity编辑器重启, 以减少可能遇到的问题.

> 这里将 `visualscripting` 也删除是因为 `visualscripting` 依赖了UGUI, 会自动引入UGUI(即使我们删除了UGUI的情况下).
>
> 未来不排除有更多插件依赖UGUI, 可以查看依赖关系后, 也同样做删除操作.

-   5. 验证

修改 UGUI目录下 `Runtime/UI/Core/Button.cs` , 找到 `Press()` 函数, 给其增加一行日志打印.

```csharp
private void Press()
{
    Debug.Log("Hello UGUI Button."); // 新增日志行

    if (!IsActive() || !IsInteractable())
        return;

    UISystemProfilerApi.AddMarker("Button.onClick", this);
    m_OnClick.Invoke();
}
```

给场景中增加一个 `Button` , 运行后点击一下, 看下日志窗口是否有新的日志打印吧!


## 待修复(填坑)问题 {#待修复--填坑--问题}

TODO: TextMeshPro 的文字对齐的 icon 显示有问题.


## 尽情冲浪 {#尽情冲浪}

快来一起在UGUI中尽情冲浪🏄‍♀️🏄吧.
