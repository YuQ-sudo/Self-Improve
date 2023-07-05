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
  
* 确定对象被使用前已经被初始化
  * 例如int x;这种方式不能保证x被初始化为0
    * c part of c++ 表示c++是以c为基础的，其中的区块(blocks)、语句(statements)、数组(arrays)和指针(pointers)都来自c。注意，c语言部分和专属于c++部分的初始化方式不一样。
    * vector stl 有此保证，确保都进行了初始化。
    * 除了内置类型之外的任何东西，初始化在构造函数内执行。需要确保每一个成员都进行初始化。赋值和初始化区别。使用成员初始列表进行初始化。
    * 在成员初值列钟条列各个成员时，最好以其声明次序为次序。
    * 为免除跨编译单元之初始化次序问题，使用local static 对象替换non local static对象

    ```cpp
      class FileSystem
      {

      };
      extern FileSystem tfs;
      假设其他人在创建一个class Directory
      class Directory
      {
        
      }
      构造函数内部调用tfs对象
      Directory::Directory()
      {
        tfs.成员;
      }
      风险点：tfs必须在directory之前初始化。（不同编译单元初始化次序）
      解决方式：
      FileSystem& tfs()
      {
        static FileSystem fs;
        return fs;
      }
      多线程之间会存在竞速形势，需要在线程启动之前进行初始化成功。

    ```

* 为驳回编译器自动提供的机能，可将相应的成员函数声明为private并不予以实现。
  * 可参考Boost noncopyable的实现 <https://blog.csdn.net/qiuyumin430/article/details/80886899>
* 资源管理
  * 内存管理、文件描述器、互斥锁、图形界面中的字型和笔刷、数据库连接、网络sockets等。
    * 文件描述符 <https://blog.csdn.net/zwjyyy1203/article/details/97105647>
      * 每一个文件描述符会与一个打开的文件相对应
      * 不同的文件描述符也可能指向同一个文件
      * 相同的文件可以被不同的进程打开，也可以在不同的进程被多次打开
    * 进程级的文件描述符表 系统级的文件描述表 文件系统的i-node表
    * 实际应用中 如果出现too many open files。可以通过增大进程可用的文件描述符数量来解决。基本原因是因为程序bug，打开了大量的文件连接而没有释放导致的。
    * ulimit -n 或者sysctl -a | grep -i file-max --color或者 cat /proc/sys/fs/file-max
  * auto_ptr是个类指针对象，其析构函数会自动调用delete。
  * auto_ptr 若通过copy构造函数或者赋值操作符复制他们，他们会变成nullptr，而赋值的指针将获得唯一拥有权;
  * std::tr1::shared_ptr 不同于auto_ptr所指向的对象不会是null
  * get()函数返回只能指针的原始智能指针的复件。

    ```cpp
      std::auto_ptr<Investment> p(createInvestment());
      std::auto_ptr<Investment> p1(p); //p为null;
      p = p1;//p1为null
    ```

    * 智能指针有时候需要创建删除器否则可能造成内存泄漏
    <https://blog.csdn.net/lijinqi1987/article/details/78523071>
      * 自定义Deleter作为lambda函数或函数对象

      ```cpp
      std::shared_ptr<Sample> p4(new Sample[12], [](Sample* x){
                                std::cout<<"DELETER FUNCTION CALLED\n";
                                delete[] x;})
      ```

* 以独立语句将newed对象存储在智能指针内。
* 接口设置正确性、高效性、封装性、维护性、延展性等
* 引用往往以指针实现出来。如果有个对象属于内置类型，pass by value往往比pass by reference效率高一些。（汇编出来代码 多一步取地址）
* <https://www.bbsmax.com/A/x9J239ae56
### 图说设计模式

#### 简单工厂模式

* 模式定义
  * 简单工厂模式(Simple Factory Pattern)：又称为静态工厂方法(Static Factory Method)模式，它属于类创建型模式。在简单工厂模式中，可以根据参数的不同返回不同类的实例。简单工厂模式专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类
* 模式结构
  * Factory:工厂角色 Product: 抽象产品角色 ConcreteProduct：具体产品角色 
  * 简单工厂模式的要点在于：当你需要什么，只需要传入一个正确的参数，就可以获取你所需要的对象，而无须知道其创建细节。
* 代码示例
  
```cpp
  #include "Factory.h"
  #include "ConcreteProductA.h"
  #include "ConcreteProductB.h"
  Product* Factory::createProduct(string proname){
      if ( "A" == proname )
      {
        return new ConcreteProductA();
      }
      else if("B" == proname)
      {
        return new ConcreteProductB();
      }
      return  NULL;
  }
```

#### 工厂模式

* 模式定义
  * 工厂方法模式(Factory Method Pattern)又称为工厂模式，也叫虚拟构造器(Virtual Constructor)模式或者多态工厂(Polymorphic Factory)模式，它属于类创建型模式。在工厂方法模式中，工厂父类负责定义创建产品对象的公共接口，而工厂子类则负责生成具体的产品对象，这样做的目的是将产品类的实例化操作延迟到工厂子类中完成，即通过工厂子类来确定究竟应该实例化哪一个具体产品类。
* 模式结构
  * Product：抽象产品 ConcreteProduct：具体产品 Factory：抽象工厂 ConcreteFactory：具体工厂
* 使用说明
  * 增加一个具体工厂类 一个具体产品类 共用一个抽象工厂父类。具体接口实现在产品类。
* 优缺点
  * 在添加新产品时，需要编写新的具体产品类，而且还要提供与之对应的具体工厂类，系统中类的个数将成对增加，在一定程度上增加了系统的复杂度，有更多的类需要编译和运行，会给系统带来一些额外的开销。
  * 由于考虑到系统的可扩展性，需要引入抽象层，在客户端代码中均使用抽象层进行定义，增加了系统的抽象性和理解难度，且在实现时可能需要用到DOM、反射等技术，增加了系统的实现难度。（可参考CNStreamStruct的实现）
  * 工厂方法模式的主要优点是增加新的产品类时无须修改现有系统，并封装了产品对象的创建细节，系统具有良好的灵活性和可扩展性


#### 抽象工厂模式

* 模式动机
  * 在工厂方法模式中具体工厂负责生产具体的产品，每一个具体工厂对应一种具体产品，工厂方法也具有唯一性，一般情况下，一个具体工厂中只有一个工厂方法或者一组重载的工厂方法。但是有时候我们需要一个工厂可以提供多个产品对象，而不是单一的产品对象。
  * 当系统所提供的工厂所需生产的具体产品并不是一个简单的对象，而是多个位于不同产品等级结构中属于不同类型的具体产品时需要使用抽象工厂模式。
  * 抽象工厂模式是所有形式的工厂模式中最为抽象和最具一般性的一种形态

#### C++中once_flag、call_once使用

https://zhuanlan.zhihu.com/p/71900518