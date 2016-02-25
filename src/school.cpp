#include "menu.h"
#include "db.h"
#include "school.h"
#include "tablemaker.h"
#include <iostream>
#include <algorithm>
#include <cwchar>
#include <clocale>
#include <locale>
#include <stdexcept>

using namespace std;

#if __WCHAR_MAX__ > 0x10000 //Linux下使用32位wchar_t
db<student> db_student(string("config/student_wchar.db"));
db<teacher> db_teacher(string("config/teacher_wchar.db"));
db<course> db_course(string("config/course_wchar.db"));
db<student_course> db_student_course(string("config/student_course_wchar.db"));
db<teacher_course> db_teacher_course(string("config/teacher_course_wchar.db"));
#else //Windows下使用16位wchar_t，数据不通用
db<student> db_student(string("config/student_wchar_16.db"));
db<teacher> db_teacher(string("config/teacher_wchar_16.db"));
db<course> db_course(string("config/course_wchar_16.db"));
db<student_course> db_student_course(string("config/student_course_wchar_16.db"));
db<teacher_course> db_teacher_course(string("config/teacher_course_wchar_16.db"));
#endif

wchar_t* course_id2name(int);

template < typename T >
T getinput(wstring s)
{
	T tmp;
	wcout << s;
	wcin >> tmp;
	return tmp;
}

bool yesorno(wstring tips) //选择“是”“否”
{
	wchar_t flag;
	while(not (flag == L'y' or flag == L'Y' or flag == L'N' or flag == L'n' ))
	{
		wcout << tips << L"(y/n)？";
		wcin >> flag;
	}
	if(flag == L'y' or flag == L'Y')
		return true;
	if(flag == L'n' or flag == L'N')
		return false;
	return false;
}

wchar_t* student_id2name(int student_id) //转换学生id到名字
{
		vector<student>::iterator it = find_if(db_student.getData().begin(),
			db_student.getData().end(),
			student_id_equal(student_id));
		return it->name;
}

void print_student_info()
{
	//打印学生信息
	tablemaker tb(2);
	
	tb << L"学号" << L"姓名";
	for(int i=0; i<db_student.getData().size(); i++)
	{
				tb << db_student.getData()[i].id
				   << db_student.getData()[i].name;
	}
	tb.put();
}

void print_student_info(student s)
{
	//打印单个学生信息
	tablemaker tb(2);
	tb << L"学号";
	tb << L"姓名";
	tb << s.id;
	tb << s.name;
	tb.put();
}

vector<student>::iterator choose_student() //根据用户输入选择学生（并打印学生信息）
{
	wifstream infile("config/choose_student.config");
	try{
		infile.imbue(std::locale(""));
	}
	catch(runtime_error){}

	MenuCreator a(infile);
	bool flag = false;
	vector<student>::iterator it;
	int userinput,student_id;
	wstring student_name;
	print_student_info();
	while(not flag)
	{
		int selected = a.print_and_choose();
		switch(selected)
		{
			case 1:
				userinput = getinput<int>(L"请输入序号：");
				if(userinput > db_student.getData().size() or userinput <= 0)
				{
					wcout << L"无此序号。" << endl;
					break;
				}
				it = db_student.getData().begin() + userinput -1;
				flag = true;
				break;
			case 2:
				student_id = getinput<int>(L"请输入学号：");
				it = find_if(db_student.getData().begin(),
					db_student.getData().end(),
					student_id_equal(student_id));

				if(it == db_student.getData().end())
					wcout << L"无此学号" << endl;
				else
					flag = true;
				break;

			case 3:
				student_name = getinput<wstring>(L"请输入姓名：");
				it = find_if(db_student.getData().begin(),
					db_student.getData().end(),
					student_name_equal(student_name));

				if(it == db_student.getData().end())
					wcout << L"无此姓名." << endl;
				else
					flag = true;
				break;
			
			default:
				return db_student.getData().end();
				break;
		}
	}

	wcout << L"学生信息如下：" << endl;
	print_student_info(*it);
	return it;
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
	
	ns.id = getinput<int>(L"请输入学号：");
	vector<student>::iterator it = find_if(db_student.getData().begin(),
		db_student.getData().end(),
		student_id_equal(ns.id));

	if(it != db_student.getData().end()) //学号唯一
	{
		wcout << L"学号已存在！";
		return;
	}
	
	wprintf(L"请输入姓名：");
	wcin >> ns.name;
	if(yesorno(L"确认添加"))
	{
		db_student.getData().push_back(ns);
		db_student.putData();
		wprintf(L"添加成功！\n");
	}
	else
		wprintf(L"取消添加。");
	if(yesorno(L"继续添加"))
		add_student();
}

