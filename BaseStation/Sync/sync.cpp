#include <iostream>
#include <string>
#include "json.hh"
#include "crypto.hpp"
#include "data.hpp"

using json = nlohmann::json;

/*
    Author: Toby Dunn

    Entry point for the device and base station sync process
    here wireless informtion and encryption keys are exchanged
*/

int main()
{
    DataStorage* storage = DataStorage::getSingleton();
    if(!storage->init())
    {
        printf("Failed to connect to database\n");
        return -1;
    }

    KeyData_t data;
    if(!GenerateKey(&data))
    {
        printf("Failed to generate key\n");
        return 1;
    }

    char encoded_key[25] = {};
    char encoded_iv[25] = {}; 

    if(!EncodeKey(data.key,encoded_key,25))
    {
        printf("Failed to encode key\n");
        return -1;
    }

    json j;
    std::string str((char*)encoded_key);
    j["key"] = str;

    if(!EncodeKey(data.iv,encoded_iv,25))
    {
        printf("Failed to encode iv\n");
        return -1;
    }

    str.assign((char*)encoded_iv);
    j["iv"] = str;


    if(!storage->insertDevice(encoded_key,encoded_iv))
    {
        printf("Failed to insert into database \n");
        return -1;
    }

    j["id"] = storage->getLastInsertID();

    std::cout << j.dump() << std::endl;

    delete storage;

    return 0;
}