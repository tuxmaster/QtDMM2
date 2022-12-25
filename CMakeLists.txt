cmake_minimum_required(VERSION 3.25)
project(QDMM2 VERSION 0.3.0 LANGUAGES CXX)
message(STATUS "Creating makefile for ${PROJECT_NAME} ${PROJECT_VERSION}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 5.15 COMPONENTS Core Gui Widgets REQUIRED)
message(STATUS "Using Qt ${Qt5Core_VERSION}")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

option(DISPLAYTEST "Displayt test modus" OFF)
option(DIGITAL "Use digital scale" OFF)

if((${DIGITAL}) AND (${DISPLAYTEST}))
	message(FATAL_ERROR "DISPLAYTEST and DIGITAL are exclusive.")
endif()

set(UI_PATH ui)

set (CPP_HEADER
  src/analogapp.h
  src/analogdisplay.h
  src/digitalapp.h
  src/digitaldisplay.h
  src/displaycolor.h
  src/dmmdisplay.h
  src/lcdnumber.h
  src/plotdata.h
  src/plotgrid.h
  src/plotscale.h
  src/plotwid.h
  src/unitstring.h
)

set(CPP_SOURCES
  src/analogapp.cpp
  src/analogdisplay.cpp
  src/digitalapp.cpp
  src/digitaldisplay.cpp
  src/displaycolor.cpp
  src/dmmdisplay.cpp
  src/lcdnumber.cpp
  src/plotdata.cpp
  src/plotgrid.cpp
  src/plotscale.cpp
  src/plotwid.cpp
  src/unitstring.cpp
)

set(UI_FILES
    ${UI_PATH}/dmmprefs.ui
)

if(${DISPLAYTEST})
	message(STATUS "Using the display test mode.")
	list(APPEND CPP_SOURCES "src/main_displaytest.cpp")
else()
	if(${DIGITAL})
		message(STATUS "Using the digital scale.")
		list(APPEND CPP_SOURCES "src/main_digital.cpp")
	else()
		message(STATUS "Using the analog scale.")
		list(APPEND CPP_SOURCES "src/main_analog.cpp")
	endif()
endif()

set(PROJECT_SOURCES
    ${CPP_HEADER}
    ${CPP_SOURCES}
    ${UI_FILES}
)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_PATH})
set(BIN_TARGET_PATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}" CACHE PATH "Target directory for the app" FORCE)


message(STATUS "Install binary to ${BIN_TARGET_PATH}")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Gui Qt::Widgets)

install(TARGETS ${PROJECT_NAME} DESTINATION ${BIN_TARGET_PATH})

