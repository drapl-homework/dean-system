#include "menu.h"
#include "db.h"
#include "school.h"
#include "tablemaker.h"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <iomanip>

using namespace std;

db<student> db_student(string("config/student.db"));
db<teacher> db_teacher(string("config/teacher.db"));
db<course> db_course(string("config/course.db"));
db<student_course> db_student_course(string("config/student_course.db"));
db<teacher_course> db_teacher_course(string("config/teacher_course.db"));

char* course_id2name(int);

template < typename T >
T getinput(string s)
{
	T tmp;
	cout << s;
	cin >> tmp;
	return tmp;
}

bool yesorno(string tips) //选择“是”“否”
{
	char flag;
	while(not (flag == 'y' or flag == 'Y' or flag == 'N' or flag == 'n' ))
	{
		cout << tips << "(y/n)？";
		cin >> flag;
	}
	if(flag == 'y' or flag == 'Y')
		return true;
	if(flag == 'n' or flag == 'N')
		return false;
}

char* student_id2name(int student_id)
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
	
	tb << "学号";
	tb << "姓名";
	for(int i=0; i<db_student.getData().size(); i++)
	{
				tb << db_student.getData()[i].id;
				tb << db_student.getData()[i].name;
	}
	tb.put();
}

void print_student_info(student s)
{
	//打印单个学生信息
	tablemaker tb(2);
	tb << "学号";
	tb << "姓名";
	tb << s.id;
	tb << s.name;
	tb.put();
}

vector<student>::iterator choose_student() //根据用户输入选择学生（并打印学生信息）
{
	ifstream infile("config/choose_student.config");
	MenuCreator a(infile);
	bool flag = false;
	vector<student>::iterator it;
	int userinput,student_id;
	string student_name;
	print_student_info();
	while(not flag)
	{
		int selected = a.print_and_choose();
		switch(selected)
		{
			case 1:
				userinput = getinput<int>("请输入序号：");
				if(userinput > db_student.getData().size() or userinput <= 0)
				{
					cout << "无此序号。" << endl;
					break;
				}
				it = db_student.getData().begin() + userinput -1;
				flag = true;
				break;
			case 2:
				student_id = getinput<int>("请输入学号：");
				it = find_if(db_student.getData().begin(),
					db_student.getData().end(),
					student_id_equal(student_id));

				if(it == db_student.getData().end())
					cout << "无此学号" << endl;
				else
					flag = true;
				break;

			case 3:
				student_name = getinput<string>("请输入姓名：");
				it = find_if(db_student.getData().begin(),
					db_student.getData().end(),
					student_name_equal(student_name));

				if(it == db_student.getData().end())
					cout << "无此姓名." << endl;
				else
					flag = true;
				break;
			
			default:
				return db_student.getData().end();
				break;
		}
	}

	cout << "学生信息如下：" << endl;
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
	
	ns.id = getinput<int>("请输入学号：");
	vector<student>::iterator it = find_if(db_student.getData().begin(),
		db_student.getData().end(),
		student_id_equal(ns.id));

	if(it != db_student.getData().end()) //学号唯一
	{
		cout << "学号已存在！";
		return;
	}
	
	printf("请输入姓名：");
	cin >> ns.name;
	if(yesorno("确认添加"))
	{
		db_student.getData().push_back(ns);
		db_student.putData();
		printf("添加成功！\n");
	}
	else
		printf("取消添加。");
	if(yesorno("继续添加"))
		add_student();
}

void del_student()
{
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
	
	if(yesorno("是否确定删除"))
	{
		db_student.getData().erase(it);
		db_student.putData();
		cout << "删除成功！" << endl;
	}
	else
		cout << "取消删除。" << endl;

	if(yesorno("是否继续删除"))
		del_student();
}

void chg_student()
{
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
		
	if(yesorno("是否确定修改"))
	{
		bool flag = false;
		if(yesorno("是否修改学号"))
		{
			int newid = getinput<int>("输入新学号：");
			vector<student>::iterator it2 = find_if(db_student.getData().begin(),
				db_student.getData().end(),
				student_id_equal(newid));
		
			if(it2 != db_student.getData().end()) //学号唯一
			{
				cout << "学号已存在！";
				return;
			}
			it->id = newid;
			flag = true;
		}
		if(yesorno("是否修改姓名"))
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

	if(yesorno("是否继续修改"))
		chg_student();
}

char* teacher_id2name(int teacher_id)
{
		vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
			db_teacher.getData().end(),
			teacher_id_equal(teacher_id));
		return it->name;
}

