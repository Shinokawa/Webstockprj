# CMakeLists.txt

# 1. 指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 声明项目名称和使用的语言
project(WEBSO LANGUAGES CXX)

# 3. 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 添加包含目录
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/traderapi64_se_windows)

# 5. 指定源文件
file(GLOB SOURCES "src/*.cpp")

# 6. 添加可执行文件
add_executable(WEBSO ${SOURCES})

# 7. 链接CTP API的静态库
target_link_libraries(WEBSO "${PROJECT_SOURCE_DIR}/traderapi64_se_windows/thostmduserapi_se.lib")

# 8. 设置运行时库路径（确保DLL能被找到）
# 复制DLL到可执行文件所在目录
add_custom_command(TARGET WEBSO POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/traderapi64_se_windows/thostmduserapi_se.dll"
        $<TARGET_FILE_DIR:WEBSO>)