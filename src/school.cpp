#include "menu.h"
#include "db.h"
#include "school.h"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

db<student> db_student(string("config/student.db"));
db<teacher> db_teacher(string("config/teacher.db"));
db<course> db_course(string("config/course.db"));
db<student_course> db_student_course(string("config/student_course.db"));
db<teacher_course> db_teacher_course(string("config/teacher_course.db"));

template < typename T >
T getinput(string s)
{
	T tmp;
	cout << s;
	cin >> tmp;
	return tmp;
}

bool yesorno(string tips)
{
	char flag;
	while(not (flag == 'y' or flag == 'Y' or flag == 'N' or flag == 'n' ))
	{
		cout << tips;
		cin >> flag;
	}
	if(flag == 'y' or flag == 'Y')
		return true;
	if(flag == 'n' or flag == 'N')
		return false;
}

void print_student_info()
{
	//打印学生信息
	printf("%15s%15s%15s\n", "序号", "学号", "姓名");
	for(int i=0; i<db_student.getData().size(); i++)
		printf("%15d%15d%15s\n", i+1,
				db_student.getData()[i].id,
				db_student.getData()[i].name);
}

void print_student_info(student s)
{
	//打印单个学生信息
	printf("%15s%15s\n", "学号", "姓名");
	printf("%15d%15s\n", s.id,
			s.name);
}

void student_sort_by_id_less()
{
	sort(db_student.getData().begin(), db_student.getData().end(),
			student_id_less);
}

void student_sort_by_id_more()
{
	sort(db_student.getData().begin(), db_student.getData().end(),
			student_id_more);
}

void add_student()
{
	student ns;
	printf("请输入学号：");
	cin >> ns.id;
	printf("请输入姓名：");
	cin >> ns.name;
	if(yesorno("确认添加(y/n)？"))
	{
		db_student.getData().push_back(ns);
		db_student.putData();
		printf("添加成功！\n");
	}
	else
		printf("取消添加。");
	if(yesorno("继续添加（y/n）？"))
		add_student();
}

void del_student_by_id()
{
	int id;
	cout << "请输入学号：";
	cin >> id;
	vector<student>::iterator it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(id));

	if(it == db_student.getData().end())
		cout << "无此id." << endl;
	else
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
		if(yesorno("是否确定删除(y/n)？"))
		{
			db_student.getData().erase(it);
			db_student.putData();
			cout << "删除成功！" << endl;
		}
		else
			cout << "取消删除。" << endl;
	}
	if(yesorno("是否继续删除(y/n)？"))
		del_student_by_id();
}

void del_student_by_name()
{
	string name;
	cout << "请输入姓名：";
	cin >> name;
	vector<student>::iterator it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_name_equal(name));

	if(it == db_student.getData().end())
		cout << "无此姓名." << endl;
	else
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
		if(yesorno("是否确定删除(y/n)？"))
		{
			db_student.getData().erase(it);
			db_student.putData();
			cout << "删除成功！" << endl;
		}
		else
			cout << "取消删除。" << endl;
	}
	if(yesorno("是否继续删除(y/n)？"))
		del_student_by_id();
}

void chg_student_by_id()
{
	int id;
	cout << "请输入学号：";
	cin >> id;
	vector<student>::iterator it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(id));

	if(it == db_student.getData().end())
		cout << "无此id." << endl;
	else
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
		if(yesorno("是否确定修改(y/n)？"))
		{
			bool flag = false;
			if(yesorno("是否修改学号(y/n)？"))
			{
				flag = true;
				int newid;
				cout << "输入新学号：";
				cin >> newid;
				it->id = newid;
			}
			if(yesorno("是否修改姓名(y/n)？"))
			{
				flag = true;
				char newname[35];
				cout << "输入新姓名：";
				cin >> newname;
				strcpy(it->name, newname);
			}
			if(flag)
			{
				db_student.putData();
				cout << "修改成功！" << endl;
			}
			else
				cout << "未做改动。" << endl;
		}
		else
			cout << "取消修改。" << endl;
	}
	if(yesorno("是否继续修改(y/n)？"))
		chg_student_by_id();
}