// 复制粘贴替换：stu*dent->teacher, 学*号->教师编号，学生->教师
void print_teacher_info()
{
	//打印教师信息
	tablemaker tb(2);
	tb << "教师编号";
	tb << "姓名";
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
	tb << "教师编号";
	tb << "姓名";
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
	ifstream infile("config/choose_teacher.config");
	MenuCreator a(infile);
	bool flag = false;
	vector<teacher>::iterator it;
	int selected = a.print_and_choose();
	int userinput,teacher_id;
	string teacher_name;
	print_teacher_info();
	while(not flag)
	{
		switch(selected)
		{
			case 1:
				userinput = getinput<int>("请输入序号：");
				if(userinput > db_teacher.getData().size() or userinput <= 0)
				{
					cout << "无此序号。" << endl;
					break;
				}
				it = db_teacher.getData().begin() + userinput -1;
				flag = true;
				break;
			case 2:
				teacher_id = getinput<int>("请输入教师编号：");
				it = find_if(db_teacher.getData().begin(),
					db_teacher.getData().end(),
					teacher_id_equal(teacher_id));

				if(it == db_teacher.getData().end())
					cout << "无此教师编号" << endl;
				else
					flag = true;
				break;

			case 3:
				teacher_name = getinput<string>("请输入姓名：");
				it = find_if(db_teacher.getData().begin(),
					db_teacher.getData().end(),
					teacher_name_equal(teacher_name));

				if(it == db_teacher.getData().end())
					cout << "无此姓名." << endl;
				else
					flag = true;
				break;
			
			default:
				return db_teacher.getData().end();
				break;
		}
	}

	cout << "教师信息如下：" << endl;
	print_teacher_info(*it);
	return it;
}

void add_teacher()
{
	teacher ns;
	ns.id = getinput<int>("请输入教师编号：");
	vector<teacher>::iterator it = find_if(db_teacher.getData().begin(),
		db_teacher.getData().end(),
		teacher_id_equal(ns.id));

	if(it != db_teacher.getData().end()) //编号唯一
	{
		cout << "编号已存在！";
		return;
	}
	
	printf("请输入姓名：");
	cin >> ns.name;
	if(yesorno("确认添加"))
	{
		db_teacher.getData().push_back(ns);
		db_teacher.putData();
		printf("添加成功！\n");
	}
	else
		printf("取消添加。");
	if(yesorno("继续添加"))
		add_teacher();
}

void del_teacher()
{
	vector<teacher>::iterator it = choose_teacher();
	if(it == db_teacher.getData().end())
		return;

	vector<teacher_course>::iterator it2 = find_if(db_teacher_course.getData().begin(),
		db_teacher_course.getData().end(),
		teacher_course_teacher_id_equal(it->id)); //老师是否有课
	if(it2 != db_teacher_course.getData().end())
		cout << "该老师有课，以下为该老师的课程信息。若删除该老师，对应课程会受到影响。" << endl;
		
	for(vector<teacher_course>::iterator it3 = db_teacher_course.getData().begin();
			it3 != db_teacher_course.getData().end(); it3++) //检查是否有一个人上的课
		if(it3->teacher_id == it->id) //找到该老师的课程
		{
			if(count_if(db_teacher_course.getData().begin(),
			 db_teacher_course.getData().end(),
			 teacher_course_course_id_equal(it3->course_id)) == 1) //这门课只有一个人上
			{
				cout << course_id2name(it3->course_id) << "课程只有一个人上，请先删除该课程。" << endl;
				return;
			}
		}

	if(yesorno("是否确定删除"))
	{
		db_teacher.getData().erase(it);
		db_teacher.putData();
		
		for(vector<teacher_course>::iterator it3 = db_teacher_course.getData().begin();
			it3 != db_teacher_course.getData().end(); it3++) //删除对应的teacher_course
			if(it3->teacher_id == it->id) //找到该老师的课程
				db_teacher_course.getData().erase(it3);
		db_teacher_course.putData();
		
		cout << "删除成功！" << endl;
	}
	else
		cout << "取消删除。" << endl;

	if(yesorno("是否继续删除"))
		del_teacher();
}

