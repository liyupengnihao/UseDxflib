#include "CADAnalysisDlldxflib.h"

dxflibCreationClass::~dxflibCreationClass()
{

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
    printAttributes();
}

/**
 * Sample implementation of the method which handles line entities.
 * 处理线段
 */
void dxflibCreationClass::addLine(const DL_LineData& data) {
    printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
        data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
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
    printAttributes();
}


/**
 * Sample implementation of the method which handles polyline entities.
 * 多线段实体
 */
void dxflibCreationClass::addPolyline(const DL_PolylineData& data) {
    printf("POLYLINE \n");
    printf("flags: %d\n", (int)data.flags);
    printAttributes();
}


/**
 * Sample implementation of the method which handles vertices.
 * 处理顶点
 */
void dxflibCreationClass::addVertex(const DL_VertexData& data) {
    printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
        data.x, data.y, data.z,
        data.bulge);
    printAttributes();
}

//3D数据
void dxflibCreationClass::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE\n");
    for (int i = 0; i < 4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f\n",
            i, data.x[i], data.y[i], data.z[i]);
    }
    printAttributes();
}

//打印属性
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