#include "runCaeserCipher.hpp"

#include <iostream>
#include <string>
#include <vector>

std::string runCaeserCipher(const std::string& inputText, const size_t key,
                            const bool encrypt)
{    // Create the alphabet container and output string
    const std::string alphabet{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    std::string outputText;
    size_t new_pos;
    // Loop over the input text
    for (const char c : inputText) {
        // Find the position of the character in the alphabet
        size_t pos = alphabet.find(c);
        // Apply the shift, handling wrap-around using modulo arithmetic
        if (encrypt) {
            new_pos = (pos + key) % alphabet.size();
        } else {
            new_pos = (pos - key) % alphabet.size();
        }
        // Add the new character to the output string
        outputText += alphabet[new_pos];
    }
    // Finally (after the loop), return
    return outputText;
}