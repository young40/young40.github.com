+++
title = "领读URP 004: 调试工具"
date = 2025-04-30T10:38:23+08:00
lastmod = 2025-04-30T10:38:30+08:00
tags = ["Unity", "URP"]
categories = ["领读URP"]
draft = true
+++

## 三个文件 {#三个文件}

UniversalRenderPipelineAsset.cs
UniversalRenderPipelineAsset.DefaultResources.cs
UniversalRenderPipelineAssetPrefiltering.cs

class UniversalRenderPipelineAsset


## 功能 {#功能}

-   保存asset文件
-   创建asset文件
-   升级asset文件版本
-   各种enum
-   创建CreatePipeline
-   提供默认???使用的材质和shader
-   proflteri???
-   实现了几个接口


## 基类 {#基类}

RenderPipelineAsset
