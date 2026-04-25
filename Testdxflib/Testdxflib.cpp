// Testdxflib.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

///////////////////////////////////////////////////////dxf组码与组值类型
//0 - 9：字符串类型
//10 - 59：双精度浮点型
//60 - 79：16位整型
//90 - 99：32位整型
//100 - 102：字符串类型
//105：16进制句柄值



#include <iostream>
#include <dl_creationadapter.h>
#include <dl_dxf.h>

class Testdxflib:public DL_CreationAdapter
{
public:
    Testdxflib() = default;
    ~Testdxflib();
    void addLayer(const DL_LayerData& data)override final;//final禁止后续子类继续重写
    void addPoint(const DL_PointData& data)override final;
    void addLine(const DL_LineData& data)override final;
    void addArc(const DL_ArcData& data)override final;
    void addCircle(const DL_CircleData& data)override final;
    void addPolyline(const DL_PolylineData& data)override final;
    void addVertex(const DL_VertexData& data)override final;
    void add3dFace(const DL_3dFaceData& data)override final;

    //根据需要添加

    void printAttributes();
};

Testdxflib::~Testdxflib()
{

}
/**
 * Sample implementation of the method which handles layers.
 * 处理层的句柄
 */
void Testdxflib::addLayer(const DL_LayerData& data)
{
    printf("LAYER: %s flags: %d\n", data.name.c_str(), data.flags);
    printAttributes();
}

/**
 * Sample implementation of the method which handles point entities.
 * 处理点实体
 */
void Testdxflib::addPoint(const DL_PointData& data) {
    printf("POINT    (%6.3f, %6.3f, %6.3f)\n",
        data.x, data.y, data.z);
    printAttributes();
}

/**
 * Sample implementation of the method which handles line entities.
 * 处理线段
 */
void Testdxflib::addLine(const DL_LineData& data) {
    printf("LINE     (%6.3f, %6.3f, %6.3f) (%6.3f, %6.3f, %6.3f)\n",
        data.x1, data.y1, data.z1, data.x2, data.y2, data.z2);
    printAttributes();
}

/**
 * Sample implementation of the method which handles arc entities.
 * 弧形实体
 */
void Testdxflib::addArc(const DL_ArcData& data) {
    printf("ARC      (%6.3f, %6.3f, %6.3f) %6.3f, %6.3f, %6.3f\n",
        data.cx, data.cy, data.cz,
        data.radius, data.angle1, data.angle2);
    printAttributes();
}

/**
 * Sample implementation of the method which handles circle entities.
 * 圆形实体
 */
void Testdxflib::addCircle(const DL_CircleData& data) {
    printf("CIRCLE   (%6.3f, %6.3f, %6.3f) %6.3f\n",
        data.cx, data.cy, data.cz,
        data.radius);
    printAttributes();
}


/**
 * Sample implementation of the method which handles polyline entities.
 * 多线段实体
 */
void Testdxflib::addPolyline(const DL_PolylineData& data) {
    printf("POLYLINE \n");
    printf("flags: %d\n", (int)data.flags);
    printAttributes();
}


/**
 * Sample implementation of the method which handles vertices.
 * 处理顶点
 */
void Testdxflib::addVertex(const DL_VertexData& data) {
    printf("VERTEX   (%6.3f, %6.3f, %6.3f) %6.3f\n",
        data.x, data.y, data.z,
        data.bulge);
    printAttributes();
}

//3D数据
void Testdxflib::add3dFace(const DL_3dFaceData& data) {
    printf("3DFACE\n");
    for (int i = 0; i < 4; i++) {
        printf("   corner %d: %6.3f %6.3f %6.3f\n",
            i, data.x[i], data.y[i], data.z[i]);
    }
    printAttributes();
}

//打印属性
void Testdxflib::printAttributes() {
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

void usage();
void testReading(char* file);
/// <summary>
/// 创建或写入一个全新的dxf文件
/// </summary>
void testWriting();


void usage() {
    std::cout << "\nUsage: test <DXF file>\n\n";
}


void testReading(char* file) {
    // Load DXF file into memory:
    std::cout << "Reading file " << file << "...\n";
    Testdxflib* creationClass = new Testdxflib();
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(file, creationClass)) { // if file open failed
        std::cerr << file << " could not be opened.\n";
        return;
    }
    delete dxf;
    delete creationClass;
}



