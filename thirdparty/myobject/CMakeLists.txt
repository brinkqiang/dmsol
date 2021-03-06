cmake_minimum_required(VERSION 3.2)

PROJECT(myobject)
INCLUDE(cmake/ModuleImport.cmake)
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(MYOBJECT_VERSION_MAJOR "1")
SET(MYOBJECT_VERSION_MINOR "0")
SET(MYOBJECT_VERSION_PATCH "1")
SET(MYOBJECT_VERSION "${MYOBJECT_VERSION_MAJOR}.${MYOBJECT_VERSION_MINOR}.${MYOBJECT_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${MYOBJECT_VERSION}")

ADD_DEFINITIONS(-DUSE_MYOBJECT)
ADD_DEFINITIONS(-DLUA_COMPAT_MODULE -DLUA_COMPAT_APIINTCASTS)
OPTION(USE_LUA_INNER "use lua inner" OFF)
OPTION(USE_MYOBJECT_TEST "use myobject test" ON)

#ModuleImport("dmlog" "thirdparty/dmlog")

IF (WIN32)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/windows)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/lua)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/myobject)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/test)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/bin)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

IF (USE_MYOBJECT_TEST)
    FILE(GLOB DMLUA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua/*.h
    )

    LIST(FILTER DMLUA_SOURCES EXCLUDE REGEX "lua.c$")
    LIST(FILTER DMLUA_SOURCES EXCLUDE REGEX "luac.c$")
    LIST(FILTER DMLUA_SOURCES EXCLUDE REGEX "wmain.c$")

    FILE(GLOB LUA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua/lua.c
    )
ENDIF(USE_MYOBJECT_TEST)


FILE(GLOB MYOBJECT_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/*.h

${CMAKE_CURRENT_SOURCE_DIR}/src/myobject/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/myobject/*.cc
${CMAKE_CURRENT_SOURCE_DIR}/src/myobject/*.c
${CMAKE_CURRENT_SOURCE_DIR}/src/myobject/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/src/myobject/*.h
)

IF (WIN32)
    LIST(APPEND MYOBJECT_SOURCES)
ENDIF(WIN32)

FILE(GLOB_RECURSE MYOBJECTTEST_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc
${CMAKE_CURRENT_SOURCE_DIR}/test/*.c
${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp
${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)

IF (WIN32)
    IF (USE_LUA_INNER)
        ADD_LIBRARY(lua53 SHARED ${DMLUA_SOURCES})
        SET_TARGET_PROPERTIES(lua53 PROPERTIES COMPILE_FLAGS "-DLUA_BUILD_AS_DLL -DLUA_CORE")
        
        ADD_EXECUTABLE(lua ${LUA_SOURCES})
        TARGET_LINK_LIBRARIES(lua lua53)
    ENDIF(USE_LUA_INNER)

    ADD_LIBRARY(myobject SHARED ${MYOBJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/myobject/myobject_module.def)
    TARGET_LINK_LIBRARIES(myobject lua53)

    SET_TARGET_PROPERTIES(myobject PROPERTIES COMPILE_FLAGS "-DLUA_BUILD_AS_DLL -DLUA_LIB")

    IF (USE_MYOBJECT_TEST)
        ADD_EXECUTABLE(myobjecttest ${MYOBJECTTEST_SOURCES})
        TARGET_LINK_LIBRARIES(myobjecttest myobject)
        SET_TARGET_PROPERTIES(myobjecttest PROPERTIES COMPILE_FLAGS "-DLUA_BUILD_AS_DLL")
    ENDIF(USE_MYOBJECT_TEST)

ELSEIF (APPLE)
    ADD_DEFINITIONS(-DLUA_USE_MACOSX)
    IF (USE_LUA_INNER)
        ADD_LIBRARY(lua53 SHARED ${DMLUA_SOURCES})
        SET_TARGET_PROPERTIES(lua53 PROPERTIES COMPILE_FLAGS "-Wl,-E" )

        ADD_EXECUTABLE(lua ${LUA_SOURCES})
        TARGET_LINK_LIBRARIES(lua lua53 dl)
    ENDIF(USE_LUA_INNER)

    ADD_LIBRARY(myobject SHARED ${MYOBJECT_SOURCES})
    SET_TARGET_PROPERTIES(myobject PROPERTIES COMPILE_FLAGS "-Wl,-E" )
    SET_TARGET_PROPERTIES(myobject PROPERTIES PREFIX "")

    TARGET_LINK_LIBRARIES(myobject lua53)

    IF (USE_MYOBJECT_TEST)
        ADD_EXECUTABLE(myobjecttest ${MYOBJECTTEST_SOURCES})
        TARGET_LINK_LIBRARIES(myobjecttest myobject)
    ENDIF(USE_MYOBJECT_TEST)   
ELSEIF (UNIX)
    ADD_DEFINITIONS(-DLUA_USE_LINUX)

    IF (USE_LUA_INNER)
        ADD_LIBRARY(lua53 SHARED ${DMLUA_SOURCES})
        SET_TARGET_PROPERTIES(lua53 PROPERTIES COMPILE_FLAGS "-Wl,-E" )

        ADD_EXECUTABLE(lua ${LUA_SOURCES})
        TARGET_LINK_LIBRARIES(lua lua53 m dl)
    ENDIF(USE_LUA_INNER)

    ADD_LIBRARY(myobject SHARED ${MYOBJECT_SOURCES})
    SET_TARGET_PROPERTIES(myobject PROPERTIES COMPILE_FLAGS "-Wl,-E" )
    SET_TARGET_PROPERTIES(myobject PROPERTIES PREFIX "")
    TARGET_LINK_LIBRARIES(myobject lua53)
    
    IF (USE_MYOBJECT_TEST)
        ADD_EXECUTABLE(myobjecttest ${MYOBJECTTEST_SOURCES})
        TARGET_LINK_LIBRARIES(myobjecttest myobject)
    ENDIF(USE_MYOBJECT_TEST)   
ENDIF ()

#ADD_CUSTOM_COMMAND(
#TARGET myobjecttest
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different
#${CMAKE_CURRENT_SOURCE_DIR}/config/test.csv
#$<TARGET_FILE_DIR:myobjecttest>
#)

#INSTALL(DIRECTORY include/ DESTINATION include)
#INSTALL(TARGETS myobject
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)

#SET(MYOBJECT_LIBS "myobject")
#MARK_AS_ADVANCED(${MYOBJECT_LIBS})