void chg_student_by_name()
{
	string name;
	cout << "请输入姓名：";
	cin >> name;
	vector<student>::iterator it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_name_equal(name));

	if(it == db_student.getData().end())
		cout << "无此姓名." << endl;
	else
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
		if(yesorno("是否确定修改(y/n)？"))
		{
			bool flag = false;
			if(yesorno("是否修改学号(y/n)？"))
			{
				flag = true;
				int newid;
				cout << "输入新学号：";
				cin >> newid;
				it->id = newid;
			}
			if(yesorno("是否修改姓名(y/n)？"))
			{
				flag = true;
				char newname[35];
				cout << "输入新姓名：";
				cin >> newname;
				strcpy(it->name, newname);
			}
			if(flag)
			{
				db_student.putData();
				cout << "修改成功！" << endl;
			}
			else
				cout << "未做改动。" << endl;
		}
		else
			cout << "取消修改。" << endl;
	}

	if(yesorno("是否继续修改(y/n)？"))
		chg_student_by_name();
}

// 复制粘贴替换：stu*dent->teacher, 学*号->教师编号，学生->教师
void print_teacher_info()
{
	//打印教师信息
	printf("%15s%15s%15s\n", "序号", "教师编号", "姓名");
	for(int i=0; i<db_teacher.getData().size(); i++)
		printf("%15d%15d%15s\n", i+1,
				db_teacher.getData()[i].id,
				db_teacher.getData()[i].name);
}

void print_teacher_info(teacher s)
{
	//打印单个教师信息
	printf("%15s%15s\n", "教师编号", "姓名");
	printf("%15d%15s\n", s.id,
			s.name);
}

void teacher_sort_by_id_less()
{
	sort(db_teacher.getData().begin(), db_teacher.getData().end(),
			teacher_id_less);
}

void teacher_sort_by_id_more()
{
	sort(db_teacher.getData().begin(), db_teacher.getData().end(),
			teacher_id_more);
}

void add_teacher()
{
	teacher ns;
	printf("请输入教师编号：");
	cin >> ns.id;
	printf("请输入姓名：");
	cin >> ns.name;
	if(yesorno("确认添加(y/n)？"))
	{
		db_teacher.getData().push_back(ns);
		db_teacher.putData();
		printf("添加成功！\n");
	}
	else
		printf("取消添加。");
	if(yesorno("继续添加（y/n）？"))
		add_teacher();
}

void del_teacher_by_id()
{
	int id;
	cout << "请输入教师编号：";
	cin >> id;
	vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
			db_teacher.getData().end(),
			teacher_id_equal(id));

	if(it == db_teacher.getData().end())
		cout << "无此id." << endl;
	else
	{
		cout << "教师信息如下：" << endl;
		print_teacher_info(*it);
		if(yesorno("是否确定删除(y/n)？"))
		{
			db_teacher.getData().erase(it);
			db_teacher.putData();
			cout << "删除成功！" << endl;
		}
		else
			cout << "取消删除。" << endl;
	}
	if(yesorno("是否继续删除(y/n)？"))
		del_teacher_by_id();
}

void del_teacher_by_name()
{
	string name;
	cout << "请输入姓名：";
	cin >> name;
	vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
			db_teacher.getData().end(),
			teacher_name_equal(name));

	if(it == db_teacher.getData().end())
		cout << "无此姓名." << endl;
	else
	{
		cout << "教师信息如下：" << endl;
		print_teacher_info(*it);
		if(yesorno("是否确定删除(y/n)？"))
		{
			db_teacher.getData().erase(it);
			db_teacher.putData();
			cout << "删除成功！" << endl;
		}
		else
			cout << "取消删除。" << endl;
	}
	if(yesorno("是否继续删除(y/n)？"))
		del_teacher_by_id();
}

