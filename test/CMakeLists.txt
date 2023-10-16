add_executable(test_hash test_hash.cpp)
add_executable(test_compare test_compare.cpp)

target_link_libraries(test_hash data-structure)
target_link_libraries(test_compare data-structure)

add_test(NAME test_hash COMMAND test_hash)
add_test(NAME test_compare COMMAND test_compare)

set_tests_properties(test_hash PROPERTIES TIMEOUT 2)
set_tests_properties(test_compare PROPERTIES TIMEOUT 2)
