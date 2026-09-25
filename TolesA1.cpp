#include <iostream>
#include <string>
using namespace std;

//Author ChatGPT
bool isDigit(char c) {
    return c >= '0' && c <= '9'; 
}
//Author ChatGPT
bool isAddressChar(char c) {
    return isDigit(c) || c == '.' || c == ':'; 
}

//Author ChatGPT

// Parse one decimal number starting at pos.
// maxDigits limits the number of digits.
// Returns the position immediately after the number.
bool parseNumber(const string& str, size_t& pos, int maxDigits,
                 int maxValue, int& value) {
    size_t start = pos;
    int digits = 0;
    int number = 0;

    if (pos >= str.size() || !isDigit(str[pos]))
        return false;

    // Leading zero is allowed only when the number is exactly zero.
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

    // More digits than allowed.
    if (pos < str.size() && isDigit(str[pos]))
        return false;

    if (number < 1 || number > maxValue)
        return false;

    value = number;
    (void)start; // start is not otherwise needed
    return true;
}

bool extractIPv4(const std::string& str,
                 unsigned long& outAddress,
                 int& outPort) {
    // Failure defaults.
    outAddress = 0;
    outPort = -1;

    for (size_t start = 0; start < str.size(); ++start) {
        if (!isDigit(str[start]))
            continue;

        // A candidate cannot begin in the middle of another
        // address-like sequence.
        if (start > 0 && isAddressChar(str[start - 1]))
            continue;

        size_t pos = start;
        int octet[4];

        // Parse four octets.
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

        // Optional port.
        int port = -1;

        if (pos < str.size() && str[pos] == ':') {
            ++pos;

            // A colon requires a valid port.
            if (!parseNumber(str, pos, 5, 65535, port)) {
                continue;
            }
        }

        // The candidate must end here. This prevents partial matches
        // such as extracting 192.168.1.1 from 192.168.1.1.999.
        if (pos < str.size() && isAddressChar(str[pos]))
            continue;

        // Build the 32-bit IPv4 value manually.
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

void printIPv4(unsigned long address, int port) {
    unsigned long a = (address >> 24) & 255;
    unsigned long b = (address >> 16) & 255;
    unsigned long c = (address >> 8)  & 255;
    unsigned long d = address & 255;

    cout << "Extracted IPv4 address: "
         << a << "." << b << "." << c << "." << d
         << " (decimal value: " << address
         << ", port: ";

    if (port == -1)
        cout << "none";
    else
        cout << port;

    cout << ")\n";
}

int main() {
    string inputText;

    cout << "Enter noisy string of text: ";
    getline(cin, inputText);

    unsigned long outAddress;
    int outPort;

    bool found = extractIPv4(inputText, outAddress, outPort);

    if (found) {
        printIPv4(outAddress, outPort);
    }
    else {
        cout << "No address found.\n";
    }

    return 0;
}