void chg_teacher_by_id()
{
	int id;
	cout << "请输入教师编号：";
	cin >> id;
	vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
			db_teacher.getData().end(),
			teacher_id_equal(id));

	if(it == db_teacher.getData().end())
		cout << "无此id." << endl;
	else
	{
		cout << "教师信息如下：" << endl;
		print_teacher_info(*it);
		if(yesorno("是否确定修改(y/n)？"))
		{
			bool flag = false;
			if(yesorno("是否修改教师编号(y/n)？"))
			{
				flag = true;
				int newid;
				cout << "输入新教师编号：";
				cin >> newid;
				it->id = newid;
			}
			if(yesorno("是否修改姓名(y/n)？"))
			{
				flag = true;
				char newname[35];
				cout << "输入新姓名：";
				cin >> newname;
				strcpy(it->name, newname);
			}
			if(flag)
			{
				db_teacher.putData();
				cout << "修改成功！" << endl;
			}
			else
				cout << "未做改动。" << endl;
		}
		else
			cout << "取消修改。" << endl;
	}
	if(yesorno("是否继续修改(y/n)？"))
		chg_teacher_by_id();
}

void chg_teacher_by_name()
{
	string name;
	cout << "请输入姓名：";
	cin >> name;
	vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
			db_teacher.getData().end(),
			teacher_name_equal(name));

	if(it == db_teacher.getData().end())
		cout << "无此姓名." << endl;
	else
	{
		cout << "教师信息如下：" << endl;
		print_teacher_info(*it);
		if(yesorno("是否确定修改(y/n)？"))
		{
			bool flag = false;
			if(yesorno("是否修改教师编号(y/n)？"))
			{
				flag = true;
				int newid;
				cout << "输入新教师编号：";
				cin >> newid;
				it->id = newid;
			}
			if(yesorno("是否修改姓名(y/n)？"))
			{
				flag = true;
				char newname[35];
				cout << "输入新姓名：";
				cin >> newname;
				strcpy(it->name, newname);
			}
			if(flag)
			{
				db_teacher.putData();
				cout << "修改成功！" << endl;
			}
			else
				cout << "未做改动。" << endl;
		}
		else
			cout << "取消修改。" << endl;
	}

	if(yesorno("是否继续修改(y/n)？"))
		chg_teacher_by_name();
}

void print_course_info()
{
	//打印课程信息
	printf("%15s%15s%15s%15s\n", "序号", "课程编号", "课程名称","学分");
	for(int i=0; i<db_course.getData().size(); i++)
		printf("%15d%15d%15s%15d\n", i+1,
				db_course.getData()[i].id,
				db_course.getData()[i].name,
				db_course.getData()[i].credit);
}

void print_course_info(course s)
{
	//打印单个课程信息
	printf("%15s%15s%15s\n", "课程编号", "课程名称","学分");
	printf("%15d%15s%15d\n", s.id,
			s.name,s.credit);
}

void course_sort_by_id_less()
{
	sort(db_course.getData().begin(), db_course.getData().end(),
			course_id_less);
}

void course_sort_by_id_more()
{
	sort(db_course.getData().begin(), db_course.getData().end(),
			course_id_more);
}

void course_sort_by_credit_less()
{
	sort(db_course.getData().begin(), db_course.getData().end(),
			course_credit_less);
}

void course_sort_by_credit_more()
{
	sort(db_course.getData().begin(), db_course.getData().end(),
			course_credit_more);
}

void add_course()
{
	course ns;
	printf("请输入课程编号：");
	cin >> ns.id;
	printf("请输入课程名称：");
	cin >> ns.name;
	printf("请输入学分：");
	cin >> ns.credit;
	if(yesorno("确认添加(y/n)？"))
	{
		db_course.getData().push_back(ns);
		db_course.putData();
		printf("添加成功！\n");
	}
	else
		printf("取消添加。");
	if(yesorno("继续添加（y/n）？"))
		add_course();
}

