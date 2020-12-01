#include "createobjects.h"

#include"vtkCornerAnnotation.h"
#include"vtkTextActor.h"
#include"vtkTextProperty.h"
#include"vtkSmartPointer.h"
#include"vtkActor.h"
#include"vtkTextActor.h"
#include<vtkPoints.h>
#include<vtkHexahedron.h>
#include<vtkUnstructuredGrid.h>
#include<vtkDataSetSurfaceFilter.h>
#include<vtkDataSetMapper.h>
#include<vtkProperty.h>
#include<vtkSphereSource.h>
#include<vtkCubeSource.h>
#include<vtkCylinderSource.h>
#include<vtkTriangleFilter.h>
#include<vtkStripper.h>
#include<vtkTransformPolyDataFilter.h>
#include<vtkTransform.h>
#include<vtkActor2D.h>

#include<vtkAnnotatedCubeActor.h>
#include<vtkOrientationMarkerWidget.h>
#include <vtkLegendBoxActor.h>
#include<vtkTextProperty.h>
#include<vtkProperty2D.h>
#include<vtkImageData.h>
#include<vtkClipPolyData.h>
#include<vtkPlane.h>
#include<vtkCellArray.h>
#include<vtkPolyDataMapper.h>
#include<vtkAssembly.h>
#include<vtkPolyDataMapper2D.h>
#include<vtkFollower.h>
#include<vtkRenderer.h>
#include<vtkVectorText.h>
#include<vtkLinearExtrusionFilter.h>
#include<vtkTextActor3D.h>
#include<vtkAxes.h>
#include<vtkTubeFilter.h>
#include<vtkLineSource.h>

#include <vtkCaptionActor2D.h>
#include <vtkCoordinate.h>
#include <vtkFollower.h>

#include<vector>
#include<QString>
#include<QDebug>



CreateObjects::CreateObjects()
{
    this->annotation=vtkSmartPointer<vtkCornerAnnotation>::New();
}

void CreateObjects::createAnnotation(int location,double RGB[3],QString text1)
{
    this->annotation->SetLinearFontScaleFactor(1);
    this->annotation->SetNonlinearFontScaleFactor(2);
    this->annotation->SetMaximumFontSize(15 );
    this->annotation->SetText(location,text1.toLatin1().data());//toLatin1().data() is Qt equivalent of const char
    this->annotation->GetTextProperty()->SetColor(RGB);
    this->annotation->GetTextProperty()->SetFontFamilyToTimes();
}



vtkSmartPointer<vtkPolyData> CreateObjects::createSphere(double radius,int thetaR,int phiR,double center[3])
{
    vtkSmartPointer<vtkSphereSource> sph=
            vtkSmartPointer<vtkSphereSource>::New();
    sph->SetRadius(radius);
    sph->SetThetaResolution(thetaR);
    sph->SetPhiResolution(phiR);
    sph->SetCenter(center);
    sph->Update();

    vtkSmartPointer<vtkTriangleFilter>triangler=
            vtkSmartPointer<vtkTriangleFilter>::New();
    triangler->SetInputConnection(sph->GetOutputPort());

    vtkSmartPointer<vtkStripper>stripper=
            vtkSmartPointer<vtkStripper>::New();
    stripper->SetInputConnection(triangler->GetOutputPort());

    return stripper->GetOutput();
}

vtkSmartPointer<vtkPolyData> CreateObjects::createCube(double x,double y,double z,double center[])
{
    vtkSmartPointer<vtkCubeSource> cube=
            vtkSmartPointer<vtkCubeSource>::New();
    cube->SetXLength(x);
    cube->SetYLength(y);
    cube->SetZLength(z);
    cube->SetCenter(center);
    cube->Update();

    vtkSmartPointer<vtkTriangleFilter>triangler=
            vtkSmartPointer<vtkTriangleFilter>::New();
    triangler->SetInputConnection(cube->GetOutputPort());

    vtkSmartPointer<vtkStripper>stripper=
            vtkSmartPointer<vtkStripper>::New();
    stripper->SetInputConnection(triangler->GetOutputPort());
    return stripper->GetOutput();

}

