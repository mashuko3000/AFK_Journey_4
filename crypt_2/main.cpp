#include "inc/utils.hpp"
#include<iostream>

int main() {
    des_cipher des;
    des.setup_keys({0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08});

    service service(des);

    std::string secret = "Hello World!";
    bytes_t data(secret.begin(), secret.end());

    bytes_t enc = service.encrypt_bytes(data);
    bytes_t dec = service.decrypt_bytes(enc);

    std::cout << "Decrypted: " << std::string(dec.begin(), dec.end()) << std::endl;

    service.encrypt_file("test.txt", "test.enc");
    service.decrypt_file("test.enc", "test.dec");

    return 0;
}