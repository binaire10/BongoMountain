find_program(GNUPLOT_COMMAND gnuplot)

define_property(TARGET PROPERTY PLOT_NAME BRIEF_DOCS "plot name" FULL_DOCS "plot name")
define_property(TARGET PROPERTY PLOT_TYPE BRIEF_DOCS "plot type" FULL_DOCS "plot type")

function(generate_plot target)
    set(oneValueArgs OUTPUT FORMAT_AXIS FUNCTION XLABEL YLABEL)
    set(manyValueArgs SOURCES)
    cmake_parse_arguments(PLOT "" "${oneValueArgs}" "${manyValueArgs}" ${ARGN})
    set(ARG "")
    foreach (inTarget ${PLOT_SOURCES})
        if (NOT "${ARG}" STREQUAL "")
            string(APPEND ARG " ,")
        endif ()
        string(APPEND ARG "'$<TARGET_PROPERTY:${inTarget},OUTPUT_NAME>' using ${PLOT_FORMAT_AXIS} with $<TARGET_PROPERTY:${inTarget},PLOT_TYPE> title '$<TARGET_PROPERTY:${inTarget},PLOT_NAME>'")
    endforeach ()

    set(LABEL "")
    if (NOT "${PLOT_XLABEL}" STREQUAL "")
        string(APPEND LABEL "set xlabel '${PLOT_XLABEL}'; ")
    endif ()
    if (NOT "${PLOT_YLABEL}" STREQUAL "")
        string(APPEND LABEL "set ylabel '${PLOT_YLABEL}'; ")
    endif ()

    add_custom_command(
            OUTPUT ${PLOT_OUTPUT}
            COMMAND ${GNUPLOT_COMMAND} -e "${LABEL}" -e "set term png; set output '${PLOT_OUTPUT}'; ${PLOT_FUNCTION}; plot ${ARG}"
            DEPENDS ${PLOT_SOURCES}
            VERBATIM
    )
    add_custom_target(${target} DEPENDS ${PLOT_OUTPUT})
    set_property(TARGET ${target} PROPERTY OUTPUT_NAME "${PLOT_OUTPUT}")
endfunction()