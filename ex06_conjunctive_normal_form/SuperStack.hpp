#ifndef SUPERSTACK_HPP
#define SUPERSTACK_HPP

#include <exception>
#include <set>
#include <stack>

template <typename T>
class SuperStack : public std::stack<T> {
   public:
	SuperStack();
	SuperStack(SuperStack<T> const& st);
	SuperStack(SuperStack<T>&& st);
	virtual ~SuperStack();
	SuperStack<T>& operator=(SuperStack<T> const& st);
	SuperStack<T>& operator=(SuperStack<T>&& st);

	T	 popout();
	void addToTrash(T);
	bool isTrash(T);

	class SuperStackEmptyException : public std::exception {
	   public:
		virtual const char* what() const throw() {
			return ("SuperStack:exception: stack is empty");
		}
	};

   private:
	std::set<T> trash;
};

template <typename T>
SuperStack<T>::SuperStack() : std::stack<T>() {}

template <typename T>
SuperStack<T>::SuperStack(SuperStack<T> const& st)
	: std::stack<T>(static_cast<std::stack<T> >(st)) {}

template <typename T>
SuperStack<T>::SuperStack(SuperStack<T>&& st) : std::stack<T>(static_cast<std::stack<T> >(st)) {}

template <typename T>
SuperStack<T>::~SuperStack() {}

template <typename T>
SuperStack<T>& SuperStack<T>::operator=(SuperStack<T> const& st) {
	if (this != &st)
		static_cast<std::stack<T> >(*this) = static_cast<std::stack<T> >(st);
	return (*this);
}

template <typename T>
SuperStack<T>& SuperStack<T>::operator=(SuperStack<T>&& st) {
	if (this != &st)
		static_cast<std::stack<T> >(*this) = move(static_cast<std::stack<T> >(st));
	return (*this);
}

template <typename T>
T SuperStack<T>::popout() {
	if (this->empty())
		throw(SuperStackEmptyException());
	T top_element = this->top();
	this->pop();
	return (top_element);
}

template <typename T>
void SuperStack<T>::addToTrash(T t) {
	trash.insert(t);
}

template <typename T>
bool SuperStack<T>::isTrash(T t) {
	bool						   found = false;
	typename std::set<T>::iterator it = trash.find(t);
	if (it != trash.end()) {
		found = true;
	}
	return (found);
}
#endif
