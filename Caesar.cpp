// Winter Thomas and Ricardo Romanach
// Caeser Cipher Statistical Attack

#include <iostream>
#include <string>
#include "Caesar.h"
#include "DataManipulation.h"

/////////////////////////
// Main User Functions //
/////////////////////////

// Does all steps for decrypting a caesar cypher automatically
// numberOfResults can be 1 to 26
void Caesar::solveCaesar(std::string encryptedText, int numberOfResults){
    // Remove all non letters from the encrypted text
    std::string cleanEncryptedText = cleanText(encryptedText);
    // Get the correlation Frequencies for the clean text
    double* correlationFrequencies = getCorrelationOfFrequencies(cleanEncryptedText);
    // Get the indexs of the most likely results
    int* topShifts = getTopShifts(correlationFrequencies, numberOfResults);
    // print the solution(s)
    printResults(topShifts, correlationFrequencies, numberOfResults, encryptedText);

    delete[] correlationFrequencies;
    delete[] topShifts;
}

// Returns unshifted string using the top result from the statistical attack
std::string Caesar::getMostLikelyDecryption(std::string encryptedText){
    // Remove all non letters from the encrypted text
    std::string cleanEncryptedText = cleanText(encryptedText);
    // Get the correlation Frequencies for the clean text
    double* correlationFrequencies = getCorrelationOfFrequencies(cleanEncryptedText);
    // Get the indexs of the most likely results
    int* topShifts = getTopShifts(correlationFrequencies, 1);
    // Get the solution
    std::string result = decrypt(encryptedText, topShifts[0]);

    delete[] correlationFrequencies;
    delete[] topShifts;

    return result;
}

// When provided an text and a shift, it unshifts the text by that amount
// Primarily used for decrypting
std::string Caesar::decrypt(std::string ciphertext, int shift){
    int storeForShift;
    std::string result = "";

    for (char i : ciphertext){
        // If there is a non letter, no shift is needed
        if (!isalpha(i)){
            result += i;
            continue;
        }

        // Shift letter
        storeForShift = convertLetterToNumber(toupper(i));
        storeForShift -= shift;

        // Account for shift overflowing
        if (storeForShift < 0){
            storeForShift += LANGUAGE_LETTER_COUNT;
        }

        // If letter is capital
        if (isupper(i)){
            result += convertNumberToLetter(storeForShift);
        }
        // If letter is lowercase
        else {
            result += tolower(convertNumberToLetter(storeForShift));
        }
    }

    return result;
}

// Prompts for a text and a shift amount and then shifts the text by that amount
std::string Caesar::encrypt(){
    std::string text;
    int key;

    text = DataManipulation::getUserInput("Please enter the text to encrypt:");

    std::cout << "\nHow much would you like to shift the text?\n";

    key = DataManipulation::getIntegerInput(0, 25);

    return encrypt(text, key);
}

// When provided a text and a shift, it shifts the text by that amount
// Primarily used for encrypting
std::string Caesar::encrypt(std::string text, int shift){
    int storeForShift;
    std::string result = "";

    for (char i : text){
        // If there is a non letter, no shift is needed
        if (!isalpha(i)){
            result += i;
            continue;
        }

        // Shift letter
        storeForShift = convertLetterToNumber(toupper(i));
        storeForShift += shift;
        
        // Account for shift overflowing
        if (storeForShift > 25){
            storeForShift -= LANGUAGE_LETTER_COUNT;
        }
        
        // If letter is capital
        if (isupper(i)){
            result += convertNumberToLetter(storeForShift);
        }
        // If letter is lowercase
        else {
            result += tolower(convertNumberToLetter(storeForShift));
        }
    }
    return result;
}

////////////////////
// Core Functions //
////////////////////