void chg_teacher()
{
	vector<teacher>::iterator it = choose_teacher();
	if(it == db_teacher.getData().end())
		return;

	if(yesorno("是否确定修改"))
	{
		bool flag = false;
		if(yesorno("是否修改编号"))
		{
			int newid = getinput<int>("输入新编号：");
			vector<teacher>::iterator it2 = find_if(db_teacher.getData().begin(),
				db_teacher.getData().end(),
				teacher_id_equal(newid));
		
			if(it2 != db_teacher.getData().end()) //编号唯一
			{
				cout << "编号已存在！";
				return;
			}
			it->id = newid;
			flag = true;
		}
		if(yesorno("是否修改姓名"))
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

	if(yesorno("是否继续修改"))
		chg_teacher();
}

char* course_id2name(int);
void print_teacher_course_info(vector<teacher_course>& vtc)
{
	tablemaker tb(4);
	tb << "教师编号";
	tb << "教师名称";
	tb << "课程编号";
	tb << "课程名";
	
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
	tb << "教师编号";
	tb << "教师名称";
	
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
	tb << "教师编号";
	tb << "教师名称";
	tb << "课程编号";
	tb << "课程名";
	
	tb << tc.teacher_id;
	tb << teacher_id2name(tc.teacher_id);
	tb << tc.course_id;
	tb << course_id2name(tc.course_id);
	tb.put();
}

void print_course_info()
{
	//打印课程信息
	tablemaker tb(3);
	tb << "课程编号";
	tb << "课程名称";
	tb << "学分";
	for(int i=0; i<db_course.getData().size(); i++)
	{
		tb << db_course.getData()[i].id;
		tb << db_course.getData()[i].name;
		tb << db_course.getData()[i].credit;
	}
	tb.put();
}

void print_course_info(course s)
{
	//打印单个课程信息
	tablemaker tb(3);
	tb << "课程编号";
	tb << "课程名称";
	tb << "学分";
	tb << s.id;
	tb << s.name;
	tb << s.credit;
	tb.put(false);
}

vector<course>::iterator choose_course() //根据用户输入选择课程（并打印课程信息）
{
	ifstream infile("config/choose_course.config");
	MenuCreator a(infile);
	bool flag = false;
	vector<course>::iterator it;
	int userinput,course_id;
	string course_name;
	while(not flag)
	{
		print_course_info();
		int selected = a.print_and_choose();
		switch(selected)
		{
			case 1:
				userinput = getinput<int>("请输入序号：");
				if(userinput > db_course.getData().size() or userinput <= 0)
				{
					cout << "无此序号。" << endl;
					break;
				}
				it = db_course.getData().begin() + userinput -1;
				flag = true;
				break;
			case 2:
				course_id = getinput<int>("请输入课程编号：");
				it = find_if(db_course.getData().begin(),
					db_course.getData().end(),
					course_id_equal(course_id));

				if(it == db_course.getData().end())
					cout << "无此课程编号" << endl;
				else
					flag = true;
				break;

			case 3:
				course_name = getinput<string>("请输入课程名称：");
				it = find_if(db_course.getData().begin(),
					db_course.getData().end(),
					course_name_equal(course_name));

				if(it == db_course.getData().end())
					cout << "无此课程名称." << endl;
				else
					flag = true;
				break;
			default:
				return db_course.getData().end();
		}
	}

	cout << "课程信息如下：" << endl;
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
	ns.id = getinput<int>("请输入课程编号：");
	vector<course>::iterator it = find_if(db_course.getData().begin(),
		db_course.getData().end(),
		course_id_equal(ns.id));

	if(it != db_course.getData().end()) //编号唯一
	{
		cout << "编号已存在！";
		return;
	}
	
	printf("请输入课程名称：");
	cin >> ns.name;
	ns.credit = getinput<int>("请输入学分：");
	bool flag_has_teacher = true;
	vector<teacher_course> vtc;
	while(flag_has_teacher)
	{
		cout << "请添加上课教师：" << endl;
		vector<teacher>::iterator it = choose_teacher();
		if(it == db_teacher.getData().end())
		return;

		teacher_course tc;
		tc.teacher_id = it->id;
		tc.course_id = ns.id;
		vtc.push_back(tc);


		cout << "目前已选教师名单：" << endl;
		print_teacher_course_info_teacher_only(vtc);

		flag_has_teacher = yesorno("继续添加上课教师");
	}
	
	for(vector<teacher_course>::iterator it_tc=vtc.begin(); it_tc!=vtc.end(); it_tc++)
		db_teacher_course.getData().push_back(*it_tc);
	db_teacher_course.putData();
	
	if(yesorno("确认添加课程"))
	{
		db_course.getData().push_back(ns);
		db_course.putData();
		printf("添加课程成功！\n");
	}
	else
		printf("取消添加课程。\n");
	if(yesorno("继续添加课程"))
		add_course();
}

void del_course()
{
	vector<course>::iterator it=choose_course();
	if(it == db_course.getData().end())
		return;
	if(yesorno("是否确定删除"))
	{
		db_course.getData().erase(it);
		db_course.putData();
		cout << "删除成功！" << endl;
	}
	else
		cout << "取消删除。" << endl;

	if(yesorno("是否继续删除"))
		del_course();
}

void chg_course()
{
	vector<course>::iterator it=choose_course();
	if(it == db_course.getData().end())
		return;
	if(yesorno("是否确定修改"))
	{
		bool flag = false;
		if(yesorno("是否修改课程编号"))
		{
			int newid = getinput<int>("输入新课程编号：");
			vector<course>::iterator it2 = find_if(db_course.getData().begin(),
				db_course.getData().end(),
				course_id_equal(newid));
		
			if(it2 != db_course.getData().end()) //编号唯一
			{
				cout << "编号已存在！";
				return;
			}
			it->id = newid;
			flag = true;
		}
		if(yesorno("是否修改课程名称"))
		{
			flag = true;
			char newname[35];
			cout << "输入新课程名称：";
			cin >> newname;
			strcpy(it->name, newname);
		}
		if(yesorno("是否修改学分"))
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
	if(yesorno("是否继续修改"))
		chg_course();
}

char* course_id2name(int course_id)
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
	tb << "学号";
	tb << "姓名";
	tb << "课程编号";
	tb << "课程名";
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
	tb << "学号";
	tb << "姓名";
	tb << "课程编号";
	tb << "课程名";
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
	tb << "学号";
	tb << "姓名";
	tb << "课程编号";
	tb << "课程名";
	tb << "分数";
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].student_id == student_id)
		{
			tb << db_student_course.getData()[i].student_id;
			tb << student_id2name(db_student_course.getData()[i].student_id);
			tb << db_student_course.getData()[i].course_id;
			tb << course_id2name(db_student_course.getData()[i].course_id);
			tb << db_student_course.getData()[i].score;
		}
		tb.put();
}

