#include "socket.h"

#include <QHostAddress>
#include <QMessageBox>

Socket::Socket(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);
    m_encryptor = new SodiumEncryptor(this);

    connect(m_socket, &QTcpSocket::connected, this, [=](){
        emit connected();
        qDebug() << "Socket coonect..";     //test

        if (m_encryptor->init() == 0) {
            emit failInitEncry();
        }
        m_encryptor->generate_symmetric_key();   //生成对称密钥对

        // 1.获取服务器公钥
        vector<unsigned char> server_key(crypto_box_PUBLICKEYBYTES); // 从服务器接收
        if(m_socket->waitForReadyRead(30000)){      //阻塞读取
            QByteArray byteArray = m_socket->readAll();
            server_key.assign(byteArray.begin(), byteArray.end());
        }
        else{
            emit failInitEncry();
        }
        m_encryptor->setServerPublicKey(server_key);   //保存到encryptor对象

        // 将公钥转换为16进制字符串并打印公钥
        cout << "STEP 2 - Received Server Public Key: " << endl << m_encryptor->toHex(server_key) << endl << endl;     //test

        // 2.生成加密的对称密钥并发送到服务器
        vector<unsigned char> encrypted_symmetric_key;
        m_encryptor->encrypt_symmetric_key(encrypted_symmetric_key);             //生成加密的对称密钥
        QByteArray encrypted_symmetric_key_array(reinterpret_cast<const char*>(encrypted_symmetric_key.data()), encrypted_symmetric_key.size());
        m_socket->write(encrypted_symmetric_key_array);         //发送给服务器
        m_socket->flush();
        m_socket->waitForBytesWritten();
        cout << "STEP 3 - Generated Symmetric Key: " << endl << m_encryptor->toHex(m_encryptor->getSymmetricKey()) << endl << endl;          //test
        cout << "STEP 4 - Send Encrypted Symmetric Key: " << endl << m_encryptor->toHex(encrypted_symmetric_key) << endl << endl;            //test

        connect(m_socket, &QTcpSocket::readyRead, this, [=](){
            qDebug() << "Socket ready read..";      //test
            QByteArray pkt;
            read(pkt);
            emit received(pkt);
        });
    });

    connect(m_socket, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError socketError){
        qDebug() << "err: " << socketError;
    });
    connect(m_socket, &QTcpSocket::disconnected, this, [=](){
        emit disconnected();
        qDebug() << "Socket discoonect..";     //test
    });
    connectToServer();
}

void Socket::connectToServer(QString ip, unsigned short port)
{
    m_socket->connectToHost(QHostAddress(ip), port);
    m_socket->waitForConnected();
}

void Socket::close()
{
    m_socket->disconnectFromHost();
    m_socket->waitForDisconnected();
    m_socket->deleteLater();
    m_socket = nullptr;
}

void Socket::write(QByteArray pkt)
{
    QByteArray encryped_pkt = m_encryptor->encrypt(pkt);
    if(m_socket->waitForConnected()){
        m_socket->write(encryped_pkt);
        m_socket->flush();
        m_socket->waitForBytesWritten();
    }
}

void Socket::read(QByteArray &pkt)
{
    QByteArray encryped_pkt = m_socket->readAll();
    pkt = m_encryptor->decrypt(encryped_pkt);
}

bool Socket::isOpen()
{
    return m_socket->isOpen();
}
