#pragma once
#pragma kbuild expand

#include <iosfwd>
#include <utility>
#include <vector>

// detail fwd

template<class T, class U>
std::istream& operator>>(std::istream&, std::pair<T, U>&);

template<class T, class U>
std::ostream& operator<<(std::ostream&, const std::pair<T, U>&);

template<class T>
std::istream& operator>>(std::istream&, std::vector<T>&);

template<class T>
std::ostream& operator<<(std::ostream&, const std::vector<T>&);