#include "Scope.h"

template<class T> Scope<T>::Scope(shared_ptr<Scope<T>> next) :
		next(next) {
}

template<class T> Scope<T>::~Scope() {
}

template<class T> shared_ptr<T> Scope<T>::lookup(u32string name) {
	auto itr = defs.find(name);
	if (itr != defs.end()) {
		return (*itr).second;
	}
	if (next) {
		return next->lookup(name);
	}
	return 0;
}

template<class T> shared_ptr<T> Scope<T>::lookupLocal(u32string name) {
	auto itr = defs.find(name);
	if (itr != defs.end()) {
		return (*itr).second;
	}
	return 0;
}

template<class T> void Scope<T>::add(u32string name, shared_ptr<T> type) {
	defs[name] = type;
}

