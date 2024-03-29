// Winter Thomas and Ricardo Romanach

#include <iostream>
#include <string>
#include "Caesar.h"
#include "DataManipulation.h"

int main(){
    int userInput;
    int numberOfResults = 5;

    std::cout << "\nWould you like to encrypt or decrypt?\n";
    std::cout << "1. Encrypt\n";
    std::cout << "2. Decrypt\n";
    userInput = DataManipulation::getIntegerInput(1, 2);
    
    if (userInput == 1){
        std::string encryptedText = Caesar::encrypt();
        std::cout << "\n" << encryptedText;

        std::cout << "\n\nWould you like to write the text to a file? Y/[N]\n";
        userInput = DataManipulation::getLetterInput();
        if (userInput == 'Y'){
            std::string fileName;
            std::cout << "\nWhat file would you like to output to?\n> ";
            getline(std::cin, fileName);
            DataManipulation::writeStringToFile(fileName, encryptedText);
        }
    }
    else {
        Caesar::solveCaesar(DataManipulation::getUserInput("Please enter a ciphertext:"), numberOfResults);
    }
}