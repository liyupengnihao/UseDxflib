// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#ifdef _DEBUG
#include <random>
#include <chrono>
#include "dxflibAPIExport.h"//含对外结构体
int main() {
	//printf("正在本地测试 DLL 中的函数...\n");
	//std::cout << "正在本地测试 DLL 中的函数...\n" << std::endl;

	DxfDocument_Handle hdxfDocument = CreateDxfDocument();

	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\demo.dxf");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/myfile.dxf");
	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\newfile.dxf");
	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\newfile.txt");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newfileTwo.dxf");
	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newfileFour.dxf");

	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/demo.dxf");
	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newFive.dxf");
	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newsix.dxf");
	//ReadDXF(hdxfDocument, "C:/Users/Public/Nwt/cache/recv/程佳佳/8985FDB-FD-007-00.DXF");
	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newseven.dxf");
	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/neweight.dxf");
	ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newnine.dxf");


#pragma region 重写dxf
	DeleteAllReadEntity(hdxfDocument);

	//
	DxfEntityWrapper poly;
	///点
	poly.type = DxfEntityType::DXF_ENTITY_POINT;
	poly.data.point.pointCoord = { 25,95,0 };

	WriteSingleEntity(hdxfDocument, &poly);

	///线
	poly.type = DxfEntityType::DXF_ENTITY_LINE;
	poly.data.line.start = { 30,10,0 };
	poly.data.line.end = { 70,10,0 };

	WriteSingleEntity(hdxfDocument, &poly);

	//圆
	poly.type = DxfEntityType::DXF_ENTITY_CIRCLE;
	poly.data.circle.center = { 50,50,0 };
	poly.data.circle.radius = 20;

	WriteSingleEntity(hdxfDocument, &poly);

	//圆弧
	poly.type = DxfEntityType::DXF_ENTITY_ARC;
	poly.data.arc.center = { 100,100,0 };
	poly.data.arc.radius = 50;
	poly.data.arc.startAngle = 0;
	poly.data.arc.endAngle = 90;

	WriteSingleEntity(hdxfDocument, &poly);

	//文字
	std::string tempString = "TestSingleText";
	poly.type = DxfEntityType::DXF_ENTITY_TEXT;
	poly.data.text.insertPoint = { 50,50,0 };
	strncpy_s(
		poly.data.text.content,
		sizeof(poly.data.text.content),
		tempString.c_str(),
		_TRUNCATE
	);
	poly.data.text.height = 5;
	poly.data.text.rotation = 0;

	WriteSingleEntity(hdxfDocument, &poly);

	//多段线
	poly.type = DxfEntityType::DXF_ENTITY_POLYLINE;
	poly.data.polyline.vertexCount = 3;
	poly.data.polyline.pFlags = 1;

	int inIndex = WriteSingleEntity(hdxfDocument, &poly);

	if (inIndex >= 0) {

		DxfPoint pt;
		pt.x = 100; pt.y = 0; pt.z = 0;

		// 把文档句柄和索引传回去
		WriteSinglePolylinePeakEntity(hdxfDocument, inIndex, &pt);

		pt.x = 100; pt.y = 50; pt.z = 0;
		WriteSinglePolylinePeakEntity(hdxfDocument, inIndex, &pt);

		pt.x = 50; pt.y = 100; pt.z = 0;
		WriteSinglePolylinePeakEntity(hdxfDocument, inIndex, &pt);

		pt.x = 0; pt.y = 30; pt.z = 0;
		WriteSinglePolylinePeakEntity(hdxfDocument, inIndex, &pt);
	}

	//椭圆
	poly.type = DxfEntityType::DXF_ENTITY_ELLIPSE;
	poly.data.ellipse.EllipseCenter = { 100,100,0 };
	poly.data.ellipse.LongAxisVectorQuantity = { -10,25,0 };
	poly.data.ellipse.ShortRatioLong = 0.8;
	poly.data.ellipse.StartAngle = 0;
	poly.data.ellipse.EndAngle = std::acos(-1.0) * 1;
	WriteSingleEntity(hdxfDocument, &poly);


