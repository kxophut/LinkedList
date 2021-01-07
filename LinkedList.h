#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <iostream>
#include <sstream>

#include "Iterator.h"
#include "Exception.h"
#include "toString.h"



///класс блока списка (информация + указатели на следующий и предыдущий блоки)
template<typename T>
class Node{
private:

    T data;

    Node<T>* Prev = nullptr;

    Node<T>* Next = nullptr;

public:

    Node(){
        Prev = nullptr;
        Next = nullptr;
    }

    Node(T data_){
        data= data_;
        Prev= nullptr;
        Next= nullptr;
    }

    Node (const Node<T> &node){
        data = node.data;
        Prev = node.Prev;
        Next = node.Next;
    }

    ~Node()
    {
            Next->SetPrev(Prev);
            Prev->SetNext(Next);
    }

    T GetData(){return data;};

    Node<T>* GetNext(){ return Next; };

    Node<T>* GetPrev(){ return Prev; };

    void SetData(T Data){ data = Data; };

    void SetNext(Node<T>* node){Next=node;};

    void SetPrev(Node<T>* node){Prev=node;};
};

template <class T>
class ListIterator: public Iterator<T>
{
private:
    Node<T>* current;
public:

    ListIterator(Node<T>* curr):
            current(curr)
    {}

    bool Next() override{
        current = current->GetNext();
        if(current == nullptr) return false;
        else return true;
    };

    bool Prev() override{
        current = current->GetPrev();
        if(current == nullptr) return false;
        else return true;
    };

    T GetData() override{
        if(this->isValuable()) return current->GetData();
        else throw Exception(1);
    };

    bool isEqual(const T& item) {
        if(this->isValuable()) return (current->GetData() == item);
        else throw Exception(1);
    };
    bool isValuable() override{
        return (current != nullptr);
    };
};





///Двусвязный список из Node содержащий голову, хвост и число элементов
template<typename T>
class LinkedList:Iterable<T> {
private:

    int count;

    Node<T>* head;

    Node<T>* tail;

    friend class ListIterator<T>;

public:

    //Конструкторы


    LinkedList (){
        head = nullptr;
        tail = nullptr;
        count = 0;
    }   //Конструктор пустого списка


    LinkedList (const T* items, int itemsCount){
        if (itemsCount > 0){
            count = itemsCount;
            auto* temp = new Node<T>;
            Node<T>* Next = nullptr;
            temp->SetData(*items);
            head = temp;
            for (int i = 1; i<count; i++){
                Next = new Node<T>;
                temp->SetNext(Next);
                Next->SetPrev(temp);
                Next->SetData(*(items+i));
                temp = Next;
            }
            tail = temp;
        }
        else
            throw Exception(3);
    }   //Копирующий конструктор из массива

    LinkedList (const LinkedList <T> & list ) {
        if (list.head != nullptr){
            count = list.count;
            Node<T>* temp = new Node<T>;
            Node<T>* tempList = list.head;
            Node<T>* Next;
            temp->SetData(tempList->GetData());
            head = temp;
            tempList = tempList->GetNext();
            for (int i = 1; i<count; i++){
                Next = new Node<T>;
                temp->SetNext(Next);
                Next->SetPrev(temp);
                Next->SetData(tempList->GetData());
                temp = Next;
                tempList = tempList->GetNext();
            }
            tail = temp;
        }
        else{
            head = nullptr;
            tail = nullptr;
            count = 0;
        }
    }//Копирующий конструктор

    ~LinkedList() = default;    //Деструктор

    void Delete(){
        while ((head) && head != tail) {
            Node<T> *next = head->GetNext();
            delete head;
            head = next;
        }
        if(head) delete head;
    }

    /// Геттеры-Сеттеры

    int getCount(){ return count; };

    void setCount(int count_){ count = count_;};

    Node<T>* getHead(){ return head; };

    void setHead(Node<T>* head_){ head = head_;};

    Node<T>* getTail(){ return tail; };

    void setTail(Node<T>* tail_){ tail = tail_;};


    ///Декомпозиция

    T GetFirst(){
        if (head!= nullptr){
            return head->GetData();
        }
        else throw Exception(2);
    }   //взятие первого элемента

    T GetLast(){
        if (tail != nullptr){
            return tail->GetData();
        }
        else throw Exception(2);
    }   //взятие последнего элемента

    T Get(const int &index){
        if (index<count && index>=0){
            if (index == 0) return head->GetData();
            Node<T>* temp = head;
            for (int i = 0; i<index; i++) temp = temp->GetNext();
            return temp->GetData();
        }
        else throw Exception(1);
    }   //взятие элемента по индексу

