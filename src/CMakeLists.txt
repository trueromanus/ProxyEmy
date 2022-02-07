cmake_minimum_required(VERSION 3.14)

project(ProxyEmy LANGUAGES CXX)

include(FetchContent)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_MAJOR_VERSION 1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick LinguistTools Concurrent Qml QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools Network WebSockets Concurrent Qml QuickControls2 REQUIRED)

set(APP_ICON_RESOURCE_WINDOWS "appicon.rc")

set(PROJECT_SOURCES
        main.cpp
        globalenums.h
        ViewModels/configurationviewmodel.cpp ViewModels/configurationviewmodel.h
        ViewModels/notificationhubviewmodel.h ViewModels/notificationhubviewmodel.cpp
        ViewModels/requestslogviewmodel.h ViewModels/requestslogviewmodel.cpp
        ViewModels/addoptionscardviewmodel.h ViewModels/addoptionscardviewmodel.cpp
        Networking/httpproxyserver.cpp Networking/httpproxyserver.h
        ListModels/configurationmappinglistmodel.h ListModels/configurationmappinglistmodel.cpp
        ListModels/configurationaliaseslistmodel.h ListModels/configurationaliaseslistmodel.cpp
        ListModels/notificationpoollistmodel.h ListModels/notificationpoollistmodel.cpp
        ListModels/pagestabslistmodel.h ListModels/pagestabslistmodel.cpp
        ListModels/requestlogslistmodel.h ListModels/requestlogslistmodel.cpp
        proxyemybackend.cpp proxyemybackend.h
        Models/routemapping.h Models/routemapping.cpp
        Models/requestlogitem.h Models/requestlogitem.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ProxyEmy
        WIN32 MACOSX_BUNDLE
        ${PROJECT_SOURCES}
        ${APP_ICON_RESOURCE_WINDOWS}
    )
else()
    if(ANDROID)
        add_library(ProxyEmy SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ProxyEmy
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(ProxyEmy
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(ProxyEmy
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
target_link_libraries(ProxyEmy PRIVATE Qt6::Concurrent)
target_link_libraries(ProxyEmy PRIVATE Qt6::Qml)
target_link_libraries(ProxyEmy PRIVATE Qt6::QuickControls2)

# uncomment for using new QML_ELEMENT way
#qt_add_qml_module(ProxyEmy
#    URI ProxyEmy.Backend
#    VERSION 1.0
#    RESOURCE_PREFIX /proxy.emy/imports
#    SOURCES
#        proxyemybackend.cpp proxyemybackend.h
#)

FetchContent_Declare(
                yaml-cpp
                GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
                GIT_SHALLOW	ON
                GIT_TAG yaml-cpp-0.7.0
        )
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
        message(STATUS "Populating yaml-cpp...")
        FetchContent_Populate(yaml-cpp)
        set(YAML_CPP_BUILD_TESTS OFF)
        add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
        message(STATUS "Done.")
endif()

include_directories(BEFORE SYSTEM ${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR}/include)
target_link_libraries(ProxyEmy PRIVATE yaml-cpp)

if(WIN32)
    target_link_libraries(ProxyEmy PRIVATE Crypt32)
endif()
