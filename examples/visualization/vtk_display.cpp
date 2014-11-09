#include "vtk_display.h"

VtkDisplay::VtkDisplay() {
  m_running = true;
  m_thread = new boost::thread(&VtkDisplay::threadMain, this);
}

VtkDisplay::~VtkDisplay() {
  m_running = false;
  m_thread->interrupt();
  m_thread->join();
}


void VtkDisplay::threadMain() {
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

  vtkPlot* points = m_chartXY->AddPlot(vtkChart::POINTS);
  points->SetInputData(m_table, 0, 1);
  points->SetColor(0, 0, 0, 255);
  points->SetWidth(1.0);
  vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::CROSS);

  m_contextView->GetRenderWindow()->SetMultiSamples(16);
  m_renderWindowInteractor = m_contextView->GetInteractor();
  m_renderWindowInteractor->Initialize();

  while(m_running) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    m_mutex.lock();
    while(!m_queue.empty()) {
      float data = m_queue.back();
      m_queue.pop_back();
      m_table->SetValue(m_table->GetNumberOfRows(), 1, data);
      m_table->Modified();
    }
    m_mutex.unlock();

    //m_renderWindowInteractor->Render();
    m_contextView->Update();
    m_contextView->Render();
  }
}

void VtkDisplay::AddData(float f) {
  m_mutex.lock();
  m_queue.push_back(f);
  m_mutex.unlock();
}
