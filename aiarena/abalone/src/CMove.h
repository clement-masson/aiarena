#pragma once
#include <string>

namespace Abalone {
class CMove {

public:
int from_start, from_end, to_start;

CMove(const int s, const int e);
CMove(const int s, const int e, const int d);
std::string toPDN() const;
};

}
