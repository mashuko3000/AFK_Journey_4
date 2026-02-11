#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include <ginac/ginac.h>
#include <ginac/parser.h>

using namespace GiNaC;


void gauss(std::vector<std::vector<ex>>& A, int rows, int cols, int& rank);

int main()
{

    std::string f_filename;
    std::cout << "Enter name of file for function f : " << std::endl;
    std::cin >> f_filename;

    std::ifstream file(f_filename);
    if (!file.is_open())
    {
        std::cout << "Cannot open the file for function f(x)" << std::endl;
        return 1;
    }

    std::string line;
    getline(file, line);
    file.close();

    std::cout << "String from f file [" << line << "]" << std::endl;

    symbol x("x");
    symtab table;
    table["x"] = x;
    parser reader(table);

    ex f;

    try
    {
        f = reader(line);
        std::cout << "All good" << std::endl;
        std::cout << "f(x) = " << f << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "Parsing fail: " << e.what() << std::endl;
        return 1;
    }

    int f_deg = f.degree(x);

    std::cout << "Degree of f : " << f_deg << std::endl;

    std::string g_filename;
    std::cout << "Enter name of file for functions g : " << std::endl;
    std::cin >> g_filename;
    std::cout << g_filename << std::endl;

    std::ifstream gfile(g_filename);
    if (!gfile.is_open())
    {
        std::cout << "Cannot open the file for functions g(x)" << std::endl;
        return 1;
    }

    std::string first_line;
    getline(gfile, first_line);

    int k = stoi(first_line);
    std::cout << "Amount of g functions : " << k << std::endl;

    std::vector<ex> gens;

    int max_deg = f_deg;
    for (int i = 0; i < k; i++)
    {
        std::string g_line;
        if(!getline(gfile, g_line))
        {
            std::cout << "Error: cannot read string for g_" << i+1 << std::endl;
        }
        
        std::cout << "g_" << i + 1 << ": [" << g_line << "]" << std::endl;

        ex g_poly;
        try
        {
            g_poly = reader(g_line);
            gens.push_back(g_poly);
            std::cout << "All good -> " << g_poly << std::endl;
            int d = g_poly.degree(x);
            std::cout << "Degree of g_" << i+1 << " : " << d << std::endl;
            if (d > max_deg) max_deg = d;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
    }
    gfile.close();
    
    std::cout << "max_deg = " << max_deg << std::endl;

    int n = max_deg + 1;
    int m = gens.size();

    std::vector<std::vector<ex>> matrix(n, std::vector<ex>(m+1, 0));

    for (int deg = 0; deg < n; ++deg)
    {
        for (int j = 0; j<m; ++j)
        {
            matrix[deg][j] = gens[j].coeff(x, deg);
        }
    }

    for (int deg = 0; deg < n; ++deg)
    {
        matrix[deg][m] = f.coeff(x, deg);
    }

    std::cout << "Matrix for Gauss:" << std::endl;
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j <= m; ++j) 
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    int final_rank = 0;
    gauss(matrix, n, m+1, final_rank);

    std::cout << "Matrix after Gauss:" << std::endl;
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j <= m; ++j) 
        {
            std::cout << matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    bool consistant = true;
    for (int r = final_rank; r< n; ++r)
    {
        if(!matrix[r].back().is_zero())
        {
            consistant = false;
            break;
        }
    }

    if(!consistant)
    {
        std::cout << "does not belong" << std::endl;
    }
    else 
    {
        std::cout << "belong to the linear span" << std::endl;
        std::cout << "rank : " << final_rank << " of " << m << " variables" << std::endl;

        std::cout << "f(x) = ";
        bool first = true;
        for (int j = 0; j < m; ++j)
        {
            ex coeff = matrix[j][m].normal();
            
            if(coeff.is_zero()) continue;

            if(!first && coeff.info(info_flags::positive)) std::cout<< " + ";
            else if (!first && coeff.info(info_flags::negative)) std::cout << " - ";

            std::cout << "[" << coeff << "] * g_" << j + 1 << "(x)";
            first = false;        
        }
        std::cout<<std::endl;
    }

    return 0;
}

void gauss(std::vector<std::vector<ex>>& A, int rows, int cols, int& rank)
{
    rank = 0;
    for (int col = 0; (col < cols - 1) && (rank < rows); ++col)
    {
        int pivot = rank;
        ex max_abs = GiNaC::abs(A[rank][col]);

        for (int r = rank + 1; r < rows; ++r)
        {
            ex current_abs = GiNaC::abs(A[r][col]);
            if (current_abs > max_abs)
            {
                max_abs = current_abs;
                pivot = r;
            }
        }

        if (A[pivot][col].is_zero())
        {
            continue;
        }

        if (pivot != rank)
        {
            std::swap(A[pivot], A[rank]);
        }

        ex piv = A[rank][col];
        for (int j = col; j < cols; ++j)
        {
            A[rank][j] = (A[rank][j]/piv);
        }

        for (int r = 0; r < rows; ++r)
        {
            if (r == rank) continue;
            ex factor = A[r][col];
            for(int j = col; j < cols; ++j)
            {
                A[r][j] = (A[r][j] - factor * A[rank][j]);
            }
        }
        rank++;
    }
}