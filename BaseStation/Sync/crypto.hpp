/*
    Author: Toby Dunn
*/


#pragma once

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
