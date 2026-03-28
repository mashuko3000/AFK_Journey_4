#include <iostream>
#include<vector>

#include<Polynimial.hpp>

#include"Ordering/Lex.h"
#include"Ordering/GrLex.h"
#include"Ordering/GrevLex.h"
#include"Ordering/InvLex.h"
#include"Ordering/RinvLex.h"

#include"PolyDivision.h"



using PolyType = double;

void printVector(const std::string& name, const std::vector<int>& vec) {
    std::cout << name << ": (";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << (i + 1 == vec.size() ? "" : ", ");
    }
    std::cout << ")\n";
}

void printDivisionResult(
        const DivisionResult<PolyType>& res,
        const std::string& orderName,
        const std::string& divOrder)
{
    std::cout << "=== Ordering: " << orderName << " | Ordering divisors: " << divOrder << " ===\n";
    for (size_t i = 0; i < res.quotients.size(); ++i) {
        std::cout << "a_" << i + 1 << " = " << res.quotients[i] << "\n";
    }
    std::cout << "Remainder r = " << res.remainder << "\n\n";
}

int main() {
    try {
        std::vector<std::string> vars = {"x", "y", "z"};
        Polynomial<int> poly(vars);

        poly.addMonomial({2, 1, 0}, 4);
        poly.addMonomial({3, 0, 0}, 5);
        poly.addMonomial({0, 3, 2}, 2);
        poly.addMonomial({0, 0, 0}, 7);

        std::cout << "Polynomial support initialized.\n\n";

        LexOrder<int> lex;
        GrLexOrder<int> grlex;
        GrevLexOrder<int> grevlex;
        InvLexOrder<int> invlex;
        RinvLexOrder<int> rinvlex;

        auto demonstrateOrder = [&](const std::string& orderName, const MonomialOrder<int>& order) {
            std::cout << "=== " << orderName << " ===\n";
            auto leadingT = poly.lt(order);
            printVector("multideg(f)", poly.multideg(order));
            printVector("lm(f)      ", poly.lm(order));
            std::cout << "lc(f)      : " << poly.lc(order) << "\n";
            std::cout << "lt(f)      : " << leadingT.toString(vars) << "\n\n";
        };

        demonstrateOrder("Lexicographic (lex)", lex);
        demonstrateOrder("Graded Lexicographic (grlex)", grlex);
        demonstrateOrder("Graded Reverse Lexicographic (grevlex)", grevlex);
        demonstrateOrder("Inverse Lexicographic (invlex)", invlex);
        demonstrateOrder("Reverse Inverse Lexicographic (rinvlex)", rinvlex);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    try {
        const std::vector<std::string> vars = {"x", "y"};

        Polynomial<PolyType> f(vars);
        f.addMonomial({7, 2}, 1.0);
        f.addMonomial({3, 2}, 1.0);
        f.addMonomial({0, 1}, -1.0);
        f.addMonomial({0, 0}, 1.0);

        Polynomial<PolyType> f1(vars);
        f1.addMonomial({1, 2}, 1.0);
        f1.addMonomial({1, 0}, -1.0);

        Polynomial<PolyType> f2(vars);
        f2.addMonomial({1, 0}, 1.0);
        f2.addMonomial({0, 3}, -1.0);

        const LexOrder<PolyType> lex;
        const GrLexOrder<PolyType> grlex;

        const std::vector<Polynomial<PolyType>> divisors1 = {f1, f2};

        const auto resLex1 = Division<PolyType>::divide(f, divisors1, lex);
        printDivisionResult(resLex1, "Lex", "(f1, f2)");

        const auto resGrlex1 = Division<PolyType>::divide(f, divisors1, grlex);
        printDivisionResult(resGrlex1, "GrLex", "(f1, f2)");

        const std::vector<Polynomial<PolyType>> divisors2 = {f2, f1};

        const auto resLex2 = Division<PolyType>::divide(f, divisors2, lex);
        printDivisionResult(resLex2, "Lex", "(f2, f1)");

    } catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << "\n";
    }

    return 0;
}