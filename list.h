#ifndef _LIST_H
#define _LIST_H
/*
API:	begin, end, empty, size,				//��������
		insert, push_front, push_back,			//����
		erase, pop_front, pop_back, clear		//ɾ��
		search,									//����
*/
#include<stdlib.h>
/***********************************************
*				_list_node �ڵ�
************************************************/
template<class T>
struct _list_node {
	_list_node<T>* next = NULL;	// successor node, or first element if head
	_list_node<T>* prev = NULL;	// predecessor node, or last element if head
	T data;
};
/***********************************************
*				_list_iterator ������
*	������������ָ��1ist �Ľڵ㣬���ܽ�����ȷ�ĵ������ݼ���ȡֵ����Ա��ȡ�Ȳ�����
************************************************/
template<class T>
struct _list_iterator {
	typedef _list_iterator<T> self;

	_list_node<T>* node;

	_list_iterator() {}
	_list_iterator(_list_node<T>* node) { this->node = node; }
	/***********************************************
	*				���������
	************************************************/
	/*----------------�Ƚ� [ == ][ != ]----------------*/
	bool operator== (const self& x) const { return node == x.node; }
	bool operator!= (const self& x) const { return node != x.node; }
	/*----------------���� [ ++ ]----------------*/
	self& operator++ () {
		node = node->next;
		return *this;
	}
	/*----------------�ݼ� [ -- ]----------------*/
	self& operator-- () {
		node = node->prev;
		return *this;
	}
	/*----------------ָ�� [ * ][ -> ]----------------*/
	T operator* ()const { return (*node).data; }
	T* operator-> ()const { return &(operator*()); }
};
/***********************************************
*				list (��) ��״˫������
*
************************************************/
template<class T>
class list
{
public:
	typedef _list_iterator<T> iterator;
	typedef _list_node<T> node;
private:
	node* head;		//��״˫������ͷ��ַ

public:
	list() {
		head = new node;
		head->next = head;
		head->prev = head;
	}
	/*----------------��ĩ��ַ [ begin ][ end ]----------------*/
	iterator begin() { return iterator(head->next); }	//����ʼ��ַ
	iterator end() { return iterator(head); }			//���������ַ
	/*----------------�Ƿ�Ϊ�� [ empty ]----------------*/
	bool empty() { return head.next == head; }
	/*----------------Ԫ�ض��� [ size ]----------------*/
	int size() {
		int ans = 0;
		for (iterator it = begin(); it != end(); ++it) {
			ans++;
		}return ans;
	}
	/*----------------���� [ insert ]----------------*/
	node* insert(iterator position, const T& x) {
		node* tmp = new node();
		node* old = position.node;
		tmp->data = x;
		tmp->next = old;
		tmp->prev = old->prev;
		old->prev->next = tmp;
		old->prev = tmp;
		return tmp;
	}
	/*----------------ɾ�� [ erase ]----------------*/
	void erase(iterator position) {
		node* prev = position.node->prev;
		node* next = position.node->next;
		prev->next = next;
		next->prev = prev;
		delete position.node;
	}
	/*----------------��ĩԪ�� ����,ɾ��----------------*/
	void push_front(const T& x) { insert(begin(), x); }
	void push_back(const T& x) { insert(end(), x); }
	void pop_front() { erase(begin()); }
	void pop_back() { erase(--end()); }
	/*----------------ȫ�� [ clear ]----------------*/
	void clear() {
		for (iterator it = begin(); it != end();) {
			iterator temp = it;
			++it;
			erase(temp);
		}
	}
	/*----------------���� [ search ]----------------*/
	iterator search(const T& x) {
		for (iterator it = begin(); it != end(); ++it) {
			if (it.node->data == x)return it;
		}return NULL;
	}
};
#endif
/*/============ example ============
#include<iostream>
using namespace std;
struct STU
{
	int age;
	bool operator == (const STU& x) { return age == x.age; }
};
int main()
{
	list<STU> stulist;
	cout << "size = " << stulist.size() << endl;
	stulist.push_back({ 12 }); stulist.push_back({ 4 });
	stulist.push_back({ 15 }); stulist.push_front({ 16 });
	stulist.push_front({ 19 });
	cout << "size = " << stulist.size() << endl;
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;

	stulist.pop_front();
	stulist.pop_back();
	cout << "size = " << stulist.size() << endl;
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;

	list<STU>::iterator a = stulist.search({ 4 });
	cout << "search: " << (*a.node).data.age << '\n';
	stulist.erase(a);
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;

	stulist.clear();
	cout << "size = " << stulist.size() << endl;
	for (list<STU>::iterator it = stulist.begin(); it != stulist.end(); ++it) {
		cout << (*it.node).data.age << ' ';
	}cout << endl;
	return 0;
}
/*/