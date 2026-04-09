#include <iostream>
#include "../inc/math/GaloisField.hpp"

#include "../inc/ciphers/block/Rijndael.hpp"
#include"../inc/service/CipherContext.hpp"

#include <fstream>

using namespace crypto;

int main() {
    using P = crypto::GFPolynomial<8>;

    P mod(0x11b);
    P a(0x53);
    P b(0xCA);

    std::cout << "a = " << a.toString() << std::endl;
    std::cout << "b = " << b.toString() << std::endl;

    std::cout << "a + b = " << (a + b).toString() << std::endl;
    std::cout << "a * b mod m = " << a.multiplyMod(b, mod).toString() <<std::endl;

    P inv = a.inverse(mod);
    std::cout << inv.toString() << std::endl;

    std::cout << std::boolalpha << mod.isIrreducible() << std::endl;

    std::cout << std::boolalpha << P{0}.inverse(mod).value() << std::endl;
    return 0;
}
