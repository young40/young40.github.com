---
layout: post
title: "CocosBuilder 多分辨率"
date: 2013-08-01 22:10
comments: true
categories: [CocosBuilder, Cocos2d-x] 
---

最近两个项目大量使用了CocosBuilder, 对于开发效率提高是巨大的. 一直计划写一篇博客谈谈CocosBuilder的多分辨率问题, 懒病加上一些疙疙瘩瘩的小事情, 拖延了一个多月, 才终于下定决定要尽快完成这篇博客.

在这篇文章中, 假定你比较熟悉CocosBuilder, 如果您还不是很熟悉CocosBuilder, 推荐您看下我的另外一篇文章. [从零开始学习CocosBuilder](http://www.ityran.com/archives/2614) 我还是小菜鸟一个, 难免会有很多错误, 还望各位看客不吝赐教. 

版本:

- Cocos2d-x 2.1.4 http://cocos2d-x.googlecode.com/files/cocos2d-x-2.1.4.zip
- CocosBuilder 3.0alpha5 http://cocosbuilder.googlecode.com/files/CocosBuilder-3.0-alpha5.zip

最近几次Cocos2d-x和CocosBuilder(以下简写为2dx和ccb)都是联合发布的, 使用联合发布的版本会减少不兼容的可能.

<!--more-->

##基本概念

我觉得在分辨率中重要的概念就是`位置`, `大小`, `节点关系`, 可以通过位置, 大小, 节点关系的合理组合, 最终达到理想的界面. 当然, 换而言之, 这需要ccb的使用者更多的思考.

###位置
{% img /upload/2013-08/ccb-multi-r-01.png %}
我们可以看到ccb中位置共有6种设置. 其中前四种设置方式代表了当前节点相对于其父节点的一个顶点的绝对位置.

{% img /upload/2013-08/ccb-multi-r-02.png %}
如上图所示, 我们把`Click me` 按钮, 放在了距离其父节点右上角各50px.
这种位置的设置方式, 一般在节点需要放在父节点一个角上时很有用.

{% img /upload/2013-08/ccb-multi-r-03.png %}
第5种设置方式表示当前节点的坐标由其父节点大小(宽width, 高height)的百分比组成, 其中父节点的左下角为原点.
如上图所示一个节点的父节点大小为200px\*100px, 当前节点设置为(70%, 50%),那么当前节点距离其父节点左下角就应该是(140, 50).
百分比设置是位置设置中最常用的方式, 只有当百分比设置不适合时才去考虑其他设置方式.


{% img /upload/2013-08/ccb-multi-r-04.png %}
第6种设置方式其实就是第1种设置方式的x,y值分别乘以一个全局缩放系数. 我们接下来就讲这个全局缩放系数, 这里先假设iphone下为1, iphone retina下为2.
如上图所示, 当前节点在iphone下距离其父节点左下角(50\*1, 50\*1), 在iphone retina下则就是(50\*2, 50\*2).

###全局缩放系数




