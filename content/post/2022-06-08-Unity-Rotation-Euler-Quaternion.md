+++
title = "Unity, 旋转, 万向锁, 欧拉角和四元数"
date = 2022-06-08T23:52:01+08:00
lastmod = 2022-06-13T20:38:56+08:00
tags = ["Unity"]
categories = ["Unity"]
draft = true
+++

旋转, 处处都能看到的一种现象. 大到宇宙中星体的自转、公转, 小到车辆轮子的运动, 甚至我们的手一伸一握, 都包含着指节围绕关节的旋转.
你可能会想, 这么常见的东西一定很简单吧? 其实并没有那么简单.
作为一名游戏开发工程师, 我们能在网上找到关于欧拉角, 四元数, 万向锁的资料数不胜数, 但大都不成体系.
而且研究比较深入的反而是在运动控制, 飞行控制领域. 可能飞控不好就会炸机...

下面让我们来看下Unity中旋转相关的知识点.


## Unity中的旋转 {#unity中的旋转}

手头有一个简易的飞机模型, 放在Unity里正好合适, 你也可以选择一款自己喜欢的机型. (一方面, 旋转中的几个术语来源于飞行.
另一方面, 飞机模型比起球体, 立方体来讲更加直观.)

{{< figure src="/ox-hugo/2022-06-Rotation-001.Plane.png" >}}


### 先转一下 {#先转一下}

我们在Inspector中, 可以让飞机分别绕X, Y, Z轴旋转一下. 可以看到这三种旋转分别对应了飞机可以做的三种基本旋转:

| 轴 | 英文  | 中文 |
|---|-----|----|
| X | pitch | 俯仰角 |
| Y | yaw   | 偏航角 |
| Z | roll  | 滚转角 |

 <video width="680" height="570" controls>
 <source src="/ox-hugo/2022-06-Rotation-002.Plane-Basic-Rotation.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

PS: X,Y,Z轴的颜色正好和三原色Red,Green,Blue对应. 在游戏开发中我们还能在其他地方也看到这种对应关系.


### 当X轴旋转&plusmn;90&deg;时 {#当x轴旋转-and-plusmn-90-and-deg-时}

我们先在Inspector中将飞机绕X轴旋转90&deg;, 这时再操作Y轴与Z轴, 我们可以惊奇地发现, 旋转效果 `似乎` 是完全一致的.
我们再也不能通过旋转Y轴, 让飞机偏航(yaw)了.

 <video width="680" height="570" controls>
 <source src="/ox-hugo/2022-06-Rotation-003.Plane-X-90.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

此时(保持绕X轴旋转90&deg;), 我们先后操作Y轴和Z轴相同的度数(例如30&deg;), 我们会发现两次操作抵消了, 飞机的姿态回到了原位, 即Y,Z轴都是0&deg;的状态.

我们再让飞机的X轴旋转-90&deg;, 再操作Y轴和Z轴一定的度数(例如各30&deg;), 可以看到其效果等同于单独操作Y轴或Z轴其度数之和(30&deg;+30&deg;=60&deg;).


### 不可跨越的90&deg; {#不可跨越的90-and-deg}

Unity提供了多种旋转的Api, 其中由对象的transform提供的eulerAngles可以获取或设置欧拉角.
我们尝试用其让飞机做持续旋转的动画.

我们使用如下代码:

```csharp
void Update()
{
    transform.eulerAngles += new Vector3(30 * Time.deltaTime, 0, 0);
}
```

效果如下:

 <video width="680" height="570" controls>
 <source src="/ox-hugo/2022-06-Rotation-004.Plane-Rotation-Cant-Beyond-90.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

本来期望飞机可以绕X轴持续进行360&deg;旋转, 但可以看到从0&deg;旋转到90&deg;附近无法跨越过去, 在90&deg;附近反复横跳.
同样如果我们从0&deg;开始, 以上述反方向旋转, 则无法跨过-90&deg;.

这些看起来还真是奇怪的问题呢!


## 旋转基础 {#旋转基础}


### 三角函数 {#三角函数}

想要深入研究旋转, 基础的三角函数是必须的. 但我们在这里就不展开了, 仅给出需要了解的部分定理或公式.

-   和差化积公式
-   半角/倍角公式


### 二维空间旋转 {#二维空间旋转}

二维空间中, 物体只能围绕某一个点旋转, 我们一般选择绕原点旋转.

<a id="figure--https:--www.geogebra.org-calculator-eebadpkb"></a>

{{< figure src="/ox-hugo/2022-06-Rotation-005.2D-Rotation.svg" >}}

在上图中, 我们通过将原基向量\\(p\\), \\(q\\) 旋转&theta;&deg;得到新的基向量\\(p'\\), \\(q'\\),
所以可以得到二维空间的旋转矩阵:

