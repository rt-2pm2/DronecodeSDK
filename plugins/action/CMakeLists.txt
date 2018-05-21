add_library(dronecore_action ${PLUGIN_LIBRARY_TYPE}
    action.cpp
    action_impl.cpp
)

target_link_libraries(dronecore_action
    dronecore
)

install(FILES
    action.h
    action_result.h
    DESTINATION ${dronecore_install_include_dir}
)

install(TARGETS dronecore_action
    DESTINATION ${dronecore_install_lib_dir}
)

list(APPEND UNIT_TEST_SOURCES
    ${CMAKE_SOURCE_DIR}/plugins/action/action_test.cpp
)
set(UNIT_TEST_SOURCES ${UNIT_TEST_SOURCES} PARENT_SCOPE)
