#pragma once
#include "pch.h"

#include "dxflibAPIExport.h"
using std::string;
using std::vector;
//存多段线顶点的结构体
typedef vector<DxfPoint> PolylinePointList;
//样条曲线单个控制点数组
typedef vector<SplineControlPoint> SplineControlPointList;
//样条曲线单个拟合点数组
typedef vector<SplineFitPoint>SplineFitPointList;
//样条曲线单个节点向量，值为控制点数量+阶数+1（读与写都是如此计算）
typedef vector<SplineKnot>SplineKnotList;

class dxflibCreationClass :public DL_CreationAdapter//DL_CreationAdapter类中有空实现另一个是纯虚要全实现
{//public 代表重写的函数和DL_CreationAdapter一样。public的前面给private诺对象指针给基类指针也可调用，编译时是基类的，运行时是子类的
public:
	dxflibCreationClass() = default;
	~dxflibCreationClass();
#pragma region dxflib回调函数
	virtual void addLayer(const DL_LayerData& data)override;//图层					final禁止后续子类继续重写

	virtual void addPoint(const DL_PointData& data)override;//点

	virtual void addLine(const DL_LineData& data)override;//线段

	virtual void addCircle(const DL_CircleData& data)override;//圆

	virtual void addArc(const DL_ArcData& data)override;//圆弧

	virtual void addText(const DL_TextData& data)override;//单行文本

	virtual void addMText(const DL_MTextData& data)override;//多行文本

	virtual void addPolyline(const DL_PolylineData& data)override;//传统多段线
	virtual void addVertex(const DL_VertexData& data)override;//传统多段线顶点

	virtual void addInsert(const DL_InsertData& data)override;//块引用

	virtual void addBlock(const DL_BlockData& data)override;//块头部信息
	virtual void endBlock()override;//块内实体全部遍历后调用

	virtual void addEllipse(const DL_EllipseData& data)override;//打印椭圆

	virtual void addSpline(const DL_SplineData&)override;//样条线整体，会有控制点、拟合点、节点向量的个数
	virtual void addControlPoint(const DL_ControlPointData&)override;//控制点
	virtual void addFitPoint(const DL_FitPointData&)override;//拟合点
	virtual void addKnot(const DL_KnotData&)override;//节点向量

	virtual void endEntity()override;//所有实体结束函数，处理下实体结束时多段线
	virtual void endSequence()override;//复合实体序列结束会先进入此再进入endEntity
	//void add3dFace(const DL_3dFaceData& data)override final;//一个3D面(支持4个点)

	//根据需要添加
#pragma region 不常用
	void addXLine(const DL_XLineData& data)override;//无限长的辅助线
	void addRay(const DL_RayData& data)override;//单向无限延长的线
#pragma endregion
#pragma endregion
	void printAttributes();

	//Color ConvertColor(int colorIndex);
	DxfColor ConvertColor(int colorIndex);

	void AddPublicAttributes(DxfEntityAttr& publicAttr);

	/// <summary>
	/// 销毁m_polylineStack占用的空间与移除指针
	/// </summary>
	void RemoveAndDeletePolylineStack();



	//自定义句柄使用
	//多段线使用
	//创建句柄映射，拷贝传入的结构体并放入哈希表中
	static int CreatePolylineList(const PolylinePointList& pts);
	//通过句柄得到动态数组指针
	static PolylinePointList* GetPolylineList(int handle);
	//销毁多段线句柄
	static void DestroyPolylineList(int handle);

	//样条线使用
	static int CreateSplineControlPointList(const SplineControlPointList& pts);
	static SplineControlPointList* GetSplineControlPointList(int handle);
	static void DestroySplineControlPointList(int handle);

	static int CreateSplineFitPointList(const SplineFitPointList& pts);
	static SplineFitPointList* GetSplineFitPointList(int handle);
	static void DestroySplineFitPointList(int handle);

	static int CreateSplineKnotList(const SplineKnotList& pts);
	static SplineKnotList* GetSplineKnotList(int handle);
	static void DestroySplineKnotList(int handle);
	//std::vector<DxfEntity> g_entityList;
	std::vector<DxfEntityWrapper> g_entityList;//几何实体
	std::vector< DxfEntityWrapper> g_writeEntityList;//写入几何实体
	std::string g_currentLayer = "0";//默认图层

private:
	std::stack<DxfPolylineEntity*> m_polylineStack;//栈容器指针,多段线存储(一个dxf的全部多段线都用此暂存),读取或写入实体或块都用此缓冲，后续分开
	bool m_isCurrentEntityPolyline = false;//当前实体是否是多段线

	bool m_isCurrentBoloc = false;//当前是自定义块

	std::stack<DxfSplineEntity*> m_splineStack;//样条线存储
	bool m_isCurrentSpline = false;//当前实体是否为样条线


	//自定义的句柄
	//多段线顶点
	static inline std::unordered_map<int, PolylinePointList> s_PolylineBuffers;
	//样条线控制点
	static inline std::unordered_map<int, SplineControlPointList> s_SplineControlPointBuffers;
	//样条线拟合点
	static inline std::unordered_map<int, SplineFitPointList> s_SplineFitPointBuffers;
	//样条线向量节点
	static inline std::unordered_map<int, SplineKnotList> s_SplineKnotBuffers;
	//使用 std::atomic 保证自增操作的原子性，多线程安全
	static inline std::atomic<int> s_nextHandle{ 1 };//inline可让静态成员在头文件中定义与初始化,std::atomic确定变量的读取、修改、写回是不可分割的原子操作
	
public:
	struct Block
	{
		std::string blockName;		// 块名称
		DxfPoint alignPoint;		//对齐点
		std::vector<DxfEntityWrapper> g_blockEntityList;//块内实体
	};
	std::vector<Block> g_blockList;//每次读取前要清空
	std::vector<Block> g_writeBlockList;//写入块用此
};