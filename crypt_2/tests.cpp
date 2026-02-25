#include <gtest/gtest.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include "inc/des.hpp"
#include "inc/tripple_des.hpp"
#include "inc/utils.hpp"

class CryptoServiceTest : public ::testing::Test {
protected:
    des_cipher des;
    bytes_t default_key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    void SetUp() override {
        des.setup_keys(default_key);
    }
};

TEST_F(CryptoServiceTest, HandlesPaddingCorrectly) {
    service service(des);
    bytes_t input = {0xAA, 0xBB, 0xCC};

    bytes_t encrypted = service.encrypt_bytes(input);

    EXPECT_EQ(encrypted.size(), 8);

    bytes_t decrypted = service.decrypt_bytes(encrypted);
    EXPECT_EQ(decrypted, input);
}

TEST(CryptoLogic, RandomDataRoundtrip) {
    triple_des des3(triple_des_mode::EDE3);
    bytes_t key24(24, 0x05);
    des3.setup_keys(key24);
    service service(des3);

    for (int size : {1, 7, 8, 15, 64}) {
        bytes_t original(size);
        for(auto& b : original) b = rand() % 256;

        bytes_t decrypted = service.decrypt_bytes(service.encrypt_bytes(original));
        EXPECT_EQ(decrypted, original) << "Failed for size: " << size;
    }
}

TEST_F(CryptoServiceTest, FileTypeSupport) {
    service service(des);
    std::string test_file = "test_media.bin";
    std::string enc_file = "test_media.enc";
    std::string dec_file = "test_media.dec";

    bytes_t fake_media = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};

    std::ofstream os(test_file, std::ios::binary);
    os.write(reinterpret_cast<char*>(fake_media.data()), fake_media.size());
    os.close();

    service.encrypt_file(test_file, enc_file);
    service.decrypt_file(enc_file, dec_file);

    std::ifstream is(dec_file, std::ios::binary);
    bytes_t restored((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    EXPECT_EQ(restored, fake_media);

    std::filesystem::remove(test_file);
    std::filesystem::remove(enc_file);
    std::filesystem::remove(dec_file);
}

TEST(TripleDES, AllModesWork) {
    std::vector<triple_des_mode> modes =
            {
            triple_des_mode::EDE3, triple_des_mode::EEE3,
            triple_des_mode::EDE2, triple_des_mode::EEE2};

    bytes_t key24(24, 0x42);
    bytes_t data = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

    for (auto mode : modes)
    {
        triple_des cipher(mode);
        cipher.setup_keys(key24);
        bytes_t enc = cipher.encrypt_block(data);
        bytes_t dec = cipher.decrypt_block(enc);
        EXPECT_EQ(dec, data) << "Failed in mode: " << static_cast<int>(mode);
    }
}