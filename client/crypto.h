#pragma once

//decodes key from Base64
int decode_key(char* str,unsigned char* dest);

//decrypts a message
char* decrypt(char* message,unsigned char* key,unsigned char* iv);
char* encrypt(char* message,unsigned char* key,unsigned char* iv);