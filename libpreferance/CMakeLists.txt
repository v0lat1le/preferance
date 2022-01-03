
add_library(preferance INTERFACE "libpreferance/preferance.hpp")
target_include_directories(preferance INTERFACE ".")

if(ENABLE_TESTING)
  enable_testing()

  find_package(Catch2 2 REQUIRED)

  add_executable (preftests "libpreferance/preferance.test.cpp")
  target_link_libraries(preftests PRIVATE cxx_project_options preferance Catch2::Catch2)

  include(Catch)
  catch_discover_tests(preftests TEST_PREFIX "unittests.")
endif()
