# Qlib2d

[![Build status](https://ci.appveyor.com/api/projects/status/x2tlhf38raho27sh?svg=true)](https://ci.appveyor.com/project/bajdcc/qlib2d)

clib2d with lisp using Qt (VS2017,x64)

参考先前的cliblisp和clib2d库，将代码整合进Qt平台。

绘制采用QPainter，底层使用OpenGL。

## 改进

- 将圆的碰撞机制由多边形实现，圆=N很大时的多边形。

## 更新

- 支持初步的粒子效果（按键盘8），支持歌词播放效果

## 文章

- [【物理引擎系列】正式启用Qt！](https://zhuanlan.zhihu.com/p/48410758)
- [【物理引擎系列】弹幕粒子特效](https://zhuanlan.zhihu.com/p/48533229)