#ifndef SUPERSTACK_HPP
#define SUPERSTACK_HPP

#include <exception>
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

	T popout();

   private:
	class SuperStackEmptyException : public std::exception {
	   public:
		virtual const char* what() const throw() {
			return ("SuperStack:exception: stack is empty");
		}
	};
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
#endif