#pragma region 实体块引用
	tempString = "TestblockOne";
	poly.type = DxfEntityType::DXF_ENTITY_INSERT;
	strncpy_s(
		poly.data.insert.blockName,
		sizeof(poly.data.insert.blockName),
		tempString.c_str(),
		_TRUNCATE
	);
	poly.data.insert.position = { 0,120,0 };
	poly.data.insert.scaleX = 1.0;
	poly.data.insert.scaleY = 1.0;
	poly.data.insert.scaleZ = 1.0;
	poly.data.insert.rotation = 0;
	WriteSingleEntity(hdxfDocument, &poly);

	tempString = "TestblockOne";
	poly.type = DxfEntityType::DXF_ENTITY_INSERT;
	strncpy_s(
		poly.data.insert.blockName,
		sizeof(poly.data.insert.blockName),
		tempString.c_str(),
		_TRUNCATE
	);
	poly.data.insert.position = { 0,0,0 };
	poly.data.insert.scaleX = 1.0;
	poly.data.insert.scaleY = 1.0;
	poly.data.insert.scaleZ = 1.0;
	poly.data.insert.rotation = 0;
	WriteSingleEntity(hdxfDocument, &poly);

	tempString = "TestblockTwo";

	poly.type = DxfEntityType::DXF_ENTITY_INSERT;
	strncpy_s(
		poly.data.insert.blockName,
		sizeof(poly.data.insert.blockName),
		tempString.c_str(),
		_TRUNCATE
	);
	poly.data.insert.position = { -20,-20,0 };
	poly.data.insert.scaleX = 1.0;
	poly.data.insert.scaleY = 1.0;
	poly.data.insert.scaleZ = 1.0;
	poly.data.insert.rotation = 0;
	WriteSingleEntity(hdxfDocument, &poly);
#pragma endregion

#pragma region 样条线写入
#pragma region 控制点（权重1）与节点（随机从小到大0到1）
	//poly.type = DxfEntityType::DXF_ENTITY_SPLINE;
	//poly.data.spline.degree = 2;//次数
	//poly.data.spline.ccontrolCount = 4;
	//poly.data.spline.fitCount = 0;
	//poly.data.spline.knotCount = 7;//=degree+ccontrolCount+1
	//poly.data.spline.flags = 4;

	//inIndex = WriteSingleEntity(hdxfDocument, &poly);

	//if (inIndex >= 0)
	//{
	//	SplineControlPoint SCP = { {150,150,0}, 1 };//也是栈上

	//	WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);
	//	SCP = { {170, 200,0},1 };
	//	WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);
	//	SCP = { {180,140,0},1 };
	//	WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);
	//	SCP = { {200,170,0},1 };
	//	WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);

	//	// 随机数引擎（mt19937）
	//	std::random_device rd;  // 真随机数设备
	//	std::mt19937 rng(rd());  // 构造时直接播种

	//	std::uniform_real_distribution<double> realDist(0.0, 1.0);
	//	double Knot[7];
	//	//std::array<double, 7> Knot;
	//	for (double& knot : Knot)
	//	{
	//		knot = realDist(rng);
	//	}
	//	//冒泡小到大
	//	for (int i = 0; i < sizeof(Knot) / sizeof(Knot[0]) - 1; i++) {
	//		for (int j = 0; j < sizeof(Knot) / sizeof(Knot[0]) - 1 - i; j++) {
	//			if (Knot[j] > Knot[j + 1]) {
	//				// 交换//更大的向后排//一轮结束最大的排最后
	//				double temp = Knot[j];
	//				Knot[j] = Knot[j + 1];
	//				Knot[j + 1] = temp;
	//			}
	//		}
	//	}
	//	Knot[0] = 0;//给0看会不会郭第一个控制点
	//	for (int i = 0;i < sizeof(Knot) / sizeof(Knot[0]);i++)
	//	{
	//		SplineKnot temp = { Knot[i] };
	//		WriteSingleSplineKnotEntity(hdxfDocument, inIndex, &temp);
	//	}
	//}

#pragma endregion

#pragma region 控制点（权重改变）与节点（固定）
	poly.type = DxfEntityType::DXF_ENTITY_SPLINE;
	poly.data.spline.degree = 2;//次数
	poly.data.spline.ccontrolCount = 4;
	poly.data.spline.fitCount = 0;
	poly.data.spline.knotCount = 7;//=degree+ccontrolCount+1
	poly.data.spline.flags = 4;

	inIndex = WriteSingleEntity(hdxfDocument, &poly);

	if (inIndex >= 0)
	{
		SplineControlPoint SCP = { {-50,50,0}, 1 };//也是栈上

		WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);
		SCP = { {-30, 100,0},3 };
		WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);
		SCP = { {-20,40,0},5 };
		WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);
		SCP = { {0,70,0},9 };
		WriteSingleSplineControlPointEntity(hdxfDocument, inIndex, &SCP);

		//double Knot[7] = { 0,1,2.5,3,4,4,6 };
		for (int i = 0;i < 7;i++)
		{
			//SplineKnot temp = { Knot[i] };
			SplineKnot temp = { i };
			WriteSingleSplineKnotEntity(hdxfDocument, inIndex, &temp);
		}
	}
#pragma endregion

