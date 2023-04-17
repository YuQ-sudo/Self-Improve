# Coding

## leetcode

<[CodeAbstract]>
[CodeAbstract]: https://github.com/youngyangyang04/leetcode-master

## BookTheory

### effective c++

* 尽量以const enum inline代替#define(编译器代替预处理器): #define不被视为语言的一部分。define定义的内容没有进入到符号表。
  
  * 对于单纯变量，最好使用const对象或enums替代define
  * 对于形似函数的宏，最好使用inline函数替换define，容易发生类型安全性和无法预料行为

    ```cpp
    #define com(a,b) f((a) > (b) ? (a) : (b))
    int a = 5,b=0;
    com(++a,b); a加两次
    com(++a,10);a加一次
    ```

* const语法，出现在*左边 表示被指物常量，如果出现在右边，表示指针本身是常量。
  * 如果不想修改函数返回的值，那么就用const修饰函数返回值！避免出现给函数返回值赋值的奇怪现象。
  * 如果在成员函数中，不想修改成员变量的值，则应该用const修饰成员函数！
