# CMakeList.txt: JsonExample 的 CMake 项目，在此处包括源代码并定义

#=========================配置项目信息=======================================

# 项目名称
project(JsonExample LANGUAGES CXX)

# 设置启用QT编译器
set(CMAKE_AUTOMOC ON)

# 引入所有H文件和CPP文件
file(GLOB SRC_H_FILES "*.h")
file(GLOB SRC_CPP_FILES "*.cpp")

# 将源代码添加到此项目的可执行文件
add_executable(${PROJECT_NAME} ${SRC_H_FILES} ${SRC_CPP_FILES})

# 包含头文件目录
target_include_directories(${PROJECT_NAME} 
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/..
)



# =================引入外部依赖库==================

find_package(Qt5 COMPONENTS Core REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

# 指定依赖的库文件
add_dependencies(${PROJECT_NAME} CJsonSerializer QJsonSerializer)
target_link_libraries(${PROJECT_NAME} CJsonSerializer QJsonSerializer)
