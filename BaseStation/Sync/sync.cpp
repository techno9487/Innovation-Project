#include <iostream>
#include <string>
#include "json.hh"
#include "crypto.hpp"

using json = nlohmann::json;

/*
    Author: Toby Dunn

    Entry point for the device and base station sync process
    here wireless informtion and encryption keys are exchanged
*/
int main()
{
    KeyData_t data;
    if(!GenerateKey(&data))
    {
        printf("Failed to generate key\n");
        return 1;
    }

    unsigned char encoded_key[25] = {};
    if(!EncodeKey(data.key,encoded_key,25))
    {
        printf("Failed to encode key\n");
        return -1;
    }

    json j;
    std::string str((char*)encoded_key);
    j["key"] = str;

    if(!EncodeKey(data.iv,encoded_key,25))
    {
        printf("Failed to encode iv\n");
        return -1;
    }

    str.assign((char*)encoded_key);
    j["iv"] = str;

    std::cout << j.dump() << std::endl;

    return 0;
}