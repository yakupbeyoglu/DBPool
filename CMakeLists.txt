cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0054 NEW)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME "DBPool")
project(${PROJECT_NAME})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(INCLUDE_PATH include)
set(SOURCE_PATHS  ${CMAKE_CURRENT_SOURCE_DIR}/src)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH})

# Include Source directory as a subdirectory
add_subdirectory(${SOURCE_PATHS})

install(DIRECTORY ${INCLUDE_PATH}/ DESTINATION include)

option(COMPILE_TESTS "Enable unit tests" ON)
option(GENERATE_DOCS "Generate DOCS" OFF)
option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer")

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


target_link_libraries(${PROJECT_NAME} PUBLIC db_pool mysqlcppconn)


# Add Source directory to include paths
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

get_target_property(INCLUDE_DIRS ${PROJECT_NAME} INCLUDE_DIRECTORIES)
message("Include directories for ${PROJECT_NAME}: ${INCLUDE_DIRS}")

# Unit tests compile
if(COMPILE_TESTS)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test/)
enable_testing()
endif(COMPILE_TESTS)


# Documentation generation based on Doxygen and dot
if(GENERATE_DOCS)
find_package(Doxygen REQUIRED)
find_package(Doxygen REQUIRED dot)

DOC(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Docs/)

  set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
  set(DOXYGEN_CALL_GRAPH YES)
  set(DOXYGEN_CALLER_GRAPH YES)
  set(DOXYGEN_WARN_AS_ERROR YES)
  set(DOXYGEN_SOURCE_BROWSER YES)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_SORT_MEMBER_DOCS NO)
  set(DOXYGEN_FILE_PATTERNS *.cpp *.h *.hpp)
  set(DOXYGEN_EXCLUDE_PATTERNS */bin/* */build/* */test/* *.cc main.cpp)

  doxygen_add_docs(
      doxygen_doc
      ALL
      ${CMAKE_CURRENT_LIST_DIR}
      COMMENT "Generate doxygen documentation for ${PROJECT_NAME}"
  )
set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs/)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)
set(DOXYGEN_EXTRACT_ALL YES)
message("Doxygen output directory: ${DOXYGEN_OUTPUT_DIRECTORY}")

doxygen_add_docs(
    doc
    # ${CMAKE_SOURCE_DIR}/Source/Your_HEADEDER
    COMMENT "Generating API documentation with Doxygen"
)
endif(GENERATE_DOCS)

if(ENABLE_ADDRESS_SANITIZER)
target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif(ENABLE_ADDRESS_SANITIZER)
