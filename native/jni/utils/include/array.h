#pragma once

#include <stdlib.h>

template <class T>
class Array {
public:
	Array() : _data(0), _size(0), _capacity(0) {}
	~Array() { delete []_data; }

	class iterator {
		friend class Array;

	public:
		iterator(T* n= 0): _node(n) {}
		iterator(const iterator& i): _node(i._node) {}
		~iterator() {} // Should NOT delete _node

		const T& operator * () const { return (*_node); }
		T& operator * () { return (*_node); }
		iterator& operator ++ () {
			++_node;
			return (*this);
		}

		iterator operator ++ (int) {
			iterator temp = *this;
			++_node;
			return temp;
		}

		iterator& operator -- () {
			--_node;
			return (*this);
		}

		iterator operator -- (int) {
			iterator temp = *this;
			--_node;
			return temp;
		}

		iterator operator + (int i) const {
			iterator temp = *this;
			temp += i;
			return temp;
		}

		iterator& operator += (int i) {
			_node += i;
			return (*this);
		}

		iterator& operator = (const iterator& i) {
			_node = i._node;
			return (*this);
		}

		bool operator != (const iterator& i) const {
			return _node != i._node;
		}

		bool operator == (const iterator& i) const { return !(*this != i); }

	private:
		T*    _node;
	};

	T& operator = (const T& a) {
		_size = a._size;
		_capacity = a._capacity;
		if (_capacity) {
			_data = new T[_capacity];
			for(int i = 0; i < _size; ++i)
				_data[i] = (T&&) a[i];
		}
		return *this;
	}

	iterator begin() const { return iterator(_data); }

	iterator end() const { return iterator(_data + _size); }

	bool empty() const { return !_size; }

	size_t size() const { return _size; }

	T& operator [] (size_t i) { return _data[i]; }

	const T& operator [] (size_t i) const { return _data[i]; }

	const T& back() const { return _data[_size - 1]; }

	void push_back(const T& x) {
		if(_size == _capacity)
			expand();
		_data[_size] = x;
		++_size;
	}

	void push_back(T&& x) {
		if(_size == _capacity)
			expand();
		_data[_size] = (T&&) x;
		++_size;
	}

	void pop_front() { erase(begin()); }

	void pop_back() { if(_size) --_size; }

	bool erase(iterator pos) {
		T* d = pos._node;
		if (_size == 0 || d < _data || d >= _data + _size)
			return false;
		for (; d < _data + _size - 1; ++d)
			*d = (T&&) *(d + 1);
		--_size;
		return true;

	}

	bool erase(const T& x) {
		for (T* i = _data; i < _data + _size; ++i) {
			if(*i == x) {
				erase(iterator(i));
				return true;
			}
		}
		return false;
	}

	void clear() { _size = 0; }

	void sort() const {
		qsort(_data, _size, sizeof(T), compare);
	}

	T* data() { return _data; }

	const T* data() const { return _data; }

	// void reserve(size_t n) { ... }
	// void resize(size_t n) { ... }

private:
	T*           _data;
	size_t       _size;       // number of valid elements
	size_t       _capacity;   // max number of elements
	static int(*_cmp)(T&, T&);

	static int compare(const void *a, const void *b) {
		return _cmp ? _cmp(*((T*) a), *((T*) b)) : 0;
	}

	void expand() {
		if (_capacity == 0)
			_capacity = 1;
		else
			_capacity *= 2;
		T* temp = _data;
		_data = new T[_capacity];
		for(int i = 0; i < _size; ++i)
			_data[i] = (T&&) temp[i];
		delete [] temp;
	}
};

template<class T>
int(* Array<T>::_cmp)(T&, T&) = nullptr;
