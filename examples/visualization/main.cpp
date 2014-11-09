#include <iomanip>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>

#include <core/simulation.h>
#include <evaluators/mismatch_evaluator.h>
#include <selectors/roulette_selector.h>
#include <mutators/crossover.h>
#include <generators/fill_generator.h>
#include <mutators/pass_through.h>

namespace po = boost::program_options;

#include "vtk_display.h"

/*class VTKDisplay
{
  public:
    VTKDisplay() {
      m_view = vtkSmartPointer<vtkContextView>::New();
      m_view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
      m_view->GetRenderWindow()->SetSize(800, 600);

      //Setup the XY chart
      chart = vtkSmartPointer<vtkChartXY>::New();
      m_view->GetScene()->AddItem(chart);
      chart->SetShowLegend(true);

      m_table = vtkSmartPointer<vtkTable>::New();

      timeArray = vtkSmartPointer<vtkFloatArray>::New();
      timeArray->SetName("X Axis");
      m_table->AddColumn(timeArray);

      genArray = vtkSmartPointer<vtkFloatArray>::New();
      genArray->SetName("Generations");
      m_table->AddColumn(genArray);

      vtkPlot* points = chart->AddPlot(vtkChart::POINTS);
      points->SetInputData(m_table, 0, 1);
      points->SetColor(0, 0, 0, 255);
      points->SetWidth(1.0);
      vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::CROSS);

      m_view->GetRenderWindow()->SetMultiSamples(16);
      m_interactor = m_view->GetInteractor();
      m_thread = new boost::thread(&VTKDisplay::run, this);
    }

    ~VTKDisplay() {
        m_thread->join();
    }

    void addData(int idx, float data) {
        boost::unique_lock<boost::shared_mutex> lock(m_mutex);
        int numOfRows = m_table->GetNumberOfRows();
        m_table->SetValue(numOfRows, idx, data);
        m_table->Modified();
    }

  public:
    boost::shared_mutex m_mutex;

  private:
    vtkSmartPointer<vtkContextView> m_view;
    vtkSmartPointer<vtkTable> m_table;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
    vtkSmartPointer<vtkFloatArray> timeArray;
    vtkSmartPointer<vtkFloatArray> genArray;
    vtkSmartPointer<vtkChartXY> chart;

    boost::thread* m_thread;
    boost::condition_variable_any m_cv;
    int m_timer;
    volatile bool m_running = true;

  private:
    void run() {
      boost::unique_lock<boost::shared_mutex> lock(m_mutex);
      m_interactor->Initialize();
      m_interactor->AddObserver(vtkCommand::TimerEvent, this, &VTKDisplay::render);
      m_interactor->AddObserver(vtkCommand::ExitEvent, this, &VTKDisplay::destroy);
      m_timer = m_interactor->CreateRepeatingTimer(15);
      m_interactor->Start();
    }

    void render() {
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      boost::unique_lock<boost::shared_mutex> lock(m_mutex);
      m_view->Render();
    }

    void destroy() {
      m_running = false;
      m_interactor->DestroyTimer(m_timer);
      m_interactor->Delete();
    }
};*/


VtkDisplay* vtkVisualization;

int main(int argc, char** argv) {
  std::string target;
  unsigned int size;
  unsigned int seed;

  vtkVisualization = new VtkDisplay();


  po::options_description desc("Recognized options");
  desc.add_options()
    ("help", "Print this help message. Obviously.")
    ("target", po::value<std::string>(&target)->default_value("target"),
      "Target string to evolve towards.")
    ("size", po::value<unsigned int>(&size)->default_value(100),
      "Population size to use in the evolution.")
    ("seed", po::value<unsigned int>(&seed), "Optional seed for the RNG.");

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  // Set up RNG.
  std::random_device rd;
  std::mt19937 mt(vm.count("seed") ? seed : rd());
  std::uniform_int_distribution<int> dist(0, 25);
  const char valid[] = "abcdefghijklmnopqrstuvwxyz";

  // Aliases for cleanliness.
  using Candidate = pr::Candidate<std::string, double>;
  using Population = pr::Population<Candidate>;
  using Data = pr::Simulation<Candidate>::Data;

  // Construct Generator
  pr::FillGenerator<Candidate> fg([&]{
    std::string str(target.size(), 0);
    std::generate(str.begin(), str.end(), [&]{
      return valid[dist(mt)];
    });
    return str;
  });

  // Construct Evaluator
  pr::MismatchEvaluator<Candidate> mev(target);

  // Construct Selector
  pr::RouletteSelector<Candidate> rs;

  // Construct Mutator
  auto mut = pr::Crossover<Candidate>(2) >> pr::PassThrough<Candidate>();

  // Finally, compose the simulator instance.
  auto sim = pr::Simulation<Candidate>::build(fg, mev, rs, mut);

  // Create a breakpoint for our simulation run. This observes the population
  // after each iteration and decides if we have reached our termination
  // conditions. If so, the breakpoint should set the elite reference to the
  // candidate that has satisfied the conditions.
  // TODO: Multiple elites?
  auto breakpoint = [](const Population& pop, Candidate& elite) {
    auto match = std::find_if(pop.begin(), pop.end(), [](const Candidate& c){
      return (pr::fitness(c) == 0.0);
    });

    if (match != std::end(pop)) {
      elite = *match;
      return true;
    }

    return false;
  };

  // Register an observer function that watches the population.
  sim.addObserver([&](const Data& data) {
    std::cout << "running..." << std::endl;
    vtkVisualization->AddData(data.generation);
    //vtkVisualization->addData(1, data.generation);
  });

  // Run the actual simulation.
  sim.evolve(size, 1, breakpoint);
  std::cout << std::endl;

  // Run the brute-force attempt.
  std::string brute(target.size(), 0);
  size_t iterations = 0;
  auto start = std::chrono::high_resolution_clock::now();
  do {
    std::generate(brute.begin(), brute.end(), [&]{
      return valid[dist(mt)];
    });
    ++iterations;

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::high_resolution_clock::now() - start
    ).count();
    std::cout << "Brute force took " << iterations
      << " iterations and " << elapsed
      << " seconds.\xd";
  } while (brute != target);
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

