#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

//#include "../iceoryx_hoofs/include/iceoryx_hoofs/cxx/requires.hpp"
//#include "../iceoryx_posh/include/iceoryx_posh/runtime/posh_runtime.hpp"
//
#include "iceoryx_hoofs/cxx/requires.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include "iox/string.hpp"
#include <string.h>

// ----------------
// Regular C++ code
// ----------------
//std::vector<double> modify(const std::vector<double>& input)
//{
//  std::vector<double> output;
//
//  std::transform(
//    input.begin(),
//    input.end(),
//    std::back_inserter(output),
//    [](double x) -> double { return 2.*x; }
//  );
//
//  return output;
//}

namespace iox {
	namespace planet {
	namespace earth {
	namespace country {
		int square(int num) {
			return num * num;
		}

		void printname(const std::string &s) {
			std::cout << "cpp sys " << s << std::endl;
		}
	}
	}
	}

//	void *initRuntime_(char *ch) {
//		constexpr char str[128];
//		memcpy(str, ch, strlen(ch));
//		return iox::runtime::PoshRuntime::initRuntime(str);
//	  }
}

namespace iox {
// ----------------
// Python interface
// ----------------
namespace py = pybind11;


PYBIND11_MODULE(iceoryx_binding_python,m)
{
  m.doc() = "Iceoryx pybind11 binding module";
//  m.def("modify", &modify, "Testing function");

  m.def("square", &planet::earth::country::square);
  m.def("square", &planet::earth::country::printname);

  m.def("test",
      [](const std::string &s) {
	  planet::earth::country::printname(s);
      }
  );

//  m.def("initRuntime",
//	[](const char *s) {
//	    iox::runtime::PoshRuntime::initRuntime("");
//  	}
//  );


  //iox::runtime::PoshRuntime::initRuntime(APP_NAME);
//  m.def("initRuntime", &iox::runtime::PoshRuntime::initRuntime, py::arg("name"));

//  py::class_<iox>(m, iox)
//      .

//  py::class_<Pet>(m, "Pet")
//      .def(py::init<const std::string &>())
//      .def("setName", &Pet::setName)
//      .def("getName", &Pet::getName);
}
}

