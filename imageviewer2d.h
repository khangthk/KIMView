#ifndef IMAGEVIEWER2D_H
#define IMAGEVIEWER2D_H

#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)

#include<QWidget>
#include<QWheelEvent>
#include<QVTKWidget.h>
#include<QKeyEvent>
#include<QList>
#include<QString>
#include<QAction>
#include<QActionGroup>

#include<vtkSmartPointer.h>
#include<vtkImageData.h>
#include<vtkImageActor.h>
#include<vtkRenderer.h>
#include<vtkCornerAnnotation.h>
#include<vtkAnnotatedCubeActor.h>
#include<vtkOrientationMarkerWidget.h>
#include<vtkActor.h>
#include<vtkInteractorStyleUser.h>
#include<vtkInteractorStyleImage.h>
#include<vtkImagePlaneWidget.h>
#include<vtkDistanceWidget.h>
#include<vtkAngleWidget.h>
#include<vtkVolume.h>
#include<vtkTransform.h>
#include<vtkSphereSource.h>
#include<vtkPolyData.h>
#include<vtkBoxWidget2.h>
#include<vtkPolyData.h>
#include<vtkColorSeries.h>
#include<vtkCursor2D.h>
#include<vtkAxesActor.h>
#include<vtkScalarBarActor.h>
#include<vtkinteractorstyleimagecustom.h>
#include<vtklinecallbackdose.h>
#include<vector>
#include<vtkLineWidget2.h>
#include<vtkScalarBarActor.h>


namespace Ui {
class ImageViewer2D;
}

class ImageViewer2D : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer2D(QWidget *parent = nullptr,QActionGroup *contextMenus=0);
    ~ImageViewer2D();
    void SetImageData(vtkSmartPointer<vtkImageData>Img);
    void SetRTDose(vtkSmartPointer<vtkImageData>Dose);
    void SetInteractorStyleToRegistration();
    //0=Axial,1=Sagittal,2=Coronal
    void SetSliceOrientation(int Orientation);
    void SetUpView();
    void ShowImageAndDose(double SliceLoc);
    virtual void SliceImageAndDose(double SliceLoc);
    void ShowCursor(double *Bounds);
    void TriggerActionShowBeams();
    void TriggerActionShowDose();
    void TriggerActionShowContours();
    void TriggerReset_WL_WW();
    void DisplayROIs(double SliceLoc,int Orientation);
    void DisplayBeams(double SliceLoc,int Orientation);
    void DisplayPOIs(double SliceLoc,int Orientation);
    void DisplayScalarBar();
    void HideBeams();
    void HideContours();
    void HideDose();
    void UpdateView();
    void MoveToLocation(double loc);
    void AdjustDoseRange(double min, double max);

    double SliceStep;
    int SliceOrientation;//Axial by default
    double WindowUp=600,WindowLow=-400;
    double WindowWidth=0.0,Windowlevel=0.0;
    double Bounds[6];
    double DoseRange[2]={0.0,1.0};
    double SliceLoc;
    double (*ROIColors)[3];//50 ROIs supported as of now
    int NumOfROIs;
    bool BeamVisibility;
    bool ContourVisibility;
    bool DoseVisibility;
    bool POIVisibility;
    bool ScalarBarVisibility;

    vtkSmartPointer<vtkImageData>ImageData;
    vtkSmartPointer<vtkImageData>RTDose;
    vtkSmartPointer<vtkCornerAnnotation>ImgPositionAnnotation;
    vtkSmartPointer<vtkCornerAnnotation>WLWAnnotation;
    vtkSmartPointer<vtkAnnotatedCubeActor>AnnotatedCube;    
    vtkSmartPointer<vtkInteractorStyleUser>InteractorImage2D;
    vtkSmartPointer<vtkInteractorStyleImageCustom>InteractorImage;
    vtkSmartPointer<vtkDistanceWidget>DistanceWidget;
    vtkSmartPointer<vtkAngleWidget>AngleWidget;   
    vtkSmartPointer<vtkTransform>UserTransform;
    vtkSmartPointer<vtkActor>CursorActor;    
    vtkSmartPointer<vtkActor>ROIActor;    
    vtkSmartPointer<vtkRenderer>ViewRenderer;
    vtkSmartPointer<vtkImageActor>ImageSlice;
    vtkSmartPointer<vtkImageActor>DoseSlice;    
    vtkSmartPointer<vtkActor> CutROI(vtkPolyData* mesh, double sliceNo, double R, double G, double B, int Orientation);
    vtkSmartPointer<vtkActorCollection>ContourActors;
    vtkSmartPointer<vtkActorCollection>BeamActors;
    vtkSmartPointer<vtkActorCollection>POIActors;
    vtkSmartPointer<vtkColorSeries>BeamColors;
    vtkSmartPointer<vtkAxesActor>IsocentreAxes;
    vtkSmartPointer<vtkScalarBarActor>DoseScalarBar;
    vtkSmartPointer<vtkLookupTable>DoseLUT;
    vtkSmartPointer<vtkLineCallbackDose>lineCallbackDose;
    vtkSmartPointer<vtkLineWidget2>lineWidget;
    vtkSmartPointer<vtkScalarBarActor>ScalarBar;
    vtkSmartPointer<vtkImageReslice>ImageReslice;

    vtkSmartPointer<vtkAxesActor>Axes;
    vtkSmartPointer<vtkActor>HumanActor;
    vtkSmartPointer<vtkOrientationMarkerWidget>MarkerWidget;
    vtkSmartPointer<vtkOrientationMarkerWidget>AxesWidget;


    std::vector<vtkSmartPointer<vtkPolyData> >MeshList;
    std::vector<vtkSmartPointer<vtkPolyData> >BeamList;
    std::vector<vtkSmartPointer<vtkPolyData> >POIList;

    QList<QString>ROINames;
    QList<QString>ROITypes;
    QList<int>ROINo;



private slots:
    void on_toolButtonSagittal_clicked();

    void on_toolButtonAxial_clicked();

    void on_toolButtonCoronal_clicked();

    void on_toolButtonScreenshot_clicked();

    void on_verticalScrollBar_valueChanged(int value);

    virtual void on_toolButtonContrast_clicked();

    void on_actionMeasureDistance_triggered();

    void on_actionMeasureAngle_triggered();


    void on_actionReset_WL_WW_triggered();    

    void on_toolButtonProbe_clicked();

    void on_toolButtonScalarBar_clicked();

    void on_actionShowContours_triggered();

    void on_actionShowDose_triggered();

    void on_actionRubberBandZoom_triggered();

    void on_actionShowBeams_triggered();    

    void on_actionShow_Image_Extent_triggered();


private:
    Ui::ImageViewer2D *ui;


protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);


};

#endif // IMAGEVIEWER2D_H