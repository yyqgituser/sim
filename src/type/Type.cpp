#include "Type.h"

namespace type {

Type::Type(Kind kind, string name) :
		kind(kind), name(name) {
}

Type::~Type() {
}

bool Type::isNumer() {
	return kind == INT_TYPE || kind == DOUBLE_TYPE;
}

} /* namespace type */