vtkSmartPointer<vtkPolyData> CreateObjects::createCylinder(double radius,double height,double center[])
{
    vtkSmartPointer<vtkCylinderSource> cylinder =
            vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetCenter(center);
    cylinder->SetRadius(radius);
    cylinder->SetHeight(height);
    cylinder->SetResolution(100);
    cylinder->Update();

    vtkSmartPointer<vtkTransform> tr =
            vtkSmartPointer<vtkTransform>::New();
    tr->RotateX(90*1);

    vtkSmartPointer<vtkTransformPolyDataFilter> cylinderTr =
            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    cylinderTr->SetInputData(cylinder->GetOutput());
    cylinderTr->SetTransform(tr);
    cylinderTr->Update();

    vtkSmartPointer<vtkTriangleFilter>triangler=
            vtkSmartPointer<vtkTriangleFilter>::New();
    triangler->SetInputConnection(cylinderTr->GetOutputPort());

    vtkSmartPointer<vtkStripper>stripper=
            vtkSmartPointer<vtkStripper>::New();
    stripper->SetInputConnection(triangler->GetOutputPort());
    return stripper->GetOutput();

}



vtkSmartPointer<vtkLegendBoxActor> CreateObjects::createIsodoseLegend(std::vector<double>isodoseValues,std::vector<double>colorsR,std::vector<double>colorsG,
                                                      std::vector<double>colorsB)
{
    vtkSmartPointer<vtkLegendBoxActor>legend =
            vtkSmartPointer<vtkLegendBoxActor>::New();
    legend->SetNumberOfEntries(isodoseValues.size()+1);
    legend->BorderOff();
    vtkSmartPointer<vtkTextProperty>textProp=
            vtkSmartPointer<vtkTextProperty>::New();
    textProp->SetFontFamilyToTimes();
    legend->SetEntryTextProperty(textProp);
    legend->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
    legend->GetPositionCoordinate()->SetValue(0.90,0.1);
    legend->GetPosition2Coordinate()->SetCoordinateSystemToNormalizedViewport();
    legend->GetPosition2Coordinate()->SetValue(0.08, 0.8);
    legend->GetProperty()->SetOpacity(0.7);


    for(int i=0;i<isodoseValues.size();i++)
    {
        double color[3]={colorsR[i],colorsG[i],colorsB[i]};
        QString value;
        value=QString::number(isodoseValues[i]).append(" Gy");
        legend->SetEntry(i, static_cast<vtkPolyData *> (NULL), value.toLatin1(), color);

    }

    return legend;
}


vtkSmartPointer<vtkActor> CreateObjects::createRectangle(double x1,double x2,double y1,double y2,double gantryAngle,double collAngle,double isocenter[3])
{
    vtkSmartPointer<vtkPoints>points =
            vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> cells =
            vtkSmartPointer<vtkCellArray>::New();
    points->SetNumberOfPoints(4);
    cells->InsertNextCell(4);

    for(int p=0;p<4;p++)
    {
        cells->InsertCellPoint( p );
    }
    points->SetPoint(0,x1,0,y1);
    points->SetPoint(1,x1,0,y2);
    points->SetPoint(2,x2,0,y2);
    points->SetPoint(3,x2,0,y1);

    vtkSmartPointer<vtkPolyData>rectangle=
            vtkSmartPointer<vtkPolyData>::New();
    rectangle->Initialize();
    rectangle->SetPolys(cells);
    rectangle->SetPoints( points);

    vtkSmartPointer<vtkPolyDataMapper>rectangleMapper=
            vtkSmartPointer<vtkPolyDataMapper>::New();
    rectangleMapper->SetInputData(rectangle);

    vtkSmartPointer<vtkActor>rectangleActor=
            vtkSmartPointer<vtkActor>::New();
    rectangleActor->SetMapper(rectangleMapper);

    //rectangleActor->SetPosition(isocenter[0],isocenter[1],isocenter[2]);
    rectangleActor->SetOrigin(0,0,0);    
    rectangleActor->GetProperty()->SetRepresentationToWireframe();
    rectangleActor->GetProperty()->EdgeVisibilityOn();
    rectangleActor->GetProperty()->SetLineWidth(3.0);
    rectangleActor->RotateZ(gantryAngle);
    rectangleActor->RotateY(collAngle);
    rectangleActor->GetProperty()->SetColor(1,0,0);
    rectangleActor->PickableOff();
    return rectangleActor;
}

