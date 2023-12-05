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
        // ����ʧ��
        cout << "Failed to decrypt symmetric key" << endl;
    }
}

vector<unsigned char> SodiumEncryptor::encrypt(const unsigned char* symmetric_key, const std::string& message)
{
    // ������� nonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, sizeof nonce);

    // �����ڴ��Դ洢������Ϣ
    std::vector<unsigned char> encrypted_message(crypto_secretbox_MACBYTES + message.size());

    // ʹ�öԳ���Կ������Ϣ
    crypto_secretbox_easy(encrypted_message.data(), (unsigned char*)message.c_str(), message.size(), nonce, symmetric_key);

    // �������ݰ���nonce + ������Ϣ
    std::vector<unsigned char> packet(sizeof nonce + encrypted_message.size());
    std::copy(nonce, nonce + sizeof nonce, packet.begin());
    std::copy(encrypted_message.begin(), encrypted_message.end(), packet.begin() + sizeof nonce);

    // ������Ϻ�����ݰ�
    return packet;
}

string SodiumEncryptor::decrypt(const unsigned char* symmetric_key, const vector<unsigned char>& encrypted_message)
{
    // ������ݳ������ٵ���nonce�ĳ���
    if (!packetLenIsValid(encrypted_message.size())) {
        cout << "Error: The received data is too short to contain valid nonce and ciphertext." << endl;
        return "";
    }

    // ��ȡnonce
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    memcpy(nonce, encrypted_message.data(), crypto_secretbox_NONCEBYTES);

    // �������ĳ���
    size_t ciphertext_length = encrypted_message.size() - crypto_secretbox_NONCEBYTES;

    // ��ȡ����
    vector<unsigned char> ciphertext(ciphertext_length);
    memcpy(ciphertext.data(), encrypted_message.data() + crypto_secretbox_NONCEBYTES, ciphertext_length);

    // ��������
    vector<unsigned char> decrypted_data(ciphertext_length - crypto_secretbox_MACBYTES);
    if (crypto_secretbox_open_easy(decrypted_data.data(), ciphertext.data(), ciphertext.size(), nonce, symmetric_key) != 0) {
        // ����ʧ��
        cout << "Decryption failed." << endl;
        return "";
    }

    // �����ܺ����Ϣת��Ϊ std::string
    string decryptedString(decrypted_data.begin(), decrypted_data.end());
    return decryptedString;
}

string SodiumEncryptor::toHex(const unsigned char* data, size_t len)
{
    // ����Կת��Ϊ16�����ַ���
    ostringstream hex_stream;
    for (size_t i = 0; i < len; ++i) {
        hex_stream << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(data[i]);
    }
    // ��ӡ��Կ
    return hex_stream.str();
}

string SodiumEncryptor::toHex(const vector<unsigned char>& data)
{
    // ����Կת��Ϊ16�����ַ���
    ostringstream hex_stream;
    for (auto byte : data) {
        hex_stream << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(byte);
    }
    // ��ӡ��Կ
    return hex_stream.str();
}

vector<unsigned char> SodiumEncryptor::getPublicKey()
{
    return vector<unsigned char>(server_pk, server_pk + crypto_box_PUBLICKEYBYTES);
}

bool SodiumEncryptor::packetLenIsValid(size_t size)
{
    // ������ݳ������ٵ���nonce�ĳ���
    if (size <= crypto_secretbox_NONCEBYTES) {
        return false;
    }
    return true;
}
