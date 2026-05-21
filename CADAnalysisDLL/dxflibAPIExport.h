#pragma once
//#include "pch.h"

#ifdef CADANALYSISDLL_EXPORTS//导入导出库
#define dxflib_EXPORTS_API __declspec(dllexport)
#else
#define dxflib_EXPORTS_API __declspec(dllimport)
#endif


//当前头文件用于导出给VC++6.0或C#使用
#ifdef __cplusplus
extern "C"
{
#endif
	//多段线和实体个数要是可变大小，VC++6.0与VS2026ABI(二进制接口)不一样，无法使用各自容器

	typedef void* DxfDocument_Handle;       // 文档句柄（替代 类dxflibCreationClass）
	typedef int DxfPolylineVertexBuffer_Handle;     // 多段线内每个顶点的数据（替代 DxfPoint* 数组指针）,对应类型typedef std::vector<DxfPoint> PolylinePointList;

	typedef int DxfSplineControlPoint_Handle;	//样条线控制点
	typedef int DxfSplineFitPoint_Handle;		//样条线拟合点
	typedef int DxfSplineKnot_Handle;			//样条线节点向量
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
	/// 样条曲线单个控制点
	/// </summary>
	typedef struct SplineControlPoint {
		DxfPoint controlPoint;
		double weight;//权重默认=1，weight>1:更靠近控制点，0<weight<1:曲线远离控制点，weight=0此点几乎不起作用
	}SplineControlPoint;
	/// <summary>
	/// 样条曲线单个拟合点
	/// </summary>
	typedef struct SplineFitPoint {
		DxfPoint fitPoint;
	}SplineFitPoint;
	/// <summary>
	/// 样条曲线单个节点向量，值为控制点数量+阶数+1（读与写都是如此计算）
	/// </summary>
	typedef struct SplineKnot {
		double knot;
	}SplineKnot;

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
	/// 传统多段线，写入时顶点个数无效
	/// </summary>
	typedef struct DxfPolylineEntity {
		//DxfEntityAttr attr;
		int vertexCount;           // 顶点数量
		int pFlags;          // 多段线类型，0普通，1闭合，4曲线拟合，16 3D多段线，32 3D多边型网格，64 多边形网格闭合（N反向），128 多面体网格
		DxfPolylineVertexBuffer_Handle _vertexHandle; // 替代了 DxfPoint* vertices，动态个数
	} DxfPolylineEntity;

	/// <summary>
	/// 对应内部Insert，哪个块插入哪个位置
	/// 块引用(实体)
	/// </summary>
	typedef struct DxfInsertEntity {
		//DxfEntityAttr attr;     // 公共属性
		char blockName[256];    // 引用的块名称
		DxfPoint position;      // 插入点
		double scaleX;          // X轴缩放
		double scaleY;          // Y轴缩放
		double scaleZ;
		double rotation;        // 旋转角度
	} DxfInsertEntity;

	typedef struct DxfSplineEntity {
		//DxfEntityAttr attr;
		int degree;					// 阶数,常见三阶有理2次样条
		int ccontrolCount;			// 控制点数量，控制点与拟合点通常不同时存在，大多是控制点
		int fitCount;				// 拟合点数，控制点与拟合点通常不同时存在,大多是控制点
		int knotCount;				// 节点数量
		int flags;					// 标志	1闭合样条曲线、2周期性样条曲线、4有理样条曲线、8平面、16线性（同时还设置平面位）
		DxfSplineControlPoint_Handle	_controlPointsHandle;	// 控制点
		DxfSplineFitPoint_Handle		_fitPointHandle;		//拟合点
		DxfSplineKnot_Handle			_knotsHandle;			//向量节点
	} DxfSplineEntity;

	typedef struct DxfEllipseEntity {
		DxfPoint EllipseCenter;					//椭圆圆心
		DxfPoint LongAxisVectorQuantity;		//圆心到长轴向量
		double ShortRatioLong;					//短轴比长轴
		double StartAngle;						//开始角度
		double EndAngle;						//结束角度
	}DxfEllipseEntity;

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
		DXF_ENTITY_ELLIPSE = 8,	//椭圆
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
			DxfSplineEntity			spline;
			DxfEllipseEntity		ellipse;
		} data;
	} DxfEntityWrapper;

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
	/// 读取DXF，读取前会先销毁之前的缓存实体与块
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall ReadDXF(DxfDocument_Handle hdxfDocument, const char* filePath);

	///////////////////////////////////////////////////////////////////////////得到段相关(块，实体)
	/// <summary>
	/// 得到实体总数,
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns>实体总数，-1为失败</returns>
	dxflib_EXPORTS_API int __stdcall GetEntityCount(DxfDocument_Handle hdxfDocument);

	/// <summary>
	/// 按索引将指针指向对应的结构体，DxfEntityWrapper联合体下多段线的顶点成员为句柄要拿顶点的值要用用函数GetVertexAt
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="index"></param>
	/// <param name="outWrapper"></param>
	/// <returns>0成功，-1失败</returns>
	dxflib_EXPORTS_API int __stdcall GetEntityAt(DxfDocument_Handle hdxfDocument, int index, DxfEntityWrapper* outWrapper);//指针不行


	////////////////////////////////////读块大概率作用不大

	/// <summary>
	/// 得到块的个数
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns>个数，-1为失败</returns>
	dxflib_EXPORTS_API int __stdcall GetBlockCount(DxfDocument_Handle hdxfDocument);
	/// <summary>
	/// 得到块索引对应的名称
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="index"></param>
	/// <returns>0成功，-1失败</returns>
	dxflib_EXPORTS_API int __stdcall GetBlockAt(DxfDocument_Handle hdxfDocument, int index, char* OutBlockName, int bufferSize);
	/// <summary>
	/// 得到块内实体的个数
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="blockName"></param>
	/// <returns>个数，-1为失败</returns>
	dxflib_EXPORTS_API int __stdcall GetBlockInEntityCount(DxfDocument_Handle hdxfDocument, const int blockIndex);
	/// <summary>
	/// 得到块内对应索引的实体
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="blockName"></param>
	/// <param name="index"></param>
	/// <param name="outWrapper"></param>
	/// <returns>-1失败，0成功</returns>
	dxflib_EXPORTS_API int __stdcall GetBlockInEntityAt(DxfDocument_Handle hdxfDocument, const int blockIndex, const int entityIndex, DxfEntityWrapper* outWrapper);



	/// <summary>
	/// 销毁句柄下读取的全部实体
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DeleteAllReadEntity(DxfDocument_Handle hdxfDocument);
	/// <summary>
	/// 销毁句柄下读取的全部块
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DeleteAllReadBlock(DxfDocument_Handle hdxfDocument);


	/////////////////////////////////////////得到多段线实体数据相关
	/// <summary>
	/// 得到多段线的顶点数量(DxfPolylineEntity结构体句柄下的顶点数)
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="polylineEntity"></param>
	/// <returns>句柄显示的顶点数</returns>
	dxflib_EXPORTS_API int __stdcall GetVertexCount(DxfDocument_Handle hdxfDocument, const DxfPolylineEntity* polylineEntity);

	/// <summary>
	/// 按索引将指针指向多段线对应顶点结构体DxfPolylineEntity结构体内有句柄
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="index"></param>
	/// <param name="outPoint">数据写入指针内存下（结构体内全为POD数据类型）</param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall GetVertexAt(DxfDocument_Handle hdxfDocument, const int index, const DxfPolylineEntity* polylineEntity, DxfPoint* outPoint);

	//dxflib_EXPORTS_API void __stdcall usage();
	//dxflib_EXPORTS_API void __stdcall testReading(char* file);
	//dxflib_EXPORTS_API void __stdcall testWriting();


	//////////////////////////////////////////////////////////////////////////////////////////////写入相关


	/// <summary>
	/// 推送单个实体到动态数组，为多段线还需使用WriteSinglePolylinePeakEntity向实体中写入顶点
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="pWrapper"></param>
	/// <returns>返回最后一个索引的实体在动态数组中的索引,-1失败</returns>
	dxflib_EXPORTS_API int __stdcall WriteSingleEntity(
		DxfDocument_Handle hdxfDocument,
		const DxfEntityWrapper* pWrapper//库内复制值
	);

	/// <summary>
	/// 加入多段线顶点，实体索引相同就一直向后加
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="entityIndex">WriteSingleEntity写入多段线时返回的索引</param>
	/// <param name="pPoint"></param>
	/// <returns>0成功，-1失败</returns>
	dxflib_EXPORTS_API int __stdcall WriteSinglePolylinePeakEntity(
		DxfDocument_Handle hdxfDocument,
		int entityIndex,
		const DxfPoint* pPoint//pod类型可以跨dll传指针访问对象成员,库内拷贝值，库外销毁拷贝值还在
	);

	/// <summary>
	/// 写入块内容
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="blockName">块名称,没有块会创建新块</param>
	/// <param name="pWrapper">块实体</param>
	/// <returns>返回当前块下当前实体的索引,-1失败</returns>
	dxflib_EXPORTS_API int __stdcall WriteSingleBlock(
		DxfDocument_Handle hdxfDocument,
		const char* blockName,
		const DxfPoint* pPoint,
		const DxfEntityWrapper* pWrapper
	);
	/// <summary>
	/// 加入多段线顶点，块相同且索引相同就一直向后加
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <param name="blockName"></param>
	/// <param name="entityIndex"></param>
	/// <param name="pPoint"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall WriteSinglePolylinePeakBlock(
		DxfDocument_Handle hdxfDocument,
		const char* blockName,
		int entityIndex,
		const DxfPoint* pPoint
	);

	/// <summary>
	/// 删除要写入dxf的实例动态数组内容
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DeleteWriteVectorEntity(DxfDocument_Handle hdxfDocument);
	/// <summary>
	/// 删除要写入dxf的块动态数组内容
	/// </summary>
	/// <param name="hdxfDocument"></param>
	/// <returns></returns>
	dxflib_EXPORTS_API int __stdcall DeleteWriteVectorBlock(DxfDocument_Handle hdxfDocument);

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