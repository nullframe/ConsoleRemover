#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

uintptr_t GetPatternAddress(std::vector<unsigned char> fileBytes, unsigned char pattern[], int patternSize) {

    for (int i = 0; i < fileBytes.size(); i++) {
        bool found = true;

        // If no pattern then not found
        for (int k = 0; k < patternSize; k++) {

            if (pattern[k] == 0xAE) continue;
            if (fileBytes[i + k] != pattern[k]) {
                found = false;
                break;
            }
        }

        // Return address if pattern found
        if (found)
            return i;
    }

    return 0;
}

int main(int argc, char* argv[]) {

    // Opening the file in binary mode
    std::ifstream file(argv[1], std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << "kaban.exe" << std::endl;
        return 1;
    }

    std::vector<unsigned char> fileBytes;

    // Storing file bytes to vector
    unsigned char byte;
    while (file.read((char*)&byte, 1)) {

        fileBytes.push_back(byte);
    }

    file.close();


    // Getting PE signature address
    unsigned char pattern[] = { 0x50, 0x45, 0x00, 0x00 };

    uintptr_t peSignatureAddress = GetPatternAddress(fileBytes, pattern, 4);

    // Changing subsystem field value to GUI
    fileBytes[peSignatureAddress + 0x5c] = 2;


    // Writing back to file
    std::ofstream fileStream(argv[1], std::ios::binary);

    if (!fileStream.good())
        return 1;

    fileStream.write(reinterpret_cast<const char*>(&fileBytes[0]), fileBytes.size());

    fileStream.close();

    return 0;
}