/**
 * Created by EnergyX on 2017-10-30  
 */

#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <stdexcept>

namespace myvector
{
template <typename Object>
class Vector
{
  private:
    int m_size;      //大小,与m_capacity声明顺序不能换
    int m_capacity;  //容量
    Object *m_pObjs; //数组
    const static int SPARE_CAPACITY = 16;

  public:
    class const_iterator
    {
      protected:
        Object *m_pCur;
        /**
         * 不允许除Vector的其他类通过指针构造迭代器,只在begin和end中调用
         * 但该函数是protected，无法被Vector反问，需要额外想办法
         */
        const_iterator(Object *p) : m_pCur(p) {}
        /*不同数据结构的检索方式不同，比如vector和list不同，所以封装于一个函数*/
        Object &retrieve() const { return *m_pCur; }
        friend class Vector<Object>; //友元声明，使Vector可以反问构造函数

      public:
        const_iterator() : m_pCur(nullptr) {} //默认初始化为NULL，避免野指针问题
        const Object &operator*() const { return retrieve(); }
        /*前置++*/
        const_iterator &operator++()
        {
            ++m_pCur;
            return *this;
        }
        /*后置++*/
        const_iterator operator++(int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }
        bool operator==(const const_iterator &rhs) const { return m_pCur == rhs.m_pCur; }
        bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
    };
    class iterator : public const_iterator
    {
      protected:
        iterator(Object *p) : const_iterator(p) {}
        friend class Vector<Object>;

      public:
        iterator() : const_iterator() {}
        Object &operator*() { return const_iterator::retrieve(); }
        const Object &operator*() const { return this->retrieve(); } //或许可以不用
        /*前置++*/
        iterator &operator++()
        {
            ++this->m_pCur;
            return *this;
        }
        /*后置++*/
        iterator operator++(int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }
    };

  public:
    explicit Vector(int initSize = 0) : m_size(initSize), m_capacity(m_size + SPARE_CAPACITY)
    {
        m_pObjs = new Object[m_capacity];
    }
    Vector(const Vector &rhs) : m_size(0), m_capacity(0), m_pObjs(nullptr) { operator=(rhs); }
    ~Vector()
    {
        delete[] m_pObjs;
        m_pObjs = nullptr;
    }
    const Vector &operator=(const Vector &rhs)
    {
        if (this != &rhs)
        {
            if (m_capacity < rhs.size()) //容量不足才重新分配
            {
                delete[] m_pObjs;
                m_pObjs = new Object[rhs.capacity()];
                m_capacity = rhs.capacity();
            }
            for (int i = 0; i < rhs.size(); ++i)
                m_pObjs[i] = rhs[i];
            m_size = rhs.size();
        }
        return *this;
    }

    const_iterator begin() const { return const_iterator(m_pObjs); }
    const_iterator end() const { return const_iterator(m_pObjs + m_size); }
    iterator begin() { return iterator(m_pObjs); }
    iterator end() { return iterator(m_pObjs + m_size); }

    Object &operator[](int index) //修改
    {
        if (index >= m_size)
            throw std::range_error("Vector index out of range!");
        return m_pObjs[index];
    }
    const Object &operator[](int index) const //访问
    {
        if (index >= m_size)
            throw std::range_error("Vector index out of range!");
        return m_pObjs[index];
    }
    void reserve(int newCap)
    {
        if (newCap <= m_size)
            return;
        Object *pOldObjs = m_pObjs;
        m_pObjs = new Object[newCap];
        m_capacity = newCap;
        for (int i = 0; i < m_size; ++i)
            m_pObjs[i] = pOldObjs[i];
        delete[] pOldObjs;
    }
    void resize(int newSize)
    {
        if (newSize > m_capacity)
            reserve(2 * newSize + 1); //扩容为size的2倍
        m_size = newSize;
    }
    int size() const { return m_size; }
    int capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }
    void clear() { m_size = 0; }
    void push_back(const Object &value)
    {
        if (m_size == m_capacity)
            reserve(2 * m_size + 1);
        m_pObjs[m_size++] = value;
    }
    void pop_back()
    {
        if (m_size == 0)
            throw std::underflow_error("pop_back when empty!");
        --m_size;
    }
    const Object &back() const { return m_pObjs[m_size - 1]; }
};
}

#endif