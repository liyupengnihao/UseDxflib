#pragma once
#include "pch.h"

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
	/// <summary>
	/// 记录bool
	/// </summary>
	typedef int DxfBool;

	/// <summary>
	/// 对应内部Point
	/// 点坐标
	/// </summary>
	typedef struct DxfPoint {
		double x;
		double y;
		double z;
	}DxfPoint;

	/// <summary>
	/// 对应内部枚举ColorType
	/// 颜色类型
	/// </summary>
	typedef enum DxfColorType {
		DXFC_BYLAYER = 0,  // 跟随图层颜色
		DXFC_BYBLOCK = 1,  // 跟随块颜色
		DXFC_CUSTOM = 2   // 自定义颜色
	} DxfColorType;

	/// <summary>
	/// 对应内部Color
	/// 颜色定义
	/// </summary>
	typedef struct DxfColor {
		DxfColorType type;
		int index;
	}DxfColor;

	/// <summary>
	/// 对应内部DxfEntityAttributes
	/// 图元公共属性
	/// </summary>
	typedef struct DxfEntityAttr {
		// 核心身份属性
		char layer[256];        // 图层名，对应string layer
		char handle[64];        // 句柄，对应string handle

		// 视觉外观属性
		DxfColor color;         // 颜色，对应Color color
		double lineWeight;      // 线宽
		char linetype[64];      // 线型名，对应string linetype

		// 变换与状态属性
		double thickness;       // 厚度
		DxfBool invisible;      // 可见性
	} DxfEntityAttr;

	/// <summary>
	/// 对应内部PointDxf
	/// 点图元(实体)
	/// </summary>
	typedef struct DxfPointEntity {
		DxfEntityAttr attr;     // 公共属性
		DxfPoint coord;         // 点坐标
	} DxfPointEntity;

	/// <summary>
	/// 对应内部Line
	/// 直线段(实体)
	/// </summary>
	typedef struct DxfLineEntity {
		DxfEntityAttr attr;     // 公共属性
		DxfPoint start;         // 起点
		DxfPoint end;           // 终点
	} DxfLineEntity;

	/// <summary>
	/// 对应内部Circle
	/// 圆(实体)
	/// </summary>
	typedef struct DxfCircleEntity {
		DxfEntityAttr attr;     // 公共属性
		DxfPoint center;        // 圆心
		double radius;          // 半径
	} DxfCircleEntity;

	/// <summary>
	/// 对应内部Arc
	/// 圆弧(实体)
	/// </summary>
	typedef struct DxfArcEntity {
		DxfEntityAttr attr;     // 公共属性
		DxfPoint center;        // 圆心
		double radius;          // 半径
		double startAngle;      // 起始角度(度)
		double endAngle;        // 终止角度(度)
	} DxfArcEntity;

	/// <summary>
	/// 对应内部Text
	/// 文字(实体)
	/// </summary>
	typedef struct DxfTextEntity {
		DxfEntityAttr attr;     // 公共属性
		DxfPoint insertPoint;   // 插入点
		char content[1024];     // 文本内容
		double height;          // 字高
		double rotation;        // 旋转角度
	} DxfTextEntity;

	/// <summary>
	/// 对应内部Polyline
	/// 多线段(实体)
	/// </summary>
	typedef struct DxfPolylineEntity {
		DxfEntityAttr attr;     // 公共属性

		// 顶点数组（动态分配）
		DxfPoint* vertices;     // 顶点列表指针
		int vertexCount;        // 顶点数量
		int vertexCapacity;     // 数组容量（内部使用）

		DxfBool isClosed;       // 是否闭合
	} DxfPolylineEntity;

	/// <summary>
	/// 对应内部Insert
	/// 块引用(实体)
	/// </summary>
	typedef struct DxfInsertEntity {
		DxfEntityAttr attr;     // 公共属性
		char blockName[256];    // 引用的块名称
		DxfPoint position;      // 插入点
		double scaleX;          // X轴缩放
		double scaleY;          // Y轴缩放
		double rotation;        // 旋转角度
	} DxfInsertEntity;

	/// <summary>
	/// 对应内部Spline
	/// 样条曲线(实体)(未实现)
	/// </summary>
	typedef struct DxfSplineEntity {
		DxfEntityAttr attr;     // 公共属性
		int degree;             // 阶数

		// 控制点数组（动态分配）
		DxfPoint* controlPoints;    // 控制点指针
		int cpCount;                // 控制点数量
		int cpCapacity;             // 数组容量

		// 节点向量数组（动态分配）
		double* knots;              // 节点向量指针
		int knotCount;              // 节点数量
		int knotCapacity;           // 数组容量
	} DxfSplineEntity;

	/// <summary>
	/// 对应内部枚举DxfEntity:Type
	/// </summary>
	typedef enum DxfEntityType {
		DXF_ENTITY_POINT = 0,   // 点
		DXF_ENTITY_LINE = 1,   // 直线
		DXF_ENTITY_CIRCLE = 2,   // 圆
		DXF_ENTITY_ARC = 3,   // 圆弧
		DXF_ENTITY_TEXT = 4,   // 文字
		DXF_ENTITY_POLYLINE = 5,   // 多段线
		DXF_ENTITY_INSERT = 6,   // 块引用
		DXF_ENTITY_SPLINE = 7,   // 样条曲线
		DXF_ENTITY_UNKNOWN = 255  // 未知类型
	} DxfEntityType;

	/// <summary>
	/// 对应内部DxfEntity
	/// 联合体实体集合
	/// </summary>
	typedef struct DxfEntityWrapper {
		DxfEntityAttr attr;         // 公共属性（从attributes复制）
		DxfEntityType type;         // 图元类型

		// 联合体：根据type决定使用哪个成员
		union {
			DxfPointEntity			point;
			DxfLineEntity			line;
			DxfCircleEntity			circle;
			DxfArcEntity			arc;
			DxfTextEntity			text;
			DxfPolylineEntity		polyline;
			DxfInsertEntity			insert;
			DxfSplineEntity			spline;
		} data;
	} DxfEntityWrapper;

	/// <summary>
	/// dxf文档容器
	/// </summary>
	typedef struct DxfDocument {
		// 图元数组（动态分配）
		DxfEntityWrapper* entities;		// 图元指针数组
		int entityCount;				// 图元数量
		int entityCapacity;				// 数组容量

		// 文档元数据
		char version[32];				// DXF版本
		char encoding[32];				// 编码
		DxfPoint limitsMin;				// 图形界限最小值
		DxfPoint limitsMax;				// 图形界限最大值

		// 状态
		int lastError;					// 最后错误代码
		char errorMsg[256];				// 错误信息
	} DxfDocument;



	/// <summary>
	/// 读取DXF
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API void __stdcall ReadDXF(const char* filePath);
	
	//dxflib_EXPORTS_API void __stdcall usage();
	//dxflib_EXPORTS_API void __stdcall testReading(char* file);
	//dxflib_EXPORTS_API void __stdcall testWriting();

#ifdef __cplusplus
}
#endif

int main() {
	//printf("正在本地测试 DLL 中的函数...\n");
	//std::cout << "正在本地测试 DLL 中的函数...\n" << std::endl;

	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\demo.dxf");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/myfile.dxf");
	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\newfile.dxf");
	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\newfile.txt");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/demo.dxf");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newfileTwo.dxf");
	ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newfileFour.dxf");
	return 0;
}