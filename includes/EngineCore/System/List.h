#pragma once

#include <typeinfo>
#include <cstddef>

class _list_parent
{
public:
	_list_parent()
		: m_size(0){}

	~_list_parent(){}

protected:
	std::size_t m_size;
};

template <class _Ty>
class linked_list : _list_parent
{
public:
	struct ListNode
	{
		_Ty value;
		ListNode* m_next;

		ListNode(_Ty _value)
			: value(_value)
			, m_next(nullptr)
		{

		}
	};

	linked_list()
		: _list_parent()
		, m_first(nullptr)
		, m_last(nullptr)
	{
	}

	~linked_list(){}

	bool is_empty()
	{
		return m_first == nullptr;
	};

	void push_back(_Ty value)
	{
		ListNode* p = new ListNode(value);
		m_size++;
		if (is_empty())
		{
			m_first = p;
			m_last = p;
			return;
		}
		m_last->m_next = p;
		m_last = p; 
	};

	_Ty last()
	{
		return m_last->value;
	}

	ListNode* find(_Ty _val) {
		ListNode* p = m_first;
		while (p && p->value != _val) p = p->m_next;
		return (p && p->value == _val) ? p : nullptr;
	}

	void remove_first() {
		if (is_empty()) return;
		ListNode* p = m_first;
		m_first = p->m_next;
		delete p;
		m_size--;
	}

	void remove_last() {
		if (is_empty()) return;
		if (m_first == m_last) {
			remove_first();
			return;
		}
		ListNode* p = m_first;
		while (p->m_next != m_last) p = p->m_next;
		p->m_next = nullptr;
		delete m_last;
		m_last = p;
		m_size--;
	}

	void remove(_Ty _val) {
		if (is_empty()) return;
		if (m_first->value == _val) {
			remove_first();
			return;
		}
		else if (m_last->value == _val) {
			remove_last();
			return;
		}
		ListNode* slow = m_first;
		ListNode* fast = m_first->m_next;
		while (fast && fast->value != _val) {
			fast = fast->m_next;
			slow = slow->m_next;
		}
		if (!fast) {
			return;
		}
		slow->m_next = fast->m_next;
		delete fast;
		m_size--;
	}

	void remove(const std::size_t index)
	{
		if (is_empty()) return;
		_Ty _val = at(index);
		if (m_first->value == _val) {
			remove_first();
			return;
		}
		else if (m_last->value == _val) {
			remove_last();
			return;
		}
		ListNode* slow = m_first;
		ListNode* fast = m_first->m_next;
		while (fast && fast->value != _val) {
			fast = fast->m_next;
			slow = slow->m_next;
		}
		if (!fast) {
			return;
		}
		slow->m_next = fast->m_next;
		delete fast;
		m_size--;
	}

	_Ty operator[] (const std::size_t index) {
		if (is_empty()) return nullptr;
		ListNode* p = m_first;
		for (std::size_t i = 0; i < index; i++) {
			p = p->m_next;
			if (!p) return nullptr;
		}
		return p->value;
	}

	_Ty at(const std::size_t index) {
		if (is_empty()) return nullptr;
		ListNode* p = m_first;
		for (std::size_t i = 0; i < index; i++) {
			p = p->m_next;
			if (!p) return nullptr;
		}
		return p->value;
	}

	std::size_t size()
	{
		return m_size;
	}

	void clear()
	{
		if (is_empty()) return;
		ListNode* p = m_first->m_next;
		ListNode* pt = m_first;
		for (std::size_t i = 0; i < m_size - 1; i++) {
			delete pt;
			pt = p;
			if (p != nullptr) p = p->m_next;
		}
		delete p;
		m_size = 0;
		m_first = nullptr;
		m_last = nullptr;
	}

	// This function create new array, take care and don't forget delete this array
	_Ty* data()
	{
		if (is_empty()) return nullptr;
		_Ty* p = new _Ty[m_size];
		ListNode* pData = m_first;
		for (int i = 0; i < m_size; i++) {
			p[i] = pData->value;
			pData = pData->m_next;
		}
		return p;
	}

private:
	ListNode* m_first;
	ListNode* m_last;
};
