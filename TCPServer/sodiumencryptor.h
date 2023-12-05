#pragma once
#include <sodium.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

class SodiumEncryptor
{
public:
    int init();

    // 生成服务器的密钥对
    void generate_server_keys();

    // 解密客户端发送的对称密钥
    void decrypt_symmetric_key(const unsigned char* encrypted_key, unsigned char* decrypted_key, size_t size_encKey);

    // 创建加密数据包
    vector<unsigned char> encrypt(const unsigned char* symmetric_key, const std::string& message);

    //解密接收到的数据包
    string decrypt(const unsigned char* symmetric_key, const vector<unsigned char>& encrypted_message);

    string toHex(const unsigned char* data, size_t len);

    string toHex(const vector<unsigned char>& data);

    vector<unsigned char> getPublicKey();

    bool packetLenIsValid(size_t size);
private:
    // 服务器的密钥对
    unsigned char server_pk[crypto_box_PUBLICKEYBYTES];
    unsigned char server_sk[crypto_box_SECRETKEYBYTES];
    // 客户端的对称密钥
    //unsigned char symmetric_key[crypto_secretbox_KEYBYTES];
};

