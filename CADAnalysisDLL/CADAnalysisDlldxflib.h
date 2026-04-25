#pragma once
#include "pch.h"

class dxflibCreationClass:public DL_CreationAdapter//DL_CreationAdapter类中有空实现另一个是纯虚要全实现
{
public:
    dxflibCreationClass() = default;
    ~dxflibCreationClass();
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