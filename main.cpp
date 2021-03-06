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
    int DELKA = 10;
    string prefix = string("JanOupicky");
    if(argc >= 2){
        DELKA = atoi(argv[1]);
    }
    if(argc >= 3){
        prefix = string(argv[2]);
    }

    cout << "length of collision " << DELKA << " with prefix " << prefix << endl;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    string myString = string(prefix + to_string(0));
    unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256 sha;
    sha.CalculateDigest(digest, (const unsigned char*)myString.c_str(), myString.length());
    CryptoPP::HexEncoder encoder;

    string output;

    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();

    if(DELKA < 17) { //use long long
        unsigned int counterInt = 0;
        cout << "using long long" << endl;
        unordered_map<unsigned long long, unsigned int> seznam;
        unsigned long long temp = strtoll(output.substr(0, DELKA).c_str(), NULL, 16);
        while(seznam.count(temp) == 0){
            pair<unsigned long long, unsigned int> par (temp, counterInt);
            seznam.insert(par);
            counterInt++;
            output.clear();
            myString = string(prefix + to_string(counterInt));
            sha.CalculateDigest(digest, (const unsigned char*)myString.c_str(), myString.length());
            encoder.Put( digest, sizeof(digest) );
            encoder.MessageEnd();
            temp = strtoll(output.substr(0,DELKA).c_str(), NULL, 16);
            if(counterInt % 1000000 == 0){
                cout << counterInt << endl;
            }
        }
        cout << counterInt << " " << seznam[temp] << "; map size: " << seznam.size() << endl;
        seznam.clear();
    }else{ //long long too small use string
        unsigned long counterLong = 0;
        cout << "using string" << endl;
        unordered_map<string, unsigned long> seznam;
        while(seznam.count(output.substr(0,DELKA)) == 0){
            pair<string, unsigned long> par (output.substr(0,DELKA), counterLong);
            seznam.insert(par);
            counterLong++;
            output.clear();
            myString = string(prefix + to_string(counterLong));
            sha.CalculateDigest(digest, (const unsigned char*)myString.c_str(), myString.length());
            encoder.Put( digest, sizeof(digest) );
            encoder.MessageEnd();
            if(counterLong % 1000000 == 0){
                cout << counterLong << endl;
            }
        }
        cout << counterLong << " " << seznam[output.substr(0,DELKA)] << "; map size: " << seznam.size() << endl;
        seznam.clear();
    }


    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();

    cout << duration << "ms" << endl;

    return 0;
}