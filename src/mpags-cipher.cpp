#include "ProcessCommandLine.hpp"
#include "TransformChar.hpp"
#include "runCaeserCipher.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    // Convert the command-line arguments into a more easily usable form
    const std::vector<std::string> cmdLineArgs{argv, argv + argc};

    // Options that might be set by the command-line arguments
    bool helpRequested{false};
    bool versionRequested{false};
    std::string inputFile{""};
    std::string outputFile{""};
    bool encrypt{false};
    std::string cipherkey;

    // Process command line arguments
    const bool cmdLineStatus{
        processCommandLine(cmdLineArgs, helpRequested, versionRequested,
                           inputFile, outputFile, encrypt, cipherkey)};

    // Any failure in the argument processing means we can't continue
    // Use a non-zero return value to indicate failure
    if (!cmdLineStatus) {
        return 1;
    }

    // Handle help, if requested
    if (helpRequested) {
        // Line splitting for readability
        std::cout
            << "Usage: mpags-cipher [-h/--help] [--version] [-i <file>] [-o <file>]\n\n"
            << "Encrypts/Decrypts input alphanumeric text using classical ciphers\n\n"
            << "Available options:\n\n"
            << "  -h|--help        Print this help message and exit\n\n"
            << "  --version        Print version information\n\n"
            << "  -i FILE          Read text to be processed from FILE\n"
            << "                   Stdin will be used if not supplied\n\n"
            << "  -o FILE          Write processed text to FILE\n"
            << "                   Stdout will be used if not supplied\n\n"
            << " --encrypt         Operate in encryption mode \n\n"
            << " --decrypt         Operate in decryption mode \n\n"
            << " --key             The key to be used for encrption or decrpytion \n\n"
            << std::endl;
        // Help requires no further action, so return from main
        // with 0 used to indicate success
        return 0;
    }

    // Handle version, if requested
    // Like help, requires no further action,
    // so return from main with zero to indicate success
    if (versionRequested) {
        std::cout << "0.1.0" << std::endl;
        return 0;
    }

    // Initialise variables
    char inputChar{'x'};
    std::string inputText;
    std::string inputTextRaw;

    // Read in user input from stdin/file
    if (inputFile.empty()) {
        // process commandline input
        std::cin >> inputTextRaw;
        for (const char c : inputTextRaw) {
            inputText += transformChar(c);
        }
    } else {
        // process text file input
        std::ifstream in_file{inputFile};
        bool ok_to_read = in_file.good();

        if (ok_to_read == false) {
            std::cerr << "[error] failed to read'" << "'\n";
            return 1;
        } else {
            in_file >> inputChar;
            while (std::cin >> inputChar) {
                inputText += transformChar(inputChar);
            }
        }
    }
    // Convert key and apply the Caesar cipher
    size_t key;
    std::stringstream stream{cipherkey};
    stream >> key;
    inputText = runCaeserCipher(inputText, key, encrypt);

    // Output section
    if (outputFile.empty()) {
        // output to commandline
        std::cout << inputText << std::endl;
    } else {
        // output to file
        std::ofstream out_file{outputFile};
        bool ok_to_write = out_file.good();
        if (ok_to_write == false) {
            std::cerr << "[error] failed to write'" << "'\n";
            return 1;
        } else {
            out_file << inputText;
        }
    }

    // No requirement to return from main, but we do so for clarity
    // and for consistency with other functions
    return 0;
}
