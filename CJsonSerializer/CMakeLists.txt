# CMakeList.txt: CJsonSerializer 的 CMake 项目，在此处包括源代码并定义

#=========================配置项目信息=======================================

# 项目名称
project(CJsonSerializer LANGUAGES CXX)

# 引入所有H文件和CPP文件
file(GLOB SRC_H_FILES "*.h")
file(GLOB SRC_CPP_FILES "*.cpp")

# 将源代码添加到此项目的可执行文件
add_library(${PROJECT_NAME} ${SRC_H_FILES} ${SRC_CPP_FILES})

# 包含头文件目录
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} 
	PRIVATE
	${THIRDPARTY_DIR}/rapidjson
)
