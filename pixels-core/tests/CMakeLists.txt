# 设置 CMake 的最低版本要求和项目名
cmake_minimum_required(VERSION 3.10)
project(TimestampColumnVectorTest)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 包含源文件
include_directories(${CMAKE_SOURCE_DIR}/include)

# 查找测试文件
file(GLOB TEST_SOURCES *.cpp)

# 添加可执行文件，指定源文件
add_executable(test_TimestampColumnVector ${TEST_SOURCES})

# 链接目标
target_link_libraries(test_TimestampColumnVector ${CMAKE_SOURCE_DIR}/lib/libpixels.a)

# 添加测试
enable_testing()
add_test(NAME TimestampColumnVectorTest COMMAND test_TimestampColumnVector)


