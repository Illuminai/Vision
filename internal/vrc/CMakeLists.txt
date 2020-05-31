project(vrc)

include_directories(include)
add_executable(${PROJECT_NAME} src/vrc.cpp include/vrc.h)



function(vrc_embed_spirv vrc_output)
    set(vrc_result)
    foreach (input_file ${ARGN})
        file(RELATIVE_PATH input_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${input_file})

        set(output_spv "${PROJECT_BINARY_DIR}/${input_file}.spv")
        get_filename_component(output_file_directory "${output_spv}" DIRECTORY)
        add_custom_command(OUTPUT ${output_spv}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${output_file_directory}"
                COMMAND glslc ${input_path} -o ${output_spv}
                DEPENDS ${input_file}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Compiling ${input_file} to SPIR-V"
                VERBATIM)

        set(output_c "${PROJECT_BINARY_DIR}/${input_file}.c")
        add_custom_command(OUTPUT ${output_c}
                COMMAND vrc ${output_spv} ${output_c}
                DEPENDS ${output_spv}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Embedding ${input_file} in C source file"
                VERBATIM)
        list(APPEND vrc_result "${output_c}")
    endforeach ()
    set(${vrc_output} "${vrc_result}" PARENT_SCOPE)
endfunction()