
#include "helpers/NumberTheoryService.hpp"
#include"RSA/RSA.hpp"
#include"service/service.hpp"
#include "RSA/RSAKeyGenerator.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main()
{
    auto keys = RSAKeyGenerator::generate(1024);

    RSACipher rsa_pub(keys.e, keys.n);
    RSACipher rsa_priv(keys.d, keys.n);
    service encryptor(rsa_pub);
    service decryptor(rsa_priv);

    std::string filename = "test.png";

    try
    {
        std::string encrypted_tmp = filename + ".tmp";
        std::string result_filename = "res_" + filename;

        std::cout << "Process: Encrypting" << std::endl;
        encryptor.encrypt_file(filename, encrypted_tmp);

        std::cout << "Process: Decrypting" << std::endl;
        decryptor.decrypt_file(encrypted_tmp, result_filename);

        std::cout << "Success! Saved as: " << result_filename << std::endl;

    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal Error: " << ex.what() << std::endl;
    }


    return 0;
}