vtkSmartPointer<vtkActor> CreateObjects::createLeaf(double length,double thickness,double position[3],double isocenter[3])
{
    vtkSmartPointer<vtkCubeSource> leaf =
            vtkSmartPointer<vtkCubeSource>::New();
    leaf->SetXLength(length);
    leaf->SetYLength(0.0);
    leaf->SetZLength(thickness);

    vtkSmartPointer<vtkTransformPolyDataFilter>transformer=
            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformer->SetInputConnection(leaf->GetOutputPort());
    vtkSmartPointer<vtkTransform>transform=
            vtkSmartPointer<vtkTransform>::New();
    transform->Translate(isocenter);
    transform->Update();
    transformer->SetTransform(transform);
    transformer->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(transformer->GetOutputPort());

    vtkSmartPointer<vtkActor>leafActor=
            vtkSmartPointer<vtkActor>::New();
    leafActor->SetMapper(mapper);
    leafActor->GetProperty()->SetColor(0,0,0);
//    leafActor->GetProperty()->SetColor(1,0.5,0.75);
    leafActor->GetProperty()->SetOpacity(0.95);
    leafActor->GetProperty()->SetAmbient(1.0);
    leafActor->GetProperty()->EdgeVisibilityOn();
    leafActor->GetProperty()->SetEdgeColor(0,0,1);
    leafActor->GetProperty()->SetLineWidth(0.5);
    leafActor->GetProperty()->SetRepresentationToWireframe();
    leafActor->SetPosition(position);
    return leafActor;

}

vtkSmartPointer<vtkAssembly> CreateObjects::createVarian120MLC(double isocenter[3],double gantryAngle,double collAngle,vtkTransform *userTr)
{
    vtkSmartPointer<vtkAssembly>mlc=
            vtkSmartPointer<vtkAssembly>::New();
    int i;
    //First 10 thick leaves(1cm)
    double pos1=-205;
    double thickness;
    for(i=0;i<10;i++)
    {
        thickness=10.0;
        pos1+=thickness;
        double leafPos[3]={100.0,0.0,pos1};
        vtkSmartPointer<vtkActor>leaf=
                createLeaf(200.0,thickness,leafPos,isocenter);
        mlc->AddPart(leaf);
    }

    //Middle 40 fine leaves (0.5 cm)
    double pos2=-102.5;
    for(i=10;i<50;i++)
    {
        thickness=5.0;
        pos2+=thickness;
        double leafPos[3]={100.0,0.0,pos2};
        vtkSmartPointer<vtkActor>leaf=
                createLeaf(200.0,thickness,leafPos,isocenter);
        mlc->AddPart(leaf);
    }

    //last 10 thick leaves(1 cm)
    double pos3=95.0;
    for(i=50;i<60;i++)
    {
        thickness=10.0;
        pos3+=thickness;
        double leafPos[3]={100.0,0.0,pos3};
        vtkSmartPointer<vtkActor>leaf=
                createLeaf(200.0,thickness,leafPos,isocenter);
        mlc->AddPart(leaf);
    }

    //Create other bank
    //First 10 thick leaves(1cm)
    double pos4=-205;
//    double thickness;
    for(i=0;i<10;i++)
    {
        thickness=10.0;
        pos4+=thickness;
        double leafPos[3]={-100.0,0.0,pos4};
        vtkSmartPointer<vtkActor>leaf=
                createLeaf(200.0,thickness,leafPos,isocenter);
        //leaf->GetProperty()->SetColor(1,1,0);
        mlc->AddPart(leaf);
    }

    //Middle 40 fine leaves (0.5 cm)
    double pos5=-102.5;
    for(i=10;i<50;i++)
    {
        thickness=5.0;
        pos5+=thickness;
        double leafPos[3]={-100.0,0.0,pos5};
        vtkSmartPointer<vtkActor>leaf=
                createLeaf(200.0,thickness,leafPos,isocenter);
        //leaf->GetProperty()->SetColor(1,1,0);
        mlc->AddPart(leaf);
    }

    //last 10 thick leaves(1 cm)
    double pos6=95.0;
    for(i=50;i<60;i++)
    {
        thickness=10.0;
        pos6+=thickness;
        double leafPos[3]={-100.0,0.0,pos6};
        vtkSmartPointer<vtkActor>leaf=
                createLeaf(200.0,thickness,leafPos,isocenter);
        //leaf->GetProperty()->SetColor(1,1,0);
        mlc->AddPart(leaf);
    }
    mlc->SetOrigin(isocenter[0],isocenter[1],isocenter[2]);




    mlc->SetUserTransform(userTr);
    //First apply coll rotation then gantry, to do...
    mlc->RotateZ(gantryAngle);
    mlc->RotateY(collAngle);



    return mlc;
}