void del_student()
{
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
	
	if(yesorno(L"删除后对应选课记录也会删除，是否确定删除"))
	{
		db_student_course.getData().erase(
		remove_if(db_student_course.getData().begin(),
			db_student_course.getData().end(),
			student_course_student_id_equal(it->id)),
		db_student_course.getData().end()
		); //删除对应选课
	
		db_student.getData().erase(it);
		db_student.putData();
		db_student_course.putData();
		
		wcout << L"删除成功！" << endl;
	}
	else
		wcout << L"取消删除。" << endl;

	if(yesorno(L"是否继续删除"))
		del_student();
}

void chg_student()
{
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
		
	if(yesorno(L"是否确定修改"))
	{
		bool flag = false;
		student n(*it);
		if(yesorno(L"是否修改姓名"))
		{
			flag = true;
			wchar_t newname[35];
			wcout << L"输入新姓名：";
			wcin >> newname;
			wcscpy(n.name, newname);
		}
		if(flag)
		{
			*it = n;
			db_student.putData();
			wcout << L"修改成功！" << endl;
		}
		else
			wcout << L"未做改动。" << endl;
	}
	else
		wcout << L"取消修改。" << endl;

	if(yesorno(L"是否继续修改"))
		chg_student();
}

wchar_t* teacher_id2name(int teacher_id)
{
		vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
			db_teacher.getData().end(),
			teacher_id_equal(teacher_id));
		return it->name;
}

void print_teacher_info()
{
	//打印教师信息
	tablemaker tb(2);
	tb << L"教师编号";
	tb << L"姓名";
	for(int i=0; i<db_teacher.getData().size(); i++)
	{
		tb << db_teacher.getData()[i].id;
		tb << db_teacher.getData()[i].name;
	}
	tb.put();
}

void print_teacher_info(teacher s)
{
	//打印单个教师信息
	tablemaker tb(2);
	tb << L"教师编号";
	tb << L"姓名";
	tb << s.id;
	tb << s.name;
	tb.put();
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

vector<teacher>::iterator choose_teacher() //根据用户输入选择教师（并打印教师信息）
{
	wifstream infile("config/choose_teacher.config");
	try{
		infile.imbue(std::locale(""));
	}
	catch(runtime_error){}

	MenuCreator a(infile);
	bool flag = false;
	vector<teacher>::iterator it;
	int selected = a.print_and_choose();
	int userinput,teacher_id;
	wstring teacher_name;
	print_teacher_info();
	while(not flag)
	{
		switch(selected)
		{
			case 1:
				userinput = getinput<int>(L"请输入序号：");
				if(userinput > db_teacher.getData().size() or userinput <= 0)
				{
					wcout << L"无此序号。" << endl;
					break;
				}
				it = db_teacher.getData().begin() + userinput -1;
				flag = true;
				break;
			case 2:
				teacher_id = getinput<int>(L"请输入教师编号：");
				it = find_if(db_teacher.getData().begin(),
					db_teacher.getData().end(),
					teacher_id_equal(teacher_id));

				if(it == db_teacher.getData().end())
					wcout << L"无此教师编号" << endl;
				else
					flag = true;
				break;

			case 3:
				teacher_name = getinput<wstring>(L"请输入姓名：");
				it = find_if(db_teacher.getData().begin(),
					db_teacher.getData().end(),
					teacher_name_equal(teacher_name));

				if(it == db_teacher.getData().end())
					wcout << L"无此姓名." << endl;
				else
					flag = true;
				break;
			
			default:
				return db_teacher.getData().end();
				break;
		}
	}

	wcout << L"教师信息如下：" << endl;
	print_teacher_info(*it);
	return it;
}

void add_teacher()
{
	teacher ns;
	ns.id = getinput<int>(L"请输入教师编号：");
	vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
		db_teacher.getData().end(),
		teacher_id_equal(ns.id));

	if(it != db_teacher.getData().end()) //编号唯一
	{
		wcout << L"编号已存在！";
		return;
	}
	
	wprintf(L"请输入姓名：");
	wcin >> ns.name;
	if(yesorno(L"确认添加"))
	{
		db_teacher.getData().push_back(ns);
		db_teacher.putData();
		wprintf(L"添加成功！\n");
	}
	else
		wprintf(L"取消添加。");
	if(yesorno(L"继续添加"))
		add_teacher();
}

