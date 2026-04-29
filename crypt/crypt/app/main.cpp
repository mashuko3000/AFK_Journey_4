#include <fstream>
#include <iterator>
#include<memory>

#include <iostream>
#include "../inc/math/NumberTheoryService.hpp"
#include "../inc/service/MillerRobinPrimalityTest.hpp"
/*
#include"../inc/common/CryptoTypes.hpp"
#include "../inc/common/Common.h"
#include "../inc/core/IBlockCipher.hpp"
#include "../inc/service/CipherContext.hpp"
#include "../inc/core/IAsymmetricEncryptor.hpp"

#include"../inc/ciphers/block/TwoFish.hpp"
#include"../inc/ciphers/block/MARS.hpp"
#include "../inc/ciphers/asymmetric/NTRU.hpp"
#include "../inc/ciphers/asymmetric/ElGamal.hpp"

using namespace crypto;

crypto::bytes_t read_file(const std::string& path) {
    std::ifstream is(path, std::ios::binary);
    if (!is) throw std::runtime_error("Cannot open file: " + path);
    return bytes_t((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
}

void write_file(const std::string& path, const bytes_t& data) {
    std::ofstream os(path, std::ios::binary);
    os.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void testSymmetricFileIO(std::shared_ptr<IBlockCipher> algo)
{
    std::string originalFile = "test_input.dat";
    std::string encryptedFile = "test_encrypted.bin";
    std::string decryptedFile = "test_decrypted.dat";

    bytes_t dummyData(100, 0xAB);
    write_file(originalFile, dummyData);

    bytes_t key(16, 0x01);
    bytes_t iv(16, 0x02);
    algo->setupKeys(key);

    std::vector<CipherModeType> modes = {CipherModeType::CBC, CipherModeType::CTR};
    std::vector<PaddingModeType> paddings = {PaddingModeType::PKCS7, PaddingModeType::ISO10126};

    for (auto mode : modes) {
        for (auto pad : paddings) {
            CipherContext ctx(algo, mode, pad, iv);
            auto enc_future = ctx.encrypt_file_async(originalFile, encryptedFile);
            enc_future.wait();

            auto dec_future = ctx.decrypt_file_async(encryptedFile, decryptedFile);
            dec_future.wait();

            bytes_t result = read_file(decryptedFile);
            bool success = (result == dummyData);

            std::cout << "[FILE TEST] " << algo->name()
                      << " | Mode: " << (int)mode
                      << " | Pad: " << (int)pad
                      << " -> " << (success ? "OK" : "FAIL") << std::endl;
        }
    }
}

void testAsymmetricFileIO(std::shared_ptr<IAsymmetricEncryptor> algo) {
    std::string originalFile = "asym_input.dat";
    std::string encryptedFile = "asym_encrypted.bin";
    std::string decryptedFile = "asym_decrypted.dat";

    bytes_t smallData = {'F', 'I', 'L', 'E', '!', '!'};
    write_file(originalFile, smallData);

    if (auto ntru = std::dynamic_pointer_cast<NTRU>(algo)) {
        ntru->generateKeyPair();
    } else if (auto eg = std::dynamic_pointer_cast<ElGamal>(algo)) {
        eg->generateKeyPair();
    }

    bytes_t plain = read_file(originalFile);
    bytes_t cipher = algo->encrypt(plain);
    write_file(encryptedFile, cipher);

    bytes_t cipherIn = read_file(encryptedFile);
    bytes_t decrypted = algo->decrypt(cipherIn);
    write_file(decryptedFile, decrypted);

    bool success = (decrypted == smallData);
    std::cout << "[FILE TEST] " << algo->name() << " -> " << (success ? "OK" : "FAIL") << std::endl;
}

int main() {

    crypto::ElGamal elgamal(nullptr, 2048);
    elgamal.generateKeyPair();

    std::string original = "Hello, ElGamal Test!";
    crypto::bytes_t plain_bytes(original.begin(), original.end());

    crypto::bytes_t encrypted = elgamal.encrypt(plain_bytes);
    std::cout << "Encrypted size: " << encrypted.size() << " bytes\n";

    crypto::bytes_t decrypted_bytes = elgamal.decrypt(encrypted);
    std::string decrypted(decrypted_bytes.begin(), decrypted_bytes.end());

    std::cout << "Decrypted: " << decrypted << std::endl;


    crypto::NTRU ntru(503, 3, 256, 11, 11);
    ntru.generateKeyPair();

    std::string originalN = "NTRU Lattice-based crypto test";
    crypto::bytes_t plain_bytesN(originalN.begin(), originalN.end());

    crypto::bytes_t encryptedN = ntru.encrypt(plain_bytesN);
    crypto::bytes_t decrypted_bytesN = ntru.decrypt(encryptedN);

    std::string decryptedN(decrypted_bytesN.begin(), decrypted_bytesN.end());
    std::cout << "NTRU Decrypted: " << decryptedN << std::endl;

    try {
        std::cout << "--- Starting File-Based Encryption Tests ---\n";
        testSymmetricFileIO(std::make_shared<TwoFish>());

        testSymmetricFileIO(std::make_shared<MARS>());

        auto ntru = std::make_shared<NTRU>(503, 3, 256, 11, 8);
        testAsymmetricFileIO(ntru);

        auto elgamal = std::make_shared<ElGamal>(nullptr, 128, 0.99);
        testAsymmetricFileIO(elgamal);

        std::cout << "\nAll file tests completed." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}*/

/*
void printRoots(const std::string& label, const std::vector<bigint>& roots) {
    std::cout << label << ": ";
    if (roots.empty()) {
        std::cout << "корней нет";
    } else {
        for (size_t i = 0; i < std::min(roots.size(), (size_t)10); ++i) {
            std::cout << roots[i] << " ";
        }
        if (roots.size() > 10) std::cout << "... (всего " << roots.size() << ")";
    }
    std::cout << std::endl;
}

int main() {
    NumberTheoryService service;
    MillerRabinPrimalityTest mrTest;

    std::cout << "--- Тест 1: Поиск первообразных корней ---" << std::endl;

    // Число 7 (простое, корни: 3, 5)
    bigint n7 = 7;
    printRoots("Корни для n=7", service.searchPrimitiveRootInZn(n7, mrTest));

    // Число 10 (2*p, корни: 3, 7)
    bigint n10 = 10;
    printRoots("Корни для n=10", service.searchPrimitiveRootInZn(n10, mrTest));

    // Число 25 (p^k, корни должны быть)
    bigint n25 = 25;
    printRoots("Корни для n=25", service.searchPrimitiveRootInZn(n25, mrTest));

    // Число 8 (делится на 4, корней нет)
    bigint n8 = 8;
    printRoots("Корни для n=8", service.searchPrimitiveRootInZn(n8, mrTest));

    std::cout << "\n--- Тест 2: Факторизация (Pollard Rho) ---" << std::endl;

    bigint bigNum = 123456789; // 3^2 * 3607 * 3803
    std::set<bigint> factors;
    service.getFactors(bigNum, factors, mrTest);

    std::cout << "Факторы " << bigNum << ": ";
    for(auto f : factors) std::cout << f << " ";
    std::cout << std::endl;

    std::cout << "\n--- Тест 3: Поиск базы степени ---" << std::endl;

    bigint powerNum = 16807; // 7^5
    bigint base = service.getPrimeBase(powerNum, mrTest);
    std::cout << powerNum << " это " << base << " в какой-то степени." << std::endl;

    return 0;
}*/