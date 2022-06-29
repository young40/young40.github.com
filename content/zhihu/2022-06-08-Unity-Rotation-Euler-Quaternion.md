








旋转, 处处都能看到的一种现象. 大到宇宙中星体的自转、公转, 小到车辆轮子的运动, 甚至我们的手一伸一握, 都包含着指节围绕关节的旋转.
你可能会想, 这么常见的东西一定很简单吧? 其实并没有那么简单.
作为一名游戏开发工程师, 我们能在网上找到关于欧拉角, 四元数, 万向锁的资料数不胜数, 但大都不成体系.
而且研究比较深入的反而是在运动控制, 飞行控制领域. 可能飞控不好就会炸机...

下面让我们来看下Unity中旋转相关的知识点.


## Unity中的旋转 

手头有一个简易的飞机模型, 放在Unity里正好合适, 你也可以选择一款自己喜欢的机型. (一方面, 旋转中的几个术语来源于飞行.
另一方面, 飞机模型比起球体, 立方体来讲更加直观.)

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-001.Plane.png" />


### 先转一下 

我们在Inspector中, 可以让飞机分别绕X, Y, Z轴旋转一下. 可以看到这三种旋转分别对应了飞机可以做的三种基本旋转:

| 轴 | 英文  | 中文 |
|---|-----|----|
| X | pitch | 俯仰角 |
| Y | yaw   | 偏航角 |
| Z | roll  | 滚转角 |

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-002.Plane-Basic-Rotation.gif" />

> X,Y,Z轴的颜色正好和三原色Red,Green,Blue对应. 在游戏开发中我们还能在其他地方也看到这种对应关系.


### 当X轴旋转&plusmn;90&deg;时 

我们先在Inspector中将飞机绕X轴旋转90&deg;, 这时再操作Y轴与Z轴, 我们可以惊奇地发现, 旋转效果 `似乎` 是完全一致的.
我们再也不能通过旋转Y轴, 让飞机偏航(yaw)了.

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-003.Plane-X-90.gif" />

此时(保持绕X轴旋转90&deg;), 我们先后操作Y轴和Z轴相同的度数(例如30&deg;), 我们会发现两次操作抵消了, 飞机的姿态回到了原位, 即Y,Z轴都是0&deg;的状态.

我们再让飞机的X轴旋转-90&deg;, 再操作Y轴和Z轴一定的度数(例如各30&deg;), 可以看到其效果等同于单独操作Y轴或Z轴其度数之和(30&deg;+30&deg;=60&deg;).


### 不可跨越的90&deg; 

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

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-004.Plane-Rotation-Cant-Beyond-90.gif" />

本来期望飞机可以绕X轴持续进行360&deg;旋转, 但可以看到从0&deg;旋转到90&deg;附近无法跨越过去, 在90&deg;附近反复横跳.
同样如果我们从0&deg;开始, 以上述反方向旋转, 则无法跨过-90&deg;.

这些看起来还真是奇怪的问题呢!


## 旋转基础 


### 三角函数 

想要深入研究旋转, 基础的三角函数是必须的. 但我们在这里就不展开了, 仅给出需要了解的部分定理或公式.

-   和差化积公式
-   半角/倍角公式


### 二维空间旋转 

二维空间中, 物体只能围绕某一个点旋转, 我们一般选择绕原点旋转.

<a id="figure--https:--www.geogebra.org-calculator-eebadpkb"></a>

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-005.2D-Rotation.png" />

在上图中, 我们通过将原基向量 $$p$$ ,  $$q$$  旋转&theta;&deg;得到新的基向量 $$p'$$ ,  $$q'$$ ,
所以可以得到二维空间的旋转矩阵:

$$
R(\theta) = \begin{bmatrix}p' \\ q' \\ \end{bmatrix}
= \begin{bmatrix}
cos(\theta)  & sin(\theta) \\
-sin(\theta) & cos(\theta) \\
\end{bmatrix}
\\
$$

