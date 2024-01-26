# CMAKE version of Ubuntu 16.04 is 3.5.
cmake_minimum_required(VERSION 3.5)
project(HINT)

set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

include(${CMAKE_CURRENT_LIST_DIR}/lib/ClearSense/ClearSense.cmake)

if(MSVC)
list(APPEND LINKLIBS
	${CMAKE_CURRENT_LIST_DIR}/build/onnxruntime.lib
	${CMAKE_CURRENT_LIST_DIR}/build/onnxruntime_providers_shared.lib
  )
elseif(AARCH64)
list(APPEND LINKLIBS
	${CMAKE_CURRENT_LIST_DIR}/build/aarch64/libonnxruntime.so.1.14.1
  m
  #  sudo apt install libtbb-dev
  tbb
)
else()
list(APPEND LINKLIBS
	${CMAKE_CURRENT_LIST_DIR}/build/x64/libonnxruntime.so.1.14.1
  m
  #  sudo apt install libtbb-dev
  tbb
)
endif()


add_executable(${PROJECT_NAME} ${SRC} test.cpp)
target_include_directories(${PROJECT_NAME}    PRIVATE    ${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 17 )