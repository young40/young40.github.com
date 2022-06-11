+++
title = "UGUI源码分析(一): Image的渲染"
date = 2022-01-03T00:13:53+08:00
lastmod = 2022-06-11T10:26:44+08:00
tags = ["Unity", "UGUI"]
categories = ["UGUI源码分析"]
draft = false
+++

我们都知道在Unity中渲染一个3D的物体, 需要提供3D物体的模型, 纹理, 材质甚至Shader. 但我们在制作UI时, 并不是必须提供这些.
那UI是另起炉灶, 有一套新机制吗? 答案是否定的. 下面我们就从UGUI源码层面来看看Image的渲染.

<!--more-->


## 初遇Image组件 {#初遇image组件}

我们在Hierarchy窗口创建一个Image控件, 可以在Inspector窗口看到它由 `RectTransform`, `Canvans Renderer`, `Image` 几个Component组成.
我们可以给 `Image` 组件指定 `Source Image`, `Color`, `Material` 等属性.

我们可以看到 `Image` 组件的源码, 位于 `Runtime/UI/Core/Image.cs`, `Image` 渲染相关的类继承关系如下:


{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-001.Image-Hierarchy.png" >}}


## Mesh的生成 {#mesh的生成}

我们在一个测试Scene中加入一个Image, 并给 `Image` 类的 `OnPopulateMesh` 函数添加断点, 运行调试后, 可以看到如下调用堆栈:
![](/ox-hugo/2021-12-UGUI-Source-Reading-002.Debug-OnPopulateMesh.png)

从上图我们也可以看到, 如果 `activeSprite` 为空的情况下(一般可以理解为在Inspector窗口不设置Source Image), 则会直接调用
父类 `Graphic` 的 `OnPopulateMesh` 函数, 我们来看下其实现.

```csharp
protected virtual void OnPopulateMesh(VertexHelper vh)
{
    var r = GetPixelAdjustedRect();
    var v = new Vector4(r.x, r.y, r.x + r.width, r.y + r.height);

    Color32 color32 = color;
    vh.Clear();
    vh.AddVert(new Vector3(v.x, v.y), color32, new Vector2(0f, 0f)); //左下
    vh.AddVert(new Vector3(v.x, v.w), color32, new Vector2(0f, 1f)); //左上
    vh.AddVert(new Vector3(v.z, v.w), color32, new Vector2(1f, 1f)); //右上
    vh.AddVert(new Vector3(v.z, v.y), color32, new Vector2(1f, 0f)); //右下

    vh.AddTriangle(0, 1, 2);
    vh.AddTriangle(2, 3, 0);
}

```

我们知道, UI元素一般是按四边形来渲染, 即需要两个三角形. 在上面代码中, 可以看到变量 `v` 存储了四边形的左下角和右上角的坐标.
然后以顺时针顺序把左下角, 左上角, 右上角, 右下角, 当作四个顶点坐标传给 `VertexHelper`, 这四点的顶点索引(Index), 依次为0, 1, 2, 3.
分别以(0, 1, 2)和(2, 3, 0) 组成两个三角形, 构成了UI元素的Mesh.

我们也看到通过给 `VertexHelper` 的 `AddVert` 函数, 也设置了对应顶点的颜色及纹理坐标.

在Scene窗口的 Wireframe 模式下, 可以看到上述两个三角形组成的Mesh.
![](/ox-hugo/2021-12-UGUI-Source-Reading-003.Scene-Wireframe.png)

打开Frame Debug可以看到绘制参数, 和顶点信息, 4个顶点, 6个顶点索引.

{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-004.Frame-Debug.png" >}}

{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-005.Frame-Debug-Preview-Vertices.png" >}}

Image有多种类型, 这些类型生成Mesh的方式不同, 但都是通过 `OnPopulateMesh` 函数来提供的.

UI元素一定是按四边形来渲染吗? 答案是否定的. 当 `Image` 组件的 `Image Type` 为 `Simple` 并且勾选了 `Use Sprite Mesh` 时,
当前使用的Sprite包含透明的区域, 此时其 `Sprite Mesh` 可能不是四边形, 就不以四边形来渲染.

{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-006.Sprite-Mesh.png" >}}

`Image` 的部分参数变化, 可能也会引起Mesh的变化.
![](/ox-hugo/2021-12-UGUI-Source-Reading-007.Image-Mesh-Changed.png)


## 材质的选择 {#材质的选择}

我们从 Inspector 窗口可以看到 `Image` 组件可以设置 `Material` 属性. 实际上该属性由 `Graphic` 类提供.
材质相关的方法和属性及类继承关系如下图所示:


{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-008.Image-Hierarchy-Material.png" >}}

给 `Image` 的 `material` 属性的 getter 方法添加断点后, 调试运行, 可以看到如下调用堆栈:

{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-009.Debug-Material-getter.png" >}}

我们也从 `material` 的 getter 方法中看到, 在Inspector窗口设置的材质有最高优先级, 其次是如果给 `sprite` 设置了 `associatedAlphaSplitTexture`,
则会使用 `defaultETC1GraphicMaterial`, 最后才会使用 `defaultMaterial`.

追踪 `defaultMaterial` 的定义, 会发现它实际上是由 `Canvas.GetDefaultCanvasMaterial()` 提供,
但该方法实现在 Unity 的C++代码中, 并不开源. 根据网上资料所说, 该方法会优先调用 SPR 中提供的UI材质, 再调用内置的UI默认Shader生成材质.

我们也注意到, 在 `Graphic.materialForRendering` 属性中, 可以对材质进行修改.

这样我们就获得了渲染UI时用到的材质.


## Shader在哪里? {#shader在哪里}

材质使用的Shader在哪里呢?

我们可以在[Unity网站下载](https://unity3d.com/cn/get-unity/download/archive)到对应版本的内置Shader以作为参考. UI部分Shader位于 `./DefaultResourcesExtra/UI` 目录.


## 纹理的提供 {#纹理的提供}

可以在 `Inspector` 窗口给 `Image` 组件设置 `Source Image`, 为 `Sprite` 类型. `Sprite` 类位于 `UnityEngine` 命名空间下, 并非 UGUI 专用.

我们给 `Image` 的 `mainTexture` 属性的 getter 方法增加断点, 调试后可以看到如下堆栈:

{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-010.Debug-mainTexture-getter.png" >}}

我们看到首先如果有 `activeSprite` , 则会使用 `activeSprite.texture`. 再接着会尝试使用 `material` 中的纹理, 都不存在的情况下, 会使用默认的白色纹理.

`activeSprite` 会优先返回 `m_OverrideSprite`, 其次返回 `sprite` (即 `m_Sprite`, Inspector中设置的 Sprite).

纹理相关的方法和属性及类继承关系如下图所示:


{{< figure src="/ox-hugo/2021-12-UGUI-Source-Reading-011.Image-Texture-Hierarchy.png" >}}

我们上面分析Mesh的生成时, 提到 `VertexHelper` 需要纹理坐标. 我们可以再回头看下, `Image` 不同类型的 Mesh
是如何匹配其纹理坐标的.

例如, 我们在 `GenerateSimpleSprite` 可以看到:

```csharp
var uv = (activeSprite != null) ? Sprites.DataUtility.GetOuterUV(activeSprite) : Vector4.zero;
```


## 渲染一下 {#渲染一下}

我们从上面调试时的堆栈可以看到, 这些调用都是由 `Graphic.Rebuild` 触发, 代码如下:

```csharp
public virtual void Rebuild(CanvasUpdate update)
{
    if (canvasRenderer == null || canvasRenderer.cull)
        return;

    switch (update)
    {
        case CanvasUpdate.PreRender:
            if (m_VertsDirty)
            {
                UpdateGeometry();
                m_VertsDirty = false;
            }
            if (m_MaterialDirty)
            {
                UpdateMaterial();
                m_MaterialDirty = false;
            }
            break;
    }
}
```

我们查看 `UpdateGeometry` 和 `UpdateMaterial` 的实现, 就可以看到生成的Mesh以及被选择的纹理, 材质等, 分别设置给了
`CanvasRenderer`, 最终由 `CanvasRenderer` 来负责 `Image` (也包括所有 `Graphic` 的子类) 的渲染.
