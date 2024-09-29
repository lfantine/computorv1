#include "utils/utils.hpp"
#include "utils/basics.hpp"
#include <iostream>
#include <vector>
#include <string>

// void    reducion(std::string& str) {

// }

void    registerChunk(std::vector<chunk>& reg, chunk& ck) {
    // test is valid chunk
    for (const parcel& p : ck.values) {
        if (p.op == '/' && p.value == 0) {
            logErrorWithPrefix("cannot divid by 0.");
            throw new std::exception();
        } else if (p.op == C_VAR && ck.power > 2) {
            logErrorWithPrefix("The polynomial degree is strictly greater than 2, I can't solve.");
            throw new std::exception();
        }
    }

    reg.push_back(ck);
}

void    chunkerize(const std::string& str, std::vector<chunk>& equat, std::vector<chunk>& equal) {
    std::vector<char>   ops = {};
    std::vector<std::string> raw_chunks = split_chunks(str,ops);
    size_t equal_idx = 0;
    for (size_t e = 0; e < ops.size(); e++) {
        if (ops[e] == '=') {
            equal_idx = e;
            break;
        }
    }
    std::cout << "equal_idx = " << equal_idx << std::endl;

    for (size_t idx = 0; idx < raw_chunks.size(); idx++) {
        const std::string str = raw_chunks[idx];
        std::cout << str << " => " << idx << std::endl;
        size_t i = 0;
        chunk   ck = initChunk();

        // skip starts white space again
        while (i < str.length() && std::isspace(str[i])) {i++;}

        if (str[i] == '-') {ck.sign = -1;}
        i++;

        char last_op = ' ';
        // second loop to make the chunk
        for (; i<str.length(); i++) {
            // skip white space bef parcel parcel
            while (i < str.length() && std::isspace(str[i])) {i++;}
            char c = str[i];

            // third loop to make parcel
            for (; i<str.length() && (str[i]!='*' && str[i]!='/'); i++) {
                // skip white space bef parcel parcel
                while (i < str.length() && std::isspace(str[i])) {i++;}

                if (std::isdigit(c)) {
                    
                    ck.values.push_back({
                        last_op, // 3 choice : ' ' , '/' , '*' 
                        std::stof(str.substr(i))
                    });

                    //skip all nb
                    while (i < str.length() && (std::isdigit(str[i]) || str[i] == '.')) {
                        i++;
                    }
                }

                else if (c == C_VAR) {
                    i++;
                    ck.X = true;

                    // std::cout << "Pass by C_VAR FOUND " << std::endl;

                    ck.values.push_back({
                        C_VAR,
                        0
                    });

                    //skip white space
                    while (i < str.length() && std::isspace(str[i])) {i++;}

                    if (str[i] == '^') { // check if a power is given ( else its 1 )
                        i++;
                        //skip whitespace
                        while (i < str.length() && std::isspace(str[i])) {i++;}
                        if (std::isdigit(str[i])) {
                            // std::cout << "power found : " << str.substr(i) << std::endl;
                            ck.power = std::stoi(str.substr(i));
                            while (i < str.length() && (std::isdigit(str[i]) || str[i] == '.')) {
                                i++;
                            }
                        }
                    }
                }

                if (str[i] == '*' || str[i] == '/') { // permet de ne pas sauter le * quand il arrive a la fin de la parcel
                    i--;
                }
            }

            // std::cout << "char de sortie de (" <<  str << ") : " << str[i] << std::endl;
            if (str[i] == '*' || str[i] == '/') {
                last_op = str[i];
            } else {
                // skip white space between parcel
                while (i < str.length() && std::isspace(str[i])) {i++;}
            }
        }

        registerChunk( idx <= equal_idx ? equat : equal,ck);
    }
}

void    factorize(std::vector<chunk>& equa) {
    for (chunk& ck : equa) {
        std::vector<parcel> new_value;
        parcel value_parcel;
        new_value.push_back(value_parcel);

        for (const parcel& p : ck.values) {
            if (p.op == C_VAR) {
                new_value.push_back(p);
            } else {
                if (new_value[0].op == '?') {
                    if (p.op == '/') {
                        new_value[0].value = p.value;
                    } else {
                        new_value[0].value = p.value;
                    }
                    new_value[0].op = p.op;
                } else {
                    if (new_value[0].op == '/') {

                        if (p.op == '/') {
                            new_value[0].value *= p.value;
                        } else {
                            new_value[0].value = p.value / new_value[0].value;
                            new_value[0].op = ' ';
                        }

                    } else {

                        if (p.op == '/') {
                            new_value[0].value /= p.value;
                        } else {
                            new_value[0].value *= p.value;
                        }
                    }
                }
            }
        }
        ck.values.clear();
        for (size_t i = 0; i < new_value.size(); i++) {
            if (new_value[i].op == '?') {
                ck.values.push_back({
                    ' ',
                    1
                });
            } else {
                ck.values.push_back(new_value[i]);
            }
        }
    }
}

