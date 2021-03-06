function(create_test test)
    declare_PID_Component(
        TEST_APPLICATION
        NAME  ${test}
        DIRECTORY ${test}
    )

    declare_PID_Component_Dependency(
        COMPONENT ${test}
        NATIVE open-phri
    )

    run_PID_Test(
        NAME test-${test}
        COMPONENT ${test})

    set(PY_TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${test}/${test}.py")
    if(RUN_PYTHON_TESTS AND EXISTS ${PY_TEST_FILE})
        add_test(NAME "test-${test}-py" COMMAND ${PYTHON_EXECUTABLE} ${PY_TEST_FILE})
    endif()
endfunction()


create_test(default_controller)
create_test(proxy_generators)
create_test(add_remove_constraints)
create_test(velocity_constraint)
create_test(joint_velocity_constraint)
create_test(power_constraint)
create_test(stop_constraint)
create_test(potential_field_generator)
create_test(interpolators)
