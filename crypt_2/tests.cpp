#include <gtest/gtest.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include "inc/des.hpp"
#include "inc/tripple_des.hpp"
#include "inc/utils.hpp"
#include "padding/zeros_padding.hpp"
#include "padding/ansi_padding.hpp"
#include "context/cipher_context.hpp"

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
    std::string test_file = "prob_theory_3_1";
    std::string enc_file = "test_media.enc";
    std::string dec_file = "test_media.dec";
/*
    bytes_t fake_media = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};

    std::ofstream os(test_file, std::ios::binary);
    os.write(reinterpret_cast<char*>(fake_media.data()), fake_media.size());
    os.close();
*/

    service.encrypt_file(test_file, enc_file);
    service.decrypt_file(enc_file, dec_file);

    std::ifstream is(dec_file, std::ios::binary);
    bytes_t restored((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

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

#include "../inc/tripple_des.hpp"
#include "../context/cipher_context.hpp"
#include<gtest/gtest.h>

class CipherModeTest : public ::testing::Test
{
protected:
    bytes_t key = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    bytes_t iv = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    bytes_t input_data = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x31, 0x32, 0x33};
};

TEST_F(CipherModeTest, ECB_Cycle) {
    triple_des* algo = new triple_des();
    algo->setup_keys(key);

    CipherMode mode = CipherMode::ECB;
    PaddingType pad = PaddingType::AnsiX923;
    CipherContext ctx(algo, mode, pad);

    bytes_t cipher, decrypted;
    ctx.encrypt(input_data, cipher, 2);
    ctx.decrypt(cipher, decrypted, 2);

    EXPECT_EQ(decrypted, input_data);
}

TEST_F(CipherModeTest, CBC_Cycle) {
    triple_des* algo = new triple_des();
    algo->setup_keys(key);

    CipherMode mode = CipherMode::CBC;
    PaddingType pad = PaddingType::Zeros;
    CipherContext ctx(algo, mode, pad, iv);

    bytes_t cipher, decrypted;
    ctx.encrypt(input_data, cipher, 1);
    ctx.decrypt(cipher, decrypted, 1);

    EXPECT_EQ(decrypted, input_data);
}

TEST_F(CipherModeTest, PCBC_Cycle) {
    triple_des* algo = new triple_des();
    algo->setup_keys(key);

    CipherMode mode = CipherMode::PCBC;
    PaddingType pad = PaddingType::AnsiX923;
    CipherContext ctx(algo, mode, pad, iv);

    bytes_t cipher, decrypted;
    ctx.encrypt(input_data, cipher, 1);
    ctx.decrypt(cipher, decrypted, 1);

    EXPECT_EQ(decrypted, input_data);
}



TEST(PaddingTest, ZerosPaddingAddRemove) {
    ZerosPadding pad;
    bytes_t data = {0x01, 0x02, 0x03};
    size_t block_size = 8;

    bytes_t padded = pad.add(data, block_size);
    EXPECT_EQ(padded.size(), 8);
    EXPECT_EQ(padded[7], 0x00);

    bytes_t unpadded = pad.remove(padded, block_size);
    EXPECT_EQ(unpadded, data);
}

TEST(PaddingTest, AnsiX923CheckLastByte) {
    AnsiX923 pad;
    bytes_t data = {0xAA, 0xBB};
    size_t block_size = 8;

    bytes_t padded = pad.add(data, block_size);
    EXPECT_EQ(padded.size(), 8);
    EXPECT_EQ(padded[7], 6);

    bytes_t unpadded = pad.remove(padded, block_size);
    EXPECT_EQ(unpadded, data);
}