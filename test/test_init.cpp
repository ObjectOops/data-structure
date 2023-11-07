#include <cassert>
#include <string>
#include <iostream>

int main() {

    std::string compile_test_names_fail [] {
        "fail_1.cpp",    // Fail when class template does not match argument template.
        "fail_2.cpp",    // Same.
        "fail_3.cpp",    // Fail when class / argument template does not have an appropriate hash method.
        "fail_4.cpp"     // Weird map initialization argument types must match the structure's.
    };
    std::string compile_test_names_success [] {
        "success_1.cpp", // Normal.
        "success_2.cpp", // Second constructor.
        "success_3.cpp", // Custom hash and compare fuctions.
        "success_4.cpp", // Same.
        "success_5.cpp"  // Weird map initialization.
    };

    std::string dir {"init/"};

    for (std::string i : compile_test_names_fail) {
        std::string cmd {"cd ../../test && g++ -std=c++11 " + dir + i};
        int ret {system(cmd.c_str())};
        std::cout << cmd << std::endl;
        assert(WEXITSTATUS(ret) != 0);
    }

    for (std::string i : compile_test_names_success) {
        std::string cmd {"cd ../../test && g++ -std=c++11 " + dir + i};
        int ret {system(cmd.c_str())};
        std::cout << cmd << std::endl;
        assert(WEXITSTATUS(ret) == 0);
    }

    return 0;
}
