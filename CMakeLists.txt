cmake_minimum_required (VERSION 3.14)


option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

if(ENABLE_TESTING)
  list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

project ("preferance" CXX)

if(ENABLE_TESTING)
  include(CTest)
endif()

add_library(cxx_project_options INTERFACE)
target_compile_features(cxx_project_options INTERFACE cxx_std_20)

add_subdirectory(libpreferance)
