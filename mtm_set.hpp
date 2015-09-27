#ifndef MTM_SET_HPP_
#define MTM_SET_HPP_

/* The minimum of headers required */
#include <utility>
#include <iterator>
#include <exception>
#include <memory>
#include <assert.h>

/* The C Set generic ADT */
#include "mtm_set.h"

namespace mtm {

	/**
	 * Generic Set Class
	 *
	 * template <class T, class CmpFcn = std::less<T> >
	 * class set
	 *
	 * T - Stored data type
	 * CmpFcn - Function object class performing comparison. Default is
	 * 	   std::less<T>, a (template) class provided in the STL which
	 * 	   uses T::operator<(T const& other) for comparison.
	 *
	 * Implements a set container type. A const_iterator class is provided
	 * to access the elements of the set. Element type (template parameter)
	 * must implement (public) copy constructor, destructor and operator <() .
	 *
	 * Note: set iterator must be constant (disallowing element modification
	 * since that might affect the order of elements in the set.
	 *
	 * The following public members are available:
	 *
	 * Types:
	 *  class const_iterator - allows iteration over set elements
	 *  value_type - typedef for T
	 *  const_reference - typedef for T const&
	 *  result_type - result value of set insert (see set insert documentation).
	 *
	 * Functions:
	 *  set - set constructor. initializes empty set.
	 *  set(const set& other) - copy constructor, copies all elements from other.
	 *  operator= - assignment operator. copies all elements from other.
	 *  ~set - destroys the set and frees all memory allocated.
	 *
	 * Functions for iteration:
	 *  begin - create a const iterator to first element of the set.
	 *  end - create a const iterator to one-past-the-last element of the set.
	 *  cbegin - create a const iterator to first element of the set.
	 *  cend - create a const iterator to one-past-the-last element of the set.
	 *
	 *  Notes:
	 *  1. In the case of the set container, both begin() and cbegin() need to
	 *     return const_iterator (and same for end(),cend()) to disallow editing
	 *     set elements.
	 *     Both versions of functions are needed to provide STL compatibility.
	 *
	 *  2. In C++ 11, iteration over constant containers can be done either by
	 *     iteration:
	 *
	 *     typedef mtm::set<my_element_type> my_set_t;
	 *     my_set_t cont;
	 *
	 *     for(my_set_t::const_iterator it=cont.cbegin(); it!=cont.end(); ++it)
	 *     {
	 *        //--- TODO: DO-SOMETHING ---//
	 *     }
	 *
	 *     Or, directly over the elements:
	 *     for(my_set_t::const_reference el : cont)
	 *     {
	 *       // At each iteration el contains the value of one of the
	 *       //  set elements. Iteration is done in ascending order, induced by
	 *       //  my_element_type::operator <()
	 *     }
	 *
	 * Other member functions:
	 *  size - number of elements in set
	 *
	 *  find - obtain const iterator to element. If element not found, return value
	 *         must compare to set<T>::end();
	 *  find const - identical to non-const find(). Both return const_iterator to
	 *				disallow modification of set elements.
	 *
	 *  insert - inserts element. Return value is a pair <const_iterator, bool>.
	 *           if the element was inserted, the iterator will be pointing to it
	 *           and second will be true. Otherwise (an element with the same value
	 *           exists), element will not be inserted, and the iterator will be
	 *		    pointing to the existing element in the set.
	 *
	 *  erase(T const& element) - erases given value from the set.
	 *  erase(const_iterator iter) - erases element pointed to by iterator.
	 *
	 *  clear() - erases all elements in the set.
	 */

