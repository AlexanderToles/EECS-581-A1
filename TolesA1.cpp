#include <iostream>
using namespace std;
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort){
    //on success, outAddress holds 32-bit address. outPort holds port number or -1 if not found
    //on failure, outAddress set to 0, outPort set to -1.
    //only digits (1-9), periods (.) and colons (:) allowed
    //canidates are formated as four octets seperated by periods. (octet.octet.octet.octet) with each octed being 3 digits 1-255
    //with no leading zeros unless the number is literally zero
    //an optional :port may follow the fourth octet, with the port being 1-5 digits with values ranging from 1-65535 with same leading zero rule
    //if colon is present, port must be correct or else entire canidate is rejected
    //canidate must match grammar in full, no partial matches or truncated matches
    
    return 0;
}

int main(){
    string inputText;
    unsigned long outAddress; //32-bit IPv4 address
    int outPort; //port number
    cout << "Enter noisy string of text: ";
    cin >> inputText;
    bool found = extractIPv4(inputText,outAddress,outPort;
    while(!found){
        if(found){
            cout << "IPv4 Found: " << outAddress;
            if(outPort > 0){
                cout << outPort << "\n";
            }
        }
        else{
            cout << "No address found.\n";
        }
    }
    //print extracted IPv4 in this format: Extracted IPv4 address: A.B.C.D (decimal value: N, port: P) where N is the 32-bit decimal value and P is the port number or the literal text none.
    return 0;
}
