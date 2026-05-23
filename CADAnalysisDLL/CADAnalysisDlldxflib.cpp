#include "pch.h"
#include "CADAnalysisDlldxflib.h"

//静态成员定义
//哈希表实现句柄
//多段线顶点
//std::unordered_map<int, PolylinePointList> dxflibCreationClass::s_PolylineBuffers;
//样条线控制点
//std::unordered_map<int, SplineControlPointList> dxflibCreationClass::s_SplineControlPointBuffers;
//样条线拟合点
//std::unordered_map<int, SplineFitPointList> s_SplineFitPointBuffers;
//样条线向量节点
//std::unordered_map<int, SplineKnotList> s_SplineKnotBuffers;
//int dxflibCreationClass::s_nextHandle = 1;

dxflibCreationClass::~dxflibCreationClass()
{

}

DxfColor dxflibCreationClass::ConvertColor(int colorIndex)
{
	DxfColor c;
	if (colorIndex == -1) {
		c.type = DxfColorType::DXFC_BYBLOCK;
		c.index = 0;
	}
	else if (colorIndex == -2) {
		c.type = DxfColorType::DXFC_BYLAYER;
		c.index = 0;
	}
	else {
		c.type = DxfColorType::DXFC_CUSTOM;
		c.index = colorIndex;
	}
	return c;
}
/**
 * Sample implementation of the method which handles layers.
 * 处理层的句柄
 */
void dxflibCreationClass::addLayer(const DL_LayerData& data)
{
	printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
	printAttributes();
}

/**
 * Sample implementation of the method which handles point entities.
 * 处理点实体
 */
void dxflibCreationClass::addPoint(const DL_PointData& data) {
	printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
		data.x, data.y, data.z);

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_POINT;
	AddPublicAttributes(entity.attr);


	//PointDxf pointDxfData;
	//pointDxfData.pointCoord = { data.x,data.y,data.z };

	//entity.entityData = pointDxfData;
	//g_entityList.push_back(entity);//动态数组推送

	DxfPointEntity pointDxfData;
	pointDxfData.pointCoord = { data.x,data.y,data.z };
	entity.data.point = pointDxfData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{//实体段内
		g_entityList.push_back(entity);
	}

	printAttributes();
}

/**
 * Sample implementation of the method which handles line entities.
 * 处理线段
 */
void dxflibCreationClass::addLine(const DL_LineData& data) {
	printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
		data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_LINE;
	AddPublicAttributes(entity.attr);

	DxfLineEntity lineData;
	lineData.start = { data.x1,data.y1,data.z1 };
	lineData.end = { data.x2,data.y2,data.z2 };
	entity.data.line = lineData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}

	printAttributes();
}
void dxflibCreationClass::addText(const DL_TextData& data)
{
	printf("TEXT     (%6.3f, %6.3f, %6.3f) height: %6.3f angle: %6.3f text: %s\n",
		data.ipx, data.ipy, data.ipz,
		data.height, data.angle, data.text.c_str());

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_TEXT;
	AddPublicAttributes(entity.attr);

	DxfTextEntity textData;
	textData.insertPoint = { data.ipx,data.ipy, data.ipz };
	strncpy_s(
		textData.content,
		sizeof(textData.content),
		data.text.c_str(),
		_TRUNCATE
	);
	textData.height = data.height;
	textData.rotation = data.angle;
	entity.data.text = textData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}

	printAttributes();
}

void dxflibCreationClass::addMText(const DL_MTextData& data)
{
	printf("MTEXT     (%6.3f, %6.3f, %6.3f) height: %6.3f angle: %6.3f text: %s\n",
		data.ipx, data.ipy, data.ipz,
		data.height, data.angle, data.text.c_str());

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_TEXT;
	AddPublicAttributes(entity.attr);

	DxfTextEntity textMData;
	textMData.insertPoint = { data.ipx,data.ipy,data.ipz };
	strncpy_s(
		textMData.content,
		sizeof(textMData.content),
		data.text.c_str(),
		_TRUNCATE
	);
	textMData.height = data.height;
	textMData.rotation = data.angle;
	entity.data.text = textMData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}

	printAttributes();
}
/**
 * Sample implementation of the method which handles arc entities.
 * 弧形实体
 */
