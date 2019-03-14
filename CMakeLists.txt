cmake_minimum_required(VERSION "3.3")
project(sysexxer-ng VERSION "1.0" LANGUAGES C CXX)
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

option(ENABLE_GETTEXT "Enable internationalization with gettext" ON)

if(ENABLE_GETTEXT)
  find_package(Intl REQUIRED)
  find_package(Gettext REQUIRED)
endif()

add_executable(sysexxer-ng
  "sources/main.cc"
  "sources/config.cc"
  "sources/window.cxx"
  "sources/dnd_hold_browser.cc"
  "sources/hyperlink_button.cc"
  "sources/system_exclusive.cc"
  "sources/sysex_send.cc"
  "sources/utility.cc"
  "sources/device/midi.cc"
  "sources/device/midi_apis.cc")
target_compile_definitions(sysexxer-ng
  PRIVATE "LOCALE_DIRECTORY=\"${CMAKE_INSTALL_FULL_LOCALEDIR}\""
  PRIVATE "PROJECT_VERSION=\"${PROJECT_VERSION}\"")
target_include_directories(sysexxer-ng
  PRIVATE "sources" "${FLTK_INCLUDE_DIR}")
target_link_libraries(sysexxer-ng PRIVATE "${FLTK_LIBRARIES}")

include(RtMidi)
target_link_libraries(sysexxer-ng PRIVATE RtMidi)

add_library(simpleini STATIC "thirdparty/simpleini/ConvertUTF.cpp")
target_include_directories(simpleini PUBLIC "thirdparty/simpleini")
target_link_libraries(sysexxer-ng PRIVATE simpleini)

add_library(ring_buffer STATIC "thirdparty/ring_buffer/ring_buffer.cpp")
target_include_directories(ring_buffer PUBLIC "thirdparty/ring_buffer")
target_link_libraries(sysexxer-ng PRIVATE ring_buffer)

add_subdirectory("thirdparty/fmidi" EXCLUDE_FROM_ALL)
target_link_libraries(sysexxer-ng PRIVATE fmidi)

if(ENABLE_GETTEXT)
  target_compile_definitions(sysexxer-ng PRIVATE "ENABLE_NLS=1")
  target_include_directories(sysexxer-ng PRIVATE ${Intl_INCLUDE_DIRS})
  target_link_libraries(sysexxer-ng PRIVATE ${Intl_LIBRARIES})
  target_include_directories(sysexxer-ng PRIVATE "thirdparty/gettext/include")
endif()

target_link_libraries(sysexxer-ng PRIVATE "${CMAKE_THREAD_LIBS_INIT}")

install(TARGETS sysexxer-ng DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "resources/application/sysexxer-ng.desktop" DESTINATION "${CMAKE_INSTALL_DATADIR}/applications")
install(FILES "resources/application/sysexxer-ng.png" DESTINATION "${CMAKE_INSTALL_DATADIR}/pixmaps")
install(FILES "resources/mime/sysexxer-ng-syx.xml" DESTINATION "${CMAKE_INSTALL_DATADIR}/mime/packages")

if(ENABLE_GETTEXT)
  set(TRANSLATIONS "fr")
  foreach(translation ${TRANSLATIONS})
    gettext_process_po_files("${translation}" ALL INSTALL_DESTINATION "${CMAKE_INSTALL_LOCALEDIR}" PO_FILES
      "po/${translation}/sysexxer-ng.po")
  endforeach()
endif()

install(FILES "resources/man/sysexxer-ng.1" DESTINATION "${CMAKE_INSTALL_MANDIR}/man1")
foreach(translation "fr")
  install(FILES "resources/man/sysexxer-ng.${translation}.1" DESTINATION "${CMAKE_INSTALL_MANDIR}/${translation}/man1" RENAME "sysexxer-ng.1")
endforeach()