void testWriting() {

#pragma region 初始化与创建文件
    DL_Dxf* dxf = new DL_Dxf();
    DL_Codes::version exportVersion = DL_Codes::AC1015;
    DL_WriterA* dw = dxf->out("newfile.dxf", exportVersion);//清空文本内容
    if (dw == NULL) {
        printf("Cannot open file 'newfile.dxf' \
               for writing.");
        // abort function e.g. with return
    }
#pragma endregion


    dxf->writeHeader(*dw);
    /*
    // int variable:
    dw->dxfString(9, "$INSUNITS");
    dw->dxfInt(70, 4);
    // real (double, float) variable:
    dw->dxfString(9, "$DIMEXE");
    dw->dxfReal(40, 1.25);
    // string variable:
    dw->dxfString(9, "$TEXTSTYLE");
    dw->dxfString(7, "Standard");
    // vector variable:
    dw->dxfString(9, "$LIMMIN");
    dw->dxfReal(10, 0.0);
    dw->dxfReal(20, 0.0);
    */
    dw->sectionEnd();

#pragma region 写入表段
    dw->sectionTables();
    dxf->writeVPort(*dw);

    dw->tableLinetypes(3);
    dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
    dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
    dw->tableEnd();

    int numberOfLayers = 3;
    dw->tableLayers(numberOfLayers);

    dxf->writeLayer(*dw,
        DL_LayerData("0", 0),
        DL_Attributes(
            std::string(""),      // leave empty
            DL_Codes::black,        // default color
            100,                  // default width
            "CONTINUOUS", 1.0));       // default line style

    dxf->writeLayer(*dw,
        DL_LayerData("mainlayer", 0),
        DL_Attributes(
            std::string(""),
            DL_Codes::red,
            100,
            "CONTINUOUS", 1.0));

    dxf->writeLayer(*dw,
        DL_LayerData("anotherlayer", 0),
        DL_Attributes(
            std::string(""),
            DL_Codes::black,
            100,
            "CONTINUOUS", 1.0));

    dw->tableEnd();

    dw->tableStyle(1);
    dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "txt", ""));
    dw->tableEnd();

    dxf->writeView(*dw);
    dxf->writeUcs(*dw);

    dw->tableAppid(1);
    dxf->writeAppid(*dw, "ACAD");
    dw->tableEnd();

    dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);

    dxf->writeBlockRecord(*dw);
    dxf->writeBlockRecord(*dw, "myblock1");
    dxf->writeBlockRecord(*dw, "myblock2");
    dw->tableEnd();

    dw->sectionEnd();
#pragma endregion

#pragma region 写入块定义信息
    dw->sectionBlocks();
    dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Model_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space");
    dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
    dxf->writeEndBlock(*dw, "*Paper_Space0");

    dxf->writeBlock(*dw, DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock1");

    dxf->writeBlock(*dw, DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
    // ...
    // write block entities e.g. with dxf->writeLine(), ..
    // ...
    dxf->writeEndBlock(*dw, "myblock2");

    dw->sectionEnd();
#pragma endregion

#pragma region 写入实体段

    dw->sectionEntities();

    // write all entities in model space:
    dxf->writePoint(//写入点实体
        *dw,
        DL_PointData(10.0,
            45.0,
            0.0),
        DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

    dxf->writeLine(//写入线
        *dw,
        DL_LineData(25.0,   // start point
            30.0,
            0.0,
            100.0,   // end point
            120.0,
            0.0),
        DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

    //// 写入圆实体
    //dxf->writeCircle(
    //    *dw,
    //    DL_CircleData(    // 圆心坐标 (cx, cy, cz) 和 半径 (radius)
    //        50.0,         // cx - 圆心 X 坐标
    //        60.0,         // cy - 圆心 Y 坐标  
    //        0.0,          // cz - 圆心 Z 坐标
    //        30.0          // radius - 半径
    //    ),
    //    DL_Attributes("mainlayer", 256, -1, "BYLAYER", 1.0));

      // 写入圆实体
    dxf->writeCircle(
        *dw,
        DL_CircleData(    // 圆心坐标 (cx, cy, cz) 和 半径 (radius)
            50.0,         // cx - 圆心 X 坐标
            60.0,         // cy - 圆心 Y 坐标  
            0.0,          // cz - 圆心 Z 坐标
            10.0          // radius - 半径
        ),
        DL_Attributes("mainlayer", 125, -1, "BYLAYER", 1.0));

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
}


int main()
{
    std::cout << "Hello World!\n";

    usage();
    char FilePath[1024] = "E:/VSCODE/CADAnalysisDLL/DxfFile/examples/readwrite/demo.dxf";
    testReading(FilePath);
    testWriting();
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
