# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置

cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)


#=========================配置全局项目信息=======================================

# 设置项目名称
project(JsonSerializer)

# 设置c++语言标准，这里使用c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定qt5所在的路径
set(Qt5_DIR C:/Qt/Qt5.15.2/5.15.2/msvc2019_64/lib/cmake/Qt5)

# 设置项目文件的输出目录
if(CMAKE_BUILD_TYPE MATCHES "Debug")
	set(BUILD_POSTFIX "d")
endif()

set(PROJECT_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/output/bin${BUILD_POSTFIX})
set(PROJECT_MODULE_OUTPUT_DIR ${PROJECT_OUTPUT_DIR})
set(PROJECT_APP_OUTPUT_DIR ${PROJECT_OUTPUT_DIR})

# 设置库文件输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_MODULE_OUTPUT_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_APP_OUTPUT_DIR})


# 设置第三方库目录
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/libs)

#=========================添加子项目目录=======================================

add_subdirectory(CJsonSerializer)
add_subdirectory(QJsonSerializer)
add_subdirectory(JsonExample)


IF(CMAKE_SYSTEM_NAME MATCHES "Windows")    

    # 复制qt插件文件
    MESSAGE(STATUS "Start copying qt plugins files.")
    FILE(COPY ${Qt5_DIR}/../../../plugins/platforms
              #${Qt5_DIR}/../../../plugins/imageformats
              #${Qt5_DIR}/../../../plugins/styles
              DESTINATION ${PROJECT_MODULE_OUTPUT_DIR}/plugins)

    # 复制qt库文件
    MESSAGE(STATUS "Start copying QT library files.")
    FILE(COPY ${Qt5_DIR}/../../../bin/Qt5Core${BUILD_POSTFIX}.dll
              #${Qt5_DIR}/../../../bin/Qt5Gui${BUILD_POSTFIX}.dll
              #${Qt5_DIR}/../../../bin/Qt5Widgets${BUILD_POSTFIX}.dll
              DESTINATION ${PROJECT_MODULE_OUTPUT_DIR})

    MESSAGE(STATUS "Copy all the library files successfully.")

ENDIF()


# ==================打印环境信息==================

# 打印项目输出路径
message( STATUS "项目输出目录 = ${PROJECT_OUTPUT_DIR}" )
