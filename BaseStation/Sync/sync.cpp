#include <stdio.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

/*
    Author: Toby Dunn

    Entry point for the device and base station sync process
    here wireless informtion and encryption keys are exchanged
*/

//Key size for the program to use
#define KEY_SIZE 16

//Holds data for a generated key
struct KeyData_t
{
    unsigned char key[KEY_SIZE];
    unsigned char iv[KEY_SIZE];
};

//Generates a key and IV and stores the information in the
//structure passed as a pointer
int GenerateKey(KeyData_t* data);

//Encodes the given key to base64
int EncodeKey(unsigned char* key,unsigned char* output,int output_length);

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

    printf("%s\n",encoded_key);

    return 0;
}

int GenerateKey(KeyData_t* data)
{
    //Generate a random key
    if(!RAND_bytes(data->key,KEY_SIZE))
    {
        printf("Failed to generate device key\n");
        return -1;
    }

    //Generate a random IV
    if(!RAND_bytes(data->iv,KEY_SIZE))
    {
        printf("Failed to generate IV\n");
        return -1;
    }

    return 1;
}

int EncodeKey(unsigned char* key,unsigned char* encoded,int output_length)
{
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* output = BIO_new(BIO_s_mem());
    BIO_set_close(output,BIO_CLOSE);
    BIO_push(b64,output);

    BIO_write(b64,key,KEY_SIZE);
    BIO_flush(b64);

    char *pBuf;
    long length = BIO_get_mem_data(output,&pBuf);
    
    if(output_length < length)
    {
        return 0;
    }

    memcpy(encoded,pBuf,length-1);
        
    BIO_free_all(b64);

    return 1;
}