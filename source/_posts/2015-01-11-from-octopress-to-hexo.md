title: 博客切换到Hexo
date: 2015-01-11 04:59:39
tags:
---

太惭愧了, 过去的一年时间里, 居然没有怎么写博客. 回想起来, 除了恋爱, 结婚, 生子什么事情也没有办成.

谢谢亲爱的陪伴, 上帝赐予我们小生命. 没有你们, 我实在是难以向自己交差.

OctoPress 是一个基于Github的伟大的博客系统, 但我在使用的过程中一直觉得不顺手.
正巧, 看到了Hexo 这个新兴的博客系统. 尝试了之后, 决定转向Hexo.
Hexo以下几点特别吸引我:
<!--more-->

- 基于Node.js
  + 我一直是Node.js的脑残粉丝, 再加上OctoPress是基于我所不熟悉的Ruby的
- 轻量化
- 兼容OctoPress
- 等等...

迁移的过程也很简单:

1. 创建空分支
   `git checkout --orphan hexo`
   用上面的命令创建一个空的分支`hexo`
   删除目录下的文件, 仅保留git相关文件

2. 创建Hexo环境
   参见Hexo的文档, 不再赘述
   http://hexo.io/docs/setup.html
   可以用`hexo server`, 在本地看下是否正常
   正常后将`hexo`分支推送到你的github pages仓库

3. 迁移
   依旧是参见Hexo的文档
   http://hexo.io/docs/migration.html
   我还迁移了保存图片的目录

4. 发布
   OctoPress发布在了Github上的master分支, 所以要清除其原有内容.
   * 切换github pages仓库的默认分支为`hexo`
   * `git push origin :master` 删除远程master分支
   * `git branch -D master` 删除本地master分支
   * 设置发布信息, 发布的branch 为master

5. 测试一下吧
   `hexo deploy`
   