> 之所以选择原点是因为我们总是可以对物体做平移操作, 先平移至原点, 旋转之后再平移回去.
> 从数学上讲, 平移操作是非线形操作, 我们需要将二维空间升至三维空间做平移, 即引入齐次坐标.
> 平移和旋转恰好可以互不影响地存在于升维后的矩阵中, 所以我们可以简便地采用以原点为中心点做旋转.


### 坐标系与旋转正方向 

在三维空间中, 左手坐标系中采用左手法则确定旋转正方向, 右手坐标系采用右手法则确定旋转正方向. Unity中采用了左手坐标系, 因此我们下面提到的旋转都遵循左手法则.


### 绕坐标轴旋转 

我们先看一下三维空间中绕x轴旋转. 因为绕x轴旋转, 则其x坐标不变, 也意味着其x基向量不变, 与二维空间的旋转类似.

<a id="figure--https:--www.geogebra.org-calculator-xvzyhqhr"></a>

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-006.3D-Rotation-X-Axis.png" />

由上图我们可以得到原基向量 $$p$$ ,  $$q$$ ,  $$r$$  旋转&theta;&deg;后得到新的基向量 $$p'$$ ,  $$q'$$ ,  $$r'$$ , 也得到三维空间中绕x轴的旋转矩阵:

$$
R_x(\theta) = \begin{bmatrix}p' \\ q' \\ r' \\ \end{bmatrix}
= \begin{bmatrix}
1 & 0       & 0      \\
0 & cos(\theta)  & sin(\theta) \\
0 & -sin(\theta) & cos(\theta) \\
\end{bmatrix}
\\
$$

同理, 我们可以很快得出绕y轴的旋转矩阵.

$$
R_y(\theta) = \begin{bmatrix} p' \\ q' \\ r' \\ \end{bmatrix}
= \begin{bmatrix}
cos(\theta) & 0 & -sin(\theta) \\
0      & 1 & 0       \\
sin(\theta) & 0 & cos(\theta)  \\
\end{bmatrix}
\\
$$

以及绕z轴的旋转矩阵.

$$
R_z(\theta) = \begin{bmatrix} p' \\ q' \\ r' \\ \end{bmatrix}
= \begin{bmatrix}
cos(\theta)  & sin(\theta) & 0 \\
-sin(0) & cos(0) & 0 \\
0       & 0      & 1 \\
\end{bmatrix}
\\
$$


### 绕任意轴的旋转 

旋转也可以围绕任意的轴, 我们仍然不考虑平移的问题, 假设旋转轴通过原点.

<a id="figure--https:--www.geogebra.org-calculator-tdqqruce"></a>

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-007.3D-Rotation-By-Any.png" />

如上图所示, 向量  $$v$$  (米黄色)围绕单位向量  $$n$$  旋转 &theta;&deg;, 得到新的向量  $$v'$$ (土黄色).
 $$v$$  可以分解为和  $$n$$  平行的和垂直的两个变量  $$v_{\parallel}$$  (红色),  $$v_{\perp}$$ (绿色), 既 $$v=v_{\parallel}+v_{\perp}$$ .
