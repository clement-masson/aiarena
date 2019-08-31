#pragma once
#include <string>

namespace Abalone {
class CMove {

public:
int from_start, from_end, to_start;

CMove(const int s, const int e);
CMove(const int f, const int f2, const int t);
std::string toPDN() const;
};

}