\\[
R(\theta) = \begin{bmatrix}p' \\\\ q' \\\\ \end{bmatrix}
= \begin{bmatrix}
cos(\theta)  & sin(\theta) \\\\
-sin(\theta) & cos(\theta) \\\\
\end{bmatrix}
\\]

> 之所以选择原点是因为我们总是可以对物体做平移操作, 先平移至原点, 旋转之后再平移回去.
> 从数学上讲, 平移操作是非线形操作, 我们需要将二维空间升至三维空间做平移, 即引入齐次坐标.
> 平移和旋转恰好可以互不影响地存在于升维后的矩阵中, 所以我们可以简便地采用以原点为中心点做旋转.


### 坐标系与旋转正方向 {#坐标系与旋转正方向}

在三维空间中, 左手坐标系中采用左手法则确定旋转正方向, 右手坐标系采用右手法则确定旋转正方向. Unity中采用了左手坐标系, 因此我们下面提到的旋转都遵循左手法则.


### 绕坐标轴旋转 {#绕坐标轴旋转}

我们先看一下三维空间中绕x轴旋转. 因为绕x轴旋转, 则其x坐标不变, 也意味着其x基向量不变, 与二维空间的旋转类似.

<a id="figure--https:--www.geogebra.org-calculator-xvzyhqhr"></a>

{{< figure src="/ox-hugo/2022-06-Rotation-006.3D-Rotation-X-Axis.png" >}}

