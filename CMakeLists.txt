cmake_minimum_required(VERSION 3.15)
project(hex CXX)

set(CMAKE_CXX_STANDARD 17)
if (NOT ANDROID)
    find_package(Vulkan REQUIRED)
endif()
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(tinyobjloader INTERFACE)
target_include_directories(tinyobjloader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyobjloader)

find_program(GLSLC_EXECUTABLE glslc HINTS "$ENV{VULKAN_SDK}/bin")

if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found! Make sure Vulkan SDK is installed and VULKAN_SDK env var is set.")
endif()

file(GLOB SHADER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.comp"
)

set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPIRV ${SHADER_OUTPUT_DIR}/${FILE_NAME}.spv)

    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIR}
        COMMAND ${GLSLC_EXECUTABLE} ${SHADER} -o ${SPIRV}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME}"
        VERBATIM
    )

    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach()

add_custom_target(Shaders ALL DEPENDS ${SPIRV_BINARY_FILES})

file(GLOB MODELS
    "${CMAKE_CURRENT_SOURCE_DIR}/models/*.obj"
)

set(MODELS_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/models)

foreach(MODEL ${MODELS})
    get_filename_component(FILE_NAME ${MODEL} NAME)
    set(MODEL_OUTPUT ${MODELS_OUT_DIR}/${FILE_NAME})

    add_custom_command(
        OUTPUT ${MODEL_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${MODELS_OUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${MODEL} ${MODEL_OUTPUT}
        DEPENDS ${MODEL}
        COMMENT "Coping model ${FILE_NAME}"
        VERBATIM
    )

    list(APPEND MODELS_DIR ${MODEL_OUTPUT})
endforeach()

add_custom_target(Models ALL DEPENDS ${MODELS_DIR})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm tinyobjloader)
add_dependencies(${PROJECT_NAME} Shaders Models)

if (NOT ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE vulkan) # system lib from NDK
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
