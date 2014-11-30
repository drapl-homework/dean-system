#include <string>

struct student //学生数据项
{
	int id; //学号
	char name[35]; //姓名
};

int student_id_less(student s1, student s2)
{
	return s1.id < s2.id;
}

int student_id_more(student s1, student s2)
{
	return s1.id > s2.id;
}

class student_id_equal
{
	public:
		student_id_equal(int a): x(a) {}
		bool operator()(student s1) { return s1.id == x;}
	private:
		int x;
};

class student_name_equal
{
	public:
		student_name_equal(string a): x(a) {}
		bool operator()(student s1) { return x == s1.name;}
	private:
		string x;
};

struct teacher //教师数据项
{
	int id; //编号
	char name[35]; //姓名
};

int teacher_id_less(teacher s1, teacher s2)
{
	return s1.id < s2.id;
}

int teacher_id_more(teacher s1, teacher s2)
{
	return s1.id > s2.id;
}

class teacher_id_equal
{
	public:
		teacher_id_equal(int a): x(a) {}
		bool operator()(teacher s1) { return s1.id == x;}
	private:
		int x;
};

class teacher_name_equal
{
	public:
		teacher_name_equal(string a): x(a) {}
		bool operator()(teacher s1) { return x == s1.name;}
	private:
		string x;
};

struct course //课程数据项
{
	int id; //课程编号
	char name[35]; //课程名
	int credit; //学分
};

int course_id_less(course s1, course s2)
{
	return s1.id < s2.id;
}

int course_id_more(course s1, course s2)
{
	return s1.id > s2.id;
}

int course_credit_less(course s1, course s2)
{
	return s1.credit < s2.credit;
}

int course_credit_more(course s1, course s2)
{
	return s1.credit > s2.credit;
}

class course_id_equal
{
	public:
		course_id_equal(int a): x(a) {}
		bool operator()(course s1) { return s1.id == x;}
	private:
		int x;
};

class course_name_equal
{
	public:
		course_name_equal(string a): x(a) {}
		bool operator()(course s1) { return x == s1.name;}
	private:
		string x;
};

struct student_course //学生选课、成绩数据项
{
	int student_id; //学生学号
	int course_id; //选课课程编号
	int score; //分数
};

class student_course_student_id_equal
{
	public:
		student_course_student_id_equal(int a): x(a) {}
		bool operator()(student_course s1) { return x == s1.student_id;}
	private:
		int x;
};

class student_course_course_id_equal
{
	public:
		student_course_course_id_equal(int a): x(a) {}
		bool operator()(student_course s1) { return x == s1.course_id;}
	private:
		int x;
};

class student_course_equal
{
	public:
		student_course_equal(int a, int b): x(a),y(b) {}
		bool operator()(student_course s1) { return x == s1.student_id and y == s1.course_id;}
	private:
		int x;
		int y;
};


struct teacher_course //教师上课数据项
{
	int teacher_id; //教师编号
	int course_id; //课程编号
};