void print_course_schedule(int course_id)
{
	//打印选课信息
	tablemaker tb(5);
	tb << "学号";
	tb << "姓名";
	tb << "课程编号";
	tb << "课程名";
	tb << "分数";
	for(int i=0; i<db_student_course.getData().size(); i++)
		if(db_student_course.getData()[i].course_id == course_id)
		{
				tb << db_student_course.getData()[i].student_id;
				tb << student_id2name(db_student_course.getData()[i].student_id);
				tb << db_student_course.getData()[i].course_id;
				tb << course_id2name(db_student_course.getData()[i].course_id);
				tb << db_student_course.getData()[i].score;
		}
		tb.put();
}

void get_per_student_course()
{
	vector<student>::iterator it=choose_student();
	if(it == db_student.getData().end())
		return;

	cout << "学生课表如下：" << endl;
	print_student_info(*it);
	
	print_student_schedule(it->id);
}

void get_per_course()
{
	bool flag = false;

	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return;
	cout << "课程学生信息如下：" << endl;

	print_course_schedule(it2->id);
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
	
	cout << "选课信息如下：" << endl;
	print_student_course_info(nst);
	
	if(yesorno("确认提交"))
	{
		db_student_course.getData().push_back(nst);
		db_student_course.putData();
		cout << "提交成功！" << endl;
	}
	
	if(yesorno("是否继续添加"))
		add_student_course();
}

