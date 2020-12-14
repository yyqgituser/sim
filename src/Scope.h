#ifndef SRC_SCOPE_H_
#define SRC_SCOPE_H_

#include <memory>
#include <map>
#include <string>

using namespace std;

template <class T>
class Scope {
public:
	shared_ptr<Scope<T>> next;

	map<u32string, shared_ptr<T>> defs;

	Scope(shared_ptr<Scope<T>> next):
		next(next) {
	}

	virtual ~Scope() {}

	shared_ptr<T> lookup(u32string name) {
		auto itr = defs.find(name);
		if (itr != defs.end()) {
			return (*itr).second;
		}
		if (next) {
			return next->lookup(name);
		}
		return 0;
	}

	shared_ptr<T> lookupLocal(u32string name) {
		auto itr = defs.find(name);
		if (itr != defs.end()) {
			return (*itr).second;
		}
		return 0;
	}

	void add(u32string name, shared_ptr<T> symbol) {
		defs[name] = symbol;
	}
};

#endif /* SRC_SCOPE_H_ */
