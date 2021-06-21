#ifndef STACKS_H
#define STACKS_H

#include <iostream>
using namespace std;

// just a piece of code that determines if the passed parameter is an iterator
template <class _iter>
using _iter_cat_t = typename iterator_traits<_iter>::iterator_category;

template <class... _Types>
using void_tt = void;

template <class _Ty, class = void>
constexpr bool _is_iterator_v_stack = false;

template <class _Ty>
constexpr bool _is_iterator_v_stack<_Ty, void_tt<_iter_cat_t<_Ty>>> = true;

#define init_name(x) x.name = #x

namespace Py {
	// This Stack will consist of a Binary Node and will be Implemented using Doubly Linked List
	template<typename T>
	struct DualNode {
		DualNode<T>* prev;
		T data;
		DualNode<T>* next;
	};

	/*
	This Stack Implmenatation Contains Methods like:
		- an initialiser_list<> based construction
		- an iterator based construction
		- show
		- push
		- pop
		- top
		- clear
		- dump
		- min
		- max

	*/

	template<typename T>
	class Stack {
		using pointer = DualNode<T>*;
		using value = DualNode<T>;

		pointer _top_ = NULL;
		pointer _bottom_ = NULL;

		int _size_ = 0;

	public:
		char const* name = "none";

		Stack() = default;
		Stack(initializer_list<T> init_l) {
			typename initializer_list<T>::iterator it = init_l.begin();

			this->_bottom_ = new value{NULL};
			this->_bottom_->data = *it;
			this->_bottom_->prev = NULL;
			this->_bottom_->next = NULL;

			++it;

			this->_size_++;
			this->_top_ = this->_bottom_;

			while (it != init_l.end()) {
				pointer temp = new value{NULL};
				temp->data = *it;
				temp->prev = NULL ;
				temp->next = NULL;

				// linking the top to temp
				this->_top_->next = temp;
				temp->prev = this->_top_;

				// making temp the top
				this->_top_ = temp;

				temp = NULL;
				this->_size_++;
				++it;
			}
		}

		// supports iterators that have operator * and ++ overloaded
		template<class _Iter, enable_if_t<_is_iterator_v_stack<_Iter>, int> = 0>
		Stack(_Iter begin, _Iter end) {
			_Iter it = begin;
			this->_bottom_ = new value{ NULL };
			this->_bottom_->data = *it;
			this->_bottom_->prev = NULL;
			this->_bottom_->next = NULL;

			++it;

			this->_size_++;
			this->_top_ = this->_bottom_;

			while (it != end) {
				pointer temp = new value{ NULL };
				temp->data = *it;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_top_->next = temp;
				temp->prev = this->_top_;

				// making temp the top
				this->_top_ = temp;

				temp = NULL;
				this->_size_++;
				++it;
			}
		}

		T top() { 
			if (this->_top_) return this->_top_->data;
			else return T{};
		}
		int size() { return this->_size_; }

	private:
		void null_out() { this->_top_ = NULL; this->_bottom_ = NULL; this->_size_ = 0; }
		
		void _copy_values_(pointer begin) {
			this->_bottom_ = new value{ NULL };
			this->_bottom_->data = begin->data;
			this->_bottom_->prev = NULL;
			this->_bottom_->next = NULL;

			begin = begin->next;

			this->_size_++;
			this->_top_ = this->_bottom_;

			while (begin) {
				pointer temp = new value{ NULL };
				temp->data = begin->data;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_top_->next = temp;
				temp->prev = this->_top_;

				// making temp the top
				this->_top_ = temp;

				temp = NULL;

				this->_size_++;
				begin = begin->next;
			}
		}

	public:
		// copy construtor
		Stack(const Stack& obj) {
			if (obj._size_ == 0) {
				this->null_out();
			}
			else {
				this->_copy_values_(obj._bottom_);
			}
		}

		// move constructor
		Stack(Stack&& obj) noexcept {
			this->_top_ = obj._top_;
			this->_bottom_ = obj._bottom_;
			this->_size_ = obj._size_;
			obj.null_out();
		}

		// copy assignments
		Stack& operator=(const Stack& obj) {
			this->clear();
			this->null_out();

			if (obj._size_ == 0) {}
			else {this->_copy_values_(obj._bottom_);}

			return *this;
		}

		// move assignments
		Stack& operator=(Stack&& obj) noexcept  {
			this->clear();
			this->null_out();

			this->_top_ = obj._top_;
			this->_bottom_ = obj._bottom_;
			this->_size_ = obj._size_;

			obj.null_out();
			return *this;
		}

		Stack& push(T elem) {
			if (this->_size_ == 0) {
				this->_bottom_ = new value{ NULL };
				this->_bottom_->data = elem;
				this->_bottom_->prev = NULL;
				this->_bottom_->next = NULL;

				this->_top_ = this->_bottom_;
			}

			else {
				pointer temp = new value{ NULL };
				temp->data = elem;
				temp->prev = NULL;
				temp->next = NULL;

				// linking the top to temp
				this->_top_->next = temp;
				temp->prev = this->_top_;

				// making temp the top
				this->_top_ = temp;

				temp = NULL;
			}

			this->_size_++;

			return *this;
		}

		Stack& dump(Stack obj) {
			if (obj._size_ == 0) {}
			else {
				// dumping in this context means putting another stack on top of this stack
				// in other words concatenation
				this->_top_->next = obj._bottom_;
				obj._bottom_->prev = this->_top_;

				this->_top_ = obj._top_;
				this->_size_ += obj._size_;

				obj.null_out();
			}
			return *this;
		}

		T pop() {
			if (this->_size_ == 0) { return T{}; }
			else {
				T val;
				pointer temp = this->_top_;
				// top shifts down
				this->_top_ = this->_top_->prev;

				// temp is deleted
				val = temp->data;
				delete temp;
				temp = NULL;
				this->_size_--;
				return val;
			}
		}

		T max() {
			T max_val = this->_bottom_->data;
			pointer temp = this->_bottom_;
			while (temp) {
				if (temp->data > max_val) { max_val = temp->data; }
				temp = temp->next;
			}
			return max_val;
		}

		T min() {
			T min_val = this->_bottom_->data;
			pointer temp = this->_bottom_;
			while (temp) {
				if (temp->data < min_val) { min_val = temp->data; }
				temp = temp->next;
			}
			return min_val;
		}

		void show() {
			pointer temp = this->_top_;
			while (temp) {
				cout << temp->data << " ";
				temp = temp->prev;
			}cout << endl;
			temp = NULL;
		}

		Stack& clear() {
			pointer temp = NULL;
			while (this->_top_) {
				temp = this->_top_;
				this->_top_ = this->_top_->prev;
				delete temp;
				temp = NULL;
			}
			return *this;
		}

		void log_properties(char const* entry = "", bool log_obj_details = false, bool show_log_count = true) {
			static int _i = 0;

			if (show_log_count)
				cout << "Log Record Count: " << _i << endl;

			if (entry != "")
				cout << "Debug Entry: " << entry << endl;

			if (log_obj_details) {
				cout << "Object Address: " << this << endl;
				cout << "Object Type: " << typeid(*this).name() << endl;
			}

			cout << "Object Name: " << this->name << endl;
			cout << "Content: "; this->show();
			cout << "Size: " << this->_size_ << endl;
			cout << endl;
			_i++;
		}

		~Stack() {
			this->clear();
			this->_size_ = 0;
		}
	};
}

#endif // !STACKS_H