	template<class T, class CmpFcn = std::less<T> >
	class set
	{
	public:
		/** iterator type for the container */
		class const_iterator;
		/** element data type */
		typedef T value_type;
		/** const reference to element data type */
		typedef T const& const_reference;
		/** set insert result type */
		typedef std::pair<const_iterator, bool> result_type;
		/**
		 * Ctor/CCtor/Dtor/assignment operator 
		 *  In case that memory allocation fails constructors/operator= should 
		 *  throw std::bad_alloc. 
		 */
		set();
		set(const set& other);
		set& operator=(set const& other);
		~set();
		/** 
		 * Iteration functions
		 * Should always succeed. Error cases may be handled by assert()
		 */
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const;
		const_iterator cend() const;
		/** returns the number of elements in the set */
		int size() const;
		/** 
		 * find
		 * obtain const iterator to element. If element not found, return value
		 *  must compare to set<T>::end();  
		 */
		const_iterator find(T const&);
		/**
		 * find const 
		 *  a const overload of find. obtain const iterator to element. If 
		 *  element not found, return value must compare to set<T>::cend();
		 */
		const_iterator find(T const&) const;
		/**
		 * insert 
		 *  inserts an element to the set.
		 *  Return value is a pair <const_iterator, bool>.
		 *  - if the element was inserted, the iterator will be pointing to it
		 *    and second will be true.
		 *  - Otherwise(an element with the same value already exists), element 
		 *    will not be inserted, and the iterator will point to the existing 
		 *    element in the set.
		 *
		 *  Does not invalidate iterators. 
		 */
		result_type insert(T const& data);
		/**
		 * erase(T const& element) 
		 *  erases given value from the set. 
		 *  Throws ElementNotFound() if value does not exist in the set. 
		 *
		 *  Does not invalidate iterators pointing to other elements. 
		 */
		void erase(T const& element);
		/**
		 * erase(const_iterator iter) 
		 *  erases element pointed to by iterator.
		 *  Throws InvalidIterator() if iterator does not point to an element. 
		 *
		 *  Does not invalidate iterators pointing to other elements. 
		 */
		void erase(const_iterator iter);
		/**
		 * clear
		 *  erases all elements in the set. After invocation size() returns 0. 
		 */
		void clear();
		//--------------- Exception types: -------------
		// A general set exception class: 
		class Exception: public std::exception
		{
		};
		class ElementNotFound: public Exception
		{
		};
		class InvalidIterator: public Exception
		{
		};

	private:
		/** Underlying C set object */
		Set m_CSet;
		/** Functions for C set object */
		static SetElement CopyElementFcn(SetElement lmnt);
		static void DestroyElementFcn(SetElement lmnt);
		static int CompareElementFcn(SetElement left, SetElement right);
	};

	///////////
	// iterator
	///////////

	/** 
	 * Const iterator class for the generic set implementation. 
	 * Iterator is inherited with and std::forward_iterator_tag to signal to the
	 *	STL that it may only be advanced in the forward direction. 
	 */
	template<class T, class CmpFcn>
	class set<T, CmpFcn>::const_iterator: public std::iterator<
			std::forward_iterator_tag, T>
	{
	public:
		/** Prefix and postfix operators to advance the iterator */
		const_iterator & operator++()
		{
			m_Current = setGetNext(m_Owner->m_CSet, m_Current);
			return *this;
		}
		const_iterator operator++(int)
		{
			const_iterator newIterator(*this);
			++*this;
			return newIterator;
		}

		/**
		 * Dereference operator to obtain value the iterator points to. 
		 * returns const reference type since this is const_iterator. 
		 */
		T const& operator*() const;

		/** auto-generated functions that are kept as is */
		const_iterator(const_iterator const&) = default;
		const_iterator& operator=(const_iterator const&) = default;
		~const_iterator() = default;

		bool operator==(const_iterator const& other) const;
		bool operator!=(const_iterator const& other) const;

	private:
		/** Needed to allow the set access the private constructor */
		friend class set;

		/** Set object the iterator belongs to */
		set<T, CmpFcn> const* m_Owner;

		/** Element of the C implementation the iterator currently points to */
		SetIterator m_Current;

		/** Constructor for the iterator, to be used by set<T>::begin() */
		const_iterator(set<T, CmpFcn> const* owner, SetIterator cset_iter);
	};

	///////////
	// set funcs
	///////////

	template<class T, class CmpFcn>
	set<T, CmpFcn>::set() :
			m_CSet(NULL)
	{
		m_CSet = setCreate(CopyElementFcn, DestroyElementFcn,
				CompareElementFcn);
		if (NULL == m_CSet) {
			throw Exception();
		}
	}

	template<class T, class CmpFcn>
	set<T, CmpFcn>::set(const set& sourceSet) :
			m_CSet(NULL)
	{
		m_CSet = setCopy(sourceSet.m_CSet);
		if (NULL == m_CSet) {
			throw Exception();
		}
	}

	template<class T, class CmpFcn>
	set<T, CmpFcn>& set<T, CmpFcn>::operator=(const set<T, CmpFcn>& sourceSet)
	{
		if (this == &sourceSet) {
			return *this;
		}
		setDestroy(m_CSet);
		m_CSet = setCopy(sourceSet.m_CSet);
		if (NULL == m_CSet) {
			throw Exception();
		}
		return *this;
	}

	template<class T, class CmpFcn>
	set<T, CmpFcn>::~set()
	{
		setDestroy(m_CSet);
	}

