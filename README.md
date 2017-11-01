# 实现vector类模板
1. 一个基本数组，通过指针指向分配的内存块，一个存数组容量的变量，一个存数组元素个数（长度）的变量
2. 三大函数：复制构造函数、operator=提供深复制、析构回收数组
3. resize改变vector长度，reserve改变vector容量，push_back/pop_back增/减元素，size返回长度，empty返回是否空，clear清空，capacity返回容量，
4. 下标[]操作：访问函数返回常量引用，修改函数返回引用