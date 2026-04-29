//
// Created by maria on 27.04.2026.
//
#include <iostream>
#include <memory>
#include <random>
#include "../inc/service/Socket.hpp"
#include "../inc/service/DiffieHellman.hpp"
#include "../inc/service/CipherContext.hpp"
#include "../inc/ciphers/block/Rijndael.hpp"
#include "../inc/ciphers/block/MARS.hpp"

using namespace crypto;
using bigint = boost::multiprecision::cpp_int;
using bytes_t = std::vector<uint8_t>;

const std::string P_HEX = "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"
                          "29024E088A67CC74020BBEA63B139B22514A08798E3404DD"
                          "EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"
                          "E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
                          "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D"
                          "C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F"
                          "83655D23DCA3AD961C62F356208552BB9ED529077096966D"
                          "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B"
                          "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9"
                          "DE2BCBF6955817183995497CEA956AE515D2261898FA0510"
                          "15728E5A8AACAA68FFFFFFFFFFFFFFFF";
const bigint G = 2;
const int PORT = 8080;

bigint generateRandomPrivateKey() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    bigint priv = 0;
    for (int i = 0; i < 4; ++i) {
        uint64_t limb = gen();
        priv = (priv << 64) | bigint(limb);
    }
    if (priv < 2) priv += 2;
    return priv;
}

void runAlice() {
    std::cout << "--- [ALICE (SERVER)] ---\n";
    std::unique_ptr<Socket> comm(createServer(PORT));
    std::cout << "Client connected.\n";

    bigint p("0x" + P_HEX);
    bigint privateKeyA = generateRandomPrivateKey();
    DiffieHellman dhA(p, G, privateKeyA);

    comm->sendString(dhA.getP().str());
    comm->sendString(dhA.getG().str());
    comm->sendString(dhA.getPublicKey().str());
    bigint B(comm->receiveString());
    dhA.computeSharedSecret(B);
    std::cout << "DH Key Exchange completed.\n";

    auto aes = std::make_shared<Rijndael<RijndaelBlockSize::Bits128, RijndaelKeySize::Bits128>>();
    auto mars = std::make_shared<MARS>();

    bytes_t aesKey = dhA.deriveBytes(aes->getBlockSize(), 0);
    bytes_t aesIV  = dhA.deriveBytes(aes->getBlockSize(), aes->getBlockSize());

    bytes_t marsKey = dhA.deriveBytes(mars->getBlockSize(), aes->getBlockSize() * 2);
    bytes_t marsIV  = dhA.deriveBytes(mars->getBlockSize(), aes->getBlockSize() * 3);

    aes->setupKeys(aesKey);
    mars->setupKeys(marsKey);

    CipherContext aesCtx(aes, CipherModeType::CBC, PaddingModeType::PKCS7, aesIV, 1);
    CipherContext marsCtx(mars, CipherModeType::CBC, PaddingModeType::PKCS7, marsIV, 1);

    std::string msgAes = "Hello Bob! This is AES speaking via CBC mode.";
    comm->sendBytes(aesCtx.encrypt(bytes_t(msgAes.begin(), msgAes.end())));
    std::cout << "Sent AES encrypted message.\n";

    std::string msgMars = "And this is MARS securing the perimeter.";
    comm->sendBytes(marsCtx.encrypt(bytes_t(msgMars.begin(), msgMars.end())));
    std::cout << "Sent MARS encrypted message.\n";
}

void runBob() {
    std::cout << "--- [BOB (CLIENT)] ---\n";
    std::unique_ptr<Socket> comm(createClient("127.0.0.1", PORT));
    std::cout << "Connected to Alice.\n";

    DiffieHellman dhB;
    bigint privateKeyB = generateRandomPrivateKey();

    bigint p(comm->receiveString());
    bigint g(comm->receiveString());
    bigint A(comm->receiveString());

    dhB.initFromPeer(p, g, privateKeyB);
    comm->sendString(dhB.getPublicKey().str());
    dhB.computeSharedSecret(A);
    std::cout << "DH Key Exchange completed.\n";

    auto aes = std::make_shared<Rijndael<RijndaelBlockSize::Bits128, RijndaelKeySize::Bits128>>();
    auto mars = std::make_shared<MARS>();

    bytes_t aesKey = dhB.deriveBytes(aes->getBlockSize(), 0);
    bytes_t aesIV  = dhB.deriveBytes(aes->getBlockSize(), aes->getBlockSize());

    bytes_t marsKey = dhB.deriveBytes(mars->getBlockSize(), aes->getBlockSize() * 2);
    bytes_t marsIV  = dhB.deriveBytes(mars->getBlockSize(), aes->getBlockSize() * 3);

    aes->setupKeys(aesKey);
    mars->setupKeys(marsKey);

    CipherContext aesCtx(aes, CipherModeType::CBC, PaddingModeType::PKCS7, aesIV, 1);
    CipherContext marsCtx(mars, CipherModeType::CBC, PaddingModeType::PKCS7, marsIV, 1);

    auto encAesMsg = comm->receiveBytes();
    auto decAesMsg = aesCtx.decrypt(encAesMsg);
    std::cout << "Decrypted (AES): " << std::string(decAesMsg.begin(), decAesMsg.end()) << "\n";

    auto encMarsMsg = comm->receiveBytes();
    auto decMarsMsg = marsCtx.decrypt(encMarsMsg);
    std::cout << "Decrypted (MARS): " << std::string(decMarsMsg.begin(), decMarsMsg.end()) << "\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "client") {
        runBob();
    } else {
        std::cout << "Usage: ./dh_demo [client]. Running as SERVER by default.\n";
        runAlice();
    }
    return 0;
}