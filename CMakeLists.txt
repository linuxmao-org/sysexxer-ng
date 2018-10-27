cmake_minimum_required(VERSION 3.3)
project(sysexxer-ng VERSION 0.1 LANGUAGES C CXX)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

find_package(Threads REQUIRED)

set(FLTK_SKIP_OPENGL ON)
set(FLTK_SKIP_FORMS ON)
set(FLTK_SKIP_IMAGES ON)
set(FLTK_SKIP_FLUID ON)
find_package(FLTK REQUIRED)

add_executable(sysexxer-ng
  "sources/main.cc"
  "sources/window.cxx"
  "sources/system_exclusive.cc"
  "sources/sysex_send.cc"
  "sources/utility.cc"
  "sources/device/midi.cc"
  "sources/device/midi_apis.cc")

target_include_directories(sysexxer-ng
  PRIVATE "sources" "${FLTK_INCLUDE_DIR}")
target_link_libraries(sysexxer-ng PRIVATE "${FLTK_LIBRARIES}")

include(RtMidi)
target_link_libraries(sysexxer-ng PRIVATE RtMidi)

add_library(ring_buffer STATIC "thirdparty/ring_buffer/ring_buffer.cpp")
target_include_directories(ring_buffer PUBLIC "thirdparty/ring_buffer")
target_link_libraries(sysexxer-ng PRIVATE ring_buffer)

target_link_libraries(sysexxer-ng PRIVATE "${CMAKE_THREAD_LIBS_INIT}")

install(TARGETS sysexxer-ng DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "resources/application/sysexxer-ng.desktop" DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
install(FILES "resources/mime/sysexxer-ng-syx.xml" DESTINATION "${CMAKE_INSTALL_DATADIR}/mime/packages")
