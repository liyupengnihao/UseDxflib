#pragma once
#include "pch.h"

#include "dxflibAPIExport.h"
using std::string;
typedef std::vector<DxfPoint> PointList;//存顶点的

class dxflibCreationClass :public DL_CreationAdapter//DL_CreationAdapter类中有空实现另一个是纯虚要全实现
{
public:
	dxflibCreationClass() = default;
	~dxflibCreationClass();
	virtual void addLayer(const DL_LayerData& data)override;//图层					final禁止后续子类继续重写
	virtual void addPoint(const DL_PointData& data)override;//点
	virtual void addLine(const DL_LineData& data)override;//线段
	virtual void addCircle(const DL_CircleData& data)override;//圆
	virtual void addArc(const DL_ArcData& data)override;//圆弧
	virtual void addText(const DL_TextData& data)override;//单行文本
	virtual void addMText(const DL_MTextData& data)override;//多行文本

	virtual void addPolyline(const DL_PolylineData& data)override;//多段线
	virtual void addVertex(const DL_VertexData& data)override;//顶点

	virtual void addInsert(const DL_InsertData& data)override;//块引用

	virtual void addBlock(const DL_BlockData& data)override;//块头部信息
	virtual void endBlock()override;//块内实体全部遍历后调用

	virtual void endEntity()override;//实体结束函数，处理下实体结束时多段线

	//void addEntity(const DL_EntityData& data) override;

	//void add3dFace(const DL_3dFaceData& data)override final;//一个3D面(支持4个点)

	//根据需要添加

	void printAttributes();

	//Color ConvertColor(int colorIndex);
	DxfColor ConvertColor(int colorIndex);

	void AddPublicAttributes(DxfEntityAttr& publicAttr);

	/// <summary>
	/// 销毁m_polylineStack占用的空间与移除指针
	/// </summary>
	void RemoveAndDeletePolylineStack();



	//自定义句柄使用
	//创建句柄映射
	static int Create(const PointList& pts);
	//通过句柄得到动态数组指针
	static PointList* Get(int handle);
	//销毁句柄
	static void Destroy(int handle);


	//std::vector<DxfEntity> g_entityList;
	std::vector<DxfEntityWrapper> g_entityList;//几何实体
	std::vector< DxfEntityWrapper> g_writeEntityList;//写入几何实体
	std::string g_currentLayer = "0";//默认图层

private:
	std::stack<DxfPolylineEntity*> m_polylineStack;//栈容器指针,多段线存储(一个dxf的全部多段线都用此暂存),读取或写入实体或块都用此缓冲，后续分开
	bool m_isCurrentEntityPolyline = false;//当前实体是否是多段线

	bool m_isCurrentBoloc = false;//当前是自定义块


	//自定义的句柄
	static std::unordered_map<int, PointList> s_buffers;
	static int  s_nextHandle;
	
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