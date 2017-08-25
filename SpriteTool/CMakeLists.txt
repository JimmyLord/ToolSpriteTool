cmake_minimum_required( VERSION 3.0 )

#set( CMAKE_BUILD_TYPE Debug )

project( SpriteTool )

include_directories( "SpriteTool/Source" )

file( GLOB_RECURSE sourcefiles "SpriteTool/Source/*.cpp" "SpriteTool/Source/*.cc" )
list( REMOVE_ITEM sourcefiles "${CMAKE_SOURCE_DIR}/SpriteTool/Source/GameCore.cpp" )
list( REMOVE_ITEM sourcefiles "${CMAKE_SOURCE_DIR}/SpriteTool/Source/ShaderProgram.cpp" )
list( REMOVE_ITEM sourcefiles "${CMAKE_SOURCE_DIR}/SpriteTool/Source/WinMain.cpp" )
list( REMOVE_ITEM sourcefiles "${CMAKE_SOURCE_DIR}/SpriteTool/Source/GL/GLExtensions.cpp" )

#message( "${sourcefiles}" )

find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable( SpriteTool ${sourcefiles} )

target_link_libraries(
    SpriteTool
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
)