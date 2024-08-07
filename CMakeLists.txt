cmake_minimum_required(VERSION 3.5)

# 添加-Wall编译选项
add_compile_options(-Wall)
# 禁用栈保护
add_compile_options(-fno-stack-protector)

# 设置目标系统为Linux，这可以根据你的目标系统进行调整
set(CMAKE_SYSTEM_NAME Linux)

# 设置目标架构为x86_64
set(CMAKE_SYSTEM_PROCESSOR x86_64)
# 设置目标架构为ARM
#set(CMAKE_SYSTEM_PROCESSOR arm)


project(SystemMonitor LANGUAGES CXX)

# 定义项目版本号宏  主版本号（MAJOR）、次版本号（MINOR）和修订号（PATCH）
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_SYSTEM "linux")
set(PROJECT_VERSION_ARCH "x86_64")
set(PROJECT_VERSION_TIME "(2024.08.03)")
# 生成版本号字符串
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_VERSION_ALL "${PROJECT_VERSION}-${PROJECT_VERSION_SYSTEM}-${PROJECT_VERSION_ARCH} ${PROJECT_VERSION_TIME}")
message("项目版本号：${PROJECT_VERSION}")
set(VERSION_FILE "${CMAKE_SOURCE_DIR}/config/version.h.in")
configure_file("${VERSION_FILE}" "${CMAKE_BINARY_DIR}/config/version.h")  


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# 设置构建目录为build子目录
set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")
 
# 如果构建目录不存在，则创建它
if(NOT EXISTS ${CMAKE_BINARY_DIR})
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
if(NOT EXISTS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})# 如果目录不存在，则创建它
  file(MAKE_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

# 存放打包可执行文件的目录
set(tar_dir "${CMAKE_SOURCE_DIR}/${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}-${PROJECT_VERSION_SYSTEM}-${PROJECT_VERSION_ARCH}")
if(NOT EXISTS ${tar_dir})
  file(MAKE_DIRECTORY ${tar_dir})
endif()


# 提供对应模块的*.cmake文件的父目录
set(CMAKE_PREFIX_PATH "/home/jyu/install/Qt5.14.2/5.14.2/gcc_64")

# export QT_QPA_PLATFORM=xcb  # 提示信息: qt.qpa.plugin: Could not find the Qt platform plugin "dxcb" in "" ===> 则在shell中，手动执行此命令

# Qt插件目录。This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem. ===> 在shell中，手动执行此命令
# export QT_QPA_PLATFORM_PLUGIN_PATH=/home/jyu/install/Qt5.14.2/5.14.2/gcc_64/plugins


find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

# set(TS_FILES
#   res/language/SystemMonitor_zh_CN.ts
#   res/language/SystemMonitor_en_US.ts
# )


include_directories(3rdparty/json-develop/include)

include_directories(include) # 头文件目录

# 添加要编译的文件：(源文件、头文件、ui文件、资源文件)
set(PROJECT_SOURCES
    src/main.cpp
    src/systemmonitorapp.cpp
    src/systemmonitorui.cpp
    include/systemmonitorapp.h
    src/win.cpp
    include/win.h
    src/settings.cpp
    include/settings.h
    src/monitor/monitor.cpp
    include/monitor/monitor.h
    src/monitor/netspeed.cpp
    include/monitor/netspeed.h
    src/monitor/cpu_mem.cpp
    include/monitor/cpu_mem.h
    src/monitor/sys.cpp
    include/monitor/sys.h
    src/utils/data.cpp
    include/utils/data.h
    src/utils/qtdata.cpp
    include/utils/qtdata.h
    src/utils/json.cpp
    include/utils/json.h
    src/utils/qtui.cpp
    include/utils/qtui.h
    src/utils/cxxfile.cpp
    include/utils/cxxfile.h
    # ${TS_FILES}
    res.qrc
)

if(ANDROID)
  add_library(SystemMonitor SHARED
    ${PROJECT_SOURCES}
  )
else()
  add_executable(SystemMonitor
    ${PROJECT_SOURCES}
  )
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(SystemMonitor PRIVATE Qt5::Widgets)

# qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})


# --------------------------------------------------------------
# Set library search path    设置RPATH（运行时库搜索路径）
# --------------------------------------------------------------
# BUILD_RPATH
set_target_properties(SystemMonitor PROPERTIES BUILD_RPATH "$ORIGIN/../lib") # 可执行文件上一级目录下的lib目录
#set_target_properties(SystemMonitor PROPERTIES BUILD_RPATH "$ORIGIN") # 可执行文件所在目录
#set_target_properties(SystemMonitor PROPERTIES BUILD_RPATH "$ORIGIN:$ORIGIN/../lib") # 多个目录用:分隔

# INSTALL_RPATH
#set_target_properties(${PROJECT_NAME} PROPERTIES
#    OUTPUT_NAME ${PROJECT_NAME}
#    INSTALL_RPATH "${ORIGIN}/../lib"
#)
# set_target_properties(SystemMonitor PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
#set_target_properties(SystemMonitor PROPERTIES INSTALL_RPATH "$ORIGIN:/path/to/lib")
#set_target_properties(SystemMonitor PROPERTIES INSTALL_RPATH "$ORIGIN:/usr/lib/SystemMonitor")


# --------------------------------------------------------------
# Install
# --------------------------------------------------------------
set(LIBS_DIR "$ENV{DBXML_DIR}/lib")
message("$LIBS_DIR: " ${LIBS_DIR})
# install(FILES ${CMAKE_SOURCE_DIR}/pro-cc.conf DESTINATION /etc/init)
install(FILES ${CMAKE_SOURCE_DIR}/SystemMonitor.desktop DESTINATION /usr/share/applications/)
install(FILES ${CMAKE_SOURCE_DIR}/bin/SystemMonitor DESTINATION /usr/bin)


# INSTALL(FILES ${LIBS_DIR}/x86_64-linux-gnu/libc.so.6 ${LIBS_DIR}/x86_64-linux-gnu/libstdc++.so.6 DESTINATION /usr/lib)
INSTALL(FILES ${LIBS_DIR}/x86_64-linux-gnu/libc.so.6 ${LIBS_DIR}/x86_64-linux-gnu/libstdc++.so.6 DESTINATION /usr/lib/SystemMonitor)


# --------------------------------------------------------------
# CPack package and package_source targets
# --------------------------------------------------------------
# deb
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "${ROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6)")
set(CPACK_PACKAGE_DESCRIPTION "系统监视器")
set(CPACK_PACKAGE_CONTACT "1992817771@qq.com")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_ARCH}")
set(CPACK_DEBIAN_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
include(CPack)

