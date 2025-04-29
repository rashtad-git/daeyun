# Set UTF-8 encoding
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

cmake_minimum_required(VERSION 3.10)

project(MyProject)

# src 폴더 안의 모든 cpp, h 파일을 한 번에 찾기
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)

# 빌드 결과물을 bin 폴더에 저장
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include 디렉토리로 src를 추가 (헤더파일 찾기용)
include_directories(src)

# 실행 파일 만들기
add_executable(MyProject ${SOURCES})

# C++ 표준 설정 (필요에 따라 변경 가능)
set_target_properties(MyProject PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# 컴파일러 경고 레벨 높이기
if (MSVC)
    target_compile_options(MyProject PRIVATE /W4 /utf-8)
else()
    target_compile_options(MyProject PRIVATE -Wall -Wextra -pedantic)
endif()