void print_teacher_schedule(int teacher_id);
void del_teacher()
{
	vector<teacher>::iterator it = choose_teacher();
	if(it == db_teacher.getData().end())
		return;

	vector<teacher_course>::iterator it2 = find_if(db_teacher_course.getData().begin(),
		db_teacher_course.getData().end(),
		teacher_course_teacher_id_equal(it->id)); //老师是否有课
	if(it2 != db_teacher_course.getData().end())
	{
		wcout << L"该老师有课，以下为该老师的课程信息。若删除该老师，对应课程会受到影响。" << endl;
		print_teacher_schedule(it->id);
	}
		
	for(vector<teacher_course>::iterator it3 = db_teacher_course.getData().begin();
			it3 != db_teacher_course.getData().end(); it3++) //检查是否有一个人上的课
		if(it3->teacher_id == it->id) //找到该老师的课程
		{
			if(count_if(db_teacher_course.getData().begin(),
			 db_teacher_course.getData().end(),
			 teacher_course_course_id_equal(it3->course_id)) == 1) //这门课只有一个人上
			{
				wcout << course_id2name(it3->course_id) << L"课程只有一个人上，请先删除该课程。" << endl;
				return;
			}
		}

	if(yesorno(L"是否确定删除"))
	{	
		
		db_teacher_course.getData().erase(
				remove_if(db_teacher_course.getData().begin(),
					db_teacher_course.getData().end(),
					teacher_course_teacher_id_equal(it->id)),
				db_teacher_course.getData().end()
				); ////删除对应任课记录
		
		db_teacher.getData().erase(it);
		db_teacher.putData();
		db_teacher_course.putData();
		
		wcout << L"删除成功！" << endl;
	}
	else
		wcout << L"取消删除。" << endl;

	if(yesorno(L"是否继续删除"))
		del_teacher();
}

void chg_teacher()
{
	vector<teacher>::iterator it = choose_teacher();
	if(it == db_teacher.getData().end())
		return;

	if(yesorno(L"是否确定修改"))
	{
		bool flag = false;
		teacher n(*it);
		if(yesorno(L"是否修改姓名"))
		{
			flag = true;
			wchar_t newname[35];
			wcout << L"输入新姓名：";
			wcin >> newname;
			wcscpy(n.name, newname);
		}
		if(flag)
		{
			*it = n;
			db_teacher.putData();
			wcout << L"修改成功！" << endl;
		}
		else
			wcout << L"未做改动。" << endl;
	}
	else
		wcout << L"取消修改。" << endl;

	if(yesorno(L"是否继续修改"))
		chg_teacher();
}

