#include "pch.h"
#include "CADAnalysisDlldxflib.h"

dxflibCreationClass::~dxflibCreationClass()
{

}

/// <summary>
/// 转变颜色
/// </summary>
/// <param name="colorIndex">颜色索引</param>
/// <returns>颜色类型结构体</returns>
Color dxflibCreationClass::ConvertColor(int colorIndex)
{
    Color c;
    if (colorIndex == -1) {
        c.type = ColorType::ByBlock;
        c.index = 0;
    }
    else if (colorIndex == -2) {
        c.type = ColorType::ByLayer;
        c.index = 0;
    }
    else {
        c.type = ColorType::Custom;
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

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::POINTDXF;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();
    //entity.attributes.thickness = getAttributes().get
    //entity.attributes.invisible = getAttributes()

    PointDxf pointDxfData;
    pointDxfData.pointCoord = { data.x,data.y,data.z };

    entity.entityData = pointDxfData;
    g_entityList.push_back(entity);//动态数组推送

    printAttributes();
}

/**
 * Sample implementation of the method which handles line entities.
 * 处理线段
 */
void dxflibCreationClass::addLine(const DL_LineData& data) {
    printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
        data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::LINE;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();
    //entity.attributes.thickness = getAttributes().get
    //entity.attributes.invisible = getAttributes()

    //几何信息
    Line lineData;
    lineData.start = { data.x1,data.y1,data.z1 };
    lineData.end = { data.x2,data.y2,data.z2 };

    entity.entityData = lineData;
    g_entityList.push_back(entity);//动态数组推送

    printAttributes();
}
void dxflibCreationClass::addText(const DL_TextData& data)
{
    printf("TEXT     (%6.3f, %6.3f, %6.3f) height: %6.3f angle: %6.3f text: %s\n",
        data.ipx, data.ipy, data.ipz,
		data.height, data.angle, data.text.c_str());

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::TEXT;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();
    //entity.attributes.thickness = getAttributes().get
    //entity.attributes.invisible = getAttributes()
    Text textData;
    textData.inserPoint = { data.ipx,data.ipy, data.ipz };
    textData.content = data.text;
    textData.height = data.height;
    textData.rotation = data.angle;
    entity.entityData = textData;
    g_entityList.push_back(entity);

    printAttributes();
}

void dxflibCreationClass::addMText(const DL_MTextData& data)
{
    printf("MTEXT     (%6.3f, %6.3f, %6.3f) height: %6.3f angle: %6.3f text: %s\n",
        data.ipx, data.ipy, data.ipz,
        data.height, data.angle, data.text.c_str());

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::TEXT;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();

    Text textMData;
    textMData.inserPoint = { data.ipx,data.ipy,data.ipz };
    textMData.content = data.text;
    textMData.height = data.height;
    textMData.rotation = data.angle;
    entity.entityData = textMData;
    g_entityList.push_back(entity);

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

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::ARC;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();
    //entity.attributes.thickness = getAttributes().get
    //entity.attributes.invisible = getAttributes()

    Arc arcData;
    arcData.center = { data.cx, data.cy, data.cz };
    arcData.radius = data.radius;
    arcData.startAngle = data.angle1;
    arcData.endAngle = data.angle2;
    entity.entityData = arcData;
    g_entityList.push_back(entity);

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

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::CIRCLE;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();
    //entity.attributes.thickness = getAttributes().get
    //entity.attributes.invisible = getAttributes()

    Circle circleData;
    circleData.center = { data.cx,data.cy,data.cz };
    circleData.radius = data.radius;

    entity.entityData = circleData;
    g_entityList.push_back(entity);
    printAttributes();
}

#pragma region 两者一起
/**
 * Sample implementation of the method which handles polyline entities.
 * 多线段实体
 */
void dxflibCreationClass::addPolyline(const DL_PolylineData& data) {
    printf("POLYLINE \n");
    printf("flags: %d\n", (int)data.flags);

    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::POLYLINE;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();

    Polyline* pPoly = new Polyline();
    pPoly->isClosed = (data.flags & 1) == 1;
	pPoly->vertices.reserve(data.number);//多段线顶点数量
    // 将临时指针存入一个栈中，等待后续的 addVertex 回调填充
    m_polylineStack.push(pPoly);//暂存正在创建但还没完成的多段线对象

    printAttributes();
}

void dxflibCreationClass::addVertex(const DL_VertexData& data)
{
    if (!m_polylineStack.empty())
    {
        Polyline* pPoly = m_polylineStack.top();
        pPoly->vertices.push_back({ data.x, data.y, data.z });
    }
}
#pragma endregion

void dxflibCreationClass::addInsert(const DL_InsertData& data)
{
    printf("POLYLINE \n");
    
    DxfEntity entity;

    //设置类型
    entity.entityType = DxfEntity::Type::INSERT;

    //公共属性
    entity.attributes.layer = getAttributes().getLayer();
    entity.attributes.handle = getAttributes().getHandle();
    entity.attributes.color = ConvertColor(getAttributes().getColor());
    entity.attributes.lineWeight = getAttributes().getWidth();
    entity.attributes.linetype = getAttributes().getLinetype();

    Insert insertData;
    insertData.blockName = data.name;
    insertData.position = { data.ipx, data.ipy, data.ipz };
    insertData.scaleX = data.sx;
    insertData.scaleY = data.sy;
    insertData.rotation = data.angle;

    entity.entityData = insertData;
    g_entityList.push_back(entity);
}

///**
// * Sample implementation of the method which handles vertices.
// * 处理顶点
// */
//void dxflibCreationClass::addVertex(const DL_VertexData& data) {
//    printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
//        data.x, data.y, data.z,
//        data.bulge);
//    printAttributes();
//}

////3D数据
//void dxflibCreationClass::add3dFace(const DL_3dFaceData& data) {
//    printf("3DFACE\n");
//    for (int i = 0; i < 4; i++) {
//        printf("   corner %d: %6.3f %6.3f %6.3f\n",
//            i, data.x[i], data.y[i], data.z[i]);
//    }
//    printAttributes();
//}


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