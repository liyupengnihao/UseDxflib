#pragma once
#include "pch.h"

using std::string;
#pragma region 库内使用结构体,库外使用虚转换
/// <summary>
/// 三维点坐标
/// </summary>
struct Point//无_Point无法自引用，如链表就需要自引用		C
{
	double x;
	double y;
	double z;
};
/// <summary>
/// 颜色类型枚举
/// </summary>
enum class ColorType { ByLayer, ByBlock, Custom };
struct Color
{
	ColorType type;
	int index;// ACI 颜色索引 (1-255)，仅当 type == Custom 时有效
};


/// <summary>
/// 图元基类，只添加需要的基础实体属性
/// </summary>
struct DxfEntityAttributes {
	//核心身份属性
	string layer;      // 图层名 (Group Code 8) - 最重要！
	string handle;     // 句柄 (Group Code 5) - 唯一ID，可选但推荐

	//视觉外观属性
	Color color;              // 颜色索引 (Group Code 62) - 0=ByBlock, 256=ByLayer
	double lineWeight;      // 线宽 (Group Code 370) - 单位通常是毫米或枚举值
	string linetype;   // 线型名 (Group Code 6) - 如 "CONTINUOUS"实线(默认), "DASHED"(虚线),"CENTER"(中心线),"ByLayer"(跟随图层定义的线型)

	//变换与状态属性
	double thickness;       // 厚度 (Group Code 39) - 2D图中的拉伸高度，常为0(2D不用处理)
	bool invisible;         // 可见性 (Group Code 60) - false=可见, true=隐藏

};
struct PointDxf :DxfEntityAttributes
{
	Point pointCoord;
};
/// <summary>
/// 直线段
/// DXF 标识: LINE
/// dxflib 数据类: DL_LineData
/// </summary>
struct Line :DxfEntityAttributes//	C++
{
	Point start;//xyz,二维的z为0.0
	Point end;
};
/// <summary>
/// 圆
/// DXF 标识: CIRCLE
/// dxflib 数据类: DL_CircleData
/// </summary>
struct Circle :DxfEntityAttributes
{
	Point center;
	double radius;
};
/// <summary>
/// 圆弧
/// DXF 标识: ARC
/// dxflib 数据类: DL_ArcData
/// </summary>
struct Arc :DxfEntityAttributes
{
	Point center;
	double radius;
	double startAngle;
	double endAngle;
};
/// <summary>
/// 文字
/// DXF 标识: TEXT (单行) 或 MTEXT (多行)
/// dxflib 数据类: DL_TextData / DL_MTextData
/// </summary>
struct Text :DxfEntityAttributes
{
	Point inserPoint;//插入位置
	string content;//文本内容
	double height;//字高
	double rotation;//旋转角度
	//多行还有特有
};
/// <summary>
/// 多段线
/// LWPOLYLINE(二维，z都相同)
/// POLYLINE(1 个 POLYLINE 头 + N 个独立 VERTEX 顶点实体 + 1 个 SEQEND		每个顶点可独立 Z)
/// DXF 标识: LWPOLYLINE (轻量多段线) 或 POLYLINE
/// dxflib 数据类: DL_PolylineData (配合多次 addVertex 回调)
/// </summary>
struct Polyline :DxfEntityAttributes
{
	std::vector<Point> vertices;//顶点列表
	bool isClosed;//是否闭合，首尾相连
};
/// <summary>
/// 块引用
/// DXF 标识: INSERT
/// dxflib 数据类: DL_InsertData
/// 说明: 它本身没有几何形状，只是一个“指针”。它指向 BLOCKS 段中定义好的一个组件（比如一个螺栓、一扇门），并告诉软件把它放在哪里、放大多少倍
/// </summary>
struct Insert :DxfEntityAttributes
{
	std::string blockName; // 引用的块名称
	Point position;        // 插入点
	double scaleX;         // X轴缩放比例
	double scaleY;         // Y轴缩放比例
	double rotation;       // 旋转角度
};
/// <summary>
/// 样条曲线
/// DXF 标识: SPLINE
/// dxflib 数据类: DL_SplineData
/// </summary>
struct Spline :DxfEntityAttributes
{
	int degree;                // 阶数 (通常是 3)越高越圆滑
	std::vector<Point> controlPoints; // 控制点
	std::vector<double> knots;        // 节点向量 (高级数学参数)
};
/// <summary>
/// 统一容器
/// </summary>
struct DxfEntity
{//VC要调用再封一层
	DxfEntityAttributes attributes;//公共属性

	/// <summary>
	/// 区分图元类型的枚举
	/// </summary>
	enum class Type { POINTDXF=0, LINE=1, CIRCLE=2, ARC=3, TEXT=4, POLYLINE=5, INSERT=6, SPLINE=7, UNKNOWN=255 } entityType;

	/// <summary>
	/// 联合体
	/// </summary>
	boost::variant<PointDxf, Line, Circle, Arc, Text, Polyline, Insert, Spline> entityData;
};
#pragma endregion
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

	//void addEntity(const DL_EntityData& data) override;

	//void add3dFace(const DL_3dFaceData& data)override final;//一个3D面(支持4个点)

	//根据需要添加

	void printAttributes();

	Color ConvertColor(int colorIndex);

	std::vector<DxfEntity> g_entityList;
	std::string g_currentLayer = "0";//默认图层

private:
	std::stack<Polyline*> m_polylineStack;//栈容器指针
};