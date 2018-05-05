#include <iostream>
#include <unordered_map>
#include <chrono>
#include "cryptopp/cryptlib.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"


using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    unsigned int DELKA = 10;
    if(argc == 2){
        DELKA = atoi(argv[1]);
    }
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    unordered_map<string, unsigned long> seznam;
    unsigned long counter = 0;
    string prefix = string("JanOupicky");
    string myString = string(prefix + to_string(counter));
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256 sha;
    sha.CalculateDigest(digest, (const unsigned char*)myString.c_str(), myString.length());
    CryptoPP::HexEncoder encoder;

    string output;

    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();

    while(seznam.count(output.substr(0,DELKA)) == 0){
        pair<string, unsigned long> par (output.substr(0,DELKA), counter);
        seznam.insert(par);
        counter++;
        output.clear();
        myString = string(prefix + to_string(counter));
        sha.CalculateDigest(digest, (const unsigned char*)myString.c_str(), myString.length());
        encoder.Put( digest, sizeof(digest) );
        encoder.MessageEnd();
        if(counter % 1000000 == 0){
            cout << counter << endl;
        }
    }

    cout << DELKA << endl;

    cout << counter << " " << seznam[output.substr(0,DELKA)] << endl;

    seznam.clear();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();

    cout << duration << "ms" << endl;

    return 0;
}