// Returns an array of frequencies. For each possible Caesar shift, this will calculate how
// closely the letter frequencies match English letter frequencies. (e.g. Index 0 will hold
// how closely the frequencies match for a shift of 0.)
double* Caesar::getCorrelationOfFrequencies(std::string text){
    int* letterFrequencies = getFrequencyOfLetters(text);
    // Gets number of letters in text
    int numberOfLetters = text.length();
    double total;
    double* correlationFrequencies = new double [LANGUAGE_LETTER_COUNT];

    // For each possible shift
    for (int i = 0; i < LANGUAGE_LETTER_COUNT; i++){
        total = 0;
        for (int j = 0; j < LANGUAGE_LETTER_COUNT; j++){
            // Calculates the correlation of a letter in the cipher text with its frequency of use in the English language
            total += ((double) letterFrequencies[j] / numberOfLetters) * ENGLISH_ALPHABET_FREQUENCIES[((LANGUAGE_LETTER_COUNT + j - i) % LANGUAGE_LETTER_COUNT)];
        }
        correlationFrequencies[i] = total;
    }

    delete[] letterFrequencies;

    return correlationFrequencies;
}

// Returns an array of INDICES. The indices represent the positions of the highest values
// in the input array. The amount of top results to be returned is stored in topAmount.
// topAmount can be 1 to 26
int* Caesar::getTopShifts(double* frequencies, int topAmount){
    // Make and initialize an array for the top values
    int* topResults = new int[topAmount];
    for (int i = 0; i < topAmount; i++){
        topResults[i] = -1;
    }

    double max;
    int maxIndex;
    bool isInTopList;

    // For each value to be in top list
    for (int i = 0; i < topAmount; i ++){
        max = 0.0;

        // Check if the value is the largest value in the array
        for (int j = 0; j < LANGUAGE_LETTER_COUNT; j++){
            // If its the largest value
            if(frequencies[j] > max){
                // Check if its already in the top list
                isInTopList = false;
                for (int k = 0; k < topAmount; k++){
                    if (j == topResults[k]){
                        isInTopList = true;
                    }
                }
                // If its not, set it to the new largest
                if (!isInTopList){
                    max = frequencies[j];
                    maxIndex = j;
                }        
            }
        }
        // Store the current largest value in the top results array
        topResults[i] = maxIndex;
    }

    return topResults;
}

// Prints the top most likely shift values, the frequency it matched
// with English letter frequency using that shift, and the plaintext
// translation for each shift.
void Caesar::printResults(int* topShifts, double* correlationFrequencies, int numberOfResults, std::string encryptedText){
    std::cout << "\nTop " << numberOfResults << " Shifts:\n";
    for (int i = 0; i < numberOfResults; i++)
    {
        std::cout << "Shift = " << topShifts[i] << "\tFrequency = " << correlationFrequencies[topShifts[i]] << "\n";
        // For each letter in the encrypted text, shift and print
        std::cout << decrypt(encryptedText, topShifts[i]);
        std::cout << "\n\n";
    }
}

//////////////////////
// Helper Functions //
//////////////////////

// Removes all non letter characters and capitalizes lowercase letters
std::string Caesar::cleanText(std::string rawEncryptedText){
    std::string cleanText = "";

    for (char i : rawEncryptedText){
        // Capitalize the lower letters
        if (islower(i)){
            cleanText += toupper(i);
        }
        // Include capital letters
        else if (isupper(i)){
            cleanText += i;
        }
    }

    return cleanText;
}

// Returns an array with the frequency of each letter in a given string
int* Caesar::getFrequencyOfLetters(std::string text){
    // Create and initialize the return array
    int* letterFrequency = new int[LANGUAGE_LETTER_COUNT];
    for (int i = 0; i < LANGUAGE_LETTER_COUNT; i++){
        letterFrequency[i] = 0;
    }

    // Count the amount of each letter
    for (char i : text){
        letterFrequency[convertLetterToNumber(toupper(i))]++;
    }

    return letterFrequency;
}

// Transforms a letter into its number in the alphabet 0 - 25
int Caesar::convertLetterToNumber(char letter){
    return letter - 65;
}

// Transforms a number 0 - 25 into its letter in the alphabet
char Caesar::convertNumberToLetter(int number){
    return number + 65;
}