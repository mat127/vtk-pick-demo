
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(HighlightPickedActor)

find_package(VTK COMPONENTS 
  vtkCommonColor
  vtkCommonCore
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingContextOpenGL2
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingGL2PSOpenGL2
  vtkRenderingOpenGL2 QUIET)
if (NOT VTK_FOUND)
  message("Skipping HighlightPickedActor: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(HighlightPickedActor MACOSX_BUNDLE HighlightPickedActor.cxx )
  target_link_libraries(HighlightPickedActor PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(HighlightPickedActor MACOSX_BUNDLE HighlightPickedActor.cxx )
  target_link_libraries(HighlightPickedActor PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS HighlightPickedActor
    MODULES ${VTK_LIBRARIES}
    )
endif () 