void del_course_by_id()
{
	int id;
	cout << "请输入课程编号：";
	cin >> id;
	vector<course>::iterator it = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(id));

	if(it == db_course.getData().end())
		cout << "无此id." << endl;
	else
	{
		cout << "课程信息如下：" << endl;
		print_course_info(*it);
		if(yesorno("是否确定删除(y/n)？"))
		{
			db_course.getData().erase(it);
			db_course.putData();
			cout << "删除成功！" << endl;
		}
		else
			cout << "取消删除。" << endl;
	}
	if(yesorno("是否继续删除(y/n)？"))
		del_course_by_id();
}

void del_course_by_name()
{
	string name;
	cout << "请输入课程名称：";
	cin >> name;
	vector<course>::iterator it = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_name_equal(name));

	if(it == db_course.getData().end())
		cout << "无此课程名称." << endl;
	else
	{
		cout << "课程信息如下：" << endl;
		print_course_info(*it);
		if(yesorno("是否确定删除(y/n)？"))
		{
			db_course.getData().erase(it);
			db_course.putData();
			cout << "删除成功！" << endl;
		}
		else
			cout << "取消删除。" << endl;
	}
	if(yesorno("是否继续删除(y/n)？"))
		del_course_by_id();
}

void chg_course_by_id()
{
	int id;
	cout << "请输入课程编号：";
	cin >> id;
	vector<course>::iterator it = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(id));

	if(it == db_course.getData().end())
		cout << "无此id." << endl;
	else
	{
		cout << "课程信息如下：" << endl;
		print_course_info(*it);
		if(yesorno("是否确定修改(y/n)？"))
		{
			bool flag = false;
			if(yesorno("是否修改课程编号(y/n)？"))
			{
				flag = true;
				int newid;
				cout << "输入新课程编号：";
				cin >> newid;
				it->id = newid;
			}
			if(yesorno("是否修改课程名称(y/n)？"))
			{
				flag = true;
				char newname[35];
				cout << "输入新课程名称：";
				cin >> newname;
				strcpy(it->name, newname);
			}
			if(yesorno("是否修改学分(y/n)？"))
			{
				flag = true;
				int newcredit;
				cout << "输入新学分：";
				cin >> newcredit;
				it->credit = newcredit;
			}
			if(flag)
			{
				db_course.putData();
				cout << "修改成功！" << endl;
			}
			else
				cout << "未做改动。" << endl;
		}
		else
			cout << "取消修改。" << endl;
	}
	if(yesorno("是否继续修改(y/n)？"))
		chg_course_by_id();
}

void chg_course_by_name()
{
	string name;
	cout << "请输入课程名称：";
	cin >> name;
	vector<course>::iterator it = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_name_equal(name));

	if(it == db_course.getData().end())
		cout << "无此课程名称." << endl;
	else
	{
		cout << "课程信息如下：" << endl;
		print_course_info(*it);
		if(yesorno("是否确定修改(y/n)？"))
		{
			bool flag = false;
			if(yesorno("是否修改课程编号(y/n)？"))
			{
				flag = true;
				int newid;
				cout << "输入新课程编号：";
				cin >> newid;
				it->id = newid;
			}
			if(yesorno("是否修改课程名称(y/n)？"))
			{
				flag = true;
				char newname[35];
				cout << "输入新课程名称：";
				cin >> newname;
				strcpy(it->name, newname);
			}
			if(yesorno("是否修改学分(y/n)？"))
			{
				flag = true;
				int newcredit;
				cout << "输入新学分：";
				cin >> newcredit;
				it->credit = newcredit;
			}
			if(flag)
			{
				db_course.putData();
				cout << "修改成功！" << endl;
			}
			else
				cout << "未做改动。" << endl;
		}
		else
			cout << "取消修改。" << endl;
	}
	if(yesorno("是否继续修改(y/n)？"))
		chg_course_by_name();
}

