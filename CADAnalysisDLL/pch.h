// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头

#define NOGDI         // 防止 windows.h 定义 Arc, Circle, Polygon 等宏

#include "framework.h"

#include "dl_dxf.h"//包含路径在项目属性->C/C++->常规中配置
//#include "dl_creationinterface.h"
#include "dl_creationadapter.h"

#include <string>
#include <memory>//智能指针
#include <iostream>
#include <stdio.h>//与#include <cstdio>内容一样，但<stdio.h>在全局中<cstdio>在std命名空间中，<cstdio>使用时函数要加std::
//#include <cstdio>
#include <stack>//栈
#include <cmath>//此中拿到πstd::acos(-1.0)

#include <mutex>
//#include <shared_mutex>//读多些少

#include <functional>

#include <boost/variant.hpp>
#endif //PCH_H
