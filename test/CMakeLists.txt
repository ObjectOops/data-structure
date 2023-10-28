set(test_names
    test_hash
    test_compare
    test_init
)

foreach(test_name ${test_names})
    add_executable(${test_name} ${test_name}.cpp)
    target_link_libraries(${test_name} data-structure)
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES TIMEOUT 2)
endforeach()