char* student_id2name(int student_id)
{
		vector<student>::iterator it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(student_id));
		return it->name;
}

char* course_id2name(int course_id)
{
		vector<course>::iterator it = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(course_id));
		return it->name;
}

void print_student_couese_info()
{
	//打印选课信息
	printf("%15s%15s%15s%15s%15s\n", "序号", "学号", "姓名","课程编号","课程名");
	for(int i=0; i<db_student_course.getData().size(); i++)
		printf("%15d%15d%15s%15d%15s\n", i+1,
				db_student_course.getData()[i].student_id,
				student_id2name(db_student_course.getData()[i].student_id),
				db_student_course.getData()[i].course_id,
				course_id2name(db_student_course.getData()[i].course_id));
}

void print_student_course_info(const student_course& s)
{
	//打印单个选课信息
	printf("%15s%15s%15s%15s\n", "学号", "姓名","课程编号","课程名");
	printf("%15d%15s%15d%15s\n",
				s.student_id,
				student_id2name(s.student_id),
				s.course_id,
				course_id2name(s.course_id));

}

void print_student_schedule(int student_id)
{
	//打印选课信息
	printf("%15s%15s%15s%15s%15s%15s\n", "序号", "学号", "姓名","课程编号","课程名","分数");
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].student_id == student_id)
		printf("%15d%15d%15s%15d%15s\n", i+1,
				db_student_course.getData()[i].student_id,
				student_id2name(db_student_course.getData()[i].student_id),
				db_student_course.getData()[i].course_id,
				course_id2name(db_student_course.getData()[i].course_id),
				db_student_course.getData()[i].score
				);

}

void print_course_schedule(int course_id)
{
	//打印选课信息
	printf("%15s%15s%15s%15s%15s%15s\n", "序号", "学号", "姓名","课程编号","课程名","分数");
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].course_id == course_id)
		printf("%15d%15d%15s%15d%15s\n", i+1,
				db_student_course.getData()[i].student_id,
				student_id2name(db_student_course.getData()[i].student_id),
				db_student_course.getData()[i].course_id,
				course_id2name(db_student_course.getData()[i].course_id),
				db_student_course.getData()[i].score
				);

}

void get_per_student_course()
{
		print_student_info();
	bool flag = false; //是否取得信息
	vector<student>::iterator it;

	if(yesorno("输入学生学号(y/n)？"))
	{
		int student_id;
		cout << "请输入学生学号：";
		cin >> student_id;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(student_id));

		if(it == db_student.getData().end())
			cout << "无此学号" << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入学生姓名(y/n)？"))
	{
		char student_name[35];
		cout << "请输入学生姓名：";
		cin >> student_name;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_name_equal(student_name));

		if(it == db_student.getData().end())
			cout << "无此姓名." << endl;
		else
			flag = true;
	}
	
	cout << "学生课表如下：" << endl;
	print_student_info(*it);
	
	print_student_schedule(it->id);
}

void get_per_course()
{
	bool flag = false;
	vector<course>::iterator it2;
	print_course_info();
	if(yesorno("输入课程编号(y/n)？"))
	{
		int course_id;
		cout << "请输入课程编号：";
		cin >> course_id;
		it2 = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(course_id));

		if(it2 == db_course.getData().end())
			cout << "无此编号." << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入课程名称(y/n)？"))
	{
		char course_name[35];
		cout << "请输入课程名称：";
		cin >> course_name;
		it2 = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_name_equal(course_name));

		if(it2 == db_course.getData().end())
			cout << "无此课程." << endl;
		else
			flag= true;
	}
	
	if(flag)
	{
		cout << "课程信息如下：" << endl;
		print_course_info(*it2);
	}


	cout << "课程学生信息如下：" << endl;
	print_course_schedule(it2->id);
}

void add_student_course()
{
	print_student_info();
	bool flag = false; //是否取得信息
	vector<student>::iterator it;

	if(yesorno("输入学生学号(y/n)？"))
	{
		int student_id;
		cout << "请输入学生学号：";
		cin >> student_id;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(student_id));

		if(it == db_student.getData().end())
			cout << "无此学号" << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入学生姓名(y/n)？"))
	{
		char student_name[35];
		cout << "请输入学生姓名：";
		cin >> student_name;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_name_equal(student_name));

		if(it == db_student.getData().end())
			cout << "无此姓名." << endl;
		else
			flag = true;
	}
	
	if(flag)
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
	}
	
	flag = false;
	vector<course>::iterator it2;
	print_course_info();
	if(yesorno("输入课程编号(y/n)？"))
	{
		int course_id;
		cout << "请输入课程编号：";
		cin >> course_id;
		it2 = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(course_id));

		if(it2 == db_course.getData().end())
			cout << "无此编号." << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入课程名称(y/n)？"))
	{
		char course_name[35];
		cout << "请输入课程名称：";
		cin >> course_name;
		it2 = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_name_equal(course_name));

		if(it2 == db_course.getData().end())
			cout << "无此课程." << endl;
		else
			flag= true;
	}
	
	if(flag)
	{
		cout << "课程信息如下：" << endl;
		print_course_info(*it2);
	}
	
	student_course nst;
	nst.student_id = it->id;
	nst.course_id = it2->id;
	nst.score = -1;
	
	cout << "选课信息如下：" << endl;
	print_student_course_info(nst);
	
	if(yesorno("确认提交(y/n)？"))
	{
		db_student_course.getData().push_back(nst);
		db_student_course.putData();
		cout << "提交成功！" << endl;
	}
	
	if(yesorno("是否继续添加(y/n)？"))
		add_student_course();
}

vector<student_course>::iterator get_student_course()
{
	print_student_info();
	bool flag = false; //是否取得信息
	vector<student>::iterator it;

	if(yesorno("输入学生学号(y/n)？"))
	{
		int student_id;
		cout << "请输入学生学号：";
		cin >> student_id;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(student_id));

		if(it == db_student.getData().end())
			cout << "无此学号" << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入学生姓名(y/n)？"))
	{
		char student_name[35];
		cout << "请输入学生姓名：";
		cin >> student_name;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_name_equal(student_name));

		if(it == db_student.getData().end())
			cout << "无此姓名." << endl;
		else
			flag = true;
	}
	
	if(flag)
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
		cout << "学生课程信息如下：" << endl;
		print_student_schedule(it->id);
	}
	
	flag = false;
	vector<course>::iterator it2;
	if(yesorno("输入课程编号(y/n)？"))
	{
		int course_id;
		cout << "请输入课程编号：";
		cin >> course_id;
		it2 = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(course_id));

		if(it2 == db_course.getData().end())
			cout << "无此编号." << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入课程名称(y/n)？"))
	{
		char course_name[35];
		cout << "请输入课程名称：";
		cin >> course_name;
		it2 = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_name_equal(course_name));

		if(it2 == db_course.getData().end())
			cout << "无此课程." << endl;
		else
			flag= true;
	}
	
	vector<student_course>::iterator it3;
	it3 = find_if(db_student_course.getData().begin(),
		db_student_course.getData().end(),
		student_course_equal(it->id,it2->id));
	return it3;
}

void print_student_course()
{
	print_student_info();
	bool flag = false; //是否取得信息
	vector<student>::iterator it;

	if(yesorno("输入学生学号(y/n)？"))
	{
		int student_id;
		cout << "请输入学生学号：";
		cin >> student_id;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(student_id));

		if(it == db_student.getData().end())
			cout << "无此学号" << endl;
		else
			flag = true;
	}
	if(not flag and yesorno("输入学生姓名(y/n)？"))
	{
		char student_name[35];
		cout << "请输入学生姓名：";
		cin >> student_name;
		it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_name_equal(student_name));

		if(it == db_student.getData().end())
			cout << "无此姓名." << endl;
		else
			flag = true;
	}
	
	if(flag)
	{
		cout << "学生信息如下：" << endl;
		print_student_info(*it);
		cout << "学生课程信息如下：" << endl;
		print_student_schedule(it->id);
	}

}