void combineChunk(chunk& hostCk, const chunk& invCk, bool phase = true) {
    if (hostCk.values.size() == 0) {
        if (invCk.values.size() == 1 && phase) hostCk.power = 0;
        else hostCk.power = invCk.power;
        hostCk.sign = invCk.sign;
        if (invCk.values.size() > 1 && invCk.power == 0) {hostCk.X = false;}
        else if (invCk.values.size() <= 1) {hostCk.X = false;}
        else {hostCk.X = true;}
        hostCk.values.push_back(invCk.values[0]);
        return;
    }
    if (hostCk.sign == invCk.sign) {
        hostCk.values[0].value += invCk.values[0].value;
    } else {
        hostCk.values[0].value -= invCk.values[0].value;
        if (hostCk.values[0].value < 0) {
            hostCk.sign *= -1;
            hostCk.values[0].value *= -1;
        }
    }
}

void    reduce(std::vector<chunk>& equa) {
    chunk c0 = initChunk();
    chunk c1 = initChunk();
    chunk c2 = initChunk();

    for (const chunk& ck : equa) {
        // std::cout << "chunk to reduce : nb (" << ck.values.size() << "), value(" << ck.values[0].value << ") " << std::endl;
        if (ck.values[0].value == 0) {
            continue;
        }
        if (ck.power == 0 || ck.values.size() == 1) {
            combineChunk(c0, ck);
        } else if (ck.power == 1) {
            combineChunk(c1, ck);
        } else if (ck.power == 2) {
            combineChunk(c2, ck);
        } else {
            logErrorWithPrefix(" power invalid.");
        }
    }

    equa.clear();
    if (c0.values.size() > 0) equa.push_back(c0);
    if (c1.values.size() > 0) equa.push_back(c1);
    if (c2.values.size() > 0) equa.push_back(c2);
}

void    reduce(std::vector<chunk>& equat, std::vector<chunk>& equal) {
    for (chunk& ck : equal) {
        ck.sign *= -1;
        equat.push_back(ck);
    }
    equal.clear();
    

    chunk c0 = initChunk();
    chunk c1 = initChunk();
    chunk c2 = initChunk();

    for (const chunk& ck : equat) {
        if (ck.values[0].value == 0) {
            continue;
        }
        if (ck.power == 0) {
            combineChunk(c0, ck, false);
        } else if (ck.power == 1) {
            combineChunk(c1, ck, false);
        } else if (ck.power == 2) {
            combineChunk(c2, ck, false);
        } else {
            logErrorWithPrefix(" power invalid.");
        }
    }

    equat.clear();
    if (c0.values.size() > 0) equat.push_back(c0);
    if (c1.values.size() > 0) equat.push_back(c1);
    if (c2.values.size() > 0) equat.push_back(c2);
}

bool hasDecimalPart(float num) {
    // Comparer le nombre avec sa partie entière (tronquée)
    return std::floor(num) != num;
}

// Fonction pour trouver le PGCD de deux entiers (utilise l'algorithme d'Euclide)
int pgdc(int a, int b) {
    const int max = a > b ? a : b;
    int pgdc = 1;
    for (int i = 2; i <= max; i++ ) {
        float moda = a;
        float modb = b;
        int na = 0;
        int nb = 0;
        while (moda >= i){moda -= i; na++;}
        while (modb >= i){modb -= i; nb++;}
        if (moda == 0 && modb == 0) {
            pgdc = i;
        }
    }
    return pgdc;
}

std::string    reduceSquareRootFraction(const float d, const float denom) {
    float max = d > denom ? d : denom;
    int new_nom = d;
    float new_denom = denom;
    float keep_m = 0;
    for (int i = 2; i < max; i++) {
        if (!hasDecimalPart(d / i) && !hasDecimalPart(std::sqrt(d / i))) {
            const int t = std::sqrt(d / i);
            if (i < new_nom) {
                new_nom = i;
                if (!hasDecimalPart(denom / t)) {
                    new_denom = denom / t;
                    keep_m = 0;
                } else {
                    keep_m = t;
                }
            }
        }
    }
    std::string r = "(";
    if (keep_m != 0) {
        int gcd = pgdc(std::abs(keep_m), std::abs(new_denom));
        r += to_string(keep_m / gcd) + "√" + to_string(new_nom) + "/" + to_string(new_denom / gcd) + ")";
    } else {
        r += "√" + to_string(new_nom) + "/" + to_string(new_denom) + ")";
    }
    return r;
}


