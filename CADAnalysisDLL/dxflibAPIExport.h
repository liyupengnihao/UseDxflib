#pragma once
#include "pch.h"
#include "CADAnalysisDlldxflib.h"//放全部工具

#ifdef CADANALYSISDLL_EXPORTS//导入导出库
#define dxflib_EXPORTS_API __declspec(dllexport)
#else
#define dxflib_EXPORTS_API __declspec(dllimport)
#endif

//当前头文件用于导出
#ifdef __cplusplus
extern "C"
{
#endif
	dxflib_EXPORTS_API void __stdcall usage();
	dxflib_EXPORTS_API void __stdcall testReading(char* file);
	dxflib_EXPORTS_API void __stdcall testWriting();

#ifdef __cplusplus
}
#endif