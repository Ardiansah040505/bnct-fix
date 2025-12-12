cmake_minimum_required(VERSION 3.10)

project(B3a VERSION 1.0.0)

# -------------------------------------------------------------------
# Find Geant4
# -------------------------------------------------------------------
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Visualization drivers" ON)

if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

# -------------------------------------------------------------------
# Include directories
# -------------------------------------------------------------------
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# -------------------------------------------------------------------
# Source files
# -------------------------------------------------------------------
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.hh)

# -------------------------------------------------------------------
# Executable
# -------------------------------------------------------------------
add_executable(exampleB3a exampleB3a.cc ${SOURCES} ${HEADERS})
target_link_libraries(exampleB3a ${Geant4_LIBRARIES})

# -------------------------------------------------------------------
# Install and Macro copying
# -------------------------------------------------------------------
set(MACRO_FILES
    init_vis.mac
    vis.mac
    debug.mac
    run1.mac
    run2.mac
    run_bsa_vis.mac
    run_bsa_novis.mac
)

foreach(_macro ${MACRO_FILES})
    configure_file(${PROJECT_SOURCE_DIR}/${_macro}
                   ${PROJECT_BINARY_DIR}/${_macro}
                   COPYONLY)
endforeach()

install(TARGETS exampleB3a DESTINATION bin)
install(FILES ${MACRO_FILES} DESTINATION share/B3a)

# -------------------------------------------------------------------
# Optional: build target
# -------------------------------------------------------------------
add_custom_target(B3a ALL DEPENDS exampleB3a)

message(STATUS "Project B3a configured. Visualization = ${WITH_GEANT4_UIVIS}")