void dxflibCreationClass::addArc(const DL_ArcData& data) {
	printf("ARC      (%6.3f, %6.3f, %6.3f) %6.3f, %6.3f, %6.3f\n",
		data.cx, data.cy, data.cz,
		data.radius, data.angle1, data.angle2);

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_ARC;
	AddPublicAttributes(entity.attr);

	DxfArcEntity arcData;
	arcData.center = { data.cx, data.cy, data.cz };
	arcData.radius = data.radius;
	arcData.startAngle = data.angle1;
	arcData.endAngle = data.angle2;
	entity.data.arc = arcData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}


	printAttributes();
}

/**
 * Sample implementation of the method which handles circle entities.
 * 圆形实体
 */
void dxflibCreationClass::addCircle(const DL_CircleData& data) {
	printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f\n",
		data.cx, data.cy, data.cz,
		data.radius);

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_CIRCLE;
	AddPublicAttributes(entity.attr);

	DxfCircleEntity circleData;
	circleData.center = { data.cx,data.cy,data.cz };
	circleData.radius = data.radius;
	entity.data.circle = circleData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}


	printAttributes();
}

#pragma region 两者一起
/**
 * Sample implementation of the method which handles polyline entities.
 * 多线段实体
 */
void dxflibCreationClass::addPolyline(const DL_PolylineData& data) {//注意后续内存管理
	printf("POLYLINE \n");
	printf("flags: %d\n", (int)data.flags);

	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_POLYLINE;
	AddPublicAttributes(entity.attr);

	DxfPolylineEntity* pPoly = new DxfPolylineEntity();//后续添加此多段线顶点还要使用
	pPoly->pFlags = data.flags;
	// 将临时指针存入一个栈中，等待后续的 addVertex 回调填充
	PolylinePointList pPoints;

	pPoly->_vertexHandle = CreatePolylineList(pPoints);

	m_polylineStack.push(pPoly);//指针是副本，指向的内存一样

	//g_entityList.push_back(entity);

	printAttributes();

	m_isCurrentEntityPolyline = true;
}

void dxflibCreationClass::addVertex(const DL_VertexData& data)//注意后续内存管理
{
	if (m_polylineStack.empty())
		return;

	DxfPolylineEntity* pPoly = m_polylineStack.top();//当前正在构建的多段线，top访问栈顶元素但不移除

	//句柄转回std::vector指针
	PolylinePointList* pPoints = GetPolylineList(pPoly->_vertexHandle);
	if (!pPoints)
		return;

	pPoints->push_back({ data.x,data.y,data.z });

	//pPoly->vertices.push_back({ data.x, data.y, data.z });
}
#pragma endregion

void dxflibCreationClass::addInsert(const DL_InsertData& data)
{
	printf("Insert \n");
	printf("x scale:%lf,yscale:%lf", data.sx, data.sy);
	//std::cout << "x缩放：" << data.sx << "y缩放：" << data.sy << std::endl;
	DxfEntityWrapper entity;

	entity.type = DxfEntityType::DXF_ENTITY_INSERT;
	AddPublicAttributes(entity.attr);

	DxfInsertEntity insertData;
	strncpy_s(
		insertData.blockName,
		sizeof(insertData.blockName),
		data.name.c_str(),
		_TRUNCATE
	);
	insertData.position = { data.ipx,data.ipy,data.ipz };
	insertData.scaleX = data.sx;
	insertData.scaleY = data.sy;
	insertData.scaleZ = data.sz;
	insertData.rotation = data.angle;
	entity.data.insert = insertData;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}

}