vtkSmartPointer<vtkActor> CreateObjects::createVectorText(QString txt,double collAngle,double gantryAngle)
{
    // Create a vector text
    vtkSmartPointer<vtkVectorText>vecText =
            vtkSmartPointer<vtkVectorText>::New();
    vecText->SetText(txt.toLatin1());

    vtkSmartPointer<vtkPolyDataMapper>txtMapper=
            vtkSmartPointer<vtkPolyDataMapper>::New();
    txtMapper->SetInputConnection( vecText->GetOutputPort());
    txtMapper->ScalarVisibilityOff();
    vtkSmartPointer<vtkActor>txtActor=
            vtkSmartPointer<vtkActor>::New();
    txtActor->RotateX(90);
//    txtActor->RotateZ(gantryAngle);
    txtActor->RotateY(collAngle);
    txtActor->GetProperty()->SetColor(1,1,0);
    txtActor->SetMapper(txtMapper);
    txtActor->SetScale(8.0);
    return txtActor;

}

vtkSmartPointer<vtkActor> CreateObjects::createAxes(double shift[3],double isocenter[3])
{
    vtkSmartPointer<vtkAxes>customAxes=
            vtkSmartPointer<vtkAxes>::New();
    customAxes->SetOrigin(isocenter);
    customAxes->SetScaleFactor(8);
    customAxes->SetSymmetric(1);

    vtkSmartPointer<vtkTubeFilter>axesTubes =
            vtkSmartPointer<vtkTubeFilter>::New();
    axesTubes->SetInputConnection(customAxes->GetOutputPort());
    axesTubes->SetRadius(customAxes->GetScaleFactor()/8.0);
    axesTubes->SetNumberOfSides(31);
    axesTubes->SetCapping(1);

    vtkSmartPointer<vtkPolyDataMapper>axesMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    axesMapper->SetInputConnection(axesTubes->GetOutputPort());

    vtkSmartPointer<vtkActor>isocenterPointActor=
            vtkSmartPointer<vtkActor>::New();
    isocenterPointActor->GetProperty()->SetLineWidth(8.0);
    isocenterPointActor->SetMapper(axesMapper);
    isocenterPointActor->AddPosition(shift);
    return isocenterPointActor;

}

vtkSmartPointer<vtkCaptionActor2D> CreateObjects::createCaption2D()
{
    // Create an actor for the text
    vtkSmartPointer<vtkCaptionActor2D> captionActor =
            vtkSmartPointer<vtkCaptionActor2D>::New();
    captionActor->SetCaption("Beam Name");
    captionActor->SetAttachmentPoint(0.0,0.0,100.0);
    captionActor->BorderOff();
    captionActor->GetCaptionTextProperty()->BoldOff();
    captionActor->GetCaptionTextProperty()->ItalicOff();
    captionActor->GetCaptionTextProperty()->ShadowOff();
    captionActor->GetCaptionTextProperty()->SetFontFamilyToTimes();
    captionActor->GetTextActor()->SetTextScaleModeToViewport();
    captionActor->GetCaptionTextProperty()->SetFontSize(10.0);
    captionActor->GetProperty()->SetColor(1,1,0);
    captionActor->ThreeDimensionalLeaderOff();
    return captionActor;

}