	template<class T, class CmpFcn>
	int set<T, CmpFcn>::size() const
	{
		assert(m_CSet != NULL);
		return setGetSize(m_CSet);
	}

	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::const_iterator set<T, CmpFcn>::find(
			T const& element)
	{
		for (set<T, CmpFcn>::const_iterator it = begin(); it != end(); ++it) {
			if (*it == element) {
				return it;
			}
		}
		throw ElementNotFound();
	}

	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::const_iterator set<T, CmpFcn>::find(
			T const& element) const
	{
		for (set<T, CmpFcn>::const_iterator it = begin(); it != end(); ++it) {
			if (*it == element) {
				return it;
			}
		}
		throw ElementNotFound();
	}

	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::result_type set<T, CmpFcn>::insert(T const& data)
	{
		assert(m_CSet != NULL);
		T* dataPtr = new T(data);
		SetResult res = setAdd(m_CSet, static_cast<SetElement>(dataPtr));
		delete dataPtr;
		assert(res != SET_NULL_ARGUMENT);
		if (res == SET_OUT_OF_MEMORY) {
			throw Exception();
		}
		if (res == SET_ITEM_ALREADY_EXISTS) {
			return result_type(find(data), false);
		}
		return result_type(find(data), true); // SET_SUCCESS
	}

	template<class T, class CmpFcn>
	void set<T, CmpFcn>::erase(T const& element)
	{
		assert(m_CSet != NULL);
		T* lmntNew = new T(element);
		if (setRemove(m_CSet, static_cast<SetElement>(lmntNew))
				== SET_ITEM_DOES_NOT_EXIST) {
			delete lmntNew;
			throw ElementNotFound();
		}
		delete lmntNew;
	}

	template<class T, class CmpFcn>
	void set<T, CmpFcn>::erase(set<T, CmpFcn>::const_iterator iter)
	{
		erase(*iter);
		// if iter does not point to an element, *iter will throw InvalidIterator()
	}

	template<class T, class CmpFcn>
	void set<T, CmpFcn>::clear()
	{
		assert(m_CSet != NULL);
		setClear(m_CSet);
	}

	///////////
	// private set funcs
	///////////

	template<class T, class CmpFcn>
	int set<T, CmpFcn>::CompareElementFcn(SetElement left, SetElement right)
	{
		if (NULL == left || NULL == right)
			return 0;

		T const& leftT = *static_cast<T*>(left);
		T const& rightT = *static_cast<T*>(right);
		if (CmpFcn().operator()(leftT, rightT))
			return -1;
		if (CmpFcn().operator()(rightT, leftT))
			return 1;
		return 0;
	}

	template<class T, class CmpFcn>
	SetElement set<T, CmpFcn>::CopyElementFcn(SetElement lmnt)
	{
		if (lmnt == NULL) {
			return NULL;
		}
		const T& lmntT = *static_cast<T*>(lmnt);
		T* lmntNew = new T(lmntT);
		return static_cast<SetElement>(lmntNew);
	}

	template<class T, class CmpFcn>
	void set<T, CmpFcn>::DestroyElementFcn(SetElement lmnt)
	{
		if (lmnt == NULL) {
			return;
		}
		T* lmntTptr = static_cast<T*>(lmnt);
		delete lmntTptr;
	}

	////////
	// iteration funcs
	////////

	// iterator funcs in set
	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::const_iterator set<T, CmpFcn>::begin() const
	{
		return typename set<T, CmpFcn>::const_iterator::const_iterator(this,
				setGetFirst(m_CSet));
	}

	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::const_iterator set<T, CmpFcn>::cbegin() const
	{
		return begin();
	}

	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::const_iterator set<T, CmpFcn>::end() const
	{
		const_iterator iterator = begin();
		while (iterator.m_Current != NULL) {
			++iterator;
		}
		return iterator;
	}

	template<class T, class CmpFcn>
	typename set<T, CmpFcn>::const_iterator set<T, CmpFcn>::cend() const
	{
		return end();
	}

	// iterator funcs in iterator
	template<class T, class CmpFcn>
	set<T, CmpFcn>::const_iterator::const_iterator(set<T, CmpFcn> const* owner,
			SetIterator cset_iter) :
			m_Owner(owner), m_Current(cset_iter)
	{
	}

	template<class T, class CmpFcn>
	T const& set<T, CmpFcn>::const_iterator::operator*() const
	{
		SetElement element = setGetElement(m_Owner->m_CSet, m_Current);
		if (element == NULL) {
			throw InvalidIterator();
		}
		return *static_cast<T*>(element);
	}

	template<class T, class CmpFcn>
	bool set<T, CmpFcn>::const_iterator::operator!=(
			const_iterator const& other) const
	{
		return !(*this == other);
	}

	template<class T, class CmpFcn>
	bool set<T, CmpFcn>::const_iterator::operator==(
			const_iterator const& other) const
	{
		return m_Current == other.m_Current;
	}

////////////////////////////////////////////////////////
//########## Add other functions' implementation here.
////////////////////////////////////////////////////////
}

#endif // #ifndef MTM_SET_H_