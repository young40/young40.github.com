+++
title = "Unity, 旋转, 万向锁, 欧拉角和四元数"
date = 2022-06-08T23:52:01+08:00
lastmod = 2022-06-09T18:07:04+08:00
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


### 当X轴旋转90&deg;时 {#当x轴旋转90-and-deg-时}

我们先在Inspector中将飞机绕X轴旋转90&deg;, 这时再操作Y轴与Z轴, 我们可以惊奇地发现, 旋转效果 `似乎` 是完全一致的.
我们再也不能通过旋转Y轴, 让飞机偏航(yaw)了.

 <video width="680" height="570" controls>
 <source src="/ox-hugo/2022-06-Rotation-003.Plane-X-90.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>
