#include <iostream>
#include"runApp.hpp"
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    try
    {
        runApp("dataset_300k.txt");
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time: " << diff << " ms" << std::endl;
    return 0;
}
