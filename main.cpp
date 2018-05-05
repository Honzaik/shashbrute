#include <iostream>
#include <unordered_map>
#include <cstdlib>
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
    unordered_map<long long, unsigned long> seznam;
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

    long long temp = strtoll(output.substr(0,DELKA).c_str(), NULL, 16);


    while(seznam.count(temp) == 0){
        pair<long long, unsigned long> par (temp, counter);
        seznam.insert(par);
        counter++;
        output.clear();
        myString = string(prefix + to_string(counter));
        sha.CalculateDigest(digest, (const unsigned char*)myString.c_str(), myString.length());
        encoder.Put( digest, sizeof(digest) );
        encoder.MessageEnd();
        temp = strtoll(output.substr(0,DELKA).c_str(), NULL, 16);
        if(counter % 1000000 == 0){
            cout << counter << endl;
        }
    }

    cout << counter << " " << seznam[temp] << endl;

    seznam.clear();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();

    cout << duration << "ms" << endl;

    return 0;
}