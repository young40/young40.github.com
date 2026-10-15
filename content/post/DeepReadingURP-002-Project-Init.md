+++
title = "领读URP 002: 最简URP项目工程"
date = 2025-04-28T10:56:47+08:00
lastmod = 2025-04-30T10:34:32+08:00
tags = ["Unity", "URP"]
categories = ["领读URP"]
draft = true
+++

## 新建Unity工程 {#新建unity工程}

使用Unity Hub, 选择6000.2.0f1???, 建立一个"3D(Build-In Render Pipeline)项目, 如果之前没有下载模版, 需要先下载模版.
6.0版本(???)不再默认安装BIRP项目模版.

使用git管理项目.
使用gitignore.
_/  <https://www.toptal.com/developers/gitignore/api/unity>
<http://www.gitignore.io>_


## 引入URP包 {#引入urp包}


## 文件变化 {#文件变化}

-   新增
    Assets/DefaultVolumeProfile.asset
    Assets/UniversalRenderPipelineGlobalSettings.asset

    com.unity.render-pipelines.universal/Editor/AssetPostProcessors/UniversalRenderPipelineGlobalSettingsPostprocessor.cs
    似乎由其生成, 但也有其他路径的可能.
    UniversalRenderPipelineGlobalSettings.Ensure();

   URP Global Settings Asset has been created for you. If you want to modify it, go to Project Settings &gt; Graphics &gt; URP Global Settings Asset
UnityEditor.AssetPostprocessingInternal:PostprocessAllAssets (string[],string[],string[],string[],string[],bool) (at /home/bokken/build/output/unity/unity/Editor/Mono/AssetPostprocessor.cs:395)

-   修改
     Packages/manifest.json
    "com.unity.render-pipelines.universal": "17.2.0",

    依赖:
    Packages/packages-lock.json
    com.unity.render-pipelines.core
    Scriptable Render Pipeline Core
    Shader Graph
    Universal Render Pipeline Config

    Package Manager截图展示依赖
-   URP config的意义


## URP包迁移 {#urp包迁移}

XXXXX: 迁移至Packages目录, 删除@后面的hash. 是否有必要??? 因为总会在packagecache生成???
放入Package目录显示custom, 其他目录再添加显示为Local, 是否有区别? Local时是否影响那个包的变量?
是否影响
4:#include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Lighting.hlsl"

Package Manager窗口展示添加package


## URP, SRP包, 改名字 {#urp-srp包-改名字}

6.1版本改名


## URP包和Unity版本之间关系 {#urp包和unity版本之间关系}


## 新建asset {#新建asset}

 默认有两种, URP/2D
 指定Graphic Setting中Pipeline asset
 ProjectSettings/GraphicsSettings.asset发生变化,
   m_CustomRenderPipeline: {fileID: 11400000, guid: bf0b5430a3999018998d23d3dfb55377, type: 2}
Assets/DefaultVolumeProfile.asset也有很大变化

新增了ProjectSettings/URPProjectSettings.asset文件


## 运行 {#运行}

此时已经是URP渲染流程了.


## <https://github.com/Unity-Technologies/UnityCsReference> {#https-github-dot-com-unity-technologies-unitycsreference}

最好clone到本地, 并切换到准确的分支(tag)
根据当前Unity版本(????)