void dxflibCreationClass::addBlock(const DL_BlockData& data)
{
	printf("Block \n");

	std::cout << "name:" << data.name << "  align x:" << data.bpx << "y:" << data.bpy << "z:" << data.bpz << std::endl;
	//printf("name:%s    align x:%lf,y:%lf,z:%lf",data.name, data.bpx, data.bpy,data.bpz);
	Block blockData;
	blockData.blockName = data.name;
	blockData.alignPoint = { data.bpx,data.bpy,data.bpz };
	g_blockList.push_back(blockData);

	if (data.name != "*Model_Space" && data.name != "*Paper_Space" && data.name != "*Paper_Space0")//测试用容器
		m_isCurrentBoloc = true;
}

void dxflibCreationClass::endBlock()
{
	printf("Block Entity End\n");

	m_isCurrentBoloc = false;
}

void dxflibCreationClass::addEllipse(const DL_EllipseData& data)
{
	printf("Ellipse\n");
	std::cout << "center(" << data.cx << "," << data.cy << "," << data.cz <<
		")LongAxisVectorQuantity(" << data.mx << "," << data.my << "," << data.mz <<
		")ShortAxis/LongAxis:" << data.ratio << "StartAngle" << data.angle1 << "EndAngle" << data.angle2 << std::endl;

	DxfEntityWrapper entity;
	entity.type = DxfEntityType::DXF_ENTITY_ELLIPSE;
	AddPublicAttributes(entity.attr);

	DxfEllipseEntity ellipseData;

	ellipseData.EllipseCenter = { data.cx,data.cy,data.cz };
	ellipseData.LongAxisVectorQuantity = { data.mx,data.my,data.mz };
	ellipseData.ShortRatioLong = data.ratio;
	ellipseData.StartAngle = data.angle1;
	ellipseData.EndAngle = data.angle2;

	if (m_isCurrentBoloc)
	{//块内
		if (!g_blockList.empty())
		{
			Block& lastBlock = g_blockList.back();//返回的是元素的引用

			lastBlock.g_blockEntityList.push_back(entity);
		}
	}
	else
	{
		g_entityList.push_back(entity);
	}
}

void dxflibCreationClass::addSpline(const DL_SplineData& data)
{
	printf("Ellipse\n");
	std::cout << "degree:" << data.degree << "Node Vector Quantity nKnots Count:" << data.nKnots << "Control Point Count" << data.nControl << "Fit Point Count" << data.nFit << "Flags" << data.flags << std::endl;

	DxfEntityWrapper entity;
	entity.type = DxfEntityType::DXF_ENTITY_SPLINE;
	AddPublicAttributes(entity.attr);

	DxfSplineEntity* pSpline = new DxfSplineEntity();
	pSpline->degree = data.degree;
	pSpline->ccontrolCount = data.nControl;
	pSpline->fitCount = data.nFit;
	pSpline->knotCount = data.nKnots;
	pSpline->flags = data.flags;
	SplineControlPointList scpList;
	pSpline->_controlPointsHandle = CreateSplineControlPointList(scpList);
	SplineFitPointList sfpList;
	pSpline->_fitPointHandle = CreateSplineFitPointList(sfpList);
	SplineKnotList skList;
	pSpline->_knotsHandle = CreateSplineKnotList(skList);

	m_splineStack.push(pSpline);
	printAttributes();

	m_isCurrentSpline = true;
}

void dxflibCreationClass::addControlPoint(const DL_ControlPointData& data)
{
	if (m_splineStack.empty())
		return;

	DxfSplineEntity* pSpline = m_splineStack.top();//当前正在构建的多段线，top访问栈顶元素但不移除

	//句柄转回std::vector指针
	SplineControlPointList* pSplineControlPointList = GetSplineControlPointList(pSpline->_controlPointsHandle);
	if (!pSplineControlPointList)
		return;

	pSplineControlPointList->push_back({ { data.x,data.y,data.z } ,data.w});

	//pPoly->vertices.push_back({ data.x, data.y, data.z });
}

