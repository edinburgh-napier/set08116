cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
endif(MSVC)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(set08116_coursework)
#### Setup Directories ####
#Main output directory
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/bin")
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
# Ouput all DLLs from all libs into main build folder

### Add External Dependencies ####


add_subdirectory("../framework" "framework_build/")

#Glew and Devil are dumb and don't listen to our build directories, so manually set it.
set(IL_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "Output location for Devil .dlls/.so" FORCE)
set(IL_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE PATH "Output location for Devil .libs" FORCE)

set_target_properties(libglew_shared PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
set_target_properties(IL PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
set_target_properties(ILU PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)


file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h res/shaders/*.frag res/shaders/*.vert res/shaders/*.geom res/shaders/*.comp)
file(GLOB_RECURSE NOT_RESOURCE_FILES src/*.cpp src/*.h)
file(GLOB_RECURSE RESOURCE_FILES  src/*.*)
LIST(REMOVE_ITEM RESOURCE_FILES ${NOT_RESOURCE_FILES}) 
add_executable(coursework ${SOURCE_FILES})
target_link_libraries(coursework PRIVATE enu_graphics_framework)

#copy General resources to build post build script
add_custom_target(copy_resources ALL 
 COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
 COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/../framework/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

add_dependencies(coursework copy_resources)

if(RESOURCE_FILES)
message("Resources: ${RESOURCE_FILES}")
  #copy Project Specific Resources
  FOREACH(res ${RESOURCE_FILES})
	get_filename_component(fn ${res} NAME)
	add_custom_command(TARGET coursework POST_BUILD  
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${res} $<TARGET_FILE_DIR:coursework>/res/${fn})
  ENDFOREACH()
endif(RESOURCE_FILES)

set_target_properties(coursework PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)

set_target_properties(enu_graphics_framework PROPERTIES FOLDER "DEPS")