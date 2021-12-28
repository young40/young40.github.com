+++
title = "UGUI源码分析(一): Image的渲染 "
date = 2021-12-26T11:33:24+08:00
lastmod = 2021-12-28T15:26:58+08:00
tags = ["Unity", "UGUI"]
categories = ["UGUI源码分析"]
draft = true
+++

我们都知道在Unity中渲染一个3D的物体, 需要提供3D物体的模型, 纹理, 材质甚至Shader. 但我们在制作UI时, 并不需要提供这些(Image还是可以提供材质??????????).
那UI是另起炉灶, 有一套新机制吗? 答案是否定的. 下面我们就从UGUI源码层面来看看Image的渲染.

<!--more-->


## 初遇Image组件 {#初遇image组件}

我们在Hierarchy窗口创建一个Image(什么? 对象??), 可以在Inspector窗口看到它由 `RectTransform`, `Canvans Renderer`, `Image` 几个Component组成.
我们可以给 `Image` 组件指定 `Source Image`, `Color`, `Material` 等属性.

我们可以看到 `Image` 组件的源码, 位于 `Runtime/UI/Core/Image.cs`, `Image` 类继承关系如下:

```plantuml
@startuml

namespace UnityEngine.UI {
    class Image
    class MaskableGraphic
    class Graphic
    class UIBehaviour
    class MonoBehaviour

    MaskableGraphic <|-- Image
    Graphic         <|-- MaskableGraphic
    UIBehaviour     <|-- Graphic
    MonoBehaviour   <|-- UIBehaviour
}

@enduml
```

{{< figure src="/ox-hugo/001.Image-Hierarchy.png" >}}