wchar_t* course_id2name(int);
void print_teacher_course_info(vector<teacher_course>& vtc)
{
	tablemaker tb(4);
	tb << L"教师编号";
	tb << L"教师名称";
	tb << L"课程编号";
	tb << L"课程名";
	
	for(vector<teacher_course>::iterator it=vtc.begin(); it!=vtc.end(); it++)
	{
		tb << it->teacher_id;
		tb << teacher_id2name(it->teacher_id);
		tb << it->course_id;
		tb << course_id2name(it->course_id);
	}
	tb.put();
}

void print_teacher_course_info_teacher_only(vector<teacher_course>& vtc)
{
	tablemaker tb(2);
	tb << L"教师编号";
	tb << L"教师名称";
	
	for(vector<teacher_course>::iterator it=vtc.begin(); it!=vtc.end(); it++)
	{
		tb << it->teacher_id;
		tb << teacher_id2name(it->teacher_id);
	}
	tb.put();
}

void print_teacher_course_info(teacher_course tc)
{
	tablemaker tb(4);
	tb << L"教师编号";
	tb << L"教师名称";
	tb << L"课程编号";
	tb << L"课程名";
	
	tb << tc.teacher_id;
	tb << teacher_id2name(tc.teacher_id);
	tb << tc.course_id;
	tb << course_id2name(tc.course_id);
	tb.put();
}

void print_course_info()
{
	//打印课程信息
	tablemaker tb(4);
	tb << L"课程编号";
	tb << L"课程名称";
	tb << L"学分";
	tb << L"任课老师";
	
	for(int i=0; i<db_course.getData().size(); i++)
	{
		tb << db_course.getData()[i].id;
		tb << db_course.getData()[i].name;
		tb << db_course.getData()[i].credit;
		
		wstring teachers;
		for(vector<teacher_course>::iterator it = db_teacher_course.getData().begin();
			it != db_teacher_course.getData().end(); it++)
		if(it->course_id == db_course.getData()[i].id)
		{
			teachers += teacher_id2name(it->teacher_id);
			teachers += L"，";
		}
		tb << teachers;
	}
	tb.put();
}

void print_course_info(course s)
{
	//打印单个课程信息
	tablemaker tb(3);
	tb << L"课程编号";
	tb << L"课程名称";
	tb << L"学分";
	tb << s.id;
	tb << s.name;
	tb << s.credit;
	tb.put(false);
}

vector<course>::iterator choose_course() //根据用户输入选择课程（并打印课程信息）
{
	wifstream infile("config/choose_course.config");
	try{
		infile.imbue(std::locale(""));
	}
	catch(runtime_error){}

	MenuCreator a(infile);
	bool flag = false;
	vector<course>::iterator it;
	int userinput,course_id;
	wstring course_name;
	while(not flag)
	{
		print_course_info();
		int selected = a.print_and_choose();
		switch(selected)
		{
			case 1:
				userinput = getinput<int>(L"请输入序号：");
				if(userinput > db_course.getData().size() or userinput <= 0)
				{
					wcout << L"无此序号。" << endl;
					break;
				}
				it = db_course.getData().begin() + userinput -1;
				flag = true;
				break;
			case 2:
				course_id = getinput<int>(L"请输入课程编号：");
				it = find_if(db_course.getData().begin(),
					db_course.getData().end(),
					course_id_equal(course_id));

				if(it == db_course.getData().end())
					wcout << L"无此课程编号" << endl;
				else
					flag = true;
				break;

			case 3:
				course_name = getinput<wstring>(L"请输入课程名称：");
				it = find_if(db_course.getData().begin(),
					db_course.getData().end(),
					course_name_equal(course_name));

				if(it == db_course.getData().end())
					wcout << L"无此课程名称." << endl;
				else
					flag = true;
				break;
			default:
				return db_course.getData().end();
		}
	}

	wcout << L"课程信息如下：" << endl;
	print_course_info(*it);
	return it;
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
	ns.id = getinput<int>(L"请输入课程编号：");
	vector<course>::iterator it = find_if(db_course.getData().begin(),
		db_course.getData().end(),
		course_id_equal(ns.id));

	if(it != db_course.getData().end()) //编号唯一
	{
		wcout << L"编号已存在！";
		return;
	}
	
	wprintf(L"请输入课程名称：");
	wcin >> ns.name;
	ns.credit = getinput<int>(L"请输入学分：");
	bool flag_has_teacher = true;
	vector<teacher_course> vtc;
	while(flag_has_teacher)
	{
		wcout << L"请添加上课教师：" << endl;
		vector<teacher>::iterator it = choose_teacher();
		if(it == db_teacher.getData().end())
		return;

		teacher_course tc;
		tc.teacher_id = it->id;
		tc.course_id = ns.id;
		vtc.push_back(tc);


		wcout << L"目前已选教师名单：" << endl;
		print_teacher_course_info_teacher_only(vtc);

		flag_has_teacher = yesorno(L"继续添加上课教师");
	}
	
	for(vector<teacher_course>::iterator it_tc=vtc.begin(); it_tc!=vtc.end(); it_tc++)
		db_teacher_course.getData().push_back(*it_tc);
	db_teacher_course.putData();
	
	if(yesorno(L"确认添加课程"))
	{
		db_course.getData().push_back(ns);
		db_course.putData();
		wprintf(L"添加课程成功！\n");
	}
	else
		wprintf(L"取消添加课程。\n");
	if(yesorno(L"继续添加课程"))
		add_course();
}

void del_course()
{
	vector<course>::iterator it=choose_course();
	if(it == db_course.getData().end())
		return;
	if(yesorno(L"删除后对应选课记录和任课记录也会删除，是否确定删除"))
	{
		db_student_course.getData().erase(
				remove_if(db_student_course.getData().begin(),
					db_student_course.getData().end(),
					student_course_course_id_equal(it->id)),
				db_student_course.getData().end()
				); //删除对应选课记录

		db_teacher_course.getData().erase(
				remove_if(db_teacher_course.getData().begin(),
					db_teacher_course.getData().end(),
					teacher_course_course_id_equal(it->id)),
				db_teacher_course.getData().end()
				); ////删除对应任课记录

		db_course.getData().erase(it);
		db_student_course.putData();
		db_teacher_course.putData();
		db_course.putData();

		wcout << L"删除成功！" << endl;
	}
	else
		wcout << L"取消删除。" << endl;

	if(yesorno(L"是否继续删除"))
		del_course();
}

void chg_course()
{
	vector<course>::iterator it=choose_course();
	if(it == db_course.getData().end())
		return;
	if(yesorno(L"是否确定修改"))
	{
		bool flag = false;
		course n(*it);
		if(yesorno(L"是否修改课程名称"))
		{
			flag = true;
			wchar_t newname[35];
			wcout << L"输入新课程名称：";
			wcin >> newname;
			wcscpy(n.name, newname);
		}
		if(yesorno(L"是否修改学分"))
		{
			flag = true;
			n.credit = getinput<int>(L"输入新学分：");
		}
		if(flag)
		{
			*it = n;
			db_course.putData();
			wcout << L"修改成功！" << endl;
		}
		else
			wcout << L"未做改动。" << endl;
	}
	else
		wcout << L"取消修改。" << endl;
	if(yesorno(L"是否继续修改"))
		chg_course();
}

wchar_t* course_id2name(int course_id)
{
		vector<course>::iterator it = find_if(db_course.getData().begin(),
			db_course.getData().end(),
			course_id_equal(course_id));
		return it->name;
}

