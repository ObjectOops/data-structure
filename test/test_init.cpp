#include <cassert>
#include <string>
#include <iostream>

using namespace std;

int main() {

    string compile_test_names_fail [] {
        "fail_1.cpp",    // Fail when class template does not match argument template.
        "fail_2.cpp",    // Same.
        "fail_3.cpp"     // Fail when class / argument template does not have an appropriate hash method.
    };
    string compile_test_names_success [] {
        "success_1.cpp", // Normal.
        "success_2.cpp", // Second constructor.
        "success_3.cpp", // Custom hash and compare fuctions.
        "success_4.cpp"  // Same.
    };

    string dir {"init/"};

    for (string i : compile_test_names_fail) {
        string cmd {"cd ../../test && g++ -std=c++11 " + dir + i};
        int ret {system(cmd.c_str())};
        cout << cmd << endl;
        assert(WEXITSTATUS(ret) != 0);
    }

    for (string i : compile_test_names_success) {
        string cmd {"cd ../../test && g++ -std=c++11 " + dir + i};
        int ret {system(cmd.c_str())};
        cout << cmd << endl;
        assert(WEXITSTATUS(ret) == 0);
    }

    return 0;
}
