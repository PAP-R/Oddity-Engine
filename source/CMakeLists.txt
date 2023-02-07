set(BINARY ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

set_target_properties(${BINARY}_run ${BINARY}_lib PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)