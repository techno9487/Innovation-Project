#include "crypto.h"
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <iostream>
#include <openssl/err.h>

int decode_key(char* str,unsigned char* dest)
{
    BIO* bio = BIO_new_mem_buf(str,-1);
    BIO *b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64,bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    int length = BIO_read(bio,dest,strlen(str));

    BIO_free_all(bio);
    return length;
}

char* encode(unsigned char* data,int size)
{
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO* output = BIO_new(BIO_s_mem());
    BIO_push(b64,output);

    BIO_write(b64,data,size);
    BIO_flush(b64);

    char *pBuf;
    long length = BIO_get_mem_data(output,&pBuf);

    //std::cout << strlen(pBuf) << std::endl;

    char* buffer = (char*)malloc(strlen(pBuf)+1);
    memset(buffer,0,strlen(pBuf)+1);
    memcpy(buffer,pBuf,strlen(pBuf));
        
    BIO_free_all(b64);

    return buffer;
}

char* decrypt(char* message,unsigned char* key,unsigned char* iv)
{
    unsigned char* input_buffer = (unsigned char*)malloc(strlen(message));
    int data_size = decode_key(message,input_buffer);

    EVP_CIPHER_CTX context = {};

    if(!EVP_DecryptInit_ex(&context, EVP_aes_128_cbc(),NULL, key, iv))
    {
        std::cout << "Failed to setup decryption" << std::endl;
        return NULL;
    }

    int size = strlen(message)+16;
    unsigned char* buffer = (unsigned char*)malloc(size);
    if(buffer == nullptr)
    {
        std::cout << "Failed to allocate encryption buffer" << std::endl;
        return nullptr;
    }

    memset(buffer,0,size);

    
    int outLength;
    int total = 0;
    if(!EVP_DecryptUpdate(&context, buffer, &outLength, input_buffer, data_size))
    {
        delete buffer;
        delete input_buffer;
        std::cout << "Failed to update encryption context with data" << std::endl;
        return NULL;
    }
    total += outLength;
    delete input_buffer;
    
    if(!EVP_DecryptFinal_ex(&context, buffer+outLength,&outLength))
    {
        std::cout << "Failed to finalize buffer" << std::endl;
        delete buffer;
        return NULL;
    }

    total += outLength;

    char* output_buffer = (char*) malloc(total+1);
    memcpy(output_buffer,buffer,total);
    delete buffer;

    return output_buffer;
}

char* encrypt(char* message,unsigned char* key,unsigned char* iv)
{
    
    EVP_CIPHER_CTX context = {};

    
    if(!EVP_EncryptInit_ex(&context, EVP_aes_128_cbc(),NULL, key, iv))
    {
        std::cout << "Failed to setup encryption" << std::endl;
        return NULL;
    }

    int size = strlen(message)+17+16;

    unsigned char* buffer = (unsigned char*)malloc(size);
    if(buffer == nullptr)
    {
        std::cout << "Failed to allocate encryption buffer" << std::endl;
        return nullptr;
    }

    
    int outLength;
    int total = 0;
    if(!EVP_EncryptUpdate(&context, buffer, &outLength,(const unsigned char*)message, strlen(message)))
    {
        delete buffer;
        //EVP_CIPHER_CTX_free(context);
        std::cout << "Failed to update encryption context with data" << std::endl;
        return NULL;
    }
    total += outLength;
    
    if(!EVP_EncryptFinal_ex(&context, buffer+outLength,&outLength))
    {
        std::cout << "Failed to finalize buffer" << std::endl;
        //EVP_CIPHER_CTX_free(context);
        delete buffer;
        return NULL;
    }

    total += outLength;

    //std::cout << total << std::endl;

    char* encoded = encode(buffer,total);
    delete buffer;

    //EVP_CIPHER_CTX_free(context);
    
    return encoded;
    
}