vector<student_course>::iterator get_student_course()
{
	vector<student>::iterator it=choose_student();
	if(it == db_student.getData().end())
		return db_student_course.getData().end();;
		
	vector<course>::iterator it2=choose_course();
	if(it2 == db_course.getData().end())
		return db_student_course.getData().end();
		
	vector<student_course>::iterator it3; it3 = find_if( 
	db_student_course.getData().begin(),
		db_student_course.getData().end(),
		student_course_equal(it->id,it2->id));
	return it3;
}

void print_student_course()
{
	print_student_info();
	bool flag = false; //是否取得信息
	vector<student>::iterator it = choose_student();
	if(it == db_student.getData().end())
		return;
	

	cout << "学生信息如下：" << endl;
	print_student_info(*it);
	cout << "学生课程信息如下：" << endl;
	print_student_schedule(it->id);

}

void del_student_course()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
		cout << "无此选课." << endl;
	else
		if(yesorno("确认删除"))
		{
			db_student_course.getData().erase(it3);
			db_student_course.putData();
			cout << "删除成功" << endl;
		}
	if(yesorno("是否继续删除"))
		del_student_course();
}

void add_score()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
		cout << "无此选课." << endl;
	else
		if(yesorno("确认添加"))
		{
			int score = getinput<int>("请输入分数：");
			it3->score = score;
			db_student_course.putData();
		}
	if(yesorno("是否继续添加"))
		add_score();
}

void del_score()
{
	vector<student_course>::iterator it3 = get_student_course();
	if(it3 == db_student_course.getData().end())
		cout << "无此选课." << endl;
	else
		if(yesorno("确认删除"))
		{
			it3->score = -1;
			db_student_course.putData();
		}
	if(yesorno("是否继续删除"))
		del_score();

}

void welcome_page()
{
	cout << "欢迎使用教务系统！" << endl;
	cout << "====================" << endl;
	
	tablemaker tb(2);
	tb << "当前课程数量";
	tb << db_course.getData().size();
	tb << "任课教师数量";
	tb << db_teacher.getData().size();
	tb << "学生选课数量";
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
	
	cout << "信息如下：" << endl;
	print_teacher_course_info(nst);
	
	if(yesorno("确认提交"))
	{
		db_teacher_course.getData().push_back(nst);
		db_teacher_course.putData();
		cout << "提交成功！" << endl;
	}
	
	if(yesorno("是否继续添加"))
		add_teacher_course();
}

int main()
{
	ifstream infile("config/school.config");
	MenuCreator a(infile);
	
	a.bind("__MAIN__", welcome_page);
	
	a.bind("1_edit_student", print_student_info);
	a.bind("1_student_sort_by_id_less", student_sort_by_id_less);
	a.bind("2_student_sort_by_id_more", student_sort_by_id_more);
	a.bind("3_add_student", add_student);
	a.bind("4_del_student", del_student);
	a.bind("5_change_student", chg_student);
	
	a.bind("1_edit_teacher", print_teacher_info);
	a.bind("1_teacher_sort_by_id_less", teacher_sort_by_id_less);
	a.bind("2_teacher_sort_by_id_more", teacher_sort_by_id_more);
	a.bind("3_add_teacher", add_teacher);
	a.bind("4_del_teacher", del_teacher);
	a.bind("5_change_student", chg_teacher);
	
	a.bind("2_edit_course_list", print_course_info);
	a.bind("1_course_sort_by_id_less", course_sort_by_id_less);
	a.bind("20_course_sort_by_id_more", course_sort_by_id_more);
	a.bind("21_course_sort_by_credit_less", course_sort_by_credit_less);
	a.bind("22_course_sort_by_credit_more", course_sort_by_credit_more);
	a.bind("3_add_course", add_course);
	a.bind("4_del_course", del_course);
	a.bind("5_change_course", chg_course);

	a.bind("3_edit_student_course", print_student_course_info);
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
