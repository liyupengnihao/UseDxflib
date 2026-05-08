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
	//多段线和实体个数要是可变大小，VC++6.0与VS2026ABI(二进制接口)不一样，无法使用各自容器

	typedef void* DxfDocument_Handle;       // 文档句柄（替代 类dxflibCreationClass）
	typedef void* DxfDataBuffer_Handle;     // 多段线内每个顶点的数据（替代 DxfPoint* 数组指针）,对应类型typedef std::vector<DxfPoint> PointList;

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
		DxfColorType type;//颜色类型
		int index;//类型0，对应256，类型1对应0，类型2对应1到255
	}DxfColor;

	/// <summary>
	/// 对应内部DxfEntityAttributes
	/// 图元公共属性
	/// </summary>
	typedef struct DxfEntityAttr {
		// 核心身份属性
		char layer[256];        // 图层名，对应string layer
		//char handle[64];        // 句柄，对应string handle
		//int handle;        // 句柄，对应string handle

		// 视觉外观属性
		DxfColor color;         // 颜色，对应Color color
		double lineWeight;      // 线宽
		char linetype[64];      // 线型名，对应string linetype

		double lineTypeScale;	// 线型比例

		// 变换与状态属性
		//double thickness;       // 厚度
		//DxfBool invisible;      // 可见性
	} DxfEntityAttr;

	/// <summary>
	/// 对应内部PointDxf
	/// 点图元(实体)
	/// </summary>
	typedef struct DxfPointEntity {
		//DxfEntityAttr attr;     // 公共属性
		DxfPoint pointCoord;         // 点坐标
	} DxfPointEntity;

	/// <summary>
	/// 对应内部Line
	/// 直线段(实体)
	/// </summary>
	typedef struct DxfLineEntity {
		//DxfEntityAttr attr;     // 公共属性
		DxfPoint start;         // 起点
		DxfPoint end;           // 终点
	} DxfLineEntity;

	/// <summary>
	/// 对应内部Circle
	/// 圆(实体)
	/// </summary>
	typedef struct DxfCircleEntity {
		//DxfEntityAttr attr;     // 公共属性
		DxfPoint center;        // 圆心
		double radius;          // 半径
	} DxfCircleEntity;

	/// <summary>
	/// 对应内部Arc
	/// 圆弧(实体)
	/// </summary>
	typedef struct DxfArcEntity {
		//DxfEntityAttr attr;     // 公共属性
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
		//DxfEntityAttr attr;     // 公共属性
		DxfPoint insertPoint;   // 插入点
		char content[1024];     // 文本内容
		double height;          // 字高
		double rotation;        // 旋转角度
	} DxfTextEntity;

	/// <summary>
	/// 对应内部Polyline
	/// 多线段(实体)
	/// </summary>
	//typedef struct DxfPolylineEntity {
	//	DxfEntityAttr attr;     // 公共属性

	//	// 顶点数组（动态分配）
	//	DxfPoint* vertices;     // 顶点列表指针
	//	int vertexCount;        // 顶点数量
	//	int vertexCapacity;     // 数组容量（内部使用）

	//	DxfBool isClosed;       // 是否闭合
	//} DxfPolylineEntity;

	typedef struct DxfPolylineEntity {
		//DxfEntityAttr attr;
		int vertexCount;           // 顶点数量
		int pFlags;          // 多段线类型，0普通，1闭合，4曲线拟合，16 3D多段线，32 3D多边型网格，64 多边形网格闭合（N反向），128 多面体网格
		DxfDataBuffer_Handle _vertexHandle; // 替代了 DxfPoint* vertices，动态个数
	} DxfPolylineEntity;

	/// <summary>
	/// 对应内部Insert
	/// 块引用(实体)
	/// </summary>
	typedef struct DxfInsertEntity {
		//DxfEntityAttr attr;     // 公共属性
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
	//typedef struct DxfSplineEntity {
	//	DxfEntityAttr attr;     // 公共属性
	//	int degree;             // 阶数

	//	// 控制点数组（动态分配）
	//	DxfPoint* controlPoints;    // 控制点指针
	//	int cpCount;                // 控制点数量
	//	int cpCapacity;             // 数组容量

	//	// 节点向量数组（动态分配）
	//	double* knots;              // 节点向量指针
	//	int knotCount;              // 节点数量
	//	int knotCapacity;           // 数组容量
	//} DxfSplineEntity;

	//typedef struct DxfSplineEntity {
	//	//DxfEntityAttr attr;
	//	int degree;                // 阶数
	//	int cpCount;               // 控制点数量
	//	int knotCount;             // 节点数量
	//	DxfDataBufferHandle _controlPointsHandle; // 【修改】替代了 DxfPoint* controlPoints
	//	DxfDataBufferHandle _knotsHandle;         // 【修改】替代了 double* knots
	//} DxfSplineEntity;

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
		DXF_ENTITY_SPLINE = 7,   // 样条曲线(暂未实现)
		DXF_ENTITY_UNKNOWN = 255  // 未知类型
	} DxfEntityType;

	/// <summary>
	/// 对应内部DxfEntity
	/// 联合体实体集合,代表一个实体
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
			/*DxfSplineEntity			spline;*/
		} data;
	} DxfEntityWrapper;

	//不给VC6，内部函数来给内容
	///// <summary>
	///// dxf文档容器
	///// </summary>
	//typedef struct DxfDocument {
	//	// 图元数组（动态分配）
	//	DxfEntityWrapper* entities;		// 图元指针数组,(VS2026与VC)内存管理有问题（别用指针用句柄）
	//	int entityCount;				// 图元数量
	//	int entityCapacity;				// 数组容量

	//	// 文档元数据
	//	char version[32];				// DXF版本
	//	char encoding[32];				// 编码
	//	DxfPoint limitsMin;				// 图形界限最小值
	//	DxfPoint limitsMax;				// 图形界限最大值

	//	// 状态
	//	int lastError;					// 最后错误代码
	//	char errorMsg[256];				// 错误信息
	//} DxfDocument;




		/// <summary>
	/// 创建DxfDocumentHandle句柄
	/// </summary>
	/// <returns></returns>
	dxflib_EXPORTS_API DxfDocument_Handle __stdcall CreateDxfDocument();

	/// <summary>
	/// 销毁DxfDocumentHandle句柄
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DestroyDxfDocument(DxfDocument_Handle hdxfDocument);

	/// <summary>
	/// 读取DXF
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall ReadDXF(DxfDocument_Handle hdxfDocument, const char* filePath);

	///////////////////////////////////////////////////////////////////////////得到实体相关
	/// <summary>
	/// 得到实体总数,读取前会先销毁
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns>实体总数</returns>
	dxflib_EXPORTS_API int __stdcall GetEntityCount(DxfDocument_Handle hdxfDocument);

	/// <summary>
	/// 按索引将指针指向对应的结构体
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="index"></param>
	/// <param name="outWrapper"></param>
	/// <returns>0成功，-1失败</returns>
	dxflib_EXPORTS_API int __stdcall GetEntityAt(DxfDocument_Handle hdxfDocument, int index, DxfEntityWrapper* outWrapper);//结构体指针符合POD，VC可以传

	/// <summary>
	/// 销毁句柄下读取的全部实体
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DeleteAllEntity(DxfDocument_Handle hdxfDocument);


	/////////////////////////////////////////得到多段线实体数据相关
	/// <summary>
	/// 得到多段线的顶点数量(多段线结构体内有句柄，)
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="polylineEntity"></param>
	/// <returns>句柄显示的顶点数</returns>
	dxflib_EXPORTS_API int __stdcall GetVertexCount(DxfDocument_Handle hdxfDocument, const DxfPolylineEntity* polylineEntity);

	/// <summary>
	/// 按索引将指针指向多段线对应顶点结构体
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="index"></param>
	/// <param name="outPoint"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall GetVertexAt(DxfDocument_Handle hdxfDocument, int index, const DxfPolylineEntity* polylineEntity, DxfPoint* outPoint);

	//dxflib_EXPORTS_API void __stdcall usage();
	//dxflib_EXPORTS_API void __stdcall testReading(char* file);
	//dxflib_EXPORTS_API void __stdcall testWriting();


	//////////////////////////////////////////////////////////////////////////////////////////////写入相关


	/// <summary>
	/// 推送单个实体到动态数组，为多段线还需使用WriteSinglePolylinePeakTwo向实体中写入顶点
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="pWrapper"></param>
	/// <returns>返回最后一个索引的实体在动态数组中的索引</returns>
	dxflib_EXPORTS_API int __stdcall WriteSingleEntityTwo(
		DxfDocument_Handle hdxfDocument,
		const DxfEntityWrapper* pWrapper
	);

	/// <summary>
	/// 加入多段线顶点，实体索引相同就一直向后加
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="entityIndex">WriteSingleEntityTwo写入多段线时返回的索引</param>
	/// <param name="pPoint"></param>
	/// <returns>0成功，-1失败</returns>
	dxflib_EXPORTS_API int __stdcall WriteSinglePolylinePeakTwo(
		DxfDocument_Handle hdxfDocument,
		int entityIndex,
		const DxfPoint* pPoint
	);
	/// <summary>
	/// 删除写入dxf的动态数组内容
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DeleteWriteVector(DxfDocument_Handle hdxfDocument);

	/// <summary>
	/// 将实体数组写入dxf中，无文件创建有文件修改
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="path"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall WriteDXF(DxfDocument_Handle hdxfDocument, const char* path);
#ifdef __cplusplus
}
#endif