#pragma region 拟合点(qcad中无法显示拟合点对应的线条(数据有但无法渲染出来))
	poly.type = DxfEntityType::DXF_ENTITY_SPLINE;
	poly.data.spline.degree = 2;//次数
	poly.data.spline.ccontrolCount = 0;
	poly.data.spline.fitCount = 4;
	poly.data.spline.knotCount = 0;//=degree+ccontrolCount+1
	poly.data.spline.flags = 12;

	inIndex = WriteSingleEntity(hdxfDocument, &poly);

	if (inIndex >= 0)
	{
		SplineFitPoint SFP = { 50,-50,0 };

		WriteSingleSplineFitPointEntity(hdxfDocument, inIndex, &SFP);
		SFP = { 70, 0,0};
		WriteSingleSplineFitPointEntity(hdxfDocument, inIndex, &SFP);
		SFP = { 80,-60,0};
		WriteSingleSplineFitPointEntity(hdxfDocument, inIndex, &SFP);
		SFP = { 100,-30,0};
		WriteSingleSplineFitPointEntity(hdxfDocument, inIndex, &SFP);
	}
#pragma endregion

#pragma endregion

#pragma region  写入块
	poly.type = DxfEntityType::DXF_ENTITY_LINE;
	poly.data.line.start = { 0,5,0 };
	poly.data.line.end = { 15,5,0 };

	DxfPoint alignPoint = { 0,0,0 };//与引用的插入点对齐的点

	WriteSingleBlock(
		hdxfDocument,
		"TestblockOne",
		&alignPoint,
		&poly
	);
	poly.type = DxfEntityType::DXF_ENTITY_CIRCLE;
	poly.data.circle.center = { 5,5,0 };
	poly.data.circle.radius = 5;
	WriteSingleBlock(
		hdxfDocument,
		"TestblockOne",
		&alignPoint,
		&poly
	);
	//块内椭圆
	poly.type = DxfEntityType::DXF_ENTITY_ELLIPSE;
	poly.data.ellipse.EllipseCenter = { 10,5,0 };
	poly.data.ellipse.LongAxisVectorQuantity = { 10,0,0 };
	poly.data.ellipse.ShortRatioLong = 0.5;
	poly.data.ellipse.StartAngle = 0;
	poly.data.ellipse.EndAngle = std::acos(-1.0);
	WriteSingleBlock(
		hdxfDocument,
		"TestblockOne",
		&alignPoint,
		&poly
	);

#pragma region  新块，内为多段线+样条线
	poly.type = DxfEntityType::DXF_ENTITY_POLYLINE;
	poly.data.polyline.vertexCount = 5;
	poly.data.polyline.pFlags = 1;

	inIndex = WriteSingleBlock(
		hdxfDocument,
		"TestblockTwo",
		&alignPoint,
		&poly
	);

	DxfPoint pt = { -2,-2,0 };
	WriteSinglePolylinePeakBlock(
		hdxfDocument,
		"TestblockTwo",
		inIndex,
		&pt
	);

	pt = { -20,-2,0 };
	WriteSinglePolylinePeakBlock(
		hdxfDocument,
		"TestblockTwo",
		inIndex,
		&pt
	);

	pt = { -20,-20,0 };
	WriteSinglePolylinePeakBlock(
		hdxfDocument,
		"TestblockTwo",
		inIndex,
		&pt
	);



	poly.type = DxfEntityType::DXF_ENTITY_SPLINE;
	poly.data.spline.degree = 2;//次数
	poly.data.spline.ccontrolCount = 4;
	poly.data.spline.fitCount = 0;
	poly.data.spline.knotCount = 7;//=degree+ccontrolCount+1
	poly.data.spline.flags = 4;

	inIndex = WriteSingleBlock(
		hdxfDocument,
		"TestblockTwo",
		&alignPoint,
		&poly
	);
	if (inIndex >= 0)
	{
		SplineControlPoint SCP = { {-50,-50,0}, 1 };//也是栈上

		WriteSingleSplineControlPointBlock(hdxfDocument, "TestblockTwo", inIndex, &SCP);
		SCP = { {-30, 0,0},1 };
		WriteSingleSplineControlPointBlock(hdxfDocument, "TestblockTwo", inIndex, &SCP);
		SCP = { {-20,-60,0},1 };
		WriteSingleSplineControlPointBlock(hdxfDocument, "TestblockTwo", inIndex, &SCP);
		SCP = { {0,-30,0},1 };
		WriteSingleSplineControlPointBlock(hdxfDocument, "TestblockTwo", inIndex, &SCP);

		for (int i = 0;i < 7;i++)
		{
			SplineKnot temp = { i };
			WriteSingleSplineKnotBlock(hdxfDocument, "TestblockTwo", inIndex, &temp);
		}
	}
#pragma endregion

#pragma endregion


	//WriteDXF(hdxfDocument, "newFive.dxf");
	WriteDXF(hdxfDocument, "newten.dxf");

#pragma endregion

	return 0;
}
#else
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif
