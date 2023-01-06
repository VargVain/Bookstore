# Bookstore项目开发文档

###### by 王宇澄

### 程序功能概述：

实现一个使用命令行交互的书店管理系统。功能包括：

##### 账户系统：维护不同权限等级的账户信息，并提供登录等功能。

一个账户包含用户名、ID、权限等级、密码、登陆状态等信息，用一个账户类进行封装。

###### 各权限可访问功能

店主 {7}：使用 root 帐户，可以访问书店系统所有功能，包括日志和帐户管理功能；

员工 {3}：可以访问修改图书数据相关功能等；

顾客 {1}：可以注册帐户，查询、购买图书；

游客 {0}：可以注册帐户。

具体的功能包括：登陆账户（允许嵌套登录）、登出账户、注册账户、注销账户、修改密码等。

输入的指令视为最后登录的帐户操作。退出系统视为登出所有已登录帐户。

##### 图书系统：提供图书信息、库存信息和图书交易财务记录的相关服务及数据存储功能。

包含图书信息与库存信息，同样用一个图书类进行封装。

具体的功能包括：检索图书（以ISBN升序输出）、购买图书、选择图书、修改图书信息、图书进货等。

##### 日志系统：记录书店管理系统运行过程中的各种数据，提供各类日志信息查询服务。

记录并存储所有操作，用于输出日志。

### 主体逻辑说明：

主函数通过循环逐行处理输入的信息。

用一个封装的函数接受处理过的信息，并调用各个类的对应接口以实现功能或者输出报错。

考虑到应用场景下帐户和图书数量较大，我们不将主体数据存储于内存，而实时读写文件数据。所以在类的处理中，我们以文件的形式进行读写。


### 代码逻辑说明：

`main`函数只负责循环处理输入信息，具体操作由`operation`类的多个派生类完成。

`operation`类会对输入的整行文字进行分段、检查合法性以及执行，并在这一过程中调用各个类。

主要类有：

tokenScanner类 负责将输入变为片段

error类 负责输出错误信息

database类 负责以块状链表存储信息

operation类 负责具体处理操作

login类 负责处理登录栈的各种操作

user类 存储账户信息

book类 存储图书信息

log类 提供日志操作

### 各个类的接口及成员：

##### tokenScanner类
接口：

`std::string nextToken()`返回下一个片段

`int getTokenCount()`获得总片段数

##### error类（继承自std::exception）
接口：

`ErrorException()`返回错误信息

`void error()`抛出错误信息

##### database类
接口：

`void insert(const char *_key, T _val)`插入数据

`void erase(const char *_key)`删除数据

`void modify(const char *_key, T _val)`调整数据

`T find(const char *_key)`寻找数据

##### login类：
接口：

`void select(const std::string &ISBN)`让当前用户选择指定书

`void login(std::string ID)`登录指定ID的用户

`void logout()`退出最后登录者的登录

`bool checkLogin(const std::string &UserID)`检查用户是否登录

`void flush(const std::string &ord, const std::string &change)`
ISBN数据更新时，刷新所有指定该书的数据

##### user类：

成员：

`char UserID[30];
char Password[30];
char Username[30];
int Privilege;`存储用户信息，包括ID、用户名、密码、权限等级

##### book类：

成员：

`char ISBN[20];
char BookName[60];
char Author[60];
char Keyword[60];
double Price;
int stock;`存储图书信息，包括ISBN、库存数量、价格等

##### log类：
接口：

`void void add_finance(double val)`新增交易记录

`void show_finance(int count)`输出最后count笔交易记录

### 文件存储说明：

程序运行中的user与book信息存储于块状链表库中。

登陆栈信息与日志信息独立存储于文件中。