void dxflibCreationClass::addFitPoint(const DL_FitPointData& data)
{
	if (m_splineStack.empty())
		return;

	DxfSplineEntity* pSpline = m_splineStack.top();

	SplineFitPointList* pSplineFitPointList = GetSplineFitPointList(pSpline->_fitPointHandle);
	if (!pSplineFitPointList)
		return;

	pSplineFitPointList->push_back({ {data.x,data.y,data.z} });
}

void dxflibCreationClass::addKnot(const DL_KnotData& data)
{
	if (m_splineStack.empty())
		return;

	DxfSplineEntity* pSpline = m_splineStack.top();

	SplineKnotList* pSplineKnotPointList = GetSplineKnotList(pSpline->_knotsHandle);
	if (!pSplineKnotPointList)
		return;

	pSplineKnotPointList->push_back({ data.k });
}

void dxflibCreationClass::addXLine(const DL_XLineData& data)
{

}

void dxflibCreationClass::addRay(const DL_RayData& data)
{

}
/// <summary>
/// 打印属性
/// </summary>
void dxflibCreationClass::printAttributes() {
	printf("  Attributes: Layer: %s, ", attributes.getLayer().c_str());//实体所在图层名称
	printf(" Color: ");
	if (attributes.getColor() == 256) {//使用图层定义的颜色
		printf("BYLAYER");
	}
	else if (attributes.getColor() == 0) {//使用块定义的颜色
		printf("BYBLOCK");
	}
	else {
		printf("%d", attributes.getColor());//具体颜色索引
	}
	printf(" Width: ");//线宽
	if (attributes.getWidth() == -1) {//随层
		printf("BYLAYER");
	}
	else if (attributes.getWidth() == -2) {//随块
		printf("BYBLOCK");
	}
	else if (attributes.getWidth() == -3) {//默认线宽
		printf("DEFAULT");
	}
	else {
		printf("%d", attributes.getWidth());//具体线宽值(毫米)
	}
	printf(" Type: %s\n", attributes.getLinetype().c_str());//线型名称
}

void dxflibCreationClass::AddPublicAttributes(DxfEntityAttr& publicAttr)
{
	//entity.attr.layer = getAttributes().getLayer();
	strncpy_s(
		publicAttr.layer,    // 左边：目标 char[256]
		sizeof(publicAttr.layer), // 大小固定256
		getAttributes().getLayer().c_str(), // 右边 string 转 const char*
		_TRUNCATE             // 超长自动截断
	);
	//publicAttr.handle = getAttributes().getHandle();
	publicAttr.color = ConvertColor(getAttributes().getColor());
	publicAttr.lineWeight = getAttributes().getWidth();
	strncpy_s(
		publicAttr.linetype,
		sizeof(publicAttr.linetype),
		getAttributes().getLinetype().c_str(),
		_TRUNCATE
	);
	publicAttr.lineTypeScale = getAttributes().getLinetypeScale();
}

