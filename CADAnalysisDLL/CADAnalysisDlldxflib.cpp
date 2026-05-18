#include "pch.h"
#include "CADAnalysisDlldxflib.h"

//静态成员定义
std::unordered_map<int, PointList> dxflibCreationClass::s_buffers;//哈希表实现句柄
int dxflibCreationClass::s_nextHandle = 1;

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
	PointList pPoints;

	pPoly->_vertexHandle = Create(pPoints);
	//pPoly->_vertexHandle = static_cast<DxfDataBuffer_Handle>(pPoints);

	m_polylineStack.push(pPoly);

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
		PointList* pPoints = Get(pPoly->_vertexHandle);
		//PointList* pPoints = static_cast<PointList*>(pPoly->_vertexHandle);
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
	if (!m_isCurrentEntityPolyline)return;
	//多段线在此推送到g_entityList

		// 取出完成的多段线
	DxfPolylineEntity* pPoly = m_polylineStack.top();
	m_polylineStack.pop();

	//PointList* pPoints = static_cast<PointList*>(pPoly->_vertexHandle);
	//pPoly->vertexCount = pPoints->size();

	pPoly->vertexCount = 0;

	PointList* pPoints = Get(pPoly->_vertexHandle);
	if (pPoints)
	{
		pPoly->vertexCount = static_cast<int>(pPoints->size());
	}

	// 包装并加入列表
	DxfEntityWrapper entity;
	entity.type = DXF_ENTITY_POLYLINE;
	AddPublicAttributes(entity.attr);
	entity.data.polyline = *pPoly;


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

int dxflibCreationClass::Create(const PointList& pts)
{
	int handle = s_nextHandle++;

	s_buffers.emplace(handle, pts);//拷贝pts

	return handle;
}

PointList* dxflibCreationClass::Get(int handle)
{
	auto it = s_buffers.find(handle);
	if (it == s_buffers.end())
		return nullptr;
	return &it->second;
}

void dxflibCreationClass::Destroy(int handle)
{
	s_buffers.erase(handle);
}