旋转后  $$v_{\parallel}$$  因为和  $$n$$  平行不受影响,  $$v_{\perp}$$  变为  $$v_{\perp}^{\prime}$$ .
我们可以看到  $$v_{\perp}$$  和  $$v_{\perp}^{\prime}$$  之间的夹角为 &theta; (注意: 不是向量  $$v$$  和  $$v'$$  之间的夹角为 &theta;).
还添加了一个与  $$n$$  和  $$v_{\perp}$$  都垂直的向量  $$w$$ ,  $$w$$  的长度和  $$v_{\perp}$$  及  $$v_{\perp}^{\prime}$$  相等.

根据这些已知的关系, 我们有以下等式:

 $$v_{\parallel}$$  可以看做  $$v$$  在  $$n$$  上的投影, 我们有:

$$ v_{\parallel} = (v\cdot{}n)n \tag1 \\
$$

 $$v$$  是其平行于和垂直于  $$n$$  的向量和:

$$ v_{\perp} = v - v_{\parallel} = v - (v \cdot n)n \tag2 \\
$$

 $$n$$  和  $$v_{\parallel}$$  平行:

$$ n \times v_{\parallel} = 0 \tag3 \\
$$

 $$w$$  由  $$n$$  和  $$v_{\perp}$$  叉乘所得:

$$\begin{eqnarray}
w &=& n \times v_{\perp}                   \\
  &=& n \times (v - v_{\parallel} )        \\
  &=& n \times v - n \times v_{\parallel}  \\
  &=& n \times v - 0                       \\
  &=& n \times v                           \tag4
\end{eqnarray}\\
$$

我们首先可以看到,  $$v_{\perp}^{\prime}$$  可以看做其在  $$v_{\perp}$$  和  $$w$$  上的投影向量之和, 所以我们有:

$$\begin{eqnarray}
v_{\perp}^{\prime} &=& v_{\perp}cos(\theta) + wsin(\theta) \\
                   &=& (v - (v \cdot n)n)cos(\theta) + (n \times v)sin(\theta)  \tag5
\end{eqnarray}\\
$$

旋转后的向量  $$v'$$ :

$$\begin{eqnarray}
v' &=& v_{\parallel} + v_{\perp}^{\prime} \\
   &=& (v \cdot n)n + (v - (v \cdot n)n)cos(\theta) + (n \times v)sin(\theta)  \tag6
\end{eqnarray}\\
$$

由此我们得出了任意向量  $$v$$  绕单位向量  $$n$$  旋转 &theta; 后的向量  $$v'$$  公式.

我们知道旋转矩阵由其旋转后的坐标系的基向量组成, 我们使用上面公式(6), 就可以求解新的基向量以及旋转矩阵.

$$
R(n, \theta) = \begin{bmatrix}
p^{\prime} \\ q^{\prime} \\ r^{\prime}
\end{bmatrix}
\\
$$

> 矩阵形式太复杂了, 暂时先不详细列出了.


## 旋转的几种表示方法 

旋转有多种表示方法, 例如轴角法, 旋转矩阵, 欧拉角, 四元数, 游戏引擎中用到了多种旋转表示方法. 我们来看看这些不同表示方法之间的联系.


### 旋转矩阵 

我们可以用基向量  $$p, q, r$$  旋转后的基向量  $$p', q', r'$$  组成 3&times;3 旋转矩阵. 旋转矩阵为正交矩阵.

正交矩阵就意味着, 其转置矩阵等于其逆矩阵, 并且转置矩阵和原矩阵乘积为单位矩阵I.

$$ M^T = M^{-1} \\
$$
$$ MM^T = MM^{-1} = I \\
$$

-   旋转矩阵的优点
    -   矩阵形式可以方便地在物体坐标系和惯性坐标系之间旋转向量.
    -   常见的图形学API都是使用矩阵. 无论如何存储旋转数据, 在最终渲染前都需要在某处将其转换为矩阵形式.
    -   方便坐标系嵌套类型的计算. 只需要将矩阵依次相乘即可得到最终的旋转矩阵.
    -   求逆向旋转方便. 逆向旋转需要逆矩阵, 而逆矩阵与转置矩阵相同, 通过简单的转置即可求得.

-   旋转矩阵的缺点
    -   需要更多内存. 矩阵需要存储9个数字, 相比欧拉角只需要3个数字, 四元数仅需要4个数字.
    -   矩阵非常不直观, 也几乎不可能不借助计算机计算出任意方位的旋转矩阵.
    -   矩阵可能不是合法的旋转矩阵. 矩阵蠕变可能会导致旋转矩阵不合法, 需要通过矩阵正交化解决.

> 矩阵有9个数字组成, 这相当于给旋转矩阵带来了9个自由度. 但显然不是随意填入9个数字都能是旋转矩阵.
> 旋转矩阵要求其3个行向量是单位矩阵, 并且3个行向量需要互相垂直.我们也可以理解为, 我们把三维空间的问题转化为九维空间带有六个约束的问题.


### 轴角法 

使用绕任意轴旋转的方法, 也可以表达旋转.

-   优点

    没有万向锁的问题
-   缺点
    -   插值不够平滑, 甚至会产生跳跃问题

    -   不能保证插值得到的矢量长度相等. (TODO 为什么????)
-   和四元数比较
    轴角法在三维空间中, 四元数是四维空间.


### 欧拉角 

数学家欧拉证明了单个角位移可以由多个角位移组合而成.


#### 旋转顺序 

必须明确旋转顺序, 指定其旋转轴???? Unity的旋转顺序???
Unity使用 `ZXY` 顺序.


#### 固定轴和动态轴 


#### 角度范围 


### 四元数 


#### 复数和旋转 

可以从复数的角度理解复数  $$i$$ . 将数字  $$1$$  旋转90&deg;, 可以得到  $$1i = i$$ , 继续旋转90&deg;, 可以得到  $$i\*i=-1$$ .

<img src="http://121.196.158.145/ox-hugo/2022-06-Rotation-008.i-Rotation.png" />

-   复数可以看作两元数???
-   三元数???


#### 四元数的由来 

$$i^2 = j^2 = k^2 = ijk = -1\\
$$
$$ij = k = -ji\\
$$
$$jk = i = -kj \\
$$
$$ki = j = -ik\\
$$


#### 四元数的基础 

-   四元数的记法

    四元数包含一个标量分量和一个三维向量分量. 标量分量常记作 `w`, 向量分量可以记作单一向量  $$v$$  或分开的 `x, y, z`.

    两种记法如下:
    $$[w, v]\\
$$
    $$[w, (x, y, z)]\\
$$
-   四元数与轴角法

<!--listend-->

-   负四元数

    将四元数的各分量加上负号, 得到负四元数.
    $$-q = -[w, (x, y, z)] = [-w, (-x, -y, -z)] = -[w, v] = [-w, -v]\\
$$

     $$q$$  和  $$-q$$  代表的角位移是相同的. &theta;&deg; 加上 360&deg; 的奇数倍, 不会改变四元数代表的角位移, 四元数变为负四元数.
    三维旋转的角位移有两种不同的四元数表示法, 它们互为负四元数.

-   单位四元数

    单位四元数表示没有旋转. 由负四元数可知, 几何上有两种四元数可以表示单位四元数:  $$[1, \vec{0}]$$  和  $$[-1, \vec{0}]$$ .

    但在数学上只认为  $$[1, \vec{0}]$$  为单位四元数.
     $$q$$  和 $$-q$$尽管代表了相同的角位移, 我们依然认为其不等.  $$[-1, \vec{0}]$$  也不是单位四元数.

    任意四元数  $$q$$  乘以单位四元数, 依然得到其自身.

    单位四元数的模为1.

    要区分单位旋转和单位四元数.
-   纯四元数

    标量分量为0的四元数称为纯四元数.


#### 四元数的运算 

-   模

    模也被称作范数.

    $$\begin{eqnarray} \lVert q \rVert
       &=& \lVert [w, (x, y, z)] \rVert \\
       &=& \sqrt{w^2 + x^2 + y^2 + z^2} \\
       &=& \lVert [w, v] \rVert \\
       &=& \sqrt{w^2 + \lVert v \rVert^2} \end{eqnarray}\\
$$
-   共轭和逆
    四元数的共轭记作  $$q^\*$$ , 将四元数的向量分量变负得到其共轭.
    $$\begin{eqnarray} q^\*
       &=& [w, v]^\* \\
       &=& [w, -v]  \\
       &=& [w, (x, y, z)]^\* \\
       &=& [w, (-x, -y, -z)] \end{eqnarray}\\
$$

    四元数的逆记作  $$q^{-1}$$ , 四元数的共轭除以其模得到其逆.
    $$q^{-1} = \frac{q^\*}{\lVert q \rVert} \\
$$

    四元数乘以其逆得到单位四元数.
    $$qq^{-1} = [1, \vec{0}]\\
$$

    单位四元数的共轭和逆相等.

    四元数  $$q$$  和其共轭  $$q\*$$  代表了相反的角位移. 因为是将其向量分量变负, 相当于颠倒了旋转方向.

<!--listend-->

-   乘法-叉乘

    四元数有点乘和叉乘, 如果不特别指出, 乘法指叉乘.

    -   定义

    四元数的叉乘算法如下:
    $$\begin{eqnarray}&& (w_1 + x_1i + y_1j + z_1k)(w_2 + x_2i + y_2j + z_2k) \\
       &=& w_1 w_2  + w_1 x_2i   + w_1 y_2j   + w_1 z_2k  + \\
       &&  x_1 w_2i + x_1 x_2i^2 + x_1 y_2ij  + x_1 z_2ik + \\
       &&  y_1 w_2j + y_1 x_2ji  + y_1 y_2j^2 + y_1 z_2jk + \\
       &&  z_1 w_2k + z_1 x_2ki  + z_1 y_2kj  + z_1 z_2k^2  \\
       &=& w_1 w_2                     + \textcolor{red}{w_1 x_2i}     + \textcolor{green}{w_1 y_2j}  + \textcolor{blue}{w_1 z_2k}     + \\
       &&  \textcolor{red}{x_1 w_2i}   + x_1 x_2(-1)                   + \textcolor{blue}{x_1 y_2(k)} + \textcolor{green}{x_1 z_2(-j)} + \\
       &&  \textcolor{green}{y_1 w_2j} + \textcolor{blue}{y_1 x_2(-k)} + y_1 y_2(-1)                  + \textcolor{red}{y_1 z_2(i)}    + \\
       &&  \textcolor{blue}{z_1 w_2k}  + \textcolor{green}{z_1 x_2(j)} + \textcolor{red}{z_1 y_2(-i)} + z_1 z_2(-1)                      \\
       &=& w_1 w_2 - x_1 x_2 - y_1 y_2 - z_1 z_2 +    \\
       &&  (w_1 x_2 + x_1 w_2 + y_1 z_2 - z_1 y_2)i + \\
       &&  (w_1 y_2 + y_1 w_2 + z_1 x_2 - x_1 z_2)j + \\
       &&  (w_1 z_2 + z_1 w_2 + x_1 y_2 - y_1 x_2)k
       \end{eqnarray}\\
$$

    所以四元数乘法可以定义如下:
    $$ [w_1, (x_1, y_1, z_1)][w_2, (x_2, y_2, z_2)] = \\
       \begin{bmatrix}
        w_1 w_2 - x_1 x_2 - y_1 y_2 - z_1 z_2     \\
       w_1 x_2 + x_1 w_2 + y_1 z_2 - z_1 y_2      \\
       w_1 y_2 + y_1 w_2 + z_1 x_2 - x_1 z_2      \\
       w_1 z_2 + z_1 w_2 + x_1 y_2 - y_1 x_2
       \end{bmatrix}\\
$$

    $$[w_1, v_1][w_2, v_2] =
       \begin{bmatrix} w_1 w_2-v_1\cdot{}v_2,& w_1 v_2 + w_2 v_1 + v_2 \times v_1
       \end{bmatrix}\\
$$

    -   叉乘的符号

        不需要为四元数叉乘使用符号
    -   叉乘的结合律和 ~~交换律~~

        四元数满足结合律, 但不满足交换律.
        $$(ab)c = a(bc)\\
$$
        $$ab \ne ba\\
$$
    -   叉乘的模

        四元数叉乘的模等于模的乘积.
        $$\lVert q_1q_2\rVert = \lVert q_1 \rVert \lVert q_2 \rVert\\
$$

    -   叉乘的逆

        四元数叉乘的逆等于各个四元数的逆以相反的顺序相乘.
        $$(ab)^{-1} = b^{-1}a^{-1}\\
$$
    -   叉乘和旋转

        假设有三维点  $$(x, y, z)$$ , 可以扩展为四元数  $$p = [0, (x, y, z)]$$ .
        设  $$q=[\cos{\frac{\theta}{2}}, n\sin{\frac{\theta}{2}}]$$ , 其中  $$n$$  是单位向量作为旋转轴,  $$\theta$$  为旋转角.
        下面公式可以表示  $$p$$  绕  $$n$$  旋转  $$\theta$$ :
        $$ p\prime = qpq^{-1}\\
$$

        四元数叉乘在处理多次旋转上很有优势, 我们看下将点  $$p$$  先旋转  $$a$$ , 再旋转  $$b$$ :
        $$\begin{eqnarray}
             p\prime &=& b(apa^{-1})b^{-1} \\
                     &=& (ba)p(a^{-1}b^{-1}) \\
        	     &=& (ba)p(ba)^{-1}
             \end{eqnarray}\\
$$
        我们可以看到先执行  $$a$$  旋转, 再执行  $$b$$  旋转, 等价于执行乘积  $$ba$$  的单次旋转.

        可以通过改变四元数叉乘的定义, 来反转乘法顺序. 此时并没有改变四元数的基本性质和几何解释.
        $$[w_1, v_1][w_2, v_2] = [w_1w_2 - v_1\cdot{}v_2, w_1v_2 +w_2v_1+v_1\cdot{}v_2]\\
$$

        调整叉乘定义后, 旋转定义如下:
        $$p\prime = q^{-1}pq\\
$$

        连续旋转乘法和旋转发生顺序一致:
        $$\begin{eqnarray}
             p\prime &=& b^{-1}(a^{-1}pa)b \\
                     &=& (b^{-1}a^{-1})p(ab) \\
        	     &=& (ab)^{-1}p(ab)
             \end{eqnarray}\\
$$
    -   四元数的差

        利用给定方位  $$a$$  和  $$b$$ , 求出  $$a$$  到  $$b$$  的旋转角位移  $$d$$ .  $$d$$  被定义为  $$a$$  到  $$b$$  的差.
        $$\begin{eqnarray}ad &=& b \\
             a^{-1}(ad) &=& a^{-1}b \\
             (a^{-1}a)d &=& a^{-1}b \\
             [1, \vec{0}]d &=& a^{-1}b \\
             d &=& a^{-1}b
             \end{eqnarray}\\
$$

        在数学上, 两个四元数之间的差更类似于除法, 而不是普通代数意义上的减法.
    -   四元数的点乘
        $$\begin{eqnarray} q_1 q_2 &=& [w_1, v_1][w_2, v_2] = w_1 w_2 + v_1 \cdot v_2 \\
             &=& [w_1, (x_1, y_1, z_1)][w_2, (x_2, y_2, z_2)] = w_1 w_2 + x_1 x_2 + y_1 y_2 + z_1 z_2
             \end{eqnarray}\\
$$

        点乘的结果为标量.
        对于单位四元数  $$q_1$$   $$q_2$$ , $$ -1 &le; q_1 &sdot; q_2 &le; 1$$.

        通常只关心点乘的绝对值, 因为  $$q_1 \cdot q_2 = -(q_1 \cdot -q_2)$$ .  $$q_1 \cdot q_2$$  的绝对值越大说明其代表的角位移越相似.

    -   四元数的对数, 指数

        设有  $$\alpha = \frac{\theta}{2}$$ , 单位向量  $$n$$ . 有四元数:
        $$q =\begin{bmatrix} \cos\alpha & n\sin\alpha \end{bmatrix}\\
$$
         $$q$$  的对数定义为:
        $$\log{q} = \log(\begin{bmatrix}\cos{\alpha} & n\sin(\alpha)\end{bmatrix}) = \begin{bmatrix}0 & \alpha{}n\end{bmatrix}\\
$$

        四元数的指数以相反方式定义. 有四元数  $$q = [0, \alpha{}n]$$ . 则有:
        $$exp(p) = exp([0, \alpha{}n]) = [\cos\alpha, n\sin\alpha]\\
$$

        我们可以看到对于四元数而言, 对数和指数依然是互为逆运算.
        $$exp(log(q)) = q\\
$$
    -   标量乘运算

        标量乘运算定义为每个分量乘以标量.
        $$\begin{eqnarray} kq &=& k[w, v] = [kw, kv] \\
             &=& k[w, (x, y, z)] = [kw, (kx, ky, kz)]
             \end{eqnarray}\\
$$
    -   四元数的幂


#### 四元数与点 


#### 四元数的优点 

-   四元数仅需要4个数字就可以表达旋转.
-   四元数不受万向锁的困扰.
-   四元数容易插值.
-   相比矩阵可以更容易给出旋转轴与旋转角.


#### 四元数的缺点 

-   四元数由复数(???四维复数)的概念构造, 不易理解, 不符合人的直觉.


#### 四元数和轴角 

四元数和轴角法有一定关系, 绕  $$n$$  轴旋转  $$\theta$$  对应的四元数为:
$$q = \begin{bmatrix} cos(\frac{\theta}{2}) & sin(\frac{\theta}{2})n
\end{bmatrix} = \begin{bmatrix} cos(\frac{\theta}{2}) & sin(\frac{\theta}{2})n_x & sin(\frac{\theta}{2})n_y & sin(\frac{\theta}{2})n_z
\end{bmatrix}\\
$$


#### Unity与四元数 

Unity在引擎内部使用四元数存储旋转和方位, 在其文档中[^fn:1]有明确指出.

Unity提供了 `Quaternion` 类, 用于相关操作. 下面是其常用的几个方法/属性:

-   Quaternion.identity

    值为[1, \vec{0}]
    单位旋转(只读). 表示不旋转, 物体与其父对象或世界轴完全对齐.

    单位旋转和单位四元数不同.
-   Quaternion.LookRotation
    根据旋转后物体的 `forward` (和物体Z轴对齐), `upwards` (X轴与 `forward` 和 `upwards` 的叉积对齐, `upwards` 不一定与Y轴对齐) 创建Quaternion.
-   Quaternion.FromToRotation
    从 `fromDirection` 到 `toDirection` 的旋转.
-   Quaternion.Slerp
    插值
-   Quaternion.Euler
    返回欧拉角表示的四元数旋转.
-   Quaternion.Angle
    返回两个四元数之间的夹角(应该绕同一点旋转, 否则无意义)


## 参考文档 


## 文档 

[为什么你天天只看见四元数和八元数？ - 知乎](https://zhuanlan.zhihu.com/p/110997893)
[Understanding Quaternions 中文翻译《理解四元数》](https://www.qiujiawei.com/understanding-quaternions/)
[Understanding Quaternions | 3D Game Engine Programming](https://www.3dgep.com/understanding-quaternions/)
[对游戏开发中的四元数的一些理解 - 知乎](https://zhuanlan.zhihu.com/p/385337220)

[^fn:1]: [Rotation and orientation in Unity(Unity官方文档)](https://docs.unity3d.com/Manual/QuaternionAndEulerRotationsInUnity.html)
    [Unity - Scripting API: Quaternion](https://docs.unity3d.com/ScriptReference/Quaternion.html)
