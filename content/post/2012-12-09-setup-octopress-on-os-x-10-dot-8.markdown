---
layout: post
title: "在OS X上折腾OctoPress"
date: 2012-12-09T10:19:00+08:00
comments: true
categories: [杂谈]
draft: true
draft: true
---

这两天心血来潮, 突然想在github上架设个博客玩玩了, 比较好的两个选择就是OctoPress和Jekyll-Bootstrap.
昨天尝试了用Jekyll-Bootstrap, 感觉还不错啦, 我因为没有找到代码高亮的方法, 以及自定义的方法, 就放弃了.

然后今天就尝试OctoPress, 不想op需要ruby 1.9.3, 而OS X 10.8自带的是1.8.7. 果然在执行rake generate的时候出错.

我是有安装homebrew的软件包管理器的, 使用brew安装ruby, 结果Terminal下还是1.8.7.

<!--more-->

然后就学习到有个东西叫做rvm, ruby版本管理器, 允许多个ruby同时安装. 官方的源速度太慢, 所以尝试用sed换成淘宝源, 网上别人给出的sed命令报错, 所以又复习了下vim替换, 全部替换了下.

再接着安装bundler, 报错说RedCloth无法安装.网上搜索了下无果, 注意到中间的时候有个地方提示查看log, 然后发现以这个log为名字的log在系统里面太多了, 也区分不出路径. 索性将所有搜索到的都删除掉. 然后发现RedCloth安装文件需要gcc-4.2, 所以将gcc ln了一个gcc-4.2.

另外, 由于对git不是很熟悉, octopress将主目录作为source分支, 而_deploy目录作为master分支, 为了弄明白这个问题, 我花费了更长的时间.不过也总算是对git的pull, push, branch理解更深入了一点 

博客终于跑起来了, 还是非常开心.

PS:我再说两点.
1.github的服务器有缓存,更新后, 可能需要强制刷新才能看到更新.
2.分类是支持中文的, 但rake preview不支持.
3.Disqus支持中文, 但是可能需要申请.可以在Disqus设置页面找到申请链接.