void print_student_course_info()
{
	//打印选课信息
	tablemaker tb(4);
	tb << L"学号";
	tb << L"姓名";
	tb << L"课程编号";
	tb << L"课程名";
	for(int i=0; i<db_student_course.getData().size(); i++)
	{
		tb << db_student_course.getData()[i].student_id;
		tb << student_id2name(db_student_course.getData()[i].student_id);
		tb << db_student_course.getData()[i].course_id;
		tb << course_id2name(db_student_course.getData()[i].course_id);
	}
	tb.put();
}

void print_student_course_info(const student_course& s)
{
	//打印单个选课信息
	tablemaker tb(4);
	tb << L"学号";
	tb << L"姓名";
	tb << L"课程编号";
	tb << L"课程名";
	tb << s.student_id;
	tb << student_id2name(s.student_id);
	tb << s.course_id;
	tb << course_id2name(s.course_id);
	tb.put(false);
}

void print_student_schedule(int student_id)
{
	//打印选课信息
	tablemaker tb(5);
	tb << L"学号";
	tb << L"姓名";
	tb << L"课程编号";
	tb << L"课程名";
	tb << L"分数";
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].student_id == student_id)
		{
			tb << db_student_course.getData()[i].student_id;
			tb << student_id2name(db_student_course.getData()[i].student_id);
			tb << db_student_course.getData()[i].course_id;
			tb << course_id2name(db_student_course.getData()[i].course_id);
			if(db_student_course.getData()[i].score >= 0)
				tb << db_student_course.getData()[i].score;
			else
				tb << L"未登记";
		}
		tb.put();
}

void print_teacher_schedule(int teacher_id)
{
	//打印任课信息
	tablemaker tb(4);
	tb << L"编号";
	tb << L"姓名";
	tb << L"课程编号";
	tb << L"课程名";
	for(int i=0; i<db_teacher_course.getData().size(); i++)
		if(db_teacher_course.getData()[i].teacher_id == teacher_id)
		{
			tb << db_teacher_course.getData()[i].teacher_id;
			tb << teacher_id2name(db_teacher_course.getData()[i].teacher_id);
			tb << db_teacher_course.getData()[i].course_id;
			tb << course_id2name(db_teacher_course.getData()[i].course_id);
		}
		tb.put();
}

template < class T >
void print_course_schedule_if(int course_id, T func)
{
	//打印选课/成绩信息
	tablemaker tb(5);
	tb << L"学号";
	tb << L"姓名";
	tb << L"课程编号";
	tb << L"课程名";
	tb << L"分数";
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].course_id == course_id
				and func(db_student_course.getData()[i]))
		{
				tb << db_student_course.getData()[i].student_id;
				tb << student_id2name(db_student_course.getData()[i].student_id);
				tb << db_student_course.getData()[i].course_id;
				tb << course_id2name(db_student_course.getData()[i].course_id);
				if(db_student_course.getData()[i].score >= 0)
					tb << db_student_course.getData()[i].score;
				else
					tb << L"未登记";
		}
		tb.put();
}

void print_course_schedule(int course_id)
{
	//打印选课/成绩信息
	tablemaker tb(5);
	tb << L"学号";
	tb << L"姓名";
	tb << L"课程编号";
	tb << L"课程名";
	tb << L"分数";
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].course_id == course_id)
		{
				tb << db_student_course.getData()[i].student_id;
				tb << student_id2name(db_student_course.getData()[i].student_id);
				tb << db_student_course.getData()[i].course_id;
				tb << course_id2name(db_student_course.getData()[i].course_id);
				if(db_student_course.getData()[i].score >= 0)
					tb << db_student_course.getData()[i].score;
				else
					tb << L"未登记";
		}
		tb.put();
}

void get_per_student_course() //单个学生课表
{
	vector<student>::iterator it=choose_student();
	if(it == db_student.getData().end())
		return;

	wcout << L"学生课表如下：" << endl;
	
	print_student_schedule(it->id);
}

void get_per_teacher_course() //单个老师课表
{
	vector<teacher>::iterator it=choose_teacher();
	if(it == db_teacher.getData().end())
		return;

	wcout << L"教师课表如下：" << endl;
	
	print_teacher_schedule(it->id);
}

