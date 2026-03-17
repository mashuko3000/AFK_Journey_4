#include <iostream>
#include <iostream>
#include "interfaces.hpp"
#include "VernamCipher.hpp"
#include "VigenereCipher.hpp"
#include "StatisticalAttack.hpp"
#include "Attack.hpp"

int main() {
    std::cout << "run simulaions\n";

    VigenereCipher vigCipher;
    //std::string vigenereText = "RIJVSWUYVJN" "RIJVSWUYVJN" "RIJVSWUYVJN" "RIJVSWUYVJN";
    std::string plainText = "Cryptography is the practice and study of techniques for secure communication in the presence of adversarial behavior. More generally, cryptography is about constructing and analyzing protocols that prevent third parties or the public from reading private messages.";
    std::string key = "MAGIC";

    std::string vigenereCiphertext = vigCipher.encrypt(plainText, key);

    VigenereAttack vigAttack(vigenereCiphertext);
    vigAttack.executeAttack();

    VernamCipher vernamCipher;
    std::string m1 = "HELLO WORLD";
    std::string m2 = "SECRET CODE";
    std::string key_2 = "SUPERSECRET";

    std::string c1 = vernamCipher.encrypt(m1, key_2);
    std::string c2 = vernamCipher.encrypt(m2, key_2);

    VernamAttack vernamAttack(c1, c2);
    vernamAttack.executeAttack();

    return 0;
}