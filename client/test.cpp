#include <iostream>
#include "crypto.h"

int main()
{
    std::string key_str = "1234567812345678";
    std::string iv_str = "123456781234567";

    unsigned char* key = (unsigned char*) key_str.c_str();
    unsigned char* iv = (unsigned char*) iv_str.c_str();

    std::string message = "Hello my name is toby and i love doing crypto shit or a living";
    char* pMessage = (char*)message.c_str();

    char* pEncrypted = encrypt(pMessage,key,iv);
    std::cout << "Encrypted: " << pEncrypted << std::endl;

    
    char* pDecrypted = decrypt(pEncrypted,key,iv);
    std::cout << "Decrypted: " << pDecrypted << std::endl;

    std::cout << "Crypto Test status [" << strlen(pMessage) << "/" << strlen(pDecrypted) << "]" << std::endl;

    delete pEncrypted;
    delete pDecrypted;
}