// Ricardo Romanach

#include <iostream>
#include <string>
#include "VigenereSolver.h"

int main(){
    std::string encryptedText = "";
    while (encryptedText.empty())
    {
        std::cout << "\nPlease enter a ciphertext:\n";
        getline(std::cin, encryptedText);
    }

    VigenereSolver vigenereSolver(encryptedText);
    // If the results seem a little off, try writing the above line as:
    // VigenereSolver vigenereSolver(encryptedText, 2);
    // Increasing the second parameter at the end will force the solver to
    // print out more possible solutions. It's likely the correct translation
    // is nearby in the long list. However, remember, the number of printed
    // results will be equal to <second parameter> ^ <the period of the key>.

    vigenereSolver.printResults();
}