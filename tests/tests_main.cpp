// Copyright (c) 2016, Charles Lechasseur
// Distributed under the MIT license (see LICENSE).

#include <cl/optional/all_tests.h>
#include <coveo/test_framework.h>

#include <iostream>
#include <string>

// Test program entry point.
int main()
{
    std::cout << "Running tests..." << std::endl;
    int ret = coveo_tests::run_tests(&cl_tests::optional::all_tests);
    std::cout << "Done." << std::endl;

    if (ret != 0) {
        std::cout << std::endl << "Press enter to continue ";
        std::string unused;
        std::getline(std::cin, unused);
    }
    
    return ret;
}
