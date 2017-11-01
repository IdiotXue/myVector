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

  private:
    int m_size;      //大小,与m_capacity声明顺序不能换
    int m_capacity;  //容量
    Object *m_pObjs; //数组
    const static int SPARE_CAPACITY = 16;
};
}

#endif