std::string     reducedFraction(float nom, float denom, const bool isD = false, const float d = 0) {

    if (denom < 0) {
        nom *= -1;
        denom *= -1;
    }

    if (hasDecimalPart(nom) || hasDecimalPart(denom)) {
        if (d > 0 && isD) {return reduceSquareRootFraction(d, denom);}
        std::string r = "(";
        if (isD) {
            r += "√" + to_string(d);
        } else {
            r += to_string(nom);
        }
        r += "/" + to_string(denom) + ")";
        return r;
    }
    // Calculer le PGCD
    int gcd = pgdc(std::abs(nom), std::abs(denom));

    // Simplifier la fraction en divisant par le PGCD
    float n_nom = nom / gcd;
    const float n_denom = denom / gcd;
    // std::cout << "[" << nom << "/" << gcd << "] ";

    if (n_nom == -0) {
        n_nom = 0;
    }

    if (n_denom == 1) {
        return to_string(n_nom);
    } else if (gcd > 1) {
        return "(" + to_string(n_nom) + "/" + to_string(n_denom) + ")";
    } else {
        if (d > 0 && isD) {return reduceSquareRootFraction(d, denom);}
        std::string r = "(";
        if (isD) {
            r += "√" + to_string(d);
        } else {
            r += to_string(nom);
        }
        r += "/" + to_string(denom) + ")";
        return r;
    }
}

void    dNeg(const float a, const float b, const float c, const float d) {
    std::cout << "\n( with : a = (" << a << ") , b = (" << b << ") , c = (" << c << ") , d = (" << d << ") )";
    std::cout << "\n There are 2 solutions with i a complexe number (i^2=-1) : ";
    const float aa = 2 * a;
    const float mb = (b * -1);
    std::cout << "\n-> " << C_VAR << " = (-b-i√(Δ))/2a " << " = " << "(-(" << b <<")-i√(" << d <<"))/2(" << a <<") = " << "(-(" << b <<")-i√(" << d <<"))/"<< aa <<" = " << reducedFraction(mb, aa) << " - i" << reducedFraction(std::sqrt(std::abs(d)), aa, true, std::abs(d));
    std::cout << "\n-> " << C_VAR << " = (-b+i√(Δ))/2a " << " = " << "(-(" << b <<")+i√(" << d <<"))/2(" << a <<") = " << "(-(" << b <<")+i√(" << d <<"))/"<< aa <<" = " << reducedFraction(mb, aa) << " + i" << reducedFraction(std::sqrt(std::abs(d)), aa, true, std::abs(d));
}

void    dPos(const float a, const float b, const float c, const float d) {
    std::cout << "\n( with : a = (" << a << ") , b = (" << b << ") , c = (" << c << ") , d = (" << d << ") )";
    std::cout << "\n There are 2 solutions : ";
    // const float r1 = (-b - (std::sqrt(d))) / (2 * a);
    // const float r2 = (-b + (std::sqrt(d))) / (2 * a);

    std::cout << "\n-> " << C_VAR << " = (-b-√(Δ))/2a " << " = " << "(-(" << b <<")-√(" << d <<"))/2(" << a <<") = ";
    if (hasDecimalPart(std::sqrt(d))) {
        std::cout << reducedFraction(-b, (2 * a)) << " - " << reducedFraction(std::sqrt(std::abs(d)), (2 * a), true, d);
    } else {
        // std::cout << to_string(r1);
        float nom = (b * -1) - std::sqrt(d);
        std::cout << reducedFraction(nom, (2 * a));
    }
    std::cout << "\n-> " << C_VAR << " = (-b+√(Δ))/2a " << " = " << "(-(" << b <<")+√(" << d <<"))/2(" << a <<") = ";
    if (hasDecimalPart(std::sqrt(d))) {
        std :: cout << reducedFraction(-b, (2 * a)) << " + " << reducedFraction(std::sqrt(std::abs(d)), (2 * a), true, d);
    } else {
        float nom = (b * -1) + std::sqrt(d);
        std::cout << reducedFraction(nom, (2 * a));
        // std::cout << to_string(r2);
    }
}

void    d0(const float a, const float b, const float c, const float d) {
    std::cout << "\n( with : a = (" << a << ") , b = (" << b << ") , c = (" << c << ") , d = (" << d << ") )" << std::endl;
    std::cout << "\n The solution of this equaton is " << C_VAR << " = " << "-b/2a" << " = " << "-(" << b << ")" << "/2(" << a << ")"  << " = " << reducedFraction((b * -1), (2 * a));
}

