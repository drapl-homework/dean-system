
struct student //学生数据项
{
	int id; //学号
	char name[35]; //姓名
};

struct teacher //教师数据项
{
	int id; //编号
	char name[35]; //姓名
};

struct course //课程数据项
{
	int id; //课程编号
	char name[35]; //课程名
	int credit; //学分
};

struct student_course //学生选课、成绩数据项
{
	int student_id; //学生学号
	int course_id; //选课课程编号
	int score; //分数
};

struct teacher_course //教师上课数据项
{
	int teacher_id; //教师编号
	int course_id; //课程编号
};