    LinkedList<T>* GetSubList(const int &startIndex, const int &endIndex){
        if(startIndex>=0 && endIndex>=startIndex && endIndex<count) {
            Node<T> *start = head;
            for (int i = 0; i < startIndex; i++) start = start->GetNext();
            Node<T> *end;
            end = start;
            for (int i = startIndex; i < endIndex; i++) end = end->GetNext();
            LinkedList<T> *list = new LinkedList<T>;
            Node<T>* Start = new Node<T>;
            Node<T>* End;
            if (startIndex == endIndex){
                Start->SetData(start->GetData());
                End = Start;
            }
            else{
                Start->SetData(start->GetData());
                Node<T>* temp = start;
                Node<T>* tempNew;
                Node<T>* tempPrev = Start;
                while(temp!=end){
                    temp = temp->GetNext();
                    tempNew = new Node<T>;
                    tempPrev->SetNext(tempNew);
                    tempNew->SetPrev(tempPrev);
                    tempNew->SetData(temp->GetData());
                    tempPrev = tempNew;
                }
                End = tempNew;
            }
            list->setHead(Start);
            list->setTail(End);
            list->setCount(endIndex - startIndex + 1);
            return list;
        }
        else throw Exception(1);
    }   //взятие подсписка по начальному и конечному индексам

    void Set(T data, const int &index){
        if(index < 0 || index >= count)  throw Exception(1);
        else{
            if(index == 0) head->SetData(data);
            else {
                Node<T> *temp = head;
                for (int i = 0; i < index; i++)
                    temp = temp->GetNext();
                temp->SetData(data);
            }
        }
    }   //замена индексованного элемента в списке на заданный

    int GetLength(){
        return count;
    }   //длина списка

    //Операции

    void Append(const T &item){
        if(head!= nullptr) {
            Node<T> *New = new Node<T>(item);
            New->SetNext(head);
            head->SetPrev(New);
            head = New;
            count++;
        }
        else {
            Node<T> *New = new Node<T>(item);
            head = New;
            tail = head;
            count++;
        }
    }   //Дописывание элемента с начала

    void Prepend(const T &item){
        if (head!= nullptr) {
            Node<T> *New = new Node<T>(item);
            New->SetPrev(tail);
            tail->SetNext(New);
            tail = New;
            count++;
        }
        else {
            Node<T>* New = new Node<T>(item);
            head = New;
            tail = head;
            count++;
        }
    }   //Дописывание элемента с конца

    void InsertAt(const T &item, const int &index){
        if (index<=count && count>0){
            Node<T>* temp = head;
            for(int i = 1;i<index;i++) temp=temp->GetNext();
            Node<T>* New = new Node<T>(item);
            New->SetNext(temp);
            New->SetPrev(temp->GetPrev());
            temp->SetPrev(New);
            count++;
        }
        else throw Exception(1);
    }   //Вставка элемента в список по номеру

    void Swap (const int &startIndex, const int &endIndex){
        if ( startIndex < 0 || endIndex >= count || endIndex < 0 || startIndex >= count) throw Exception(1);
        if (abs(endIndex - startIndex) == 1){
            Node<T> *start = head;
            Node<T> *end = head;
            for (int i = 0; i<startIndex; i++) start = start->GetNext();
            for(int j = 0; j<endIndex; j++) end = end->GetNext();

            Node<T> * prev = start->GetPrev();
            Node<T> * next = end->GetNext();

            prev->SetNext(end);
            end->SetNext(start);
            start->SetNext(next);
            next->SetPrev(start);
            start->SetPrev(end);
            end->SetPrev(prev);
        }
        else
        if (startIndex != endIndex) {
            Node<T> *start = head;
            Node<T> *end = head;
            for (int i = 0; i<startIndex; i++) start = start->GetNext();
            for(int j = 0; j<endIndex; j++) end = end->GetNext();

            Node<T>* Next = start->GetNext();
            Node<T>* Prev = start->GetPrev();

            start->SetNext(end->GetNext());
            end->GetNext()->SetPrev(start);
            start->SetPrev(end->GetPrev());
            end->GetPrev()->SetNext(start);

            end->SetNext(Next);
            Next->SetPrev(end);
            end->SetPrev(Prev);
            Prev->SetNext(end);
        }
    }   //Смена двух элементов по индексам

    LinkedList<T>* Concat(const LinkedList<T> *list){
        LinkedList<T>* New =new LinkedList<T>();
        Node<T>* tmp = this->head;
        while(tmp!= nullptr){
            New->Prepend(tmp->GetData());
            tmp = tmp->GetNext();
        }
        tmp = list->head;
        while(tmp!= nullptr){
            New->Prepend(tmp->GetData());
            tmp = tmp->GetNext();
        }
        return New;
    }   //Конкатенация списков

    ListIterator<T>* begin() override{
        return (new ListIterator<T>(head));
    }

    ListIterator<T>* end() override{
        return (new ListIterator<T>(nullptr));
    }

    std::string String(){
        std::string res = "";
        Node<T>* temp = head;
        if(head == nullptr) Exception(2);
        else
            while (temp!=nullptr){
                res += toString<T>(temp->GetData())+" ";
                temp = temp->GetNext();
            }
        res+="\n";
        return res;
    }   //выписывание всех элементов в строку
};

#endif
