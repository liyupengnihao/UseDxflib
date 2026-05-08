// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#ifdef _DEBUG
#include "dxflibAPIExport.h"//含对外结构体
int main() {
	//printf("正在本地测试 DLL 中的函数...\n");
	//std::cout << "正在本地测试 DLL 中的函数...\n" << std::endl;

	DxfDocument_Handle hdxfDocument = CreateDxfDocument();

	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\demo.dxf");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/myfile.dxf");
	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\newfile.dxf");
	//ReadDXF("E:\\VSCODE\\CADAnalysisDLL\\CADAnalysisDLL\\newfile.txt");
	ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/demo.dxf");
	//ReadDXF("E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newfileTwo.dxf");
	//ReadDXF(hdxfDocument, "E:/VSCODE/CADAnalysisDLL/CADAnalysisDLL/newfileFour.dxf");
	DeleteAllEntity(hdxfDocument);

	DxfEntityWrapper poly;
	poly.type = DxfEntityType::DXF_ENTITY_POLYLINE;
	poly.data.polyline.vertexCount = 4;
	poly.data.polyline.pFlags = 0;

	int inIndex = WriteSingleEntityTwo(hdxfDocument, &poly);

    if (inIndex >= 0) {
        
        DxfPoint pt;
        pt.x = 0; pt.y = 0; pt.z = 0;

        // 把文档句柄和索引传回去
        WriteSinglePolylinePeakTwo(hdxfDocument, inIndex, &pt);

        pt.x = 10; pt.y = 0; pt.z = 0;
        WriteSinglePolylinePeakTwo(hdxfDocument, inIndex, &pt);

        pt.x = 20; pt.y = 30; pt.z = 0;
        WriteSinglePolylinePeakTwo(hdxfDocument, inIndex, &pt);
    }

    poly.type = DxfEntityType::DXF_ENTITY_POLYLINE;
    poly.data.polyline.vertexCount = 5;
    poly.data.polyline.pFlags = 0;

    inIndex = WriteSingleEntityTwo(hdxfDocument, &poly);

    if (inIndex >= 0) {

        DxfPoint pt;
        pt.x = 100; pt.y = 0; pt.z = 0;

        // 把文档句柄和索引传回去
        WriteSinglePolylinePeakTwo(hdxfDocument, inIndex, &pt);

        pt.x = 10; pt.y = 90; pt.z = 0;
        WriteSinglePolylinePeakTwo(hdxfDocument, inIndex, &pt);

        pt.x = 50; pt.y = 50; pt.z = 0;
        WriteSinglePolylinePeakTwo(hdxfDocument, inIndex, &pt);
    }
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