由上图我们可以得到原基向量\\(p\\), \\(q\\), \\(r\\) 旋转&theta;&deg;后得到新的基向量\\(p'\\), \\(q'\\), \\(r'\\), 也得到三维空间中绕x轴的旋转矩阵:

\\[
R\_x(\theta) = \begin{bmatrix}p' \\\ q' \\\ r' \\\ \end{bmatrix}
= \begin{bmatrix}
1 & 0       & 0      \\\\
0 & cos(\theta)  & sin(\theta) \\\\
0 & -sin(\theta) & cos(\theta) \\\\
\end{bmatrix}
\\]

同理, 我们可以很快得出绕y轴的旋转矩阵.

\\[
R\_y(\theta) = \begin{bmatrix} p' \\\\ q' \\\\ r' \\\\ \end{bmatrix}
= \begin{bmatrix}
cos(\theta) & 0 & -sin(\theta) \\\\
0      & 1 & 0       \\\\
sin(\theta) & 0 & cos(\theta)  \\\\
\end{bmatrix}
\\]

以及绕z轴的旋转矩阵.

\\[
R\_z(\theta) = \begin{bmatrix} p' \\\\ q' \\\\ r' \\\\ \end{bmatrix}
= \begin{bmatrix}
cos(\theta)  & sin(\theta) & 0 \\\\
-sin(0) & cos(0) & 0 \\\\
0       & 0      & 1 \\\\
\end{bmatrix}
\\]


### 绕任意轴的旋转 {#绕任意轴的旋转}

旋转也可以围绕任意的轴, 我们仍然不考虑平移的问题, 假设旋转轴通过原点.

<a id="figure--https:--www.geogebra.org-calculator-tdqqruce"></a>

{{< figure src="/ox-hugo/2022-06-Rotation-007.3D-Rotation-By-Any.png" >}}

如上图所示, 向量 \\(v\\) (米黄色)围绕单位向量 \\(n\\) 旋转 &theta;&deg;, 得到新的向量 \\(v'\\)(土黄色).
\\(v\\) 可以分解为和 \\(n\\) 平行的和垂直的两个变量 \\(v\_{\parallel}\\) (红色), \\(v\_{\perp}\\)(绿色), 既\\(v=v\_{\parallel}+v\_{\perp}\\).
旋转后 \\(v\_{\parallel}\\) 因为和 \\(n\\) 平行不受影响, \\(v\_{\perp}\\) 变为 \\(v\_{\perp}^{\prime}\\).
我们可以看到 \\(v\_{\perp}\\) 和 \\(v\_{\perp}^{\prime}\\) 之间的夹角为 &theta; (注意: 不是向量 \\(v\\) 和 \\(v'\\) 之间的夹角为 &theta;).
还添加了一个与 \\(n\\) 和 \\(v\_{\perp}\\) 都垂直的向量 \\(w\\), \\(w\\) 的长度和 \\(v\_{\perp}\\) 及 \\(v\_{\perp}^{\prime}\\) 相等.

根据这些已知的关系, 我们有以下等式:

\\(v\_{\parallel}\\) 可以看做 \\(v\\) 在 \\(n\\) 上的投影, 我们有:

\\[ v\_{\parallel} = (v\cdot{}n)n \tag1 \\]

\\(v\\) 是其平行于和垂直于 \\(n\\) 的向量和:

\\[ v\_{\perp} = v - v\_{\parallel} = v - (v \cdot n)n \tag2 \\]

\\(n\\) 和 \\(v\_{\parallel}\\) 平行:

\\[ n \times v\_{\parallel} = 0 \tag3 \\]

\\(w\\) 由 \\(n\\) 和 \\(v\_{\perp}\\) 叉乘所得:

\\[\begin{eqnarray}
w &=& n \times v\_{\perp}                   \\\\\
  &=& n \times (v - v\_{\parallel} )        \\\\\
  &=& n \times v - n \times v\_{\parallel}  \\\\\
  &=& n \times v - 0                       \\\\\
  &=& n \times v                           \tag4
\end{eqnarray}\\]

我们首先可以看到, \\(v\_{\perp}^{\prime}\\) 可以看做其在 \\(v\_{\perp}\\) 和 \\(w\\) 上的投影向量之和, 所以我们有:

\\[\begin{eqnarray}
v\_{\perp}^{\prime} &=& v\_{\perp}cos(\theta) + wsin(\theta) \\\\\
                   &=& (v - (v \cdot n)n)cos(\theta) + (n \times v)sin(\theta)  \tag5
\end{eqnarray}\\]

旋转后的向量 \\(v'\\):

\\[\begin{eqnarray}
v' &=& v\_{\parallel} + v\_{\perp}^{\prime} \\\\
   &=& (v \cdot n)n + (v - (v \cdot n)n)cos(\theta) + (n \times v)sin(\theta)  \tag6
\end{eqnarray}\\]

由此我们得出了任意向量 \\(v\\) 绕单位向量 \\(n\\) 旋转 &theta; 后的向量 \\(v'\\) 公式.

我们知道旋转矩阵由其旋转后的坐标系的基向量组成, 我们使用上面公式(6), 就可以求解新的基向量以及旋转矩阵.

\\[
R(n, \theta) = \begin{bmatrix}
p^{\prime} \\\ q^{\prime} \\\ r^{\prime}
\end{bmatrix}
\\]

> 矩阵形式太复杂了, 暂时先不详细列出了.


## 旋转的几种表示方法 {#旋转的几种表示方法}

旋转有多种表示方法, 例如轴角法, 旋转矩阵, 欧拉角, 四元数, 游戏引擎中用到了多种旋转表示方法. 我们来看看这些不同表示方法之间的联系.


### 旋转矩阵 {#旋转矩阵}

我们可以用基向量 \\(p, q, r\\) 旋转后的基向量 \\(p', q', r'\\) 组成 3&times;3 旋转矩阵. 旋转矩阵为正交矩阵.

正交矩阵就意味着, 其转置矩阵等于其逆矩阵, 并且转置矩阵和原矩阵乘积为单位矩阵I.

\\[ M^T = M^{-1} \\]
\\[ MM^T = MM^{-1} = I \\]

-   旋转矩阵的优点
    -   矩阵形式可以方便地在物体坐标系和惯性坐标系之间旋转向量.
    -   常见的图形学API都是使用矩阵. 无论如何存储旋转数据, 在最终渲染前都需要在某处将其转换为矩阵形式.
    -   方便坐标系嵌套类型的计算. 只需要将矩阵依次相乘即可得到最终的旋转矩阵.
    -   求逆向旋转方便. 逆向旋转需要逆矩阵, 而逆矩阵与转置矩阵相同, 通过简单的转置即可求得.

-   旋转矩阵的缺点
    -   需要更多内存. 矩阵需要存储9个数字, 相比欧拉角只需要3个数字, 四元数只需要4个数字.
    -   矩阵非常不直观, 也几乎不可能不借助计算机计算出任意方位的旋转矩阵.
    -   矩阵可能不是合法的旋转矩阵. 矩阵蠕变可能会导致旋转矩阵不合法, 需要通过矩阵正交化解决.

> 矩阵有9个数字组成, 这相当于给旋转矩阵带来了9个自由度. 但显然不是随意填入9个数字都能是旋转矩阵.
> 旋转矩阵要求其3个行向量是单位矩阵, 并且3个行向量需要互相垂直.我们也可以理解为, 我们把三维空间的问题转化为九维空间带有六个约束的问题.


### 欧拉角 {#欧拉角}

数学家欧拉证明了单个角位移可以由多个角位移组合而成.


#### 旋转顺序 {#旋转顺序}


### 四元数 {#四元数}

Unity在引擎内部使用四元数存储旋转和方位, 在其文档中[^fn:1]有明确指出.


## 参考文档 {#参考文档}

[^fn:1]: [Rotation and orientation in Unity(Unity官方文档)](https://docs.unity3d.com/Manual/QuaternionAndEulerRotationsInUnity.html)