vtkSmartPointer<vtkAssembly>CreateObjects::createGraticule()
{
    vtkSmartPointer<vtkLineSource> lineX =
            vtkSmartPointer<vtkLineSource>::New();
    lineX->SetPoint1(200,0,0);
    lineX->SetPoint2(-200,0,0);
    lineX->Update();

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapperX =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperX->SetInputConnection(lineX->GetOutputPort());

    vtkSmartPointer<vtkActor> lineActorX =
      vtkSmartPointer<vtkActor>::New();
    lineActorX->SetMapper(mapperX);
    lineActorX->GetProperty()->SetAmbient(1);
    lineActorX->GetProperty()->SetLineWidth(2.0);
    lineActorX->GetProperty()->SetColor(0,0,0);

    vtkSmartPointer<vtkLineSource> lineY=
            vtkSmartPointer<vtkLineSource>::New();
    lineY->SetPoint1(0,0,200);
    lineY->SetPoint2(0,0,-200);
    lineY->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapperY =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapperY->SetInputConnection(lineY->GetOutputPort());

    vtkSmartPointer<vtkActor> lineActorY =
      vtkSmartPointer<vtkActor>::New();
    lineActorY->SetMapper(mapperY);
    lineActorY->GetProperty()->SetAmbient(1);
    lineActorY->GetProperty()->SetLineWidth(2.0);
    //lineActorY->GetProperty()->SetColor(1,1,0);
     lineActorY->GetProperty()->SetColor(0,0,0);

    vtkSmartPointer<vtkAssembly>graticule=
            vtkSmartPointer<vtkAssembly>::New();
    graticule->AddPart(lineActorX);
    graticule->AddPart(lineActorY);

    for(int x=-200;x<=200;x+=10)
    {

        vtkSmartPointer<vtkLineSource> lineXtick=
                vtkSmartPointer<vtkLineSource>::New();
        lineXtick->SetPoint1(5,0,x);
        lineXtick->SetPoint2(-5,0,x);
        lineXtick->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapperXtick =
          vtkSmartPointer<vtkPolyDataMapper>::New();
        mapperXtick->SetInputConnection(lineXtick->GetOutputPort());

        vtkSmartPointer<vtkActor> lineActorXtick =
          vtkSmartPointer<vtkActor>::New();
        lineActorXtick->SetMapper(mapperXtick);
        //lineActorXtick->GetProperty()->SetAmbient(1);
        lineActorXtick->GetProperty()->SetLineWidth(2.0);
//        lineActorXtick->GetProperty()->SetColor(1,1,0);
        lineActorXtick->GetProperty()->SetColor(0,0,0);
        graticule->AddPart(lineActorXtick);

        vtkSmartPointer<vtkLineSource> lineYtick=
                vtkSmartPointer<vtkLineSource>::New();
        lineYtick->SetPoint1(x,0,5);
        lineYtick->SetPoint2(x,0,-5);
        lineYtick->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapperYtick =
          vtkSmartPointer<vtkPolyDataMapper>::New();
        mapperYtick->SetInputConnection(lineYtick->GetOutputPort());

        vtkSmartPointer<vtkActor> lineActorYtick =
          vtkSmartPointer<vtkActor>::New();
        lineActorYtick->SetMapper(mapperYtick);
        //lineActorYtick->GetProperty()->SetAmbient(1);
        lineActorYtick->GetProperty()->SetLineWidth(2.0);
        lineActorYtick->GetProperty()->SetColor(0,0,0);
        graticule->AddPart(lineActorYtick);

    }
    return graticule;

}












