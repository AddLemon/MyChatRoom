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

    // ���ɷ���������Կ��
    void generate_server_keys();

    // ���ܿͻ��˷��͵ĶԳ���Կ
    void decrypt_symmetric_key(const unsigned char* encrypted_key, unsigned char* decrypted_key, size_t size_encKey);

    // �����������ݰ�
    vector<unsigned char> encrypt(const unsigned char* symmetric_key, const std::string& message);

    //���ܽ��յ������ݰ�
    string decrypt(const unsigned char* symmetric_key, const vector<unsigned char>& encrypted_message);

    string toHex(const unsigned char* data, size_t len);

    string toHex(const vector<unsigned char>& data);

    vector<unsigned char> getPublicKey();

    bool packetLenIsValid(size_t size);
private:
    // ����������Կ��
    unsigned char server_pk[crypto_box_PUBLICKEYBYTES];
    unsigned char server_sk[crypto_box_SECRETKEYBYTES];
    // �ͻ��˵ĶԳ���Կ
    //unsigned char symmetric_key[crypto_secretbox_KEYBYTES];
};