void del_student_course()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
		cout << "无此选课." << endl;
	else
		if(yesorno("确认删除(y/n)？"))
		{
			db_student_course.getData().erase(it3);
			db_student_course.putData();
			cout << "删除成功" << endl;
		}
	if(yesorno("是否继续删除(y/n)？"))
		del_student_course();
}

void add_score()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
		cout << "无此选课." << endl;
	else
		if(yesorno("确认添加(y/n)？"))
		{
			int score = getinput<int>("请输入分数：");
			it3->score = score;
			db_student_course.putData();
		}
	if(yesorno("是否继续添加(y/n)？"))
		add_score();
}

void del_score()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
		cout << "无此选课." << endl;
	else
		if(yesorno("确认删除(y/n)？"))
		{
			it3->score = -1;
			db_student_course.putData();
		}
	if(yesorno("是否继续删除(y/n)？"))
		del_score();

}

int main()
{
	ifstream infile("config/school.config");
	MenuCreator a(infile);
	
	a.bind("1_edit_student", print_student_info);
	a.bind("4_del_student", print_student_info);
	a.bind("1_student_sort_by_id_less", student_sort_by_id_less);
	a.bind("2_student_sort_by_id_more", student_sort_by_id_more);
	a.bind("1_del_student_by_id", del_student_by_id);
	a.bind("2_del_student_by_name", del_student_by_name);
	a.bind("3_add_student", add_student);
	a.bind("1_chg_student_by_id",chg_student_by_id);
	a.bind("2_chg_student_by_name",chg_student_by_name);
	
	a.bind("1_edit_teacher", print_teacher_info);
	a.bind("4_del_teacher", print_teacher_info);
	a.bind("1_teacher_sort_by_id_less", teacher_sort_by_id_less);
	a.bind("2_teacher_sort_by_id_more", teacher_sort_by_id_more);
	a.bind("1_del_teacher_by_id", del_teacher_by_id);
	a.bind("2_del_teacher_by_name", del_teacher_by_name);
	a.bind("3_add_teacher", add_teacher);
	a.bind("1_chg_teacher_by_id",chg_teacher_by_id);
	a.bind("2_chg_teacher_by_name",chg_teacher_by_name);
	
	a.bind("2_edit_course_list", print_course_info);
	a.bind("4_del_course", print_course_info);
	a.bind("1_course_sort_by_id_less", course_sort_by_id_less);
	a.bind("20_course_sort_by_id_more", course_sort_by_id_more);
	a.bind("21_course_sort_by_credit_less", course_sort_by_credit_less);
	a.bind("22_course_sort_by_credit_more", course_sort_by_credit_more);
	a.bind("1_del_course_by_id", del_course_by_id);
	a.bind("2_del_course_by_name", del_course_by_name);
	a.bind("3_add_course", add_course);
	a.bind("1_chg_course_by_id",chg_course_by_id);
	a.bind("2_chg_course_by_name",chg_course_by_name);

	a.bind("3_edit_student_course", print_student_couese_info);
	a.bind("1_add_student_course", add_student_course);
	a.bind("2_del_student_course", del_student_course);

	a.bind("1_add_score",add_score);
	a.bind("2_add_score",del_score);

	a.bind("1_student_list", print_student_info);
	a.bind("11_teacher_list", print_teacher_info);
	a.bind("2_course_list", print_course_info);
	a.bind("3_student_course", get_per_student_course);
	a.bind("5_course_score", get_per_course);
	a.bind("6_student_score", get_per_student_course);

	
	a.execute();
}
