#include "sodiumencryptor.h"

int SodiumEncryptor::init()
{
    if (sodium_init() == -1) {
        return 0;
    }
    return 1;
}

void SodiumEncryptor::generate_server_keys()
{
    crypto_box_keypair(server_pk, server_sk);
}

void SodiumEncryptor::decrypt_symmetric_key(const unsigned char* encrypted_key, unsigned char* decrypted_key, size_t size_encKey)
{
    if (crypto_box_seal_open(decrypted_key, encrypted_key, size_encKey, server_pk, server_sk) != 0) {
        // 解密失败
        cout << "Failed to decrypt symmetric key" << endl;
    }
}

vector<unsigned char> SodiumEncryptor::encrypt(const unsigned char* symmetric_key, const std::string& message)
{
    // 创建随机 nonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    // 分配内存以存储加密消息
    std::vector<unsigned char> encrypted_message(crypto_secretbox_MACBYTES + message.size());

    // 使用对称密钥加密消息
    crypto_secretbox_easy(encrypted_message.data(), (unsigned char*)message.c_str(), message.size(), nonce, symmetric_key);

    // 创建数据包：nonce + 加密消息
    std::vector<unsigned char> packet(sizeof nonce + encrypted_message.size());
    std::copy(nonce, nonce + sizeof nonce, packet.begin());
    std::copy(encrypted_message.begin(), encrypted_message.end(), packet.begin() + sizeof nonce);

    // 返回组合后的数据包
    return packet;
}

string SodiumEncryptor::decrypt(const unsigned char* symmetric_key, const vector<unsigned char>& encrypted_message)
{
    // 检查数据长度至少等于nonce的长度
    if (!packetLenIsValid(encrypted_message.size())) {
        cout << "Error: The received data is too short to contain valid nonce and ciphertext." << endl;
        return "";
    }

    // 提取nonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    memcpy(nonce, encrypted_message.data(), crypto_secretbox_NONCEBYTES);

    // 计算密文长度
    size_t ciphertext_length = encrypted_message.size() - crypto_secretbox_NONCEBYTES;

    // 提取密文
    vector<unsigned char> ciphertext(ciphertext_length);
    memcpy(ciphertext.data(), encrypted_message.data() + crypto_secretbox_NONCEBYTES, ciphertext_length);

    // 解密数据
    vector<unsigned char> decrypted_data(ciphertext_length - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted_data.data(), ciphertext.data(), ciphertext.size(), nonce, symmetric_key) != 0) {
        // 解密失败
        cout << "Decryption failed." << endl;
        return "";
    }

    // 将解密后的消息转换为 std::string
    string decryptedString(decrypted_data.begin(), decrypted_data.end());
    return decryptedString;
}

string SodiumEncryptor::toHex(const unsigned char* data, size_t len)
{
    // 将公钥转换为16进制字符串
    ostringstream hex_stream;
    for (size_t i = 0; i < len; ++i) {
        hex_stream << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(data[i]);
    }
    // 打印公钥
    return hex_stream.str();
}

string SodiumEncryptor::toHex(const vector<unsigned char>& data)
{
    // 将公钥转换为16进制字符串
    ostringstream hex_stream;
    for (auto byte : data) {
        hex_stream << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(byte);
    }
    // 打印公钥
    return hex_stream.str();
}

vector<unsigned char> SodiumEncryptor::getPublicKey()
{
    return vector<unsigned char>(server_pk, server_pk + crypto_box_PUBLICKEYBYTES);
}

bool SodiumEncryptor::packetLenIsValid(size_t size)
{
    // 检查数据长度至少等于nonce的长度
    if (size <= crypto_secretbox_NONCEBYTES) {
        return false;
    }
    return true;
}
