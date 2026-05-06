#include "pch.h"
#include "dxflibAPIExport.h"//含对外结构体
#include "CADAnalysisDlldxflib.h"//含内部结构体

#pragma region 结构体转换
//inline 建议编译器将函数的代码直接“粘贴”到调用处，以消除函数调用的开销
// 以下不会直接"粘贴"
//函数体过大：内联会导致代码体积急剧膨胀（代码膨胀），影响 CPU 缓存效率。
//递归函数：无法在编译期确定展开深度。
//复杂的控制流：如包含复杂的 switch 或循环。

/// <summary>
/// string转C的char*
/// </summary>
/// <param name="out">返回的char*</param>
/// <param name="str">要转的string</param>
/// <param name="maxLen">字符长度</param>
inline void StringToC(char* out, const std::string& str, size_t maxLen)
{
	if (str.empty()) {
		out[0] = '\0';
		return;
	}
	strncpy_s(out, maxLen, str.c_str(), _TRUNCATE);
	out[maxLen - 1] = '\0';
}

/**
 * @brief 将内部 Point 转换为外部 DxfPoint
 */
inline void Convert(const Point& from, DxfPoint& to)
{
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;
}

/**
 * @brief 将内部 Color 转换为外部 DxfColor
 */
inline void Convert(const Color& from, DxfColor& to)
{
    to.type = static_cast<DxfColorType>(from.type);
    to.index = from.index;
}

/**
 * @brief 将内部 DxfEntityAttributes 转换为外部 DxfEntityAttr
 */
inline void Convert(const DxfEntityAttributes& from, DxfEntityAttr& to)
{
    StringToC(to.layer, from.layer, sizeof(to.layer));
    StringToC(to.handle, from.handle, sizeof(to.handle));
    Convert(from.color, to.color);
    to.lineWeight = from.lineWeight;
    StringToC(to.linetype, from.linetype, sizeof(to.linetype));
    to.thickness = from.thickness;
    to.invisible = from.invisible ? 1 : 0;
}

/**
 * @brief 将内部 PointDxf 转换为外部 DxfPointEntity
 */
inline void Convert(const PointDxf& from, DxfPointEntity& to)
{
    Convert(from, to.attr);  // 基类部分
    Convert(from.pointCoord, to.coord);
}

/**
 * @brief 将内部 Line 转换为外部 DxfLineEntity
 */
inline void Convert(const Line& from, DxfLineEntity& to)
{
    Convert(from, to.attr);
    Convert(from.start, to.start);
    Convert(from.end, to.end);
}

/**
 * @brief 将内部 Circle 转换为外部 DxfCircleEntity
 */
inline void Convert(const Circle& from, DxfCircleEntity& to)
{
    Convert(from, to.attr);
    Convert(from.center, to.center);
    to.radius = from.radius;
}

/**
 * @brief 将内部 Arc 转换为外部 DxfArcEntity
 */
inline void Convert(const Arc& from, DxfArcEntity& to)
{
    Convert(from, to.attr);
    Convert(from.center, to.center);
    to.radius = from.radius;
    to.startAngle = from.startAngle;
    to.endAngle = from.endAngle;
}

/**
 * @brief 将内部 Text 转换为外部 DxfTextEntity
 */
inline void Convert(const Text& from, DxfTextEntity& to)
{
    Convert(from, to.attr);
    Convert(from.inserPoint, to.insertPoint);
    StringToC(to.content, from.content, sizeof(to.content));
    to.height = from.height;
    to.rotation = from.rotation;
}

/**
 * @brief 将内部 Polyline 转换为外部 DxfPolylineEntity（涉及内存分配）
 * @param to 输出结构体
 * @param from 输入结构体
 * @return 是否成功
 */