void    allReal() {
    std::cout << "\n*------------- Equation of type X = X  --------------------*" << std::endl;
    std::cout << "\n For this equation, every real value can be a solution." << std::endl;
    std::cout << "\n => X ∈ ℝ\n" << std::endl;
}

int deg1(float b, float c) {
    std::cout << "\n*------------- Equation of type aX + b = 0  --------------------*" << std::endl;
    float mc = -c;

    std::cout << "\n 1 -> Get " << C_VAR << " on 1 side : " << b << C_VAR << " = " << mc << std::endl;

    std::cout << "\n 2 -> Get " << C_VAR << " : " << C_VAR << " = " << mc << " / " << b << std::endl;

    std::cout << "\n 3 -> Solution is : " << C_VAR << " = ";
    if (b != 1) {
        std::cout << reducedFraction(mc, b) << std::endl;
    } else {
        std::cout << mc << std::endl;
    }
    return 0;
}

int main(int argc, char** argv) {
    if (!basisArgsParse(argc, const_cast<const char**>(argv))) return 1;

    // float a;
    // float b;
    // float c;
    std::vector<chunk>  equat = {};
    std::vector<chunk>  equal = {};

    std::string input = "";
    for (size_t i = 1; i < static_cast<size_t>(argc); i++) {
        input += argv[i];
    }

    std::cout << "Your input is : " << YELLOW << input << RESET << std::endl;

    try {
        chunkerize(input, equat, equal);
    } catch (...) {
        return 1;
    }


    std::cout << "after chunkerize : " << GREEN;
    for (size_t i = 0; i < equat.size(); i++) {
        printChunk(equat[i], i);
    }
    std::cout << "= ";
    for (size_t i = 0; i < equal.size(); i++) {
        printChunk(equal[i], i);
    }
    std::cout << RESET << std::endl;

    std::cout << "\n*------------- Let's calc reduct form --------------------*" << std::endl;
    std::cout << "\n 1 -> Let's factorize :" << std::endl;
    factorize(equat);
    factorize(equal);
    std::cout << " : " << GREEN;
    for (size_t i = 0; i < equat.size(); i++) {
        printChunk(equat[i], i);
    }
    std::cout << "= ";
    for (size_t i = 0; i < equal.size(); i++) {
        printChunk(equal[i], i);
    }
    std::cout << RESET << std::endl;

    std::cout << "\n 2 -> Reduce each side :" << std::endl;
    reduce(equat);
    reduce(equal);
    printEqua(equat, equal);

    std::cout << "\n 3 -> Reduce Together :" << std::endl;
    reduce(equat, equal);
    printEqua(equat, equal);

    float temp_v = 0;
    for (const chunk& ck : equat) {
        temp_v += ck.values[0].value;
    }
    if (equat.size() == 0 || temp_v == 0) {
        allReal();
        return 0;
    }

    std::cout << "\n*------------- So here's our a, b, c  --------------------*" << std::endl;

    float a = 0;
    float b = 0;
    float c = 0;

    for (const  chunk& ck : equat) {
        if (ck.power == 0) c += ((ck.sign >= 1 ? 1 : -1) * ck.values[0].value);
        if (ck.power == 1) b += ((ck.sign >= 1 ? 1 : -1) * ck.values[0].value);
        if (ck.power == 2) a += ((ck.sign >= 1 ? 1 : -1) * ck.values[0].value);
    }

    std::cout << "\n a : " << a << std::endl;
    std::cout << " b : " << b << std::endl;
    std::cout << " c : " << c << std::endl;

    if (a == 0) {
        return deg1(b , c);
    }

    std::cout << "\n*------------- Now let's calc Δ --------------------*" << std::endl;

    float d = (b * b) - (4 * a * c);

    std::cout << "\n Δ : " << "b² - 4ac = " << "(" << b << ")² - 4(" << a << ")(" << c << ") = " << d << std::endl;

    if (d > 0) {
        std::cout << "\n*------------- Δ is > 0 --------------------*" << std::endl;
        dPos(a, b, c, d);
    } else if (d == 0) {
        std::cout << "\n*------------- Δ is equal to 0 --------------------*" << std::endl;
        d0(a, b, c, d);
    } else {
        std::cout << "\n*------------- Δ is < 0 --------------------*" << std::endl;
        dNeg(a, b, c, d);
    }

    std::cout << std::endl << std::endl;

    return 0;
}