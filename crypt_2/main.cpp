#include "inc/utils.hpp"
#include<iostream>

#include <iostream>
#include <iomanip>
#include <string>
#include "inc/tripple_des.hpp"
#include "context/cipher_context.hpp"

void print_hex(const std::string& title, const bytes_t& data) {
    std::cout << title << ": ";
    for (auto b : data) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b << " ";
    std::cout << std::dec << std::endl;
}

int main() {
    try {
        std::string plain_text = "Hello, TripleDES!";
        bytes_t data(plain_text.begin(), plain_text.end());
        bytes_t data_2(plain_text.begin(), plain_text.end());
        bytes_t key = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                       0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                       0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24};
        bytes_t iv = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

        std::cout << "--- TEST 1: PADDINGS (ECB MODE) ---\n";

        triple_des* algo1 = new triple_des(triple_des_mode::EDE3);
        algo1->setup_keys(key);

        CipherMode mode_ecb = CipherMode::ECB;
        PaddingType pad_zeros = PaddingType::Zeros;

        CipherContext ctx_zeros(algo1, mode_ecb, pad_zeros);

        bytes_t cipher_zeros, decrypted_zeros;
        ctx_zeros.encrypt(data, cipher_zeros);
        ctx_zeros.decrypt(cipher_zeros, decrypted_zeros);

        print_hex("Zeros Encrypted", cipher_zeros);
        std::cout << "Zeros Decrypted: " << std::string(decrypted_zeros.begin(), decrypted_zeros.end()) << "\n\n";

        triple_des* algo2 = new triple_des(triple_des_mode::EDE3);
        algo2->setup_keys(key);

        PaddingType pad_ansi = PaddingType::AnsiX923;
        CipherContext ctx_ansi(algo2, mode_ecb, pad_ansi);

        bytes_t cipher_ansi, decrypted_ansi;
        ctx_ansi.encrypt(data_2, cipher_ansi);
        ctx_ansi.decrypt(cipher_ansi, decrypted_ansi);

        print_hex("ANSI Encrypted ", cipher_ansi);
        std::cout << "ANSI Decrypted: " << std::string(decrypted_ansi.begin(), decrypted_ansi.end()) << "\n";

        if (cipher_zeros != cipher_ansi) {
            std::cout << ">> Result: Paddings produce different ciphertexts (Correct!)\n";
        }
        std::cout << "\n--- TEST 2: FILE ENCRYPTION (CBC MODE) ---\n";

        std::string test_file = "test_input.txt";
        std::ofstream out(test_file);
        out << "This is a secret message for Triple DES file testing. It has multiple blocks!";
        out.close();

        triple_des* algo_file = new triple_des(triple_des_mode::EDE3);
        algo_file->setup_keys(key);

        CipherMode mode_cbc = CipherMode::CBC;
        CipherContext ctx_file(algo_file, mode_cbc, pad_ansi, iv);

        std::cout << "Encrypting file...\n";
        auto fut_enc = ctx_file.encrypt_file(test_file, "encrypted.bin", 4);
        fut_enc.wait();


        std::cout << "Decrypting file...\n";
        auto fut_dec = ctx_file.decrypt_file("encrypted.bin", "decrypted_result.txt", 4);
        fut_dec.wait();

        std::cout << "Done! Check 'decrypted_result.txt'\n";

        std::cout << "\n--- TEST 3: ECB MODE ---" << std::endl;
        triple_des* algo_ecb = new triple_des(triple_des_mode::EDE3);
        algo_ecb->setup_keys(key);

        CipherMode mode_ecb_1 = CipherMode::ECB;
        CipherContext ctx_ecb(algo_ecb, mode_ecb_1, pad_ansi);

        bytes_t cipher_ecb, decrypted_ecb;
        ctx_ecb.encrypt(data, cipher_ecb, 4);
        ctx_ecb.decrypt(cipher_ecb, decrypted_ecb, 4);

        std::cout << "ECB Decrypted: " << std::string(decrypted_ecb.begin(), decrypted_ecb.end()) << std::endl;

        std::cout << "\n--- TEST 4: PCBC MODE ---" << std::endl;
        triple_des* algo_pcbc = new triple_des(triple_des_mode::EDE3);
        algo_pcbc->setup_keys(key);

        CipherMode mode_pcbc = CipherMode::PCBC;
        CipherContext ctx_pcbc(algo_pcbc, mode_pcbc, pad_ansi, iv);

        bytes_t cipher_pcbc, decrypted_pcbc;
        ctx_pcbc.encrypt(data, cipher_pcbc, 1); // PCBC только в 1 поток!
        ctx_pcbc.decrypt(cipher_pcbc, decrypted_pcbc, 1);

        std::cout << "PCBC Decrypted: " << std::string(decrypted_pcbc.begin(), decrypted_pcbc.end()) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}