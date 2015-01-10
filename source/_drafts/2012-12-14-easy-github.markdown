---
layout: post
title: "Github三板斧"
date: 2012-12-14 15:05
published: false
comments: true
categories: [Git, Github] 
---

``` sh 初始化目录
git init
```

<!--more-->

``` sh 添加文件
git add somefile.php // 添加一个新文件
git add . // 将所有未纳入版本管理的文件添加进来
```

``` sh 本地提交
git commit -a -m "add ignore file"
```

``` sh 状态查看
git status
```

``` sh 忽略文件
git-ignore .DS_Store  //忽略一个文件 本质是将该文件名添加到.gitignore, 可以在每个目录设立不同的.gitignore文件, 会影响子目录


```
