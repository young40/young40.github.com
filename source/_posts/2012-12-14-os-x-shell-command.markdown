---
layout: post
title: "OS X常用Shell命令"
date: 2012-12-14 20:45
comments: true
categories: [OS X, Shell]
---



``` sh 清理 
find ./ -name "._*" -depth -exec rm {} \;
find ./ -name "._*" | xargs rm 
```
