#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(betaG4)
set(useROOT       true            )  # use true or false (or comment to set to false)
set(useInSANE     true            )  # use true or false (or comment to set to false)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build BETAG4 with vis" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT QUIET)
if(NOT ROOT_FOUND)
   message(STATUS "G4 Examples: ROOT package not found. --> AnaEx02 example disabled")  
   return()
endif()

include_directories(${PROJECT_SOURCE_DIR}/include 
   ${Geant4_INCLUDE_DIR}
   ${GSL_INCLUDE_DIR}
   ${ROOT_INCLUDE_DIR})

#-Wno-unused-variable 
#-Wdeprecated-declarations
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -Wno-unused-parameter ${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
#---------------------------------------------------------------------------
# ROOT compile flags
#if(useROOT)
#   EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
#endif(useROOT) 

#---------------------------------------------------------------------------
# InSANE compile flags
if(useInSANE)
   EXECUTE_PROCESS(COMMAND insane-config --cflags --inc OUTPUT_VARIABLE InSANE_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${InSANE_CXX_FLAGS}")
endif(useInSANE)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(BETA BETA.cc ${sources} ${headers})
#add_executable(${myexe} ${mymain} ${sources} ${headers})

#---------------------------------------------------------------------------
# ROOT linking flags 
if(useROOT)
   EXECUTE_PROCESS(COMMAND root-config --evelibs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ROOT_LD_FLAGS}")
   #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -lGeomPainter -lGeom -lTreeViewer -lSpectrum -lSpectrumPainter -lFoam")
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${ROOT_LD_FLAGS} -lGeomPainter -lGeom -lTreeViewer -lSpectrum -lSpectrumPainter -lFoam -lEG -lEve")
   set(ROOT_LIBRARIES "${ROOT_LD_FLAGS} -lGeomPainter -lGeom -lTreeViewer -lSpectrum -lSpectrumPainter -lFoam")
endif(useROOT)

#---------------------------------------------------------------------------
# InSANE linking flags 
if(useInSANE)
   EXECUTE_PROCESS(COMMAND insane-config --libs --ldflags OUTPUT_VARIABLE InSANE_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${InSANE_LD_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${InSANE_LD_FLAGS}")

   EXECUTE_PROCESS(COMMAND lhapdf-config --libs --ldflags OUTPUT_VARIABLE LHAPDF_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LHAPDF_LD_FLAGS}")
   #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LHAPDF_LD_FLAGS}")
   set(LHAPDF_LIBRARIES "${LHAPDF_LD_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LHAPDF_LD_FLAGS}")

   EXECUTE_PROCESS(COMMAND "gsl-config --libs --ldflags" OUTPUT_VARIABLE GSL_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
   set(GSL_LIBRARIES "${GSL_LD_FLAGS}")
   #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GSL_LD_FLAGS}")
   #add_library( InSANE SHARED IMPORTED )
   #set_target_properties(InSANE PROPERTIES IMPORTED_LOCATION "/home/whit/sane08/InSANE/lib/libInSANE.so" )
   set(INSANE_LIBRARIES "-L/home/whit/lib -lInSANE")
endif(useInSANE)

message("ROOT_LIBRARIES : ${ROOT_LIBRARIES}")
message("INSANE_LIBRARIES : ${INSANE_LIBRARIES}")
message("LHAPDF_LIBRARIES : ${LHAPDF_LIBRARIES}")

# note the library dependency order matters here. The library with the most dependencies goes first.
target_link_libraries(BETA ${Geant4_LIBRARIES}  ${INSANE_LIBRARIES} ${ROOT_LIBRARIES}  ${LHAPDF_LIBRARIES})
#target_link_libraries(BETA ${Geant4_LIBRARIES} ${LHAPDF_LIBRARIES} ${GSL_LIBRARIES} ${ROOT_LIBRARIES}  ${INSANE_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(BETAG4_SCRIPTS
   BETA.in
   BETA.out
#  init.mac
#  init_vis.mac
#  run1.mac
#  run2.mac
#  vis.mac
  )

foreach(_script ${BETAG4_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(betaG4 DEPENDS BETA)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS BETA DESTINATION bin)


