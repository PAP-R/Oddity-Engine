set(BINARY ${PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true base/*.h base/*.cpp)

set(SOURCES Tracer.cpp Tracer.h ${SOURCES})

add_executable(${BINARY}_run main.cpp ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

set_target_properties(${BINARY}_run ${BINARY} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 23)