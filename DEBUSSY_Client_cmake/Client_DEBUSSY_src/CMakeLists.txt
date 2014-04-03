cmake_minimum_required(VERSION 2.8)
PROJECT(Client_DEBUSSY)

IF(WIN32)
    # #SET(CMAKE_C_FLAGS "ws2_32")
	 #set (CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")
	 set(CMAKE_PREFIX_PATH "C:\\Programmi\\Microsoft SDKs\\Windows\\v7.0\\Lib\\x64")
ENDIF(WIN32)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW) # remove if CMake >= 2.8.11 required
endif()

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set (CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")

# Widgets finds its own dependencies (QtGui and QtCore).
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)

set (CMAKE_PREFIX_PATH "${QTDIR}; ${WINSDK_LIB}")

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

SET(client_debussy_SOURCES main.cpp client_debussy.cpp)
SET(client_debussy_HEADERS client_debussy.h)

SET(client_debussy_FORMS client_debussy.ui)
SET(client_debussy_RESOURCES client_debussy.qrc)

#QT5_WRAP_CPP(client_debussy_HEADERS_MOC ${client_debussy_HEADERS})

QT5_WRAP_UI(client_debussy_FORMS_HEADERS ${client_debussy_FORMS})
QT5_ADD_RESOURCES(client_debussy_RESOURCES_RCC ${client_debussy_RESOURCES})

#INCLUDE_directories(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(client_debussy ${client_debussy_SOURCES} 
    ${client_debussy_HEADERS_MOC} 
    ${client_debussy_FORMS_HEADERS} 
    ${client_debussy_RESOURCES_RCC})
	
# Use the Widgets module from Qt 5.
qt5_use_modules(client_debussy Widgets Network)

FILE(COPY img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	
#this is the build directory	
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

INCLUDE_directories(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${client_debussy_SOURCES})