void get_per_course() //单科成绩表
{
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return;
	wcout << L"成绩信息如下：" << endl;

	print_course_schedule(it2->id);
}

void print_fail_student() //不及格同学
{
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return;
	wcout << L"不及格同学如下：" << endl;

	print_course_schedule_if(it2->id,score_less_then(60));
}

void print_excel_student() //优秀同学
{
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return;
	wcout << L"优秀同学如下：" << endl;

	print_course_schedule_if(it2->id,score_not_less_then(85));
}

void add_student_course()
{
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
		
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return;
	
	student_course nst;
	nst.student_id = it->id;
	nst.course_id = it2->id;
	nst.score = -1;
	
	wcout << L"选课信息如下：" << endl;
	print_student_course_info(nst);
	
	if(yesorno(L"确认提交"))
	{
		db_student_course.getData().push_back(nst);
		db_student_course.putData();
		wcout << L"提交成功！" << endl;
	}
	
	if(yesorno(L"是否继续添加"))
		add_student_course();
}



vector<student_course>::iterator get_student_course()
{
	vector<student>::iterator it=choose_student();
	if(it == db_student.getData().end())
		return db_student_course.getData().end();
		
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return db_student_course.getData().end();
		
	vector<student_course>::iterator it3; it3 = find_if( 
	db_student_course.getData().begin(),
		db_student_course.getData().end(),
		student_course_equal(it->id,it2->id));
	return it3;
}

vector<teacher_course>::iterator get_teacher_course()
{
	vector<teacher>::iterator it=choose_teacher();
	if(it == db_teacher.getData().end())
		return db_teacher_course.getData().end();
		
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return db_teacher_course.getData().end();
		
	vector<teacher_course>::iterator it3; it3 = find_if( 
	db_teacher_course.getData().begin(),
		db_teacher_course.getData().end(),
		teacher_course_equal(it->id,it2->id));
	return it3;
}

void print_student_course()
{
	print_student_info();
	bool flag = false; //是否取得信息
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
	

	wcout << L"学生信息如下：" << endl;
	print_student_info(*it);
	wcout << L"学生课程信息如下：" << endl;
	print_student_schedule(it->id);

}

void del_student_course()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
	{
		wcout << L"无此选课." << endl;
		return;
	}
	else
		if(yesorno(L"确认删除"))
		{
			db_student_course.getData().erase(it3);
			db_student_course.putData();
			wcout << L"删除成功" << endl;
		}
	if(yesorno(L"是否继续删除"))
		del_student_course();
}

void del_teacher_course()
{
	vector<teacher_course>::iterator it = get_teacher_course();
	if(it == db_teacher_course.getData().end())
	{
		wcout << L"无此任课." << endl;
		return;
	}

	if(count_if(db_teacher_course.getData().begin(),
		 db_teacher_course.getData().end(),
		 teacher_course_course_id_equal(it->course_id)) == 1) //这门课只有一个人上
	{
		wcout << L"课程只有一个人上，请直接删除该课程。" << endl;
		return;
	}
	
	wcout << L"任课信息如下：" << endl;
	print_teacher_course_info(*it);
	
	if(yesorno(L"确认删除"))
	{
		db_teacher_course.getData().erase(it);
		db_teacher_course.putData();
		wcout << L"删除成功" << endl;
	}
	if(yesorno(L"是否继续删除"))
		del_teacher_course();
}

void add_score()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
	{
		wcout << L"无此选课." << endl;
		return;
	}
	else
		if(yesorno(L"确认添加成绩"))
		{
			int score = getinput<int>(L"请输入分数：");
			it3->score = score;
			db_student_course.putData();
		}
	if(yesorno(L"是否继续添加"))
		add_score();
}

