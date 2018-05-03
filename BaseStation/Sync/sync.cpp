#include <stdio.h>
#include <openssl/rand.h>

/*
    Author: Toby Dunn

    Entry point for the device and base station sync process
    here wireless informtion and encryption keys are exchanged
*/

//Holds data for a generated key
struct KeyData_t
{
    unsigned char key[16];
    unsigned char iv[16];
};

//Generates a key and IV and stores the information in the
//structure passed as a pointer
int GenerateKey(KeyData_t* data);

int main()
{
    KeyData_t data;
    if(!GenerateKey(&data))
    {
        printf("Failed to generate key\n");
        return 1;
    }
    

    return 0;
}

int GenerateKey(KeyData_t* data)
{
    //Generate a random key
    if(!RAND_bytes(data->key,16))
    {
        printf("Failed to generate device key\n");
        return -1;
    }

    //Generate a random IV
    if(!RAND_bytes(data->iv,16))
    {
        printf("Failed to generate IV\n");
        return -1;
    }

    return 1;
}