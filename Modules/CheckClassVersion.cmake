string(REPLACE "/" "+" _ccv_target "${EXECUTABLE_OUTPUT_PATH}/checkClassVersion")


MACRO(check_class_version)
  CMAKE_PARSE_ARGUMENTS(ART_CCV
    "LIBRARIES"
    "UPDATE_IN_PLACE"
    ${ARGN}
    )
  IF(ART_CCV_LIBRARIES)
    MESSAGE(FATAL_ERROR "LIBRARIES option not supported at this time: "
      "ensure your library is linked to any necessary libraries not already pulled in by ART.")
  ENDIF()
  IF(ART_CCV_UPDATE_IN_PLACE)
    SET(ART_CCV_EXTRA_ARGS ${ART_CCV_EXTRA_ARGS} "-G")
  ENDIF()
  IF(NOT dictname)
    MESSAGE(FATAL_ERROR "CHECK_CLASS_VERSION must be called after BUILD_DICTIONARY.")
  ENDIF()
  IF(ROOT_python_FOUND)
    # Add the check to the end of the dictionary building step.
    add_custom_command(TARGET ${dictname}_dict POST_BUILD
      COMMAND ${PROJECT_SOURCE_DIR}/tools/checkClassVersion ${ART_CCV_EXTRA_ARGS}
      -l$<TARGET_FILE:${dictname}_dict>
      -x ${CMAKE_CURRENT_SOURCE_DIR}/classes_def.xml
      VERBATIM
      )
    if (NOT ART_FRAMEWORK_CORE)
      # If we're in art, we need to be sure that CheckClassVersion and
      # art_Framework_Core are already built; if we're outside art, this
      # is a given.
      add_dependencies(${dictname}_dict ${_ccv_target} art_Framework_Core)
    endif()
  ENDIF()
ENDMACRO()
