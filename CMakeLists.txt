cmake_minimum_required(VERSION 3.2)

PROJECT(dmsol)

INCLUDE(cmake/ModuleImport.cmake)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(DMSOL_VERSION_MAJOR "1")
SET(DMSOL_VERSION_MINOR "0")
SET(DMSOL_VERSION_PATCH "1")
SET(DMSOL_VERSION "${DMSOL_VERSION_MAJOR}.${DMSOL_VERSION_MINOR}.${DMSOL_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${DMSOL_VERSION}")

ADD_DEFINITIONS(-DUSE_DMSOL)
ADD_DEFINITIONS(-DLUA_COMPAT_MODULE)

ModuleInclude("" "")
ModuleImport("dmlog" "thirdparty/dmlog")
ModuleImport("luapb" "thirdparty/luapb")
ModuleImport("myobject" "thirdparty/myobject")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/luapb/src/lua)

FILE(GLOB_RECURSE DMSOLTEST_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/test/gtest/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/test/gtest/*.cc
${CMAKE_CURRENT_SOURCE_DIR}/test/gtest/*.c
${CMAKE_CURRENT_SOURCE_DIR}/test/gtest/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/test/gtest/*.h

${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc
${CMAKE_CURRENT_SOURCE_DIR}/test/*.c
${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)

IF (WIN32)
    ADD_EXECUTABLE(dmsoltest ${DMSOLTEST_SOURCES})
    TARGET_LINK_LIBRARIES(dmsoltest lua53 luapb libprotobuf libprotoc)
ELSEIF (APPLE)
    ADD_EXECUTABLE(dmsoltest ${DMSOLTEST_SOURCES})
    TARGET_LINK_LIBRARIES(dmsoltest lua53 luapb libprotobuf libprotoc)
ELSEIF (UNIX)
    ADD_EXECUTABLE(dmsoltest ${DMSOLTEST_SOURCES})
    TARGET_LINK_LIBRARIES(dmsoltest lua53 luapb libprotobuf libprotoc)
ENDIF ()

#ADD_CUSTOM_COMMAND(
#TARGET dmsoltest
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different
#${CMAKE_CURRENT_SOURCE_DIR}/config/test.csv
#$<TARGET_FILE_DIR:dmsoltest>
#)

#INSTALL(DIRECTORY include/ DESTINATION include)
#INSTALL(TARGETS dmsol
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)

#SET(DMSOL_LIBS "dmsol")
#MARK_AS_ADVANCED(${DMSOL_LIBS})