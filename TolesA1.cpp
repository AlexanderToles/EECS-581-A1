//CHAT GPT MODEL GPT-5.6
//Consulted Friday Sept. 25 2026, Sunday Sept. 27 2026.
//Code blocks preluded by author
//Comments are preluded by comment author.

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

#ifndef testMode 
#define testMode 0
#endif

//Author ChatGPT
bool isDigit(char c) {
    return c >= '0' && c <= '9'; //Toles: check if character is between 0 and 9 ASCII characters.
}
//Author ChatGPT
bool isAddressChar(char c) {
    return isDigit(c) || c == '.' || c == ':'; //Toles: check if character period or colon.
}

//Author ChatGPT

// GPT: Parse one decimal number starting at pos.
// GPT: maxDigits limits the number of digits.
// GPT: Returns the position immediately after the number.
bool parseNumber(const string& str, size_t& pos, int maxDigits,
                 int maxValue, int& value) {
    size_t start = pos;
    int digits = 0;
    int number = 0;

    if (pos >= str.size() || !isDigit(str[pos]))
        return false;

    // GPT: Leading zero is allowed only when the number is exactly zero.
    if (str[pos] == '0') {
        ++pos;

        // A second digit means leading zero.
        if (pos < str.size() && isDigit(str[pos]))
            return false;

        value = 0;
        return true;
    }

    while (pos < str.size() &&
           isDigit(str[pos]) &&
           digits < maxDigits) {
        number = number * 10 + (str[pos] - '0');
        ++pos;
        ++digits;

        if (number > maxValue)
            return false;
    }

    // GPT: More digits than allowed.
    if (pos < str.size() && isDigit(str[pos]))
        return false;

    if (number < 1 || number > maxValue)
        return false;

    value = number;
    (void)start; //GPT: start is not otherwise needed
    return true;
}

bool extractIPv4(const std::string& str,
                 unsigned long& outAddress,
                 int& outPort) {
    // GPT: Failure defaults.
    outAddress = 0;
    outPort = -1;

    for (size_t start = 0; start < str.size(); ++start) {
        if (!isDigit(str[start]))
            continue;

        // GPT: A candidate cannot begin in the middle of another
        // GPT: address-like sequence.
        if (start > 0 && isAddressChar(str[start - 1]))
            continue;

        size_t pos = start;
        int octet[4];

        // GPT: Parse four octets.
        bool valid = true;

        for (int i = 0; i < 4; ++i) {
            if (!parseNumber(str, pos, 3, 255, octet[i])) {
                valid = false;
                break;
            }

            if (i < 3) {
                if (pos >= str.size() || str[pos] != '.') {
                    valid = false;
                    break;
                }
                ++pos;
            }
        }

        if (!valid)
            continue;

        // GPT: Optional port.
        int port = -1;

        if (pos < str.size() && str[pos] == ':') {
            ++pos;

            // GPT: A colon requires a valid port.
            if (!parseNumber(str, pos, 5, 65535, port)) {
                continue;
            }
        }

        // GPT: The candidate must end here. This prevents partial matches
        // GPT: such as extracting 192.168.1.1 from 192.168.1.1.999.
        if (pos < str.size() && isAddressChar(str[pos]))
            continue;

        // GPT: Build the 32-bit IPv4 value manually.
        unsigned long address = 0;
        address = address * 256 + octet[0];
        address = address * 256 + octet[1];
        address = address * 256 + octet[2];
        address = address * 256 + octet[3];

        outAddress = address;
        outPort = port;
        return true;
    }

    return false;
}

//Author ChatGPT
string printIPv4(unsigned long address, int port) {
    unsigned long a = (address >> 24) & 255;
    unsigned long b = (address >> 16) & 255;
    unsigned long c = (address >> 8)  & 255;
    unsigned long d = address & 255;

    stringstream output;

    output << "Extracted IPv4 address: "
         << a << "." << b << "." << c << "." << d
         << " (decimal value: " << address
         << ", port: ";

    if (port == -1)
        output << "none";
    else
        output << port;

    output << ")\n";
    return output.str();
}

//Original author: Toles
//Modified by GPT
int main() {
    string inputText;
    unsigned long outAddress;
    int outPort;
    bool found = 0;

    #if testMode
    ofstream outFile("output.txt");
    ifstream inFile("testcases.txt");
    while (getline (inFile, inputText)){
        if(inputText == "END"){
            outFile << "Program terminated.";
            outFile.close();
            return 0;
        }
        found = extractIPv4(inputText, outAddress, outPort);
    
        if (found) {
            outFile << printIPv4(outAddress, outPort); //GPT modification
        }
        else {
            outFile << "Invalid input: no valid IPv4 address found\n";
        }   
    }
    outFile.close();
    
    #else
        while(!found){
                cout << "Enter a string (or 'END' to quit): ";
                getline(cin, inputText); //GPT modification
                if(inputText == "END"){
                    cout << "Program terminated.\n";
                    return 0;
                }
                found = extractIPv4(inputText, outAddress, outPort);
            
                if (found) {
                    cout << printIPv4(outAddress, outPort); //GPT modification
                }
                else {
                    cout << "Invalid input: no valid IPv4 address found\n";
                }   
            }
        
    #endif
    
    return 0;
}