void dxflibCreationClass::endEntity()
{
	if (m_isCurrentEntityPolyline)
	{
		//多段线在此推送到g_entityList

		// 取出完成的多段线
		DxfPolylineEntity* pPoly = m_polylineStack.top();
		m_polylineStack.pop();

		pPoly->vertexCount = 0;

		PolylinePointList* pPoints = GetPolylineList(pPoly->_vertexHandle);
		if (pPoints)
		{
			pPoly->vertexCount = static_cast<int>(pPoints->size());
		}

		// 包装并加入列表
		DxfEntityWrapper entity;
		entity.type = DXF_ENTITY_POLYLINE;
		AddPublicAttributes(entity.attr);
		entity.data.polyline = *pPoly;
		delete pPoly;

		if (m_isCurrentBoloc)
		{//块内
			if (!g_blockList.empty())
			{
				Block& lastBlock = g_blockList.back();//返回的是元素的引用

				lastBlock.g_blockEntityList.push_back(entity);
			}
		}
		else
		{
			g_entityList.push_back(entity);
		}

		m_isCurrentEntityPolyline = false;
	}
	else if (m_isCurrentSpline)
	{
		DxfSplineEntity* pSpline = m_splineStack.top();
		m_splineStack.pop();

		DxfEntityWrapper entity;
		entity.type = DXF_ENTITY_SPLINE;
		AddPublicAttributes(entity.attr);
		entity.data.spline = *pSpline;//拷贝，诺*pSpline比较大可能性能慢
		delete pSpline;

		if (m_isCurrentBoloc)
		{
			if (!g_blockList.empty())
			{
				Block& lastBlock = g_blockList.back();//返回的是元素的引用

				lastBlock.g_blockEntityList.push_back(entity);
			}
		}
		else
		{
			g_entityList.push_back(entity);
		}
		m_isCurrentSpline = false;
	}
}

void dxflibCreationClass::endSequence()
{
	if (m_isCurrentEntityPolyline)
	{

	}
	else if (m_isCurrentSpline)
	{

	}
}

void dxflibCreationClass::RemoveAndDeletePolylineStack()
{
	while (!m_polylineStack.empty())
	{
		// 先释放真正的对象内存
		delete m_polylineStack.top();
		// 再把指针从栈里扔掉
		m_polylineStack.pop();
	}
}

int dxflibCreationClass::CreatePolylineList(const PolylinePointList& pts)
{
	int handle = s_nextHandle++;

	s_PolylineBuffers.emplace(handle, pts);//拷贝pts

	return handle;
}

PolylinePointList* dxflibCreationClass::GetPolylineList(int handle)
{
	auto it = s_PolylineBuffers.find(handle);
	if (it == s_PolylineBuffers.end())
		return nullptr;
	return &it->second;
}

void dxflibCreationClass::DestroyPolylineList(int handle)
{
	s_PolylineBuffers.erase(handle);//PolylinePointList是对象，会调用对应的析构函数
}

int dxflibCreationClass::CreateSplineControlPointList(const SplineControlPointList& pts)
{
	int handle = s_nextHandle++;//先运输后赋值
	s_SplineControlPointBuffers.emplace(handle, pts);
	return handle;
}

SplineControlPointList* dxflibCreationClass::GetSplineControlPointList(int handle)
{
	auto it = s_SplineControlPointBuffers.find(handle);
	if (it == s_SplineControlPointBuffers.end())
		return nullptr;
	return &it->second;
}

void dxflibCreationClass::DestroySplineControlPointList(int handle)
{
	s_SplineControlPointBuffers.erase(handle);
}

int dxflibCreationClass::CreateSplineFitPointList(const SplineFitPointList& pts)
{
	int handle = s_nextHandle++;//先运输后赋值
	s_SplineFitPointBuffers.emplace(handle, pts);
	return handle;
}

SplineFitPointList* dxflibCreationClass::GetSplineFitPointList(int handle)
{
	auto it = s_SplineFitPointBuffers.find(handle);
	if (it == s_SplineFitPointBuffers.end())
		return nullptr;
	return &it->second;
}

void dxflibCreationClass::DestroySplineFitPointList(int handle)
{
	s_SplineFitPointBuffers.erase(handle);
}

int dxflibCreationClass::CreateSplineKnotList(const SplineKnotList& pts)
{
	int handle = s_nextHandle++;//先运输后赋值
	s_SplineKnotBuffers.emplace(handle, pts);
	return handle;
}

SplineKnotList* dxflibCreationClass::GetSplineKnotList(int handle)
{
	auto it = s_SplineKnotBuffers.find(handle);
	if (it == s_SplineKnotBuffers.end())
		return nullptr;
	return &it->second;
}

void dxflibCreationClass::DestroySplineKnotList(int handle)
{
	s_SplineKnotBuffers.erase(handle);
}