inline bool Convert(const Polyline& from, DxfPolylineEntity& to)
{
    Convert(from, to.attr);
    to.isClosed = from.isClosed ? 1 : 0;

    if (from.vertices.empty()) {
        to.vertices = nullptr;
        to.vertexCount = 0;
        to.vertexCapacity = 0;
        return true;
    }

    // 分配内存
    to.vertices = new DxfPoint[from.vertices.size()];
    if (!to.vertices) return false;

    // 复制顶点
    for (size_t i = 0; i < from.vertices.size(); ++i) {
        Convert(from.vertices[i], to.vertices[i]);
    }

    to.vertexCount = static_cast<int>(from.vertices.size());
    to.vertexCapacity = to.vertexCount;
    return true;
}

/**
 * @brief 释放 Polyline 的内存（外部使用者调用）
 */
inline void FreePolyline(DxfPolylineEntity& entity)
{
    if (entity.vertices) {
        delete[] entity.vertices;
        entity.vertices = nullptr;
    }
    entity.vertexCount = 0;
    entity.vertexCapacity = 0;
}

/**
 * @brief 将内部 Insert 转换为外部 DxfInsertEntity
 */
inline void Convert(const Insert& from, DxfInsertEntity& to)
{
    Convert(from, to.attr);
    StringToC(to.blockName, from.blockName, sizeof(to.blockName));
    Convert(from.position, to.position);
    to.scaleX = from.scaleX;
    to.scaleY = from.scaleY;
    to.rotation = from.rotation;
}

/**
 * @brief 将内部 Spline 转换为外部 DxfSplineEntity（涉及内存分配）
 */
inline bool Convert(const Spline& from, DxfSplineEntity& to)
{
    Convert(from, to.attr);
    to.degree = from.degree;

    // 分配控制点
    if (!from.controlPoints.empty()) {
        to.controlPoints = new DxfPoint[from.controlPoints.size()];
        if (!to.controlPoints) return false;
        for (size_t i = 0; i < from.controlPoints.size(); ++i) {
            Convert(from.controlPoints[i], to.controlPoints[i]);
        }
        to.cpCount = static_cast<int>(from.controlPoints.size());
        to.cpCapacity = to.cpCount;
    }
    else {
        to.controlPoints = nullptr;
        to.cpCount = 0;
        to.cpCapacity = 0;
    }

    // 分配节点
    if (!from.knots.empty()) {
        to.knots = new double[from.knots.size()];
        if (!to.knots) {
            // 清理已分配的控制点
            delete[] to.controlPoints;
            to.controlPoints = nullptr;
            return false;
        }
        memcpy(to.knots, &from.knots[0], from.knots.size() * sizeof(double));
        to.knotCount = static_cast<int>(from.knots.size());
        to.knotCapacity = to.knotCount;
    }
    else {
        to.knots = nullptr;
        to.knotCount = 0;
        to.knotCapacity = 0;
    }

    return true;
}

/**
 * @brief 释放 Spline 的内存
 */
inline void FreeSpline(DxfSplineEntity& entity)
{
    if (entity.controlPoints) {
        delete[] entity.controlPoints;
        entity.controlPoints = nullptr;
    }
    if (entity.knots) {
        delete[] entity.knots;
        entity.knots = nullptr;
    }
    entity.cpCount = 0;
    entity.cpCapacity = 0;
    entity.knotCount = 0;
    entity.knotCapacity = 0;
}

/**
     * @brief 将内部 DxfEntity 转换为外部 DxfEntityWrapper（核心转换函数）
     * @param from 内部实体
     * @param to 外部包装器
     * @return 是否成功
     */