void del_score()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
	{
		wcout << L"无此选课." << endl;
		return;
	}
	else
		if(yesorno(L"确认删除"))
		{
			it3->score = -1;
			db_student_course.putData();
		}
	if(yesorno(L"是否继续删除"))
		del_score();

}

void welcome_page()
{
	wcout << L"欢迎使用教务系统！" << endl;
	wcout << L"====================" << endl;
	
	tablemaker tb(2);
	tb << L"当前课程数量";
	tb << db_course.getData().size();
	tb << L"任课教师数量";
	tb << db_teacher.getData().size();
	tb << L"学生选课数量";
	tb << db_student_course.getData().size();
	tb.put(false);
}

void add_teacher_course()
{
	vector<teacher>::iterator it = choose_teacher();
	if(it == db_teacher.getData().end())
		return;
		
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return;
	
	teacher_course nst;
	nst.teacher_id = it->id;
	nst.course_id = it2->id;
	
	wcout << L"信息如下：" << endl;
	print_teacher_course_info(nst);
	
	if(yesorno(L"确认提交"))
	{
		db_teacher_course.getData().push_back(nst);
		db_teacher_course.putData();
		wcout << L"提交成功！" << endl;
	}
	
	if(yesorno(L"是否继续添加"))
		add_teacher_course();
}

int main()
{
	setlocale(LC_ALL, "zh_CN.UTF-8");
	wifstream infile("config/school.config", wifstream::in);
	try{
		infile.imbue(std::locale(""));
	}
	catch(runtime_error){}

	MenuCreator a(infile);
	
	a.bind(L"__MAIN__", welcome_page);
	
	a.bind(L"1_edit_student", print_student_info);
	a.bind(L"1_student_sort_by_id_less", student_sort_by_id_less);
	a.bind(L"2_student_sort_by_id_more", student_sort_by_id_more);
	a.bind(L"3_add_student", add_student);
	a.bind(L"4_del_student", del_student);
	a.bind(L"5_change_student", chg_student);
	
	a.bind(L"1_edit_teacher", print_teacher_info);
	a.bind(L"1_teacher_sort_by_id_less", teacher_sort_by_id_less);
	a.bind(L"2_teacher_sort_by_id_more", teacher_sort_by_id_more);
	a.bind(L"3_add_teacher", add_teacher);
	a.bind(L"4_del_teacher", del_teacher);
	a.bind(L"5_change_teacher", chg_teacher);
	
	a.bind(L"2_edit_course_list", print_course_info);
	a.bind(L"1_course_sort_by_id_less", course_sort_by_id_less);
	a.bind(L"20_course_sort_by_id_more", course_sort_by_id_more);
	a.bind(L"21_course_sort_by_credit_less", course_sort_by_credit_less);
	a.bind(L"22_course_sort_by_credit_more", course_sort_by_credit_more);
	a.bind(L"3_add_course", add_course);
	a.bind(L"4_del_course", del_course);
	a.bind(L"5_change_course", chg_course);
	a.bind(L"6_add_teacher_course", add_teacher_course);
	a.bind(L"7_del_teacher_course", del_teacher_course);

	a.bind(L"3_edit_student_course", print_student_course_info);
	a.bind(L"1_add_student_course", add_student_course);
	a.bind(L"2_del_student_course", del_student_course);

	a.bind(L"1_add_score",add_score);
	a.bind(L"2_add_score",del_score);

	a.bind(L"1_student_list", print_student_info);
	a.bind(L"11_teacher_list", print_teacher_info);
	a.bind(L"2_course_list", print_course_info);
	a.bind(L"3_student_course", get_per_student_course);
	a.bind(L"4_teacher_course", get_per_teacher_course);
	a.bind(L"5_course_score", get_per_course);
	a.bind(L"6_student_score", get_per_student_course);
	a.bind(L"7_fail_student", print_fail_student);
	a.bind(L"8_excel_student", print_excel_student);

	
	a.execute();
}
