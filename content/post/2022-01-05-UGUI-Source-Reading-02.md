+++
title = "UGUI源码分析(一): Mask 面具🎭之下"
date = 2022-01-05T22:12:45+08:00
lastmod = 2022-01-10T17:15:26+08:00
tags = ["Unity", "UGUI"]
categories = ["UGUI源码分析"]
draft = true
+++

`Mask 遮罩`  可以按自身的形状限制子元素的显示范围. 这句话有点抽象, 实际上自己动手做一下就很好理解.
例如, 我们常见的显示圆形头像的效果, 就是拿一个圆形的图像来限制方形头像的显示范围, 就能得到圆形头像.
下面我们就来看看Mask的背后的原理和其源码.


## 模板缓冲区和模板测试 {#模板缓冲区和模板测试}

`Mask` 的原理是模板缓冲区和模板测试. 模板缓冲区给每个像素分配一个8位的模板值, 渲染 `Mask` 时会写入指定的值.
子元素在渲染时, 会做模板测试, 如果不通过则舍弃子元素的片元, 通过则会正常渲染, 以此达到控制子元素显示范围的目的.


## Mask相关的类 {#mask相关的类}

`Mask` 相关的类和属性如下:


{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-012.Mask-Hierarchy.png" >}}


## 一个Demo {#一个demo}

我们使用下面两张图来演示 `Mask` 的效果.

![](/ox-hugo/2021-12-UGUI-Source-Reading-014.Mask-Red-Hole.png)
![](/ox-hugo/2021-12-UGUI-Source-Reading-013.Mask-Blue.png)

我们新建两个 Image 控件, 分别使用这两张图做Sprite, 蓝色控件作为子对象, 并且给红色控件添加 `Mask` 组件.

我们得到如下图效果, 我们看到了蓝色图片, 但按照红色图片的形状显示.

{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-015.Mask-Scene-Blue-Hole.png" >}}


## Mask的面具之下 {#mask的面具之下}

我们可以在Inspector里观察下, 禁用和启用 `Mask` 组件, 对 Image 控件使用材质的影响.

![](/ox-hugo/2021-12-UGUI-Source-Reading-016.Mask-Red-Disable.png)
![](/ox-hugo/2021-12-UGUI-Source-Reading-017.Mask-Red-Enable.png)

可以看到禁用 `Mask` 组件后, 使用的是 `Default UI Material`, 启用之后使用的材质被修改. 同时, 也能发现
子对象的材质也被修改.

我们打开 `Runtime/UI/Core/Mask.cs`, 并找到 `OnEnable` 方法代码如下:

```csharp
protected override void OnEnable()
{
    base.OnEnable();
    if (graphic != null)
    {
        graphic.canvasRenderer.hasPopInstruction = true;
        graphic.SetMaterialDirty();

        // Default the graphic to being the maskable graphic if its found.
        if (graphic is MaskableGraphic)
            (graphic as MaskableGraphic).isMaskingGraphic = true;
    }

    MaskUtilities.NotifyStencilStateChanged(this);
}
```

可以看到如果 `graphic is MaskableGraphic` 就设置 `isMaskingGraphic` 为 `true` , 这里的 `graphic`, 实际上就
是红色Image的 `Image` 组件.

然后调用了 `MaskUtilities.NotifyStencilStateChanged(this)`, 我们打开其实现.

```csharp
public static void NotifyStencilStateChanged(Component mask)
{
    var components = ListPool<Component>.Get();
    mask.GetComponentsInChildren(components);
    for (var i = 0; i < components.Count; i++)
    {
        if (components[i] == null || components[i].gameObject == mask.gameObject)
            continue;

        var toNotify = components[i] as IMaskable;
        if (toNotify != null)
            toNotify.RecalculateMasking();
    }
    ListPool<Component>.Release(components);
}
```

从上面这段代码中可以看到, `NotifyStencilStateChanged` 会寻找子对象上所有的实现了 `IMaskable` 接口的组件,
并调用其 `RecalculateMasking` 方法.

我们看下 `MaskableGraphic.RecalculateMasking` 方法:

```csharp
public virtual void RecalculateMasking()
{
    // Remove the material reference as either the graphic of the mask has been enable/ disabled.
    // This will cause the material to be repopulated from the original if need be. (case 994413)
    StencilMaterial.Remove(m_MaskMaterial);
    m_MaskMaterial = null;
    m_ShouldRecalculateStencil = true;
    SetMaterialDirty();
}
```

这里主要是将 `m_ShouldRecalculateStencil` 置为 true.

此时, 我们依然没有看到对材质的修改. 材质的修改在什么地方呢? 还记得[上篇](https://www.young40.com/post/2021-12-26-ugui-source-reading-01/) 中提到的 `Graphic.materialForRendering` 可能对
材质进行修改吗? 是的, 材质就是在这里通过 `IMaterialModifier` 接口的 `GetModifiedMaterial` 方法来实现的.

`Mask` 组件和 `MaskableGraphic` 都实现了 `IMaterialModifier` 接口, 我们先看下 `Mask` 的 `GetModifiedMaterial` 方法:

```csharp
public virtual Material GetModifiedMaterial(Material baseMaterial)
{
    if (!MaskEnabled())
        return baseMaterial;

    var rootSortCanvas = MaskUtilities.FindRootSortOverrideCanvas(transform);
    var stencilDepth = MaskUtilities.GetStencilDepth(transform, rootSortCanvas);
    if (stencilDepth >= 8)
    {
        Debug.LogWarning("Attempting to use a stencil mask with depth > 8", gameObject);
        return baseMaterial;
    }

    int desiredStencilBit = 1 << stencilDepth;

    // if we are at the first level...
    // we want to destroy what is there
    if (desiredStencilBit == 1)
    {
        var maskMaterial = StencilMaterial.Add(baseMaterial, 1, StencilOp.Replace, CompareFunction.Always, m_ShowMaskGraphic ? ColorWriteMask.All : 0);
        StencilMaterial.Remove(m_MaskMaterial);
        m_MaskMaterial = maskMaterial;

        var unmaskMaterial = StencilMaterial.Add(baseMaterial, 1, StencilOp.Zero, CompareFunction.Always, 0);
        StencilMaterial.Remove(m_UnmaskMaterial);
        m_UnmaskMaterial = unmaskMaterial;
        graphic.canvasRenderer.popMaterialCount = 1;
        graphic.canvasRenderer.SetPopMaterial(m_UnmaskMaterial, 0);

        return m_MaskMaterial;
    }

    //otherwise we need to be a bit smarter and set some read / write masks
    var maskMaterial2 = StencilMaterial.Add(baseMaterial, desiredStencilBit | (desiredStencilBit - 1), StencilOp.Replace, CompareFunction.Equal, m_ShowMaskGraphic ? ColorWriteMask.All : 0, desiredStencilBit - 1, desiredStencilBit | (desiredStencilBit - 1));
    StencilMaterial.Remove(m_MaskMaterial);
    m_MaskMaterial = maskMaterial2;

    graphic.canvasRenderer.hasPopInstruction = true;
    var unmaskMaterial2 = StencilMaterial.Add(baseMaterial, desiredStencilBit - 1, StencilOp.Replace, CompareFunction.Equal, 0, desiredStencilBit - 1, desiredStencilBit | (desiredStencilBit - 1));
    StencilMaterial.Remove(m_UnmaskMaterial);
    m_UnmaskMaterial = unmaskMaterial2;
    graphic.canvasRenderer.popMaterialCount = 1;
    graphic.canvasRenderer.SetPopMaterial(m_UnmaskMaterial, 0);

    return m_MaskMaterial;
}
```

这段代码的逻辑比较简单, 首先计算了当前对象到所在 `Canvas` 的模版深度, 即其所有父对象中拥有 `Mask` 组件的数量.

利用其模板深度做位运算, 计算出掩码值. 然后利用 `StencilMaterial.Add` 修改材质. 我们查看下其源代码, 替换材质部分核心源码如下:

```csharp
newEnt.customMat.SetFloat("_Stencil", (float)stencilID);
newEnt.customMat.SetFloat("_StencilOp", (float)operation);
newEnt.customMat.SetFloat("_StencilComp", (float)compareFunction);
newEnt.customMat.SetFloat("_StencilReadMask", (float)readMask);
newEnt.customMat.SetFloat("_StencilWriteMask", (float)writeMask);
```

要结合着 Shader 部分的代码一起来看, 可以在 `UI-Default.shader` 中找到模板相关代码如下:

```glsl
Properties
{
    _StencilComp ("Stencil Comparison", Float) = 8
    _Stencil ("Stencil ID", Float) = 0
    _StencilOp ("Stencil Operation", Float) = 0
    _StencilWriteMask ("Stencil Write Mask", Float) = 255
    _StencilReadMask ("Stencil Read Mask", Float) = 255

    _ColorMask ("Color Mask", Float) = 15
}

SubShader
{
  Stencil
  {
      Ref [_Stencil]
      Comp [_StencilComp]
      Pass [_StencilOp]
      ReadMask [_StencilReadMask]
      WriteMask [_StencilWriteMask]
   }
}
```

意味着实际上是通过控制Shader中 `Stencil` 配置, 来达到写入模版值或者模版测试的目的.

下面这段代码的 `Stencil` 的配置为:

```csharp
StencilMaterial.Add(baseMaterial, 1, StencilOp.Replace, CompareFunction.Always, m_ShowMaskGraphic ? ColorWriteMask.All : 0);
```

```glsl
Stencil {
  Ref 1
  Comp Always
  Pass Replace
  ReadMask 255
  WriteMask 255
}

```

这段配置的含义是, 将参考值 1 写入模板缓冲区.

下面这段代码的 `Stencil` 的配置为:

```csharp
StencilMaterial.Add(baseMaterial, 1, StencilOp.Zero, CompareFunction.Always, 0);
```

```glsl
Stencil {
  Ref 1
  Comp Always
  Pass Zero
  ReadMask 255
  WriteMask 255
}
```

这段配置的含义是 将参考值 0 写入模板缓冲区(Pass Zero的含义如此, 应该是会忽略其他配置).
这个材质被 `canvasRenderer.SetPopMaterial` 调用, 具体源码在Unity的C++部分, 各种资料也语焉不详, 但作用是清空了模板缓存.

我们在上面看到了 `Mask` 组件, 写入和


## 参考 {#参考}

-   <https://www.cnblogs.com/iwiniwin/p/15131528.html>
-   <https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/02%20Stencil%20testing/>
-   <https://docs.unity3d.com/cn/2019.4/Manual/script-Mask.html>
-   <https://docs.unity3d.com/cn/2021.2/Manual/SL-Stencil.html>
