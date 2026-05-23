#include "pch.h"
#include "dxflibAPIExport.h"//含对外结构体
#include "CADAnalysisDlldxflib.h"//含内部结构体


#pragma region 纯C导出实现
/// <summary>
/// 
/// </summary>
/// <param name="filePath">dxf文件路径</param>
/// <returns></returns>
dxflib_EXPORTS_API int __stdcall ReadDXF(DxfDocument_Handle hdxfDocument, const char* filePath)
{
	if (!hdxfDocument)return -1;
	DeleteAllReadEntity(hdxfDocument);
	DeleteAllReadBlock(hdxfDocument);


	DL_Dxf dxf;

	//dxflibCreationClass* creator = new dxflibCreationClass();
	dxflibCreationClass* creator = (dxflibCreationClass*)hdxfDocument;

	//creator->RemoveAndDeletePolylineStack();


	bool success = dxf.in(filePath, creator);//dxflib库的读取函数此行

#ifdef _DEBUG
	if (success)
	{
		std::cout << "DXF 文件读取成功！" << std::endl;

		// --- 读取完成后，数据已经都在 creator->g_entityList 里了 ---

		std::cout << "共读取到 " << creator->g_entityList.size() << " 个实体。" << std::endl;

		//验证块内数据
		for (const auto& block : creator->g_blockList)
		{
			std::cout << "发现块，名称：" << block.blockName << "对齐点(" << block.alignPoint.x << "," << block.alignPoint.y << "," << block.alignPoint.z << ")" << std::endl;

			for (const auto& entity : block.g_blockEntityList)
			{
				// 根据 entityType 判断具体类型并处理
				if (entity.type == DxfEntityType::DXF_ENTITY_POINT)
				{
					//PointDxf pointDxfData = boost::get<PointDxf>(entity.entityData);
					DxfPointEntity dxfPointEntity = entity.data.point;

					std::cout << "块内发现点：坐标(" << dxfPointEntity.pointCoord.x << "," << dxfPointEntity.pointCoord.y << "," << dxfPointEntity.pointCoord.z << ")" << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_LINE) {
					// 获取联合体中的 Line 数据
					DxfLineEntity dxfLineEntity = entity.data.line;
					std::cout << "块内发现直线段: 起点(" << dxfLineEntity.start.x << ", " << dxfLineEntity.start.y << "," << dxfLineEntity.start.z <<
						"),终点（" << dxfLineEntity.end.x << "," << dxfLineEntity.end.y << "," << dxfLineEntity.end.z << "）" << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_CIRCLE) {
					DxfCircleEntity circleData = entity.data.circle;
					std::cout << "块内发现圆: 圆心(" << circleData.center.x << "," << circleData.center.y << "," << circleData.center.z << ")，半径：" << circleData.radius << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_ARC)
				{
					DxfArcEntity arcData = entity.data.arc;
					std::cout << "块内发现圆弧:圆心(" << arcData.center.x << "," << arcData.center.y << "," << arcData.center.z << ")，半径：" << arcData.radius << ",开始角度：" << arcData.startAngle << ",结束角度：" << arcData.endAngle << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_TEXT)
				{
					DxfTextEntity textData = entity.data.text;
					std::cout << "块内发现文本:插入位置：(" << textData.insertPoint.x << "," << textData.insertPoint.y << "," << textData.insertPoint.z <<
						")字高：" << textData.height << "旋转角度：" << textData.rotation << "\r\n文本内容：" << textData.content << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_POLYLINE)
				{//打印的全是0
					DxfPolylineEntity polylineData = entity.data.polyline;
					std::cout << "块内发现多段线:顶点个数" << polylineData.vertexCount << "收尾是否相接：" << polylineData.pFlags << std::endl;
					DxfPoint tempPoint;
					for (int i = 0;i < polylineData.vertexCount;i++)
					{
						GetVertexAt(hdxfDocument, i, &polylineData, &tempPoint);
						std::cout << "顶点" << i << "(" << tempPoint.x << "," << tempPoint.y << "," << tempPoint.z << ")" << std::endl;
					}
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_INSERT)
				{
					DxfInsertEntity insertEntity = entity.data.insert;
					std::cout << "块内发现块引用:引用名称" << insertEntity.blockName << "插入点：(" << insertEntity.position.x << "," << insertEntity.position.y << "," << insertEntity.position.z <<
						")X轴缩放" << insertEntity.scaleX << "Y轴缩放" << insertEntity.scaleY << "Z轴缩放" << insertEntity.scaleZ << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_ELLIPSE)
				{
					DxfEllipseEntity ellipseEntity = entity.data.ellipse;
					std::cout<<"块内发现椭圆:中心点("<< ellipseEntity.EllipseCenter.x << "," << ellipseEntity.EllipseCenter.y << "," << ellipseEntity.EllipseCenter.z <<
						")LongAxisVectorQuantity(" << ellipseEntity.LongAxisVectorQuantity.x << "," << ellipseEntity.LongAxisVectorQuantity.y << "," << ellipseEntity.LongAxisVectorQuantity.z <<
						")ShortAxis/LongAxis:" << ellipseEntity.ShortRatioLong << "StartAngle" << ellipseEntity.StartAngle << "EndAngle" << ellipseEntity.EndAngle << std::endl;
				}
				else if (entity.type == DxfEntityType::DXF_ENTITY_SPLINE)
				{
					DxfSplineEntity splineEntity = entity.data.spline;
					std::cout << "发现样条曲线:阶数:" << splineEntity.degree<<"控制点数量："<<splineEntity.ccontrolCount<<"拟合点数量："<<splineEntity.fitCount<<"节点数量:"<<splineEntity.knotCount<<"曲线标志:"<<splineEntity.flags<< std::endl;
					int i = 0;
					for (const auto& controlPointVector : *dxflibCreationClass::GetSplineControlPointList(splineEntity._controlPointsHandle))//GetSplineControlPointList返回的是指针，遍历要的是对象
					{//常量引用，开销臂auto复制更小
						std::cout << "控制点" << ++i << ":位置(" << controlPointVector.controlPoint.x << "," << controlPointVector.controlPoint.y << "," << controlPointVector.controlPoint.z << ")权重" << controlPointVector.weight << std::endl;
					}
					i = 0;
					for (const auto& fitPointVector : *dxflibCreationClass::GetSplineFitPointList(splineEntity._fitPointHandle))
					{
						std::cout << "拟合点" << ++i << ":位置(" << fitPointVector.fitPoint.x << "," << fitPointVector.fitPoint.y << "," << fitPointVector.fitPoint.z << ")" << std::endl;
					}
					i = 0;

					vector<SplineKnot>* pFitPointList = (vector<SplineKnot>*)dxflibCreationClass::GetSplineKnotList(splineEntity._knotsHandle);
					for (std::vector<SplineKnot>::iterator it = pFitPointList->begin();it != pFitPointList->end();++it)
					{
						std::cout << "节点向量" << ++i << ":" << it->knot << std::endl;
					}
					/*for (const auto& knotVector : *dxflibCreationClass::GetSplineKnotList(splineEntity._knotsHandle))
					{
						std::cout << "节点向量" << ++i << ":" << knotVector.knot << std::endl;
					}*/
				}
			}
		}

		// 验证实体数据
		for (const auto& entity : creator->g_entityList) {//类似C#中的foreach，auto自动推断类型为DxfEntity
			// 根据 entityType 判断具体类型并处理
			if (entity.type == DxfEntityType::DXF_ENTITY_POINT)
			{
				//PointDxf pointDxfData = boost::get<PointDxf>(entity.entityData);
				DxfPointEntity dxfPointEntity = entity.data.point;

				std::cout << "发现点：坐标(" << dxfPointEntity.pointCoord.x << "," << dxfPointEntity.pointCoord.y << "," << dxfPointEntity.pointCoord.z << ")" << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_LINE) {
				// 获取联合体中的 Line 数据
				DxfLineEntity dxfLineEntity = entity.data.line;
				std::cout << "发现直线段: 起点(" << dxfLineEntity.start.x << ", " << dxfLineEntity.start.y << "," << dxfLineEntity.start.z <<
					"),终点（" << dxfLineEntity.end.x << "," << dxfLineEntity.end.y << "," << dxfLineEntity.end.z << "）" << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_CIRCLE) {
				DxfCircleEntity circleData = entity.data.circle;
				std::cout << "发现圆: 圆心(" << circleData.center.x << "," << circleData.center.y << "," << circleData.center.z << ")，半径：" << circleData.radius << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_ARC)
			{
				DxfArcEntity arcData = entity.data.arc;
				std::cout << "发现圆弧:圆心(" << arcData.center.x << "," << arcData.center.y << "," << arcData.center.z << ")，半径：" << arcData.radius << ",开始角度：" << arcData.startAngle << ",结束角度：" << arcData.endAngle << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_TEXT)
			{
				DxfTextEntity textData = entity.data.text;
				std::cout << "发现文本:插入位置：(" << textData.insertPoint.x << "," << textData.insertPoint.y << "," << textData.insertPoint.z <<
					")字高：" << textData.height << "旋转角度：" << textData.rotation << "\r\n文本内容：" << textData.content << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_POLYLINE)
			{//打印的全是0
				DxfPolylineEntity polylineData = entity.data.polyline;
				std::cout << "发现多段线:顶点个数" << polylineData.vertexCount << "收尾是否相接：" << polylineData.pFlags << std::endl;
				DxfPoint tempPoint;
				for (int i = 0;i < polylineData.vertexCount;i++)
				{
					GetVertexAt(hdxfDocument, i, &polylineData, &tempPoint);
					std::cout << "顶点" << i << "(" << tempPoint.x << "," << tempPoint.y << "," << tempPoint.z << ")" << std::endl;
				}
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_INSERT)
			{
				DxfInsertEntity insertEntity = entity.data.insert;
				std::cout << "发现块引用:引用名称" << insertEntity.blockName << "插入点：(" << insertEntity.position.x << "," << insertEntity.position.y << "," << insertEntity.position.z <<
					")X轴缩放" << insertEntity.scaleX << "Y轴缩放" << insertEntity.scaleY << "Z轴缩放" << insertEntity.scaleZ << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_ELLIPSE)
			{
				DxfEllipseEntity ellipseEntity = entity.data.ellipse;
				std::cout << "发现椭圆:中心点(" << ellipseEntity.EllipseCenter.x << "," << ellipseEntity.EllipseCenter.y << "," << ellipseEntity.EllipseCenter.z <<
					")LongAxisVectorQuantity(" << ellipseEntity.LongAxisVectorQuantity.x << "," << ellipseEntity.LongAxisVectorQuantity.y << "," << ellipseEntity.LongAxisVectorQuantity.z <<
					")ShortAxis/LongAxis:" << ellipseEntity.ShortRatioLong << "StartAngle" << ellipseEntity.StartAngle << "EndAngle" << ellipseEntity.EndAngle << std::endl;
			}
			else if (entity.type == DxfEntityType::DXF_ENTITY_SPLINE)
			{
				DxfSplineEntity splineEntity = entity.data.spline;
				std::cout << "发现样条曲线:阶数:" << splineEntity.degree << "控制点数量：" << splineEntity.ccontrolCount << "拟合点数量：" << splineEntity.fitCount << "节点数量:" << splineEntity.knotCount << "曲线标志:" << splineEntity.flags << std::endl;
				int i = 0;
				for (const auto& controlPointVector : *dxflibCreationClass::GetSplineControlPointList(splineEntity._controlPointsHandle))//GetSplineControlPointList返回的是指针，遍历要的是对象
				{//常量引用，开销臂auto复制更小
					std::cout << "控制点" << ++i << ":位置(" << controlPointVector.controlPoint.x << "," << controlPointVector.controlPoint.y << "," << controlPointVector.controlPoint.z << ")权重" << controlPointVector.weight << std::endl;
				}
				i = 0;
				for (const auto& fitPointVector : *dxflibCreationClass::GetSplineFitPointList(splineEntity._fitPointHandle))
				{
					std::cout << "拟合点" << ++i << ":位置(" << fitPointVector.fitPoint.x << "," << fitPointVector.fitPoint.y << "," << fitPointVector.fitPoint.z << ")" << std::endl;
				}
				i = 0;

				vector<SplineKnot>* pFitPointList = (vector<SplineKnot>*)dxflibCreationClass::GetSplineKnotList(splineEntity._knotsHandle);
				for (std::vector<SplineKnot>::iterator it = pFitPointList->begin();it != pFitPointList->end();++it)
				{
					std::cout << "节点向量" << ++i << ":" << it->knot << std::endl;
				}
				//for (const auto& knotVector : *dxflibCreationClass::GetSplineKnotList(splineEntity._knotsHandle))
				//{
				//	std::cout << "节点向量" << ++i << ":" << knotVector.knot << std::endl;
				//}
			}
		}
	}
	else {
		std::cerr << "读取 DXF 文件失败" << std::endl;
	}
#endif
	//// 清理内存
	//delete creator;
	return 0;
}

