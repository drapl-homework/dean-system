文件结构：
	db.h - 数据库模块
	menu.h - 对话菜单模块
	tablemaker.h - 表格模块
	school.h - 学生、老师、课程数据存储结构和相关函数
	school.cpp - 主程序
	config/ - 菜单文件和配置文件

Linux平台下，g++-4.7.3、clang-3.3编译通过；
因为涉及了中文的输入、输出，所以使用了wchar_t存储中文数据。wchar_t的大小在不同平台可能不一样。所以存储的数据不一定能跨平台使用。
文件都是UTF-8编码，未在Windows平台上测试
