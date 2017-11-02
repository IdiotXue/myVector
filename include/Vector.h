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
    class const_iterator
    {
      protected:
        const Vector<Object> *owner; //检测insert/erase时，itt是否属于正确的表
        Object *m_pCur;
        /**
         * 不允许除Vector的其他类通过指针构造迭代器,只在begin和end中调用
         * 但该函数是protected，无法被Vector反问，需要额外想办法
         */
        const_iterator(const Vector<Object> &belongTo, Object *p) : owner(&belongTo), m_pCur(p) {}
        /*不同数据结构的检索方式不同，比如vector和list不同，所以封装于一个函数*/
        Object &retrieve() const { return *m_pCur; }
        void assertIsValid() const
        {
            if (owner == nullptr || m_pCur == nullptr)
                throw std::invalid_argument("invalid NULL pointer in iterator!");
            if (m_pCur == owner->end().m_pCur) //只判断到达尾部，而没有判断超出尾部，因为暂时只有+1操作
                throw std::out_of_range("pointer out of range in iterator!");
        }
        friend class Vector<Object>; //友元声明，使Vector可以反问构造函数

      public:
        const_iterator() : owner(nullptr), m_pCur(nullptr) {} //默认初始化为NULL，避免野指针问题
        const Object &operator*() const
        {
            assertIsValid();
            return retrieve();
        }
        /*前置++*/
        const_iterator &operator++()
        {
            assertIsValid();
            ++m_pCur;
            return *this;
        }
        /*后置++*/
        const_iterator operator++(int)
        {
            assertIsValid();
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
        iterator(const Vector<Object> &belongTo, Object *p) : const_iterator(belongTo, p) {}
        friend class Vector<Object>;

      public:
        iterator() : const_iterator() {}
        Object &operator*()
        {
            this->assertIsValid();
            return const_iterator::retrieve();
        }
        const Object &operator*() const
        {
            this->assertIsValid();
            return this->retrieve();
        } //或许可以不用
        /*前置++,返回类型改变，需要重载*/
        iterator &operator++()
        {
            this->assertIsValid();
            ++this->m_pCur;
            return *this;
        }
        /*后置++*/
        iterator operator++(int)
        {
            this->assertIsValid();
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

    const_iterator begin() const { return const_iterator(*this, m_pObjs); }
    const_iterator end() const { return const_iterator(*this, m_pObjs + m_size); }
    iterator begin() { return iterator(*this, m_pObjs); }
    iterator end() { return iterator(*this, m_pObjs + m_size); }
    iterator erase(const iterator &itrToRem)
    {
        itrToRem.assertIsValid();
        assertIsBelong(itrToRem);
        if (itrToRem.owner != this) //判断itr是否属于这个表
            throw std::invalid_argument("itr isn't belong to this vector!");
        Object *pNewObjs = new Object[m_capacity];
        int index = 0, itrIndex;
        for (Vector<Object>::iterator itr = this->begin(); itr != this->end(); ++itr)
        {
            if (itr == itrToRem)
            {
                itrIndex = index;
                continue;
            }
            pNewObjs[index++] = *itr;
        }
        delete[] m_pObjs;
        m_pObjs = pNewObjs;
        pNewObjs = nullptr;
        --m_size;
        return iterator(*this, m_pObjs + itrIndex);
    }
    iterator insert(const iterator &itrToIns, const Object &value)
    {
        itrToIns.assertIsValid();
        assertIsBelong(itrToIns);
        Object *pNewObjs = new Object[m_capacity];
        int index = 0, itrIndex;
        for (Vector<Object>::iterator itr = this->begin(); itr != this->end(); ++itr)
        {
            if (itr == itrToIns)
            {
                itrIndex = index;
                pNewObjs[index++] = value;
            }
            pNewObjs[index++] = *itr;
        }
        delete[] m_pObjs;
        m_pObjs = pNewObjs;
        pNewObjs = nullptr;
        ++m_size;
        return iterator(*this, m_pObjs + itrIndex);
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
    void assertIsBelong(const iterator &itr)
    {
        if (itr.owner != this) //判断itr是否属于这个表
            throw std::invalid_argument("itr isn't belong to this vector!");
    }
};
}

#endif