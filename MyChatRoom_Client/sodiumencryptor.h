#ifndef SODIUMENCRYPTOR_H
#define SODIUMENCRYPTOR_H

#include <QObject>
#include <sodium.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

class SodiumEncryptor : public QObject
{
    Q_OBJECT
public:
    explicit SodiumEncryptor(QObject *parent = nullptr);

    int init();

    //生成对称密钥
    void generate_symmetric_key();

    // 使用服务器公钥加密对称密钥
    void encrypt_symmetric_key(vector<unsigned char>& encrypted_symmetric_key);

    // 创建加密数据包
    QByteArray encrypt(const QByteArray& message);

    //解密接收到的数据包
    QByteArray decrypt(const QByteArray& encrypted_message);

    string toHex(const unsigned char* data, size_t len);

    string toHex(const vector<unsigned char>& data);

    void setServerPublicKey(const vector<unsigned char>& server_key);

    vector<unsigned char> getSymmetricKey();

    bool packetLenIsValid(size_t size);

signals:

private:
    // 服务器的公钥
    unsigned char m_server_pk[crypto_box_PUBLICKEYBYTES];

    // 生成的对称密钥
    unsigned char m_symmetric_key[crypto_secretbox_KEYBYTES];


};

#endif // SODIUMENCRYPTOR_H
