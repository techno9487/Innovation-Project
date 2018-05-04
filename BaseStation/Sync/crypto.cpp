#include "crypto.hpp"
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>

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