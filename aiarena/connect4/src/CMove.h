#pragma once
#include <string>

namespace Connect4 {
class CMove {

public:
int column;
CMove(const int c);
std::string toPDN() const;
};

}
