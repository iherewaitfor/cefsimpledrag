# cefsimple
本项目缺少
[cef_binary_3.3626.1895.g7001d56_windows32.tar.bz2](http://opensource.spotify.com/cefbuilds/cef_binary_3.3626.1895.g7001d56_windows32.tar.bz2)的部分二进制文件,以便减少项目大小，更快捷地提交。clone下来后，需将以下几个文件夹复制根目录。

- Debug
- Release
- Resources

复制完后，整个项目的目录结构如下

```
cefsimple
cef_paths.gypi
cef_paths2.gypi
cmake
CMakeLists.txt
Debug
include
libcef_dll
LICENSE.txt
README.txt
README.md
Release
Resources
```


## 本项目相关环境
- Visual Studio
   - 本文使用VS2015。  Version 14.0.25431.01 Update3
   - ![image](https://note.youdao.com/yws/api/personal/file/B2FD176B94584A50BB18414C132986DA?method=download&shareKey=b997c4a1c7e7a4ef5efee5991b6789f1)
- 下载相关Cmake
   - [Cmake下载地址https://cmake.org/download/](https://cmake.org/download/)
   - 本文下载cmake-3.12.2-win32-x86.zip
- 下载libcef二进制版本
   - [libcef二进制版本下载地址http://opensource.spotify.com/cefbuilds/index.html](http://opensource.spotify.com/cefbuilds/index.html)
   - 本文下载的是cef_binary_3.3626.1895.g7001d56_windows32.tar.bz2, 与chrome72版本对应。
   - [libcef版本与chrome版本的对应关系，请查看https://bitbucket.org/chromiumembedded/cef/wiki/BranchesAndBuilding](https://bitbucket.org/chromiumembedded/cef/wiki/BranchesAndBuilding)