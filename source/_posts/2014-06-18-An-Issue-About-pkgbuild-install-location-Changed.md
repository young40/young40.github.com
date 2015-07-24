title: 'pkgbuild 安装提示成功但找不到App的问题'
categories: [杂谈]
tags:
- Mac OS X
- pkgbuild
date: 2014-06-18 17:53:52
---

这几天用 `pkgbuild` 来打包 `CocoStudio for Mac`, 遇到了一个极其诡异的问题. 安装打包好的 `CocoStudio.pkg`, 有时在`/Applications` 里面居然找不到 `CocoStudio`. -_-!!!


打包的参数如下:
<!--more-->

```
pkgbuild --root App\
			 --component-plist pkgCocoStudio.plist\
			 --install-location /Applications\
			 --identifier com.ccs.pkg\
			 pkgCocoStudio.pkg


```
其中在 `App` 目录下有 `CocoStudio.app` .

在图形界面下安装显示成功, 无奈只有在终端里面调用 `installer` 安装, 同样显示成功, 有时依然在 `/Applications` 中找不到 `CocoStudio`.

没有任何报错, 放狗来搜也没有看到什么有用的信息. 而且pkg打包相关的资料本来就很少. -_-!!

郁闷了几天之后, 测试一直催促我修复这个问题, 可惜总是一筹莫展. 终于要下定决心要解决这个问题.

既然安装成功, 又找不到安装后的App, 那么就来找找到底安装到了什么地方吧.
在终端中祭出`find / | grep CocoStudio.app` 全盘搜索, 好吧, 我知道我这样干很暴力.
经过漫长的等待之后, 发现除了上面提到的`App` 目录下, 并无`CocoStudio.app`.

这是为什么捏??? ????

苦思冥想, 不得其解. 突然又想到另外一件莫名其妙的诡异事件.
`App` 目录经常无故被修改成 `root` 权限, 而安装`CocoStudio` 确实又调用了 `root` 权限.
难道安装时没有用指定的`--install-location /Applications`, 而是覆盖到了 `App` 目录下?????? 

既然猜测是覆盖到了 `App` 下面, 那么马上删除 `App`, 再次安装, 果真好了. 重新在`App` 下生成`CocoStudio.app`, 并且运行一遍, 再次安装. 果真看到 `App` 下的 `CocoStudio.app` 被覆盖了.

好吧, 真心给跪了!!!!-_^


PS: 找了一圈也没有发现强制指定路径的方法.




