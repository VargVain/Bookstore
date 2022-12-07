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

`main`函数只负责循环处理输入信息，由一个封装的函数调用各个类的对应接口。

主要类有：

tokenScanner类 负责将输入变为片段

error类 负责输出错误信息

login类 负责处理登录栈

user类 存储账户并提供更改账户信息的接口

stock类 存储图书信息并提供图书操作的接口

log类 提供日志操作

### 各个类的接口及成员：

##### tokenScanner类
接口：

`setInput(std::string str)`输入

`bool hasMoreTokens()`判断是否有剩余片段

`std::string nextToken()`读取并返回下一个片段

`bool CheckScanner(TokenScanner scanner, something)`检查输入是否符合格式

##### error类（继承自std::exception）
接口：

`ErrorException(std::string message)`记录错误信息

`std::string getMessage() const`返回错误信息

成员：

`std::string message`错误信息

`void error(std::string message)`输出错误信息（其实不属于Error类中）

##### login类：
接口：

`user getCurrentUser()`返回当前用户

`void Login(std::string ID)`登录指定ID的用户

`void Logout()`退出最后登录者的登录

成员：

`std::stack< std::string > LoginStack`登录栈

##### user类：
接口：

`bool checkPasswd(std::string ID, std::string passwd)`检验密码

`void addUser(std::string ID, std::string passwd, int privilege, std::string username)`创建信息如指令格式所示的帐户

`void deleteUser(std::string ID)`删除指定帐户

成员：

`struct account{}`存储用户信息，包括ID、用户名、密码、权限等级

`std::map<std::string, account> accountTable`存储所有账户，以ID为索引

##### stock类：
接口：

`void showBook(std::string s)`以 `[ISBN] `字典升序依次输出满足要求的图书信息

`double buyBook(std::string ISBN, int quantity)`购买指定数量的指定图书（由于本系统不包括支付系统，故仅需减少库存），以浮点数输出购买图书所需的总金额。

`void selectBook(std::string ISBN)`以当前帐户选中指定图书

`void modify(std::string, std::string, std::string, std::string, double)`以指令中的信息更新选中图书的信息

`void import(int Quantity, double TotalCost)`以指定交易总额购入指定数量的选中图书，增加其库存数

成员：

`struct book{}`存储图书信息，包括ISBN、库存数量、价格等

`std::map<std::string, book> bookTable`存储所有图书，以ISBN为索引

##### log类：
接口：

`void log_business(int count)`输出最后count笔交易记录

`void log()`返回赏心悦目的日志记录

成员：

`std::vector<int>` 交易记录

`std::vector< std::string > logTable` 日志记录 

### 文件存储说明：

所有运行过程中产生的信息都存储在相关的文本文档中。