bool Convert(const DxfEntity& from, DxfEntityWrapper& to)
{
    Convert(from.attributes, to.attr);
    to.type = (DxfEntityType)(int)from.entityType;
    //联合体转换
    if (from.entityType == DxfEntity::Type::POINTDXF)
    {
        PointDxf pointData = boost::get<PointDxf>(from.entityData);
        Convert(pointData, to.data.point);
    }
    else if (from.entityType == DxfEntity::Type::LINE)
    {
        Line 
    }
    else if (from.entityType == DxfEntity::Type::CIRCLE)
    {

    }
    else if (from.entityType == DxfEntity::Type::ARC)
    {

    }
    else if (from.entityType == DxfEntity::Type::TEXT)
    {

    }
    else if (from.entityType == DxfEntity::Type::POLYLINE)
    {

    }
    else if (from.entityType == DxfEntity::Type::INSERT)
    {

    }
    else if (from.entityType == DxfEntity::Type::SPLINE)
    {

    }
    else if (from.entityType == DxfEntity::Type::UNKNOWN)
    {

    }
    to.data =from.entityData
}
#pragma endregion






/// <summary>
/// 
/// </summary>
/// <param name="filePath">dxf文件路径</param>
/// <returns></returns>
dxflib_EXPORTS_API void __stdcall ReadDXF(const char* filePath)
{
	DL_Dxf dxf;

	dxflibCreationClass* creator = new dxflibCreationClass();

	bool success = dxf.in(filePath, creator);

	if (success)
	{
		std::cout << "DXF 文件读取成功！" << std::endl;

		// --- 读取完成后，数据已经都在 creator->g_entityList 里了 ---

		std::cout << "共读取到 " << creator->g_entityList.size() << " 个实体。" << std::endl;

		// 验证数据
		for (const auto& entity : creator->g_entityList) {//类似C#中的foreach，auto自动推断类型为DxfEntity
			// 根据 entityType 判断具体类型并处理
			if (entity.entityType == DxfEntity::Type::POINTDXF)
			{
				PointDxf pointDxfData = boost::get<PointDxf>(entity.entityData);
				std::cout << "发现点：坐标("<<pointDxfData.pointCoord.x<<","<<pointDxfData.pointCoord.y<<","<<pointDxfData.pointCoord.z<<")" << std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::LINE) {
				// 获取联合体中的 Line 数据
				Line lineData = boost::get<Line>(entity.entityData);
				std::cout << "发现直线: 起点(" << lineData.start.x << ", " << lineData.start.y << ","<< lineData.start.z<<"),终点（"<<lineData.end.x<<","<<lineData.end.y<<","<<lineData.end.z<<"）" << std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::CIRCLE) {
				Circle circleData = boost::get<Circle>(entity.entityData);
				std::cout << "发现圆: 圆心("<<circleData.center.x<<","<<circleData.center.y<<","<<circleData.center.z<< ")，半径："<<circleData.radius<< std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::ARC)
			{
				Arc arcData = boost::get<Arc>(entity.entityData);
				std::cout << "发现圆弧:圆心("<< arcData.center.x<<","<<arcData.center.y<<","<<arcData.center.z<<")，半径："<<arcData.radius<<",开始角度："<<arcData.startAngle<<",结束角度："<<arcData.endAngle << std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::TEXT)
			{
				Text textData = boost::get<Text>(entity.entityData);
				std::cout << "发现文本:插入位置：("<<textData.inserPoint.x<<","<<textData.inserPoint.y<<","<<textData.inserPoint.z<<
					")字高："<<textData.height<<"旋转角度："<<textData.rotation<<"\r\n文本内容："<<textData.content<< std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::POLYLINE)
			{
				Polyline polylineData = boost::get<Polyline>(entity.entityData);
				std::cout << "发现多段线:顶点个数"<<polylineData.vertices.size() <<"收尾是否相接："<<polylineData.isClosed << std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::INSERT)
			{
				std::cout << "发现块引用" << std::endl;
			}
			else if (entity.entityType == DxfEntity::Type::SPLINE)
			{
				std::cout << "发现样条曲线" << std::endl;
			}
		}
	}
	else {
		std::cerr << "读取 DXF 文件失败" << std::endl;
	}

	// 5. 清理内存
	delete creator;
}