dxflib_EXPORTS_API DxfDocument_Handle __stdcall CreateDxfDocument()
{
	// 在堆上创建 C++ 对象
	dxflibCreationClass* pDoc = new dxflibCreationClass();
	// 返回其指针，伪装成句柄
	return static_cast<DxfDocument_Handle>(pDoc);
}

dxflib_EXPORTS_API int __stdcall DestroyDxfDocument(DxfDocument_Handle hdxfDocument)
{
	if (hdxfDocument)
	{
		// 还原指针并删除
		dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);
		delete pDoc;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetEntityCount(DxfDocument_Handle hdxfDocument)
{
	if (!hdxfDocument) return -1;
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);
	//return pDoc->GetEntityCount();
	return pDoc->g_entityList.size();
}

dxflib_EXPORTS_API int __stdcall GetEntityAt(DxfDocument_Handle hdxfDocument, int index, DxfEntityWrapper* outWrapper)
{
	//if (!hdxfDocument || !outWrapper) return -1;
	if (!hdxfDocument) return -1;
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	try
	{
		//outWrapper = &(pDoc->g_entityList.at(index));//修改的是副本的指向
		*outWrapper = pDoc->g_entityList.at(index);
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetBlockCount(DxfDocument_Handle hdxfDocument)
{
	if (!hdxfDocument) return -1;
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);
	//return pDoc->GetEntityCount();
	return pDoc->g_blockList.size();
}

dxflib_EXPORTS_API int __stdcall GetBlockAt(DxfDocument_Handle hdxfDocument, int index, char* OutBlockName, int bufferSize)
{
	if (!hdxfDocument) return -1;
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	try
	{
		//outWrapper = &(pDoc->g_entityList.at(index));//修改的是副本的指向
		strncpy(OutBlockName, pDoc->g_blockList.at(index).blockName.c_str(), bufferSize);
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetBlockInEntityCount(DxfDocument_Handle hdxfDocument, const int blockIndex)
{
	if (!hdxfDocument) return -1;
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);
	//return pDoc->GetEntityCount();
	if (blockIndex >= pDoc->g_blockList.size())return -1;
	return pDoc->g_blockList[blockIndex].g_blockEntityList.size();
}

dxflib_EXPORTS_API int __stdcall GetBlockInEntityAt(DxfDocument_Handle hdxfDocument, const int blockIndex, const int entityIndex, DxfEntityWrapper* outWrapper)
{
	if (!hdxfDocument) return -1;
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	if (blockIndex >= pDoc->g_blockList.size())return -1;
	if (entityIndex >= pDoc->g_blockList[blockIndex].g_blockEntityList.size())return -1;

	try
	{
		*outWrapper = pDoc->g_blockList[blockIndex].g_blockEntityList.at(entityIndex);//修改的是副本的指向
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetVertexCount(DxfDocument_Handle hdxfDocument, const DxfPolylineEntity* polylineEntity)
{
	if (!hdxfDocument || !polylineEntity)return -1;

	const PolylinePointList* pVec = dxflibCreationClass::GetPolylineList(polylineEntity->_vertexHandle);
	if (!pVec)
		return 0;

	return static_cast<int>(pVec->size());
}

dxflib_EXPORTS_API int __stdcall GetVertexAt(DxfDocument_Handle hdxfDocument, const int index, const DxfPolylineEntity* polylineEntity, DxfPoint* outPoint)
{
	if (!hdxfDocument || !polylineEntity || !outPoint)return -1;

	const PolylinePointList* pVec = dxflibCreationClass::GetPolylineList(polylineEntity->_vertexHandle);

#ifdef _DEBUG
	// 加日志，打印指针值
	printf("[DEBUG] _vertexHandle = %p\n", polylineEntity->_vertexHandle);

	// **关键：尝试访问 pVec 的 size()**
	printf("[DEBUG] pVec->size() = %zu\n", pVec->size());  // 如果这里就崩溃，说明指针无效
#endif
	try
	{
		*outPoint = pVec->at(index);
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetSplineControlPointCount(DxfDocument_Handle hdxfDocument, const DxfSplineEntity* splineEntity)
{
	if (!hdxfDocument || !splineEntity)return -1;
	const SplineControlPointList* pSCP = dxflibCreationClass::GetSplineControlPointList(splineEntity->_controlPointsHandle);
	return static_cast<int>(pSCP->size());
}

dxflib_EXPORTS_API int __stdcall GetSplineControlPointAt(DxfDocument_Handle hdxfDocument, const int index, const DxfSplineEntity* splineEntity, SplineControlPoint* outControlPoint)
{
	if (!hdxfDocument || !splineEntity || !outControlPoint)return -1;
	const SplineControlPointList* pSCP = dxflibCreationClass::GetSplineControlPointList(splineEntity->_controlPointsHandle);
	try
	{
		*outControlPoint = pSCP->at(index);
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetSplineFitPointCount(DxfDocument_Handle hdxfDocument, const DxfSplineEntity* splineEntity)
{
	if (!hdxfDocument || !splineEntity)return -1;
	const SplineFitPointList* pSFP = dxflibCreationClass::GetSplineFitPointList(splineEntity->_fitPointHandle);
	return static_cast<int>(pSFP->size());
}

dxflib_EXPORTS_API int __stdcall GetSplineFitPointAt(DxfDocument_Handle hdxfDocument, const int index, const DxfSplineEntity* splineEntity, SplineFitPoint* outFitPoint)
{
	if (!hdxfDocument || !splineEntity || !outFitPoint)return -1;
	const SplineFitPointList* pSFP = dxflibCreationClass::GetSplineFitPointList(splineEntity->_fitPointHandle);
	try
	{
		*outFitPoint = pSFP->at(index);
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall GetSplineKnotCount(DxfDocument_Handle hdxfDocument, const DxfSplineEntity* splineEntity)
{
	if (!hdxfDocument || !splineEntity)return -1;
	const SplineKnotList* pSK = dxflibCreationClass::GetSplineKnotList(splineEntity->_knotsHandle);
	return static_cast<int>(pSK->size());
}

dxflib_EXPORTS_API int __stdcall GetSplineKnotPointAt(DxfDocument_Handle hdxfDocument, const int index, const DxfSplineEntity* splineEntity, SplineKnot* outKnot)
{
	if (!hdxfDocument || !splineEntity || !outKnot)return -1;
	const SplineKnotList* pSK = dxflibCreationClass::GetSplineKnotList(splineEntity->_knotsHandle);
	try
	{
		*outKnot = pSK->at(index);
	}
	catch (const std::out_of_range& e)
	{
		return -1;
	}
	return 0;
}

dxflib_EXPORTS_API int __stdcall DeleteAllReadEntity(DxfDocument_Handle hdxfDocument)
{
	if (!hdxfDocument)return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);
	pDoc->RemoveAndDeletePolylineStack();//多段线栈缓冲

	/*pDoc->g_entityList.*/

	// 遍历所有元素，释放 polyline 里的句柄内存
	for (auto& wrapper : pDoc->g_entityList)
	{
		// 只有多段线 Polyline与样条线 有动态句柄，需要释放
		if (wrapper.type == DXF_ENTITY_POLYLINE)
		{
			if (wrapper.data.polyline._vertexHandle != 0)
			{
				dxflibCreationClass::DestroyPolylineList(wrapper.data.polyline._vertexHandle);
				wrapper.data.polyline._vertexHandle = 0;
			}
		}
		else if (wrapper.type == DXF_ENTITY_SPLINE)
		{
			if (wrapper.data.spline._controlPointsHandle != 0)
			{
				dxflibCreationClass::DestroySplineControlPointList(wrapper.data.spline._controlPointsHandle);
				wrapper.data.spline._controlPointsHandle = 0;
			}
			if (wrapper.data.spline._fitPointHandle != 0)
			{
				dxflibCreationClass::DestroySplineFitPointList(wrapper.data.spline._fitPointHandle);
				wrapper.data.spline._fitPointHandle = 0;
			}
			if (wrapper.data.spline._knotsHandle != 0)
			{
				dxflibCreationClass::DestroySplineKnotList(wrapper.data.spline._knotsHandle);
				wrapper.data.spline._knotsHandle = 0;
			}
		}
	}

	pDoc->g_entityList.clear();

	//强制释放 vector 预留的内存
	pDoc->g_entityList.shrink_to_fit();

	return 0;
}

dxflib_EXPORTS_API int __stdcall DeleteAllReadBlock(DxfDocument_Handle hdxfDocument)
{
	if (!hdxfDocument)return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	pDoc->RemoveAndDeletePolylineStack();//多段线栈缓冲

	for (auto& block : pDoc->g_blockList)
	{//块下实体下的多端线句柄释放
		for (auto& entity : block.g_blockEntityList)
		{
			if (entity.type == DXF_ENTITY_POLYLINE)
			{
				if (entity.data.polyline._vertexHandle != 0)
				{
					dxflibCreationClass::DestroyPolylineList(entity.data.polyline._vertexHandle);
					entity.data.polyline._vertexHandle = 0;
				}
			}
			else if (entity.type == DXF_ENTITY_SPLINE)
			{
				if (entity.data.spline._controlPointsHandle != 0)
				{
					dxflibCreationClass::DestroySplineControlPointList(entity.data.spline._controlPointsHandle);
					entity.data.spline._controlPointsHandle = 0;
				}
				if (entity.data.spline._fitPointHandle != 0)
				{
					dxflibCreationClass::DestroySplineFitPointList(entity.data.spline._fitPointHandle);
					entity.data.spline._fitPointHandle = 0;
				}
				if (entity.data.spline._knotsHandle != 0)
				{
					dxflibCreationClass::DestroySplineKnotList(entity.data.spline._knotsHandle);
					entity.data.spline._knotsHandle = 0;
				}
			}
		}
	}
	pDoc->g_blockList.clear();
	pDoc->g_blockList.shrink_to_fit();

	return 0;
}

dxflib_EXPORTS_API int __stdcall WriteSingleEntity(
	DxfDocument_Handle hdxfDocument,
	const DxfEntityWrapper* pWrapper
)
{
	if (!hdxfDocument || !pWrapper) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	// 获取当前即将插入的索引（即当前的 size）
	//int newIndex = static_cast<int>(pDoc->g_entityList.size());
	int newIndex = static_cast<int>(pDoc->g_writeEntityList.size());

	// 安全拷贝数据
	DxfEntityWrapper localEntity = *pWrapper;//localEntity在栈上

	if (localEntity.type == DXF_ENTITY_POLYLINE)
	{//多段线顶点动态数组
		//std::vector<DxfPoint> points;
		PolylinePointList points;
		points.reserve(localEntity.data.polyline.vertexCount);
		int handle = dxflibCreationClass::CreatePolylineList(points);//CreatePolylineList内部会拷贝，points销毁也无所谓

		localEntity.data.polyline._vertexHandle = handle;
	}
	else if (localEntity.type == DXF_ENTITY_SPLINE)
	{//样条线控制点、拟合点、节点向量
		SplineControlPointList SCPL;
		SCPL.reserve(localEntity.data.spline.ccontrolCount);
		int handleSCPL = dxflibCreationClass::CreateSplineControlPointList(SCPL);
		localEntity.data.spline._controlPointsHandle = handleSCPL;

		SplineFitPointList SFPL;
		SFPL.reserve(localEntity.data.spline.fitCount);
		int handleSFPL = dxflibCreationClass::CreateSplineFitPointList(SFPL);
		localEntity.data.spline._fitPointHandle = handleSFPL;

		SplineKnotList SKL;
		SKL.reserve(localEntity.data.spline.knotCount);
		int handleSKL = dxflibCreationClass::CreateSplineKnotList(SKL);
		localEntity.data.spline._knotsHandle = handleSKL;
	}

	// 存入列表
	//pDoc->g_entityList.push_back(localEntity);
	pDoc->g_writeEntityList.push_back(localEntity);//值拷贝，出了函数localEntity被销毁与数组中无关

	// 返回索引
	return newIndex;
}

//dxflib_EXPORTS_API int __stdcall WriteSinglePolylinePeak(DxfDocument_Handle hdxfDocument, DxfPoint polylinePeak)
//{
//	/*dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);*/
//
//	return 0;
//}

dxflib_EXPORTS_API int __stdcall WriteSinglePolylinePeakEntity(
	DxfDocument_Handle hdxfDocument,
	int entityIndex,
	const DxfPoint* pPoint
)
{
	if (!hdxfDocument || !pPoint) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	// 边界检查
	//if (entityIndex < 0 || entityIndex >= pDoc->g_entityList.size()) return -1;
	if (entityIndex < 0 || entityIndex >= pDoc->g_writeEntityList.size()) return -1;

	// 根据索引找到刚才创建的那个实体
	//DxfEntityWrapper& targetEntity = pDoc->g_entityList[entityIndex];
	DxfEntityWrapper& targetEntity = pDoc->g_writeEntityList[entityIndex];

	// 再次确认类型，防止传错索引（比如把圆的索引传进来了）
	if (targetEntity.type != DXF_ENTITY_POLYLINE) return -1;
	// 还原句柄并添加数据
	// 从内部实体中取出 _vertexHandle
	DxfPolylineVertexBuffer_Handle hVertices = targetEntity.data.polyline._vertexHandle;

	if (hVertices == 0)
		return -1;

	PolylinePointList* pVec = dxflibCreationClass::GetPolylineList(hVertices);
	if (!pVec)
		return -1;

	if (pVec->size() >= targetEntity.data.polyline.vertexCount)return -1;

	pVec->push_back(*pPoint);

	return 0;
}

dxflib_EXPORTS_API int __stdcall WriteSingleSplineControlPointEntity(
	DxfDocument_Handle hdxfDocument,
	int entityIndex,
	const SplineControlPoint* pSCP
)
{
	if (!hdxfDocument || !pSCP) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	if (entityIndex < 0 || entityIndex >= pDoc->g_writeEntityList.size()) return -1;

	DxfEntityWrapper& targetEntity = pDoc->g_writeEntityList[entityIndex];
	if (targetEntity.type != DXF_ENTITY_SPLINE) return -1;

	DxfSplineControlPoint_Handle hControlPoint = targetEntity.data.spline._controlPointsHandle;

	if (hControlPoint == 0)
		return -1;

	SplineControlPointList* pSCPL = dxflibCreationClass::GetSplineControlPointList(hControlPoint);
	if (!pSCPL)
		return -1;

	if (pSCPL->size() >= targetEntity.data.spline.ccontrolCount)
		return -1;

	pSCPL->push_back(*pSCP);

	return 0;
}

dxflib_EXPORTS_API int __stdcall WriteSingleSplineFitPointEntity(
	DxfDocument_Handle hdxfDocument,
	int entityIndex,
	const SplineFitPoint* pSFP
)
{
	if (!hdxfDocument || !pSFP) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	if (entityIndex < 0 || entityIndex >= pDoc->g_writeEntityList.size()) return -1;

	DxfEntityWrapper& targetEntity = pDoc->g_writeEntityList[entityIndex];
	if (targetEntity.type != DXF_ENTITY_SPLINE) return -1;

	DxfSplineFitPoint_Handle hFitPoint = targetEntity.data.spline._fitPointHandle;

	if (hFitPoint == 0)
		return -1;

	SplineFitPointList* pSFPL = dxflibCreationClass::GetSplineFitPointList(hFitPoint);
	if (!pSFPL)
		return -1;

	if (pSFPL->size() >= targetEntity.data.spline.fitCount)
		return -1;

	pSFPL->push_back(*pSFP);

	return 0;
}

dxflib_EXPORTS_API int __stdcall WriteSingleSplineKnotEntity(
	DxfDocument_Handle hdxfDocument,
	int entityIndex,
	const SplineKnot* pSK
)
{
	if (!hdxfDocument || !pSK) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	if (entityIndex < 0 || entityIndex >= pDoc->g_writeEntityList.size()) return -1;

	DxfEntityWrapper& targetEntity = pDoc->g_writeEntityList[entityIndex];
	if (targetEntity.type != DXF_ENTITY_SPLINE) return -1;

	DxfSplineKnot_Handle hK = targetEntity.data.spline._knotsHandle;

	if (hK == 0)
		return -1;

	SplineKnotList* pSKL = dxflibCreationClass::GetSplineKnotList(hK);
	if (!pSKL)
		return -1;

	if (pSKL->size() >= targetEntity.data.spline.knotCount)
		return -1;

	pSKL->push_back(*pSK);

	return 0;
}

dxflib_EXPORTS_API int __stdcall WriteSingleBlock(
	DxfDocument_Handle hdxfDocument,
	const char* blockName,
	const DxfPoint* pPoint,
	const DxfEntityWrapper* pWrapper
)
{
	if (!hdxfDocument || !pWrapper || !pPoint) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	int blockIndex = -1;//实体要写入块的索引
	int newIndex = -1;//写入块下实体的索引

	for (int i = 0;i < pDoc->g_writeBlockList.size();i++)
	{
		if (pDoc->g_writeBlockList[i].blockName == blockName)
		{//有对应块时
			blockIndex = i;

			DxfEntityWrapper localEntity = *pWrapper;

			if (localEntity.type == DXF_ENTITY_POLYLINE)
			{
				//std::vector<DxfPoint>points;
				PolylinePointList points;
				points.reserve(localEntity.data.polyline.vertexCount);

				int handle = dxflibCreationClass::CreatePolylineList(points);

				localEntity.data.polyline._vertexHandle = handle;
			}
			else if (localEntity.type == DXF_ENTITY_SPLINE)
			{
				SplineControlPointList SCPL;
				SCPL.reserve(localEntity.data.spline.ccontrolCount);
				int handleSCPL = dxflibCreationClass::CreateSplineControlPointList(SCPL);
				localEntity.data.spline._controlPointsHandle = handleSCPL;

				SplineFitPointList SFPL;
				SFPL.reserve(localEntity.data.spline.fitCount);
				int handleSFPL = dxflibCreationClass::CreateSplineFitPointList(SFPL);
				localEntity.data.spline._fitPointHandle = handleSFPL;

				SplineKnotList SKL;
				SKL.reserve(localEntity.data.spline.knotCount);
				int handleSKL = dxflibCreationClass::CreateSplineKnotList(SKL);
				localEntity.data.spline._knotsHandle = handleSKL;
			}
			newIndex = pDoc->g_writeBlockList[i].g_blockEntityList.size();//size个数，推送后此个数和最后索引相同了
			// 存入列表

			pDoc->g_writeBlockList[i].g_blockEntityList.push_back(localEntity);
			break;
		}
	}

	if (blockIndex == -1)
	{//没有对应块时创建新的
		dxflibCreationClass::Block tempBlock;
		tempBlock.blockName = blockName;
		tempBlock.alignPoint = *pPoint;//对齐点

		blockIndex = pDoc->g_writeBlockList.size();//size个数，推送后此个数和最后索引相同了

		DxfEntityWrapper localEntity = *pWrapper;

		if (localEntity.type == DXF_ENTITY_POLYLINE)
		{
			//std::vector<DxfPoint>points;
			PolylinePointList points;
			points.reserve(localEntity.data.polyline.vertexCount);

			int handle = dxflibCreationClass::CreatePolylineList(points);

			localEntity.data.polyline._vertexHandle = handle;
		}
		else if (localEntity.type == DXF_ENTITY_SPLINE)
		{
			SplineControlPointList SCPL;
			SCPL.reserve(localEntity.data.spline.ccontrolCount);
			int handleSCPL = dxflibCreationClass::CreateSplineControlPointList(SCPL);
			localEntity.data.spline._controlPointsHandle = handleSCPL;

			SplineFitPointList SFPL;
			SFPL.reserve(localEntity.data.spline.fitCount);
			int handleSFPL = dxflibCreationClass::CreateSplineFitPointList(SFPL);
			localEntity.data.spline._fitPointHandle = handleSFPL;

			SplineKnotList SKL;
			SKL.reserve(localEntity.data.spline.knotCount);
			int handleSKL = dxflibCreationClass::CreateSplineKnotList(SKL);
			localEntity.data.spline._knotsHandle = handleSKL;
		}
		newIndex = 0;
		// 存入列表

		tempBlock.g_blockEntityList.push_back(localEntity);//拷贝

		pDoc->g_writeBlockList.push_back(tempBlock);
	}

	// 返回索引
	return newIndex;
}

dxflib_EXPORTS_API int __stdcall WriteSinglePolylinePeakBlock(
	DxfDocument_Handle hdxfDocument,
	const char* blockName,
	int entityIndex,
	const DxfPoint* pPoint
)
{
	if (!hdxfDocument || !pPoint) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	for (int i = 0;i < pDoc->g_writeBlockList.size();i++)
	{
		if (pDoc->g_writeBlockList[i].blockName == blockName)
		{//点要加入的块
			if (entityIndex < 0 || entityIndex >= pDoc->g_writeBlockList[i].g_blockEntityList.size()) return -1;
			DxfEntityWrapper& targetEntity = pDoc->g_writeBlockList[i].g_blockEntityList[entityIndex];

			//确定实体是多段线
			if (targetEntity.type != DXF_ENTITY_POLYLINE) return -1;

			DxfPolylineVertexBuffer_Handle hVertices = targetEntity.data.polyline._vertexHandle;

			if (hVertices == 0)
				return -1;

			PolylinePointList* pVec = dxflibCreationClass::GetPolylineList(hVertices);
			if (!pVec)
				return -1;

			if (pVec->size() >= targetEntity.data.polyline.vertexCount)return -1;

			pVec->push_back(*pPoint);

			return 0;
		}
	}
	return -1;
}

dxflib_EXPORTS_API int __stdcall WriteSingleSplineControlPointBlock(
	DxfDocument_Handle hdxfDocument,
	const char* blockName,
	int entityIndex,
	const SplineControlPoint* pSCP
)
{
	if (!hdxfDocument || !pSCP) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	for (int i = 0;i < pDoc->g_writeBlockList.size();i++)
	{
		if (pDoc->g_writeBlockList[i].blockName == blockName)
		{//点要加入的块
			if (entityIndex < 0 || entityIndex >= pDoc->g_writeBlockList[i].g_blockEntityList.size()) return -1;
			DxfEntityWrapper& targetEntity = pDoc->g_writeBlockList[i].g_blockEntityList[entityIndex];

			//确定实体是样条线
			if (targetEntity.type != DXF_ENTITY_SPLINE) return -1;

			DxfSplineControlPoint_Handle hSCP = targetEntity.data.spline._controlPointsHandle;

			if (hSCP == 0)
				return -1;

			SplineControlPointList* pSCPL = dxflibCreationClass::GetSplineControlPointList(hSCP);
			if (!pSCPL)
				return -1;

			if (pSCPL->size() >= targetEntity.data.spline.ccontrolCount)return -1;

			pSCPL->push_back(*pSCP);

			return 0;
		}
	}
	return -1;
}

dxflib_EXPORTS_API int __stdcall WriteSingleSplineFitPointBlock(
	DxfDocument_Handle hdxfDocument,
	const char* blockName,
	int entityIndex,
	const SplineFitPoint* pSFP
)
{
	if (!hdxfDocument || !pSFP) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	for (int i = 0;i < pDoc->g_writeBlockList.size();i++)
	{
		if (pDoc->g_writeBlockList[i].blockName == blockName)
		{//点要加入的块
			if (entityIndex < 0 || entityIndex >= pDoc->g_writeBlockList[i].g_blockEntityList.size()) return -1;
			DxfEntityWrapper& targetEntity = pDoc->g_writeBlockList[i].g_blockEntityList[entityIndex];

			//确定实体是样条线
			if (targetEntity.type != DXF_ENTITY_SPLINE) return -1;

			DxfSplineFitPoint_Handle hSFP = targetEntity.data.spline._fitPointHandle;

			if (hSFP == 0)
				return -1;

			SplineFitPointList* pSFPL = dxflibCreationClass::GetSplineFitPointList(hSFP);
			if (!pSFPL)
				return -1;

			if (pSFPL->size() >= targetEntity.data.spline.fitCount)return -1;

			pSFPL->push_back(*pSFP);

			return 0;
		}
	}
	return -1;
}

dxflib_EXPORTS_API int __stdcall WriteSingleSplineKnotBlock(
	DxfDocument_Handle hdxfDocument,
	const char* blockName,
	int entityIndex,
	const SplineKnot* pSK
)
{
	if (!hdxfDocument || !pSK) return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	for (int i = 0;i < pDoc->g_writeBlockList.size();i++)
	{
		if (pDoc->g_writeBlockList[i].blockName == blockName)
		{//点要加入的块
			if (entityIndex < 0 || entityIndex >= pDoc->g_writeBlockList[i].g_blockEntityList.size()) return -1;
			DxfEntityWrapper& targetEntity = pDoc->g_writeBlockList[i].g_blockEntityList[entityIndex];

			//确定实体是样条线
			if (targetEntity.type != DXF_ENTITY_SPLINE) return -1;

			DxfSplineKnot_Handle hSK = targetEntity.data.spline._knotsHandle;

			if (hSK == 0)
				return -1;

			SplineKnotList* pSKL = dxflibCreationClass::GetSplineKnotList(hSK);
			if (!pSKL)
				return -1;

			if (pSKL->size() >= targetEntity.data.spline.knotCount)return -1;//knotCount为负数会变无符号检测，变无穷大

			pSKL->push_back(*pSK);

			return 0;
		}
	}
	return -1;
}

dxflib_EXPORTS_API int __stdcall DeleteAllWriteBufferEntity(DxfDocument_Handle hdxfDocument)
{
	if (!hdxfDocument)return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);
	//pDoc->RemoveAndDeletePolylineStack();//多段线栈缓冲

	/*pDoc->g_entityList.*/

	// 遍历所有元素，释放 polyline 里的句柄内存
	for (auto& wrapper : pDoc->g_writeEntityList)
	{
		// 只有多段线 Polyline与样条线 有动态句柄，需要释放
		if (wrapper.type == DXF_ENTITY_POLYLINE)
		{
			if (wrapper.data.polyline._vertexHandle != 0)
			{
				dxflibCreationClass::DestroyPolylineList(wrapper.data.polyline._vertexHandle);
				wrapper.data.polyline._vertexHandle = 0;
			}
		}
		else if (wrapper.type == DXF_ENTITY_SPLINE)
		{
			if (wrapper.data.spline._controlPointsHandle != 0)
			{
				dxflibCreationClass::DestroySplineControlPointList(wrapper.data.spline._controlPointsHandle);
				wrapper.data.spline._controlPointsHandle = 0;
			}
			if (wrapper.data.spline._fitPointHandle != 0)
			{
				dxflibCreationClass::DestroySplineFitPointList(wrapper.data.spline._fitPointHandle);
				wrapper.data.spline._fitPointHandle = 0;
			}
			if (wrapper.data.spline._knotsHandle != 0)
			{
				dxflibCreationClass::DestroySplineKnotList(wrapper.data.spline._knotsHandle);
				wrapper.data.spline._knotsHandle = 0;
			}
		}
	}

	pDoc->g_writeEntityList.clear();

	// 强制释放 vector 预留的内存
	pDoc->g_writeEntityList.shrink_to_fit();
	return 0;
}

dxflib_EXPORTS_API int __stdcall DeleteAllWriteBufferBlock(DxfDocument_Handle hdxfDocument)
{
	if (!hdxfDocument)return -1;

	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	pDoc->RemoveAndDeletePolylineStack();//多段线栈缓冲

	for (auto& block : pDoc->g_writeBlockList)
	{//块下实体下的多端线句柄释放
		for (auto& entity : block.g_blockEntityList)
		{
			if (entity.type == DXF_ENTITY_POLYLINE)
			{
				if (entity.data.polyline._vertexHandle != 0)
				{
					dxflibCreationClass::DestroyPolylineList(entity.data.polyline._vertexHandle);
					entity.data.polyline._vertexHandle = 0;
				}
			}
			else if (entity.type == DXF_ENTITY_SPLINE)
			{
				if (entity.data.spline._controlPointsHandle != 0)
				{
					dxflibCreationClass::DestroySplineControlPointList(entity.data.spline._controlPointsHandle);
					entity.data.spline._controlPointsHandle = 0;
				}
				if (entity.data.spline._fitPointHandle != 0)
				{
					dxflibCreationClass::DestroySplineFitPointList(entity.data.spline._fitPointHandle);
					entity.data.spline._fitPointHandle = 0;
				}
				if (entity.data.spline._knotsHandle != 0)
				{
					dxflibCreationClass::DestroySplineKnotList(entity.data.spline._knotsHandle);
					entity.data.spline._knotsHandle = 0;
				}
			}

		}
	}
	pDoc->g_writeBlockList.clear();
	pDoc->g_writeBlockList.shrink_to_fit();
	return 0;
}

dxflib_EXPORTS_API int __stdcall WriteDXF(DxfDocument_Handle hdxfDocument, const char* path)
{
	dxflibCreationClass* pDoc = static_cast<dxflibCreationClass*>(hdxfDocument);

	DL_Dxf* dxf = new DL_Dxf();
	DL_Codes::version exportVersion = DL_Codes::AC1015;
	DL_WriterA* dw = dxf->out(path, exportVersion);
	if (dw == NULL) {
		std::cout << "Cannot open file: " << path << " for writing." << std::endl;
		delete dxf;
		delete dw;
		return -1;
	}

	//默认头
	dxf->writeHeader(*dw);
	//结束当前段
	dw->sectionEnd();


#pragma region 写入表段
	//开始写表段
	dw->sectionTables();// 开始写 TABLES 段（必须写）

	// 写入【视口配置】（默认视图）
	dxf->writeVPort(*dw);

	//写入线型（LINETYPE）
	dw->tableLinetypes(3);// 告诉CAD：下面要写 3 个线型
	dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));// 线型1：随块
	dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));// 线型2：随层
	dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));// 线型3：实线（必须有）
	dw->tableEnd();

	//写入的图层，0层必须要有
	int numberOfLayers = 3;
	dw->tableLayers(numberOfLayers);//创建3个图层

	//图层0
	dxf->writeLayer(*dw,
		DL_LayerData("0", 0),
		DL_Attributes(
			std::string(""),      // leave empty
			DL_Codes::black,        // default color
			100,                  // default width
			"CONTINUOUS", 1.0));       // default line style 实线
	//图层1
	dxf->writeLayer(*dw,
		DL_LayerData("mainlayer", 0),
		DL_Attributes(
			std::string(""),
			DL_Codes::red,
			100,
			"CONTINUOUS", 1.0));
	////图层2
	//dxf->writeLayer(*dw,
	//	DL_LayerData("anotherlayer", 0),
	//	DL_Attributes(
	//		std::string(""),
	//		DL_Codes::black,
	//		100,
	//		"CONTINUOUS", 1.0));


	dw->tableEnd();

	///写入文字类型
	dw->tableStyle(1);
	dxf->writeStyle(*dw, DL_StyleData(
		"standard",//样式名
		0,
		2.5, //字高
		1.0,//宽度因子
		0.0, 0, 2.5,
		"txt",//字体
		""));
	dw->tableEnd();

	///写入视图与坐标
	dxf->writeView(*dw);//默认视图
	dxf->writeUcs(*dw);//默认坐标系

	///写入应用程序标识（ACAD），标准AutoCAD
	dw->tableAppid(1);
	dxf->writeAppid(*dw, "ACAD");
	dw->tableEnd();

	///写入标注样式
	dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);
	//写入块记录，
	dxf->writeBlockRecord(*dw);//此块必须有
	//dxf->writeBlockRecord(*dw, "myblock1");//提前定义2个空块
	//dxf->writeBlockRecord(*dw, "myblock2");
	dw->tableEnd();

	dw->sectionEnd();
#pragma endregion

#pragma region 写入块定义信息
	dw->sectionBlocks();
#pragma region DXF必要的两个容器块，*Model_Space显示实际画面，*Paper_Space显示要打印内容
	dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Model_Space");//实体所在图纸空间
	dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Paper_Space");//布局空间（用来出图打印）
	//dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	//dxf->writeEndBlock(*dw, "*Paper_Space0");//默认布局
#pragma endregion

	for (const auto& block : pDoc->g_writeBlockList)//C++的遍历
	{

		dxf->writeBlock(*dw, DL_BlockData(block.blockName,//块名称
			0, //标志
			block.alignPoint.x, block.alignPoint.y, block.alignPoint.z));//插入块的对齐点，(0,0,0)就是块的左下角和插入的位置对齐，如插入到（10,20,0），那么块左下角和（10,20,0）对齐

		for (const auto& entity : block.g_blockEntityList)
		{//块内实体遍历
			switch (entity.type)
			{
			case DxfEntityType::DXF_ENTITY_POINT:
			{//点
				DxfPointEntity tempEntity = entity.data.point;
				dxf->writePoint(//写入点实体
					*dw,
					DL_PointData(tempEntity.pointCoord.x,
						tempEntity.pointCoord.y,
						tempEntity.pointCoord.z),
					DL_Attributes("0"/*"mainlayer"*/, 256, -1, "BYLAYER", 1.0));//在mainlayer图层上绘画，没有此图层就回到默认图层0
				//公共属性写在DL_Attributes中，暂时默认
			}
			break;

			case DxfEntityType::DXF_ENTITY_LINE:
			{//线
				DxfLineEntity tempEntity = entity.data.line;
				dxf->writeLine(//写入线
					*dw,
					DL_LineData(tempEntity.start.x,   // start point
						tempEntity.start.y,
						tempEntity.start.z,
						tempEntity.end.x,   // end point
						tempEntity.end.y,
						tempEntity.end.z),
					DL_Attributes("0"/*"mainlayer"*/, 256, -1, "BYLAYER", 1.0));
			}
			break;

			case DxfEntityType::DXF_ENTITY_CIRCLE:
			{//圆
				DxfCircleEntity tempEntity = entity.data.circle;
				dxf->writeCircle(
					*dw,
					DL_CircleData(    // 圆心坐标 (cx, cy, cz) 和 半径 (radius)
						tempEntity.center.x,         // cx - 圆心 X 坐标
						tempEntity.center.y,         // cy - 圆心 Y 坐标  
						tempEntity.center.z,          // cz - 圆心 Z 坐标
						tempEntity.radius          // radius - 半径
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
			}
			break;

			case DxfEntityType::DXF_ENTITY_ARC:
			{//圆弧
				DxfArcEntity tempEntity = entity.data.arc;
				dxf->writeArc(
					*dw,
					DL_ArcData(
						tempEntity.center.x,
						tempEntity.center.y,
						tempEntity.center.z,
						tempEntity.radius,
						tempEntity.startAngle,
						tempEntity.endAngle
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
			}
			break;

			case DxfEntityType::DXF_ENTITY_TEXT:
			{//按单行文本写入
				DxfTextEntity tempEntity = entity.data.text;
				dxf->writeText(
					*dw,
					DL_TextData(
						tempEntity.insertPoint.x,
						tempEntity.insertPoint.y,
						tempEntity.insertPoint.z,//插入点
						tempEntity.insertPoint.x,
						tempEntity.insertPoint.y,
						tempEntity.insertPoint.z,//默认左对齐与插入点相同
						tempEntity.height,
						1.0,//宽高比(大于1宽更大)
						0,//生成标志，0正常，2镜像，4倒置
						1,//水平对齐1左对齐，2居中，3右对齐，4对齐（拉伸以适应两点），5中间(几何中心)
						2,//垂直对齐1底部，2中间，3顶部
						tempEntity.content,
						"Standard",//样式名
						tempEntity.rotation
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
			}
			break;

			case DxfEntityType::DXF_ENTITY_POLYLINE:
			{//多段线
				DxfPolylineEntity tempEntity = entity.data.polyline;
				dxf->writePolyline(
					*dw,
					DL_PolylineData(
						tempEntity.vertexCount,
						0,//2D通常设置为0
						0,//2D通常设置为0
						tempEntity.pFlags,
						0.0//标高，2D的多个顶点都用此，3D忽略此
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
				//if (tempEntity._vertexHandle == nullptr)continue;
				if (tempEntity._vertexHandle == 0)continue;

				PolylinePointList* pVec = dxflibCreationClass::GetPolylineList(tempEntity._vertexHandle);
				if (!pVec || pVec->empty())
					continue;

				for (const auto& vertex : *pVec)
				{
					dxf->writeVertex(
						*dw,
						DL_VertexData(
							vertex.x,
							vertex.y,
							vertex.z,
							0//顶点与顶点之间的弧度，0就是直线。大于0凸圆弧，小于0凹圆弧，值=tan(圆弧圆心角/4)
						)
					);
				}

				dxf->writePolylineEnd(*dw);
			}
			break;

			case DxfEntityType::DXF_ENTITY_INSERT:
			{//块插入
				DxfInsertEntity tempEntity = entity.data.insert;
				dxf->writeInsert(
					*dw,
					DL_InsertData(
						tempEntity.blockName,
						tempEntity.position.x,
						tempEntity.position.y,
						tempEntity.position.z,
						tempEntity.scaleX,
						tempEntity.scaleY,
						tempEntity.scaleZ,
						tempEntity.rotation,
						1,//插入列矩阵个数
						1,//插入行矩阵个数
						0.0,//列间距
						0.0//行间距
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
			}
			break;

			case DxfEntityType::DXF_ENTITY_ELLIPSE:
			{//椭圆
				DxfEllipseEntity tempEntity = entity.data.ellipse;
				dxf->writeEllipse(
					*dw,
					DL_EllipseData(
						tempEntity.EllipseCenter.x,
						tempEntity.EllipseCenter.y,
						tempEntity.EllipseCenter.z,
						tempEntity.LongAxisVectorQuantity.x,
						tempEntity.LongAxisVectorQuantity.y,
						tempEntity.LongAxisVectorQuantity.z,
						tempEntity.ShortRatioLong,
						tempEntity.StartAngle,
						tempEntity.EndAngle
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
			}
			break;

			case DxfEntityType::DXF_ENTITY_SPLINE:
			{//样条线
				DxfSplineEntity tempEntity = entity.data.spline;
				dxf->writeSpline(
					*dw,
					DL_SplineData(
						tempEntity.degree,
						tempEntity.knotCount,
						tempEntity.ccontrolCount,
						tempEntity.fitCount,
						tempEntity.flags
					),
					DL_Attributes("0"/*"mainlayer"*/, 125, -1, "BYLAYER", 1.0));
				if (tempEntity._controlPointsHandle != 0)
				{
					SplineControlPointList* pSCPL = dxflibCreationClass::GetSplineControlPointList(tempEntity._controlPointsHandle);
					if (pSCPL && !pSCPL->empty())
					{
						for (const auto& SCPL : *pSCPL)
						{
							dxf->writeControlPoint(
								*dw,
								DL_ControlPointData(
									SCPL.controlPoint.x,
									SCPL.controlPoint.y,
									SCPL.controlPoint.z,
									SCPL.weight
								)
							);
						}
					}
				}
				if (tempEntity._fitPointHandle != 0)
				{
					SplineFitPointList* pSFPL = dxflibCreationClass::GetSplineFitPointList(tempEntity._fitPointHandle);
					if (pSFPL && !pSFPL->empty())
					{
						for (const auto& SFPL : *pSFPL)
						{
							dxf->writeFitPoint(
								*dw,
								DL_FitPointData(
									SFPL.fitPoint.x,
									SFPL.fitPoint.y,
									SFPL.fitPoint.z
								)
							);
						}
					}
				}
				if (tempEntity._knotsHandle != 0)
				{
					SplineKnotList* pSKL = dxflibCreationClass::GetSplineKnotList(tempEntity._knotsHandle);
					if (pSKL || !pSKL->empty())
					{
						for (const auto& SFPL : *pSKL)
						{
							dxf->writeKnot(
								*dw,
								DL_KnotData(
									SFPL.knot
								)
							);
						}
					}
				}
			}
			break;
			default:
				break;
			}
		}


		dxf->writeEndBlock(*dw, block.blockName);
	}

	dw->sectionEnd();
#pragma endregion

#pragma region 写入实体段

	dw->sectionEntities();
	for (unsigned int i = 0;i < pDoc->g_writeEntityList.size();i++)
	{
		switch (pDoc->g_writeEntityList[i].type)
		{
		case DxfEntityType::DXF_ENTITY_POINT:
		{//点
			DxfPointEntity tempEntity = pDoc->g_writeEntityList[i].data.point;
			dxf->writePoint(//写入点实体
				*dw,
				DL_PointData(tempEntity.pointCoord.x,
					tempEntity.pointCoord.y,
					tempEntity.pointCoord.z),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));//在mainlayer图层上绘画，没有此图层就回到默认图层0
			//公共属性写在DL_Attributes中，暂时默认
		}
		break;

		case DxfEntityType::DXF_ENTITY_LINE:
		{//线
			DxfLineEntity tempEntity = pDoc->g_writeEntityList[i].data.line;
			dxf->writeLine(//写入线
				*dw,
				DL_LineData(tempEntity.start.x,   // start point
					tempEntity.start.y,
					tempEntity.start.z,
					tempEntity.end.x,   // end point
					tempEntity.end.y,
					tempEntity.end.z),
				DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));
		}
		break;

		case DxfEntityType::DXF_ENTITY_CIRCLE:
		{//圆
			DxfCircleEntity tempEntity = pDoc->g_writeEntityList[i].data.circle;
			dxf->writeCircle(
				*dw,
				DL_CircleData(    // 圆心坐标 (cx, cy, cz) 和 半径 (radius)
					tempEntity.center.x,         // cx - 圆心 X 坐标
					tempEntity.center.y,         // cy - 圆心 Y 坐标  
					tempEntity.center.z,          // cz - 圆心 Z 坐标
					tempEntity.radius          // radius - 半径
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
		}
		break;

		case DxfEntityType::DXF_ENTITY_ARC:
		{//圆弧
			DxfArcEntity tempEntity = pDoc->g_writeEntityList[i].data.arc;
			dxf->writeArc(
				*dw,
				DL_ArcData(
					tempEntity.center.x,
					tempEntity.center.y,
					tempEntity.center.z,
					tempEntity.radius,
					tempEntity.startAngle,
					tempEntity.endAngle
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
		}
		break;

		case DxfEntityType::DXF_ENTITY_TEXT:
		{//按单行文本写入
			DxfTextEntity tempEntity = pDoc->g_writeEntityList[i].data.text;
			dxf->writeText(
				*dw,
				DL_TextData(
					tempEntity.insertPoint.x,
					tempEntity.insertPoint.y,
					tempEntity.insertPoint.z,//插入点
					tempEntity.insertPoint.x,
					tempEntity.insertPoint.y,
					tempEntity.insertPoint.z,//默认左对齐与插入点相同
					tempEntity.height,
					1.0,//宽高比(大于1宽更大)
					0,//生成标志，0正常，2镜像，4倒置
					1,//水平对齐1左对齐，2居中，3右对齐，4对齐（拉伸以适应两点），5中间(几何中心)
					2,//垂直对齐1底部，2中间，3顶部
					tempEntity.content,
					"Standard",//样式名
					tempEntity.rotation
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
		}
		break;

		case DxfEntityType::DXF_ENTITY_POLYLINE:
		{//多段线
			DxfPolylineEntity tempEntity = pDoc->g_writeEntityList[i].data.polyline;
			dxf->writePolyline(
				*dw,
				DL_PolylineData(
					tempEntity.vertexCount,
					0,//2D通常设置为0
					0,//2D通常设置为0
					tempEntity.pFlags,
					0.0//标高，2D的多个顶点都用此，3D忽略此
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
			//if (tempEntity._vertexHandle == nullptr)continue;
			if (tempEntity._vertexHandle == 0)continue;

			PolylinePointList* pVec = dxflibCreationClass::GetPolylineList(tempEntity._vertexHandle);
			if (!pVec || pVec->empty())continue;

			for (const auto& vertex : *pVec)
			{
				dxf->writeVertex(
					*dw,
					DL_VertexData(
						vertex.x,
						vertex.y,
						vertex.z,
						0//顶点与顶点之间的弧度，0就是直线。大于0凸圆弧，小于0凹圆弧，值=tan(圆弧圆心角/4)
					)
				);
			}

			dxf->writePolylineEnd(*dw);
		}
		break;

		case DxfEntityType::DXF_ENTITY_INSERT:
		{//块插入
			DxfInsertEntity tempEntity = pDoc->g_writeEntityList[i].data.insert;
			dxf->writeInsert(
				*dw,
				DL_InsertData(
					tempEntity.blockName,
					tempEntity.position.x,
					tempEntity.position.y,
					tempEntity.position.z,
					tempEntity.scaleX,
					tempEntity.scaleY,
					tempEntity.scaleZ,
					tempEntity.rotation,
					1,//插入列矩阵个数
					1,//插入行矩阵个数
					0.0,//列间距
					0.0//行间距
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
		}
		break;

		case DxfEntityType::DXF_ENTITY_ELLIPSE:
		{//椭圆
			DxfEllipseEntity tempEntity = pDoc->g_writeEntityList[i].data.ellipse;
			dxf->writeEllipse(
				*dw,
				DL_EllipseData(
					tempEntity.EllipseCenter.x,
					tempEntity.EllipseCenter.y,
					tempEntity.EllipseCenter.z,
					tempEntity.LongAxisVectorQuantity.x,
					tempEntity.LongAxisVectorQuantity.y,
					tempEntity.LongAxisVectorQuantity.z,
					tempEntity.ShortRatioLong,
					tempEntity.StartAngle,
					tempEntity.EndAngle
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
		}
		break;

		case DxfEntityType::DXF_ENTITY_SPLINE:
		{//样条线
			DxfSplineEntity tempEntity = pDoc->g_writeEntityList[i].data.spline;
			//std::cout << "写入样条线标志：" << tempEntity.flags << std::endl;
			dxf->writeSpline(
				*dw,
				DL_SplineData(
					tempEntity.degree,
					tempEntity.knotCount,
					tempEntity.ccontrolCount,
					tempEntity.fitCount,
					tempEntity.flags
				),
				DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));
			if (tempEntity._knotsHandle != 0)
			{
				SplineKnotList* pSKL = dxflibCreationClass::GetSplineKnotList(tempEntity._knotsHandle);
				if (pSKL && !pSKL->empty())
				{
					for (const auto& SFPL : *pSKL)
					{
						dxf->writeKnot(
							*dw,
							DL_KnotData(
								SFPL.knot
							)
						);
					}
				}
			}
			if (tempEntity._controlPointsHandle != 0)
			{
				SplineControlPointList* pSCPL = dxflibCreationClass::GetSplineControlPointList(tempEntity._controlPointsHandle);
				if (pSCPL && !pSCPL->empty())
				{
					for (const auto& SCPL : *pSCPL)
					{
						//std::cout << "写入样条线控制点权重：" << SCPL.weight << std::endl;
						dxf->writeControlPoint(
							*dw,
							DL_ControlPointData(
								SCPL.controlPoint.x,
								SCPL.controlPoint.y,
								SCPL.controlPoint.z,
								SCPL.weight
							)
						);
					}
				}
			}
			if (tempEntity._fitPointHandle != 0)
			{
				SplineFitPointList* pSFPL = dxflibCreationClass::GetSplineFitPointList(tempEntity._fitPointHandle);
				if (pSFPL && !pSFPL->empty())
				{
					for (const auto& SFPL : *pSFPL)
					{
						dxf->writeFitPoint(
							*dw,
							DL_FitPointData(
								SFPL.fitPoint.x,
								SFPL.fitPoint.y,
								SFPL.fitPoint.z
							)
						);
					}
				}
			}
		}
		break;
		default:
			break;
		}
	}

	dw->sectionEnd();
#pragma endregion

#pragma region 写入对象段和结束
	dxf->writeObjects(*dw);
	dxf->writeObjectsEnd(*dw);

	dw->dxfEOF();
	dw->close();
#pragma endregion
	delete dw;
	delete dxf;

	DeleteAllWriteBufferEntity(hdxfDocument);
	DeleteAllWriteBufferBlock(hdxfDocument);
	return 0;
}
#pragma endregion