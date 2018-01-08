cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
endif(MSVC)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(set08116_graphics)
#### Setup Directories ####
#Main output directory
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/bin")
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
# Ouput all DLLs from all libs into main build folder

#### Add External Dependencies ####
#enu_graphics_framework
add_subdirectory("framework")

#Glew and Devil are dumb and don't listen to our build directories, so manually set it.
set(IL_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH "Output location for Devil .dlls/.so" FORCE)
set(IL_LIBRARY_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE PATH "Output location for Devil .libs" FORCE)

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

set_target_properties(libglew_shared PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

#Grab practical folders
file(GLOB children RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" practicals/*)
SET(dirlist "")
FOREACH(child ${children})
  IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
    get_filename_component(childName ${child} NAME BASE_DIR practicals)
    LIST(APPEND dirlist ${childName})
    file(GLOB_RECURSE SOURCE_FILES ${child}/*.cpp ${child}/*.h ${child}/*.frag ${child}/*.vert ${child}/*.geom ${child}/*.comp)
    file(GLOB_RECURSE NOT_RESOURCE_FILES ${child}/*.cpp ${child}/*.h)
    file(GLOB_RECURSE RESOURCE_FILES  ${child}/*.*)
    LIST(REMOVE_ITEM RESOURCE_FILES ${NOT_RESOURCE_FILES}) 
    
    add_executable( ${childName} ${SOURCE_FILES})
   
    #dependencies
    target_link_libraries(${childName} PRIVATE enu_graphics_framework )
    #add_dependencies(${childName} enu_graphics_framework )
    
    #copy General resources to build post build script
    add_custom_command(TARGET ${childName} POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy_directory  "${PROJECT_SOURCE_DIR}/res" $<TARGET_FILE_DIR:${childName}>)
	
    add_custom_command(TARGET ${childName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${PROJECT_SOURCE_DIR}/framework/res"
			$<TARGET_FILE_DIR:${childName}>
	)
	
    if(RESOURCE_FILES)
    message("Resources: ${RESOURCE_FILES}")
      #copy Project Specific Resources
      FOREACH(res ${RESOURCE_FILES})
        get_filename_component(fn ${res} NAME)
        add_custom_command(TARGET ${childName} POST_BUILD  
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${res} $<TARGET_FILE_DIR:${childName}>/${childName}/${fn})
      ENDFOREACH()
    endif(RESOURCE_FILES)
    
    #set outDir as debugging directory for each project
    if(${MSVC})
    file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${childName}.vcxproj.user" 
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>
        <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
        <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
            <LocalDebuggerWorkingDirectory>$(TargetDir)</LocalDebuggerWorkingDirectory>
            <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
        </PropertyGroup>
        <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
            <LocalDebuggerWorkingDirectory>$(TargetDir)</LocalDebuggerWorkingDirectory>
            <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
        </PropertyGroup>
        </Project>")
    endif()
  ENDIF()
ENDFOREACH()

set_target_properties(enu_graphics_framework PROPERTIES FOLDER "DEPS")