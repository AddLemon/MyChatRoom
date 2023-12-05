#include "sodiumencryptor.h"

SodiumEncryptor::SodiumEncryptor(QObject *parent)
    : QObject{parent}
{

}

int SodiumEncryptor::init()
{
    if (sodium_init() == -1) {
        return 0;
    }
    return 1;
}

void SodiumEncryptor::generate_symmetric_key()
{
    randombytes_buf(m_symmetric_key, sizeof m_symmetric_key);
}

void SodiumEncryptor::encrypt_symmetric_key(vector<unsigned char>& encrypted_symmetric_key)
{
    unsigned char encrypted_key[crypto_box_SEALBYTES + crypto_secretbox_KEYBYTES];
    crypto_box_seal(encrypted_key, m_symmetric_key, sizeof m_symmetric_key, m_server_pk);
    vector<unsigned char> encrypted_key_vec(encrypted_key, encrypted_key + crypto_box_SEALBYTES + crypto_secretbox_KEYBYTES);
    encrypted_symmetric_key = encrypted_key_vec;
}

QByteArray SodiumEncryptor::encrypt(const QByteArray& message)
{
    // 创建随机 nonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    // 分配内存以存储加密消息
    std::vector<unsigned char> encrypted_message(crypto_secretbox_MACBYTES + message.size());

    // 使用对称密钥加密消息
    crypto_secretbox_easy(encrypted_message.data(), (unsigned char*)message.data(), message.size(), nonce, m_symmetric_key);

    // 创建数据包：nonce + 加密消息
    std::vector<unsigned char> packet(sizeof nonce + encrypted_message.size());
    std::copy(nonce, nonce + sizeof nonce, packet.begin());
    std::copy(encrypted_message.begin(), encrypted_message.end(), packet.begin() + sizeof nonce);
    QByteArray packet_array(reinterpret_cast<const char*>(packet.data()), packet.size());
    // 返回组合后的数据包
    return packet_array;
}

QByteArray SodiumEncryptor::decrypt(const QByteArray& encrypted_message)
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
    vector<unsigned char> decrypted_message(ciphertext_length - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted_message.data(), ciphertext.data(), ciphertext.size(), nonce, m_symmetric_key) != 0) {
        // 解密失败
        cout << "Decryption failed." << endl;
        return "";
    }

    // 将解密后的消息转换为QByteArray
    QByteArray packet_array(reinterpret_cast<const char*>(decrypted_message.data()), decrypted_message.size());
    return packet_array;
}

string SodiumEncryptor::toHex(const unsigned char* data, size_t len)
{
    // 将公钥转换为16进制字符串
    std::ostringstream hex_stream;
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
    std::ostringstream hex_stream;
    for (auto byte : data) {
        hex_stream << std::hex << std::setfill('0') << std::setw(2)
                   << static_cast<int>(byte);
    }
    // 打印公钥
    return hex_stream.str();
}

void SodiumEncryptor::setServerPublicKey(const vector<unsigned char>& server_key)
{
    copy(server_key.begin(), server_key.end(), m_server_pk);
}

vector<unsigned char> SodiumEncryptor::getSymmetricKey()
{
    return vector<unsigned char>(m_symmetric_key, m_symmetric_key + crypto_secretbox_KEYBYTES);
}

bool SodiumEncryptor::packetLenIsValid(size_t size)
{
    // 检查数据长度至少等于nonce的长度
    if (size <= crypto_secretbox_NONCEBYTES) {
        return false;
    }
    return true;
}
