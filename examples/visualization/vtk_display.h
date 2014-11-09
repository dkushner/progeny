#ifndef __VTK_DISPLAY_
#define __VTK_DISPLAY_

#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>

#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkPlotPoints.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkCallbackCommand.h>

class VtkDisplay {

  public:
    explicit VtkDisplay();
    ~VtkDisplay();

  public:
    void AddData(float f);

  public:
    boost::mutex m_mutex;

  private:
    void threadMain();

  private:
    vtkSmartPointer<vtkContextView> m_contextView;
    vtkSmartPointer<vtkRenderWindowInteractor> m_renderWindowInteractor;
    vtkSmartPointer<vtkTable> m_table;
    vtkSmartPointer<vtkChartXY> m_chartXY;
    vtkSmartPointer<vtkFloatArray> m_timeArr;
    vtkSmartPointer<vtkFloatArray> m_genArr;

    boost::thread* m_thread;

    std::vector<float> m_queue;

    volatile bool m_running;
};

#endif
