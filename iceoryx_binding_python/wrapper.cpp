#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

// ----------------
// Regular C++ code
// ----------------

std::vector<double> modify(const std::vector<double>& input)
{
  std::vector<double> output;

  std::transform(
    input.begin(),
    input.end(),
    std::back_inserter(output),
    [](double x) -> double { return 2.*x; }
  );

  return output;
}
//iox::runtime::PoshRuntime::initRuntime(APP_NAME);
// ----------------
// Python interface
// ----------------

namespace py = pybind11;

PYBIND11_MODULE(pyiceoryx,m)
{
  m.doc() = "pybind11 example plugin";
  m.def("modify", &modify, "Multiply all entries of a list by 2.0");

  m.def("initRuntime", &iox::runtime::PoshRuntime::initRuntime);

//  py::class_<iox>(m, iox)
//      .

//  py::class_<Pet>(m, "Pet")
//      .def(py::init<const std::string &>())
//      .def("setName", &Pet::setName)
//      .def("getName", &Pet::getName);
}

