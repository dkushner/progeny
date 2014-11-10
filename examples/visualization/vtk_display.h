#ifndef __VTK_DISPLAY_
#define __VTK_DISPLAY_

#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <core/observer.h>

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

namespace pr {

  template <typename CType>
  class VtkDisplay : public Observer<CType> {

    using ProgressData = typename pr::Observer<CType>::ProgressData;

    public:
      explicit VtkDisplay() {
        m_running = true;
        m_thread = new boost::thread(&VtkDisplay::threadMain, this);
      }

      ~VtkDisplay() {
        m_running = false;
        m_thread->interrupt();
        m_thread->join();
      }

    public:
      boost::mutex m_mutex;

    private:
      void threadMain() {
        m_contextView = vtkSmartPointer<vtkContextView>::New();

        m_contextView->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
        m_contextView->GetRenderWindow()->SetSize(800, 600);

        m_chartXY = vtkSmartPointer<vtkChartXY>::New();
        m_contextView->GetScene()->AddItem(m_chartXY);
        m_chartXY->SetShowLegend(true);

        m_table = vtkSmartPointer<vtkTable>::New();

        m_timeArr = vtkSmartPointer<vtkFloatArray>::New();
        m_timeArr->SetName("Time");

        m_table->AddColumn(m_timeArr);

        m_genArr = vtkSmartPointer<vtkFloatArray>::New();
        m_genArr->SetName("Generations");

        m_table->AddColumn(m_genArr);



        vtkPlot* points = m_chartXY->AddPlot(vtkChart::LINE);
        points->SetInputData(m_table, 0, 1);
        points->SetColor(255, 0, 0, 255);
        points->SetWidth(1.0);

        m_contextView->GetRenderWindow()->SetMultiSamples(16);
        m_renderWindowInteractor = m_contextView->GetInteractor();
        m_renderWindowInteractor->Initialize();

        m_renderWindowInteractor->CreateRepeatingTimer(1000);
        m_renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, this, &VtkDisplay::update);

        m_renderWindowInteractor->Start();
      }

      void update() {
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));

        m_mutex.lock();
          m_timeArr->InsertNextValue(m_data.elapsedTime);
          m_genArr->InsertNextValue(m_data.meanFitness);
          m_table->Modified();
        m_mutex.unlock();

        m_contextView->Update();
        m_contextView->Render();
      }

    private:
      vtkSmartPointer<vtkContextView> m_contextView;
      vtkSmartPointer<vtkRenderWindowInteractor> m_renderWindowInteractor;
      vtkSmartPointer<vtkTable> m_table;
      vtkSmartPointer<vtkChartXY> m_chartXY;
      vtkSmartPointer<vtkFloatArray> m_timeArr;
      vtkSmartPointer<vtkFloatArray> m_genArr;
      boost::thread* m_thread;
      std::vector<ProgressData> m_queue;
      volatile bool m_running;
      ProgressData m_data;

    protected:
      void onProgress(const ProgressData& data) {
        m_mutex.lock();
        m_data = data;
        m_mutex.unlock();
      }
  };
}
#endif
