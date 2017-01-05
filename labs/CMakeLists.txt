cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(set08116_graphics)
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
foreach (config ${CMAKE_CONFIGURATION_TYPES})
  string (TOUPPER ${config} config)
  set_target_properties(${_project} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
    ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
    LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}"
    #PDB_OUTPUT_DIRECTORY_${config} ${sym_path}
    VS_INTERMEDIATE_DIRECTORY_${config} "temp"
  )
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_DIRECTORY}/${config}")
  SET( VS_INTERMEDIATE_DIRECTORY_${config} "temp/${config}")
  link_directories("${OUTPUT_DIRECTORY}/${config}")
endforeach ()

include(DownloadProject.cmake)
download_project(PROJ enu_gfx
  GIT_REPOSITORY      https://github.com/edinburgh-napier/set08116_framework
  GIT_TAG             master
  ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)
add_subdirectory(${enu_gfx_SOURCE_DIR} ${enu_gfx_BINARY_DIR})
include_directories(${enu_gfx_SOURCE_DIR}/src ${enu_graphics_framework_incs}) 

#file(GLOB_RECURSE LIB_SOURCE_FILES lib_phys_utils/*.cpp lib_phys_utils/*.h)
#add_library(lib_phys_utils STATIC ${LIB_SOURCE_FILES})

#Grab practical folders
file(GLOB children RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" practicals/*)
SET(dirlist "")
FOREACH(child ${children})
  IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
    get_filename_component(childName ${child} NAME BASE_DIR practicals)
    LIST(APPEND dirlist ${childName})
    file(GLOB_RECURSE SOURCE_FILES ${child}/*.cpp ${child}/*.h ${child}/*.frag ${child}/*.vert ${child}/*.geom)
    file(GLOB_RECURSE NOT_RESOURCE_FILES ${child}/*.cpp ${child}/*.h)
    file(GLOB_RECURSE RESOURCE_FILES  ${child}/*.*)
    LIST(REMOVE_ITEM RESOURCE_FILES ${NOT_RESOURCE_FILES}) 
    
    add_executable( ${childName} ${SOURCE_FILES})
   
    #dependencies
    target_link_libraries(${childName} enu_graphics_framework )
    add_dependencies(${childName} enu_graphics_framework )
    
    #copy General resources to build post build script
    add_custom_command(TARGET ${childName} POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy_directory  "${PROJECT_SOURCE_DIR}/res" $<TARGET_FILE_DIR:${childName}>)
    
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

if(${MSVC})
      target_compile_options(enu_graphics_framework PUBLIC /MP)
ENDIF()

set_target_properties(enu_graphics_framework PROPERTIES FOLDER "DEPS")
set_target_properties(glm_dummy PROPERTIES FOLDER "DEPS")
set_target_properties(assimp PROPERTIES FOLDER "DEPS")