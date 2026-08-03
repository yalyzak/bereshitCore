#include <iostream>
#include <chrono>


// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
namespace Flages {
    void Main();
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    Flages::Main();
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::chrono::duration<double>(elapsed).count() << " s\n";
    return 0;
}