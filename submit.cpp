
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>

#ifdef WIN32
#include "GL/glew.h"
#include "GL/glaux.h"
#include "GL/glut.h"
#else
#include <GL/glut.h>
#endif 


//机器人的各部分参数
#define FEELER_RADIUS 0.1
#define FEELER_HEIGHT 1.5

//头的半径
#define HEAD_RADIUS 4.0
#define EYE_RADIUS 0.35

#define NECK_RADIUS 4.05
#define NECK_HEIGHT 0.5

#define BODY_HEIGHT 5.0
#define BODY_RADIUS 4.0

#define UPPER_ARM_HEIGHT 2.5
#define UPPER_ARM_RADIUS 0.5

#define LOWER_ARM_HEIGHT 2.5
#define LOWER_ARM_RADIUS 0.35

#define UPPER_LEG_HEIGHT 3.5
#define UPPER_LEG_RADIUS 0.65

#define LOWER_LEG_HEIGHT 3.5
#define LOWER_LEG_RADIUS 0.5

//机器人各部分的id
#define ALL 0
#define BODY 1
#define LUA 2
#define LLA 3
#define RUA 4
#define RLA 5
#define LUL 6
#define LLL 7
#define RUL 8
#define RLL 9
#define LF 10
#define RF 11

//每一个部分旋转角度
#define bendBODYv 0.1
#define bendLUAv 0.8
#define bendLLAv 0.4
#define bendRUAv 0.8
#define bendRLAv 0.4
#define bendLULv 0.8
#define bendLLLv 0.4
#define bendRULv 0.8
#define bendRLLv 0.4
#define bendLFv 0.15
#define bendRFv 0.15

//机器人是否旋转
GLboolean robot_move = false;
//控制机器人各个角度
static float angle = 0.0, ratio;
//摄像机的位置
static float x = 0.0f, y = 8.0f, z = 5.0f;
//摄像机位置增量
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;  //视线方向,初始值向z负号
GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };//指定全局环境光  w为1.0时，光源为定位光源。
GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };//漫反射
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };//镜面反射
GLfloat positionLight[] = { 0.0f, 20.0f, 0.0f, 1.0f };//光源位置

GLfloat model_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

//************************镜面反射****************************
GLfloat mat_spec1[] = { 0.6f, 0.6f, 0.6f, 1.0f };//床的颜色

GLfloat mat_spec16[] = { 0.6f, 0.6f, 0.6f, 1.0 };//床表面
//**************************漫反射*******************************//
GLfloat mat_amb_diff1[] = { 0.6f, 0.6f, 0.6f, 1.0 }; //床的颜色

GLfloat mat_amb_diff16[] = { 0.6f, 0.6f, 0.6f, 1.0 };//床表面



//设置机器人旋转角度
GLfloat theta[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0 };

//设置机器人弯曲方向
GLfloat bendingDirection[] = { 0, -1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1 };

//set states
GLboolean bending_state = GLU_TRUE;
GLboolean rotate_state = GLU_TRUE;

GLUquadricObj *p;   //绘制二次曲面首先要创建二次曲面对象

//材质结构体
typedef struct materialStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} materialStruct;

//为机器人不同的眼睛材质
materialStruct chocolateCakeEyeMaterials = { { 0.21, 0.07, 0.11 }, { 0.41, 0.17, 0.21 }, { 0.61, 0.37, 0.41 }, 15 };
//为机器人不同的头部材质
materialStruct chocolateCakeHeadMaterials = { { 0.675, 0.425, 0.05 }, { 0.775, 0.525, 0.15 }, { 1.0, 0.85, 0.45 }, 50 };
materialStruct chocolateCakeFeelerMaterials = { { 0.0, 0.0, 0.0 }, { 0.4, 0.4, 0.4 }, { 0.85, 0.65, 0.49 }, 5 };
//为机器人不同的脖子材质
materialStruct chocolateCakeNeckMaterials = { { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, 20 };
//为机器人不同的身体材质
materialStruct chocolateCakeBodyMaterials = { { 0.25, 0.05, 0.00 }, { 0.45, 0.25, 0.10 }, { 0.85, 0.65, 0.49 }, 100 };
//为机器人不同的胳膊材质
materialStruct chocolateCakeUpperArmMaterials = { { 0.45, 0.25, 0.09 }, { 0.65, 0.45, 0.29 }, { 0.85, 0.65, 0.49 }, 20 };
materialStruct chocolateCakeLowerArmMaterials = { { 0.71, 0.57, 0.52 }, { 0.91, 0.77, 0.50 }, { 0.9, 0.65, 0.39 }, 20 };
//为机器人不同的腿部材质
materialStruct chocolateCakeUpperLegMaterials = { { 0.30, 0.17, 0.0 }, { 0.55, 0.35, 0.39 }, { 0.85, 0.65, 0.49 }, 20 };
materialStruct chocolateCakeLowerLegMaterials = { { 0.45, 0.25, 0.09 }, { 0.65, 0.45, 0.29 }, { 0.85, 0.65, 0.49 }, 20 };


//机器人各部分材质指针
materialStruct *eyeMaterials;
materialStruct *headMaterials;
materialStruct *feelerMaterials;
materialStruct *neckMaterials;
materialStruct *bodyMaterials;
materialStruct *upperArmMaterials;
materialStruct *lowerArmMaterials;
materialStruct *upperLegMaterials;
materialStruct *lowerLegMaterials;

//光照结构
typedef struct lightingStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
} lightingStruct;

//太阳光
lightingStruct whiteLighting = { { 1.0, 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0, 1.0 } };

//设置当前光照
lightingStruct *currentLighting;

//设置光照位置
GLfloat light_pos0[] = { 15, 15, 15, 1.0 };

//画机器人的身体
void body()
{
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);  //angle为旋转的角度,x轴转
	gluCylinder(p, BODY_RADIUS, BODY_RADIUS, BODY_HEIGHT, 100, 100); //画圆柱
	gluDisk(p, 0, BODY_RADIUS, 100, 100);//绘制一个的圆盘
	glTranslatef(0, 0, BODY_HEIGHT);
	gluDisk(p, 0, BODY_RADIUS, 100, 100);
	glPopMatrix();

}
//画机器人的头部
void head()
{
	glPushMatrix();
	glutSolidSphere(HEAD_RADIUS, 100, 100); //渲染一个球体，r,经线，纬线
	glPopMatrix();
}
//画机器人的脖子
void neck()
{
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);//angle为旋转的角度,x轴转
	gluCylinder(p, NECK_RADIUS, NECK_RADIUS, NECK_HEIGHT, 100, 100); //画圆柱
	glPopMatrix();
}
//画机器人的身体触须
void feeler()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(p, FEELER_RADIUS, FEELER_RADIUS, FEELER_HEIGHT, 100, 100);
	glTranslatef(0.0, 0.0, FEELER_HEIGHT);
	gluDisk(p, 0, FEELER_RADIUS, 100, 100);// 绘制一个的圆盘
	glPopMatrix();

}
//画机器人的下腿部
void lower_leg()
{
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(p, LOWER_LEG_RADIUS, LOWER_LEG_RADIUS, LOWER_LEG_HEIGHT, 100, 100);
	gluDisk(p, 0, LOWER_LEG_RADIUS, 100, 100);
	glTranslatef(0, 0, LOWER_LEG_HEIGHT);
	gluDisk(p, 0, LOWER_LEG_RADIUS, 100, 100);
	glPopMatrix();

}
//画机器人的上腿部
void upper_leg()
{
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(p, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS, UPPER_LEG_HEIGHT, 100, 100);
	gluDisk(p, 0, UPPER_LEG_RADIUS, 100, 100);
	glTranslatef(0, 0, UPPER_LEG_HEIGHT);
	gluDisk(p, 0, UPPER_LEG_RADIUS, 100, 100);
	glPopMatrix();

}
//画机器人的胳膊
void lower_arm()
{
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(p, LOWER_ARM_RADIUS, LOWER_ARM_RADIUS, LOWER_ARM_HEIGHT, 100, 100);
	gluDisk(p, 0, LOWER_ARM_RADIUS, 100, 100);
	glTranslatef(0, 0, LOWER_ARM_HEIGHT);
	gluDisk(p, 0, LOWER_ARM_RADIUS, 100, 100);
	glPopMatrix();

}
//画机器人的肩膀
void upper_arm()
{
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(p, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS, UPPER_ARM_HEIGHT, 100, 100);
	gluDisk(p, 0, UPPER_ARM_RADIUS, 100, 100);
	glTranslatef(0, 0, UPPER_ARM_HEIGHT);
	gluDisk(p, 0, UPPER_ARM_RADIUS, 100, 100);
	glPopMatrix();

}
//画机器人的眼睛
void eye()
{
	glPushMatrix();
	glutSolidSphere(EYE_RADIUS, 100, 100);
	glPopMatrix();

}

//起到设定各部分材料的作用
void chooseMaterial(materialStruct *material)
{
	//物体的光照属性主要包括环境光系数、漫反射光系数、镜反射光系数和镜面指数四个
	glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient);//环境光系数
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse);//漫反射光系数
	glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);//镜反射光系数
	glMaterialfv(GL_FRONT, GL_SHININESS, &material->shininess);//镜面指数

}

//画一个整个机器人
void object()
{

	glRotatef(theta[ALL], 0, 1, 0);//all part should rotate some angles

	//左触角
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(feelerMaterials);
	glTranslatef(BODY_RADIUS / 2, BODY_HEIGHT + NECK_HEIGHT + (HEAD_RADIUS*sqrt(3.0) / 2), 0.0);
	glRotatef(theta[LF], 1, 0, 0);
	glRotatef(-30.0, 0.0, 0.0, 1.0);
	feeler();
	glPopMatrix();

	//右触角
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(feelerMaterials);
	glTranslatef(-BODY_RADIUS / 2, BODY_HEIGHT + NECK_HEIGHT + (HEAD_RADIUS*sqrt(3.0) / 2), 0.0);
	glRotatef(theta[RF], 1, 0, 0);
	glRotatef(30.0, 0.0, 0.0, 1.0);
	feeler();
	glPopMatrix();

	//画头部
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(headMaterials);
	glTranslatef(0, BODY_HEIGHT + NECK_HEIGHT, 0);
	head();
	glPopMatrix();

	//画脖子
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(neckMaterials);
	glTranslatef(0, BODY_HEIGHT + NECK_HEIGHT, 0);
	neck();
	glPopMatrix();

	//画身体
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(bodyMaterials);
	body();
	glPopMatrix();

	//左上下臂
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(upperArmMaterials);
	glTranslatef(-BODY_RADIUS * 8 / 7, 0, 0);
	glTranslatef(0.0, BODY_HEIGHT, 0);
	glRotatef(theta[LUA], 1, 0, 0);
	glTranslatef(0.0, -UPPER_ARM_HEIGHT, 0.0);
	upper_arm();
	chooseMaterial(lowerArmMaterials);
	glRotatef(theta[LLA], 1, 0, 0);
	glTranslatef(0.0, -LOWER_ARM_HEIGHT * 0.9, 0.0);
	lower_arm();
	glPopMatrix();

	//右上下臂
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(upperArmMaterials);
	glTranslatef(BODY_RADIUS * 8 / 7, 0, 0);
	glTranslatef(0.0, BODY_HEIGHT, 0);
	glRotatef(theta[RUA], 1, 0, 0);
	glTranslatef(0.0, -UPPER_ARM_HEIGHT, 0.0);
	upper_arm();
	chooseMaterial(lowerArmMaterials);
	glRotatef(theta[RLA], 1, 0, 0);
	glTranslatef(0.0, -LOWER_ARM_HEIGHT * 0.9, 0.0);
	lower_arm();
	glPopMatrix();

	//左上小腿 
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(upperLegMaterials);
	glTranslatef(-BODY_RADIUS / 2, 0, 0);
	glRotatef(theta[LUL], 1, 0, 0);
	glTranslatef(0.0, -UPPER_LEG_HEIGHT, 0.0);
	upper_leg();
	chooseMaterial(lowerLegMaterials);
	glRotatef(theta[LLL], 1, 0, 0);
	glTranslatef(0.0, -LOWER_LEG_HEIGHT * 0.9, 0.0);
	lower_leg();
	glPopMatrix();

	//右上小腿
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(upperLegMaterials);
	glTranslatef(BODY_RADIUS / 2, 0, 0);
	glRotatef(theta[RUL], 1, 0, 0);
	glTranslatef(0.0, -UPPER_LEG_HEIGHT, 0.0);
	upper_leg();
	chooseMaterial(lowerLegMaterials);
	glRotatef(theta[RLL], 1, 0, 0);
	glTranslatef(0.0, -LOWER_LEG_HEIGHT * 0.9, 0.0);
	lower_leg();
	glPopMatrix();

	//左眼
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(eyeMaterials);
	glTranslatef(-HEAD_RADIUS * 2 / 5, BODY_HEIGHT + NECK_HEIGHT + HEAD_RADIUS * 3 / 5 - EYE_RADIUS, HEAD_RADIUS * 17 / 24);
	eye();
	glPopMatrix();

	//右眼
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(eyeMaterials);
	glTranslatef(HEAD_RADIUS * 2 / 5, BODY_HEIGHT + NECK_HEIGHT + HEAD_RADIUS * 3 / 5 - EYE_RADIUS, HEAD_RADIUS * 17 / 24);
	eye();
	glPopMatrix();
	return;
}


//画机器人下面的床板
void chuang(float x, float y, float z)
{
	glPushMatrix();
	glScalef(0.2, 0.5, 0.2);
	glTranslatef(x, y, z);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec1);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  //纹理

	//床前面	
	glBegin(GL_QUADS);  //绘制由四个顶点组成的一组单独的四边形
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-37.0f, -13.0f, 10.0f); //第一个参数代表X坐标。 0.0f 是纹理的左侧。 0.5f 是纹理的中点， 1.0f 是纹理的右侧。
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-37.0f, -20.0f, 10.0f);// 第二个参数代表Y坐标。 0.0f 是纹理的底部。 0.5f 是纹理的中点， 1.0f 是纹理的顶部。
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -20.0f, 10.0f); //glTexCoord2f()是配置纹理坐标 glVertex3f()是配置图形坐标。
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, 10.0f);
	glEnd();

	//床后面
	glBegin(GL_POLYGON);  //绘制了一个凸多边形。顶点1到n定义了这个多边形。
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-37.0f, -13.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-37.0f, -20.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -20.0f, -30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, -30.0f);
	glEnd();

	//床右面
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15.0f, -13.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0f, -20.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -20.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, 10.0f);
	glEnd();



	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff16);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec16);
	//床上面
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-37.0f, -13.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-37.0f, -13.0f, 10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -13.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, -30.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);//对活动的材质单元进行二维贴图，令glVertex3f生效
	glPopMatrix();
}

/**********************显示*******************/
void display(void)
{
	//用预制的值来清空缓冲区
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //防止缓冲区中原有的颜色信息影响本次绘图

	//恢复初始坐标系
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(x, y, z, //第一组数据就是脑袋的位置，第二组数据就是眼睛看的物体的位置，第三组就是头顶朝向的方向（因为你可以歪着头看同一个物体）
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
	glEnable(GL_LIGHTING); //用于启用各种功能。功能由参数决定。
	glEnable(GL_LIGHT0);
	glPushMatrix();
	glTranslated(20, -10, -20);
	//显示机器人
	object();
	//显示 床板
	chuang(0.0f, 0.0f, 0.0f);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	//画房间的地板
	glBegin(GL_QUADS);
	glColor3f(0.5, 0, 0); //如果在glBegin()与glEnd()函数之间多次连续调用颜色函数，那么，只会显示出最后一次的颜色
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -20.0f, 60.0f);
	glTexCoord2f(0.0f, 4.0f); glVertex3f(40.0f, -20.0f, 60.0f);
	glTexCoord2f(1.0f, 4.0f); glVertex3f(40.0f, -20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -20.0f, -60.0f);
	glEnd();


	//画房间的天花板
	glBegin(GL_QUADS);
	//上面
	glColor3f(0.5, 0.5, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, 20.0f, -60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, 20.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40.0f, 20.0f, 60.0f);
	glEnd();

	//画房间的后面

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, -20.0f, 59.95f);			//后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -20.0f, 59.95f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, 20.0f, 59.95f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40.0f, 20.0f, 59.95f);
	glEnd();

	//画房间的右面
	glBegin(GL_QUADS);												//右面
	glColor3f(0, 0.5, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, -20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40.0f, -20.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40.0f, 20.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40.0f, 20.0f, -60.0f);
	glEnd();

	//画房间的左面
	glBegin(GL_QUADS);
	glColor3f(0.2, 0, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -20.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, 20.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f, 20.0f, -60.0f);
	glEnd();

	//画房间的正面的墙

	glBegin(GL_QUADS);
	glColor3f(0.2, 0.5, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -20.0f, -59.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40.0f, -20.0f, -59.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40.0f, 20.0f, -59.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f, 20.0f, -59.9f);
	glEnd();

	//画房间的门
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.8, 0.1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(39.95f, -19.95f, 35.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(39.95f, -19.95f, 55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(39.95f, 17.00f, 55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(39.95f, 17.00f, 35.0f);
	glEnd();

	//画房间的油画
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(39.89f, 0.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(39.89f, 0.0f, -52.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(39.89f, 12.0f, -52.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(39.89f, 12.0f, -40.0f);
	glEnd();

	//画房间的油画
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.6);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-39.89f, 0.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-39.89f, 0.0f, -52.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-39.89f, 12.0f, -52.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-39.89f, 12.0f, -40.0f);
	glEnd();

	//画房间的油画
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.1, 0.8);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(39.89f, 0.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(39.89f, 0.0f, -32.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(39.89f, 12.0f, -32.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(39.89f, 12.0f, 30.0f);
	glEnd();
	//画房间的油画
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.4, 0.8);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-39.89f, 0.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-39.89f, 0.0f, -32.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-39.89f, 12.0f, -32.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-39.89f, 12.0f, 30.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers(); //交换两个缓冲区指针。

}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	//启动深度测试
	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec1);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* 定义纹理 */

	glDisable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);//设置光照属性的模型。被设置的光照属性模型的特征是由pname指定的，param表示pname参数被设置的值
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight);

	//初始化视角位置
	x = 6.88;
	y = 8;
	z = 46;
	//初始化各个位置的材质
	eyeMaterials = &chocolateCakeEyeMaterials;
	headMaterials = &chocolateCakeHeadMaterials;
	feelerMaterials = &chocolateCakeFeelerMaterials;
	neckMaterials = &chocolateCakeNeckMaterials;
	bodyMaterials = &chocolateCakeBodyMaterials;
	upperArmMaterials = &chocolateCakeUpperArmMaterials;
	lowerArmMaterials = &chocolateCakeLowerArmMaterials;
	upperLegMaterials = &chocolateCakeUpperLegMaterials;
	lowerLegMaterials = &chocolateCakeLowerLegMaterials;

	p = gluNewQuadric();
	//设置当前光照
	currentLighting = &whiteLighting;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);//创建指定的光源,light可以是GL_LIGHT0、GL_LIGHT1，pname定义的是光源的属性，
	glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLighting->diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, currentLighting->specular);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

}
void bendFunc(GLint order, GLfloat MaxAngle, GLfloat MinAngle)
{
	if (theta[order] >= MaxAngle)
		bendingDirection[order] = -1;
	if (theta[order] <= MinAngle)
		bendingDirection[order] = 1;
	return;
}
void myidle()
{
	if (!robot_move) return;
	if (bending_state == GL_TRUE)
	{
		theta[BODY] += bendBODYv * bendingDirection[BODY];
		bendFunc(BODY, 15, -30);
		theta[LUA] += bendLUAv * bendingDirection[LUA];
		bendFunc(LUA, 40, -40);
		theta[LLA] += bendLUAv * bendingDirection[LLA];
		bendFunc(LLA, 20, -20);
		theta[RUA] += bendRUAv * bendingDirection[RUA];
		bendFunc(RUA, 40, -40);
		theta[RLA] += bendRLAv * bendingDirection[RLA];
		bendFunc(RLA, 20, -20);
		theta[LUL] += bendLULv * bendingDirection[LUL];
		bendFunc(LUL, 40, -40);
		theta[LLL] += bendLLLv * bendingDirection[LLL];
		bendFunc(LLL, 20, -20);
		theta[RUL] += bendRULv * bendingDirection[RUL];
		bendFunc(RUL, 40, -40);
		theta[RLL] += bendRLLv * bendingDirection[RLL];
		bendFunc(RLL, 20, -20);
		theta[LF] += bendLFv * bendingDirection[LF];
		bendFunc(LF, 15, -15);
		theta[RF] += bendRFv * bendingDirection[RF];
		bendFunc(RF, 15, -15);
	}
	if (rotate_state == GL_TRUE)
	{
		theta[ALL] += 1.0;
		if (theta[ALL] > 360)
			theta[ALL] = 0;
	}
	glutPostRedisplay();

}
void reshape(int w, int h)
{

	// 当窗口太小时，防止除零
	if (h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// 在修改前重置坐标系
	glMatrixMode(GL_PROJECTION);
	//
	glLoadIdentity();

	// 设置视窗为整个窗口
	glViewport(0, 0, w, h);

	//gluPerspective定义的就是相机的内在镜头参数
	gluPerspective(45, ratio, 1, 150);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// gluLookAt定义的是相机外在参数如相机所处位置、旋转角度
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);


}
//上下方向移动摄像机视角
void orient(float ang) {


	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	// gluLookAt定义的是相机外在参数如相机所处位置、旋转角度，
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
}

//水平方向上移动摄像机视角
void moveflat(int i) {
	x = x + i * (lx)*0.1;
	z = z + i * (lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
}
//响应键盘函数
void keyboard(int key, int x, int y)
{

	//F4控制机器人开始旋转
	if (key == GLUT_KEY_F4)
	{
		robot_move = true;
	}
	//F5控制机器人停止旋转
	if (key == GLUT_KEY_F5)
	{
		robot_move = false;
	}
	//pageup键
	if (key == GLUT_KEY_PAGE_UP)
	{
		ly += 0.05f;
	}
	//pagedown键
	if (key == GLUT_KEY_PAGE_DOWN)
	{
		ly -= 0.05f;

	}
	//方向键上键
	if (key == GLUT_KEY_UP)
	{

		moveflat(5);
	}
	//方向键下键
	if (key == GLUT_KEY_DOWN)
	{

		moveflat(-5);
	}
	//方向键左键
	if (key == GLUT_KEY_LEFT)
	{

		angle -= 0.03f;
		orient(angle);
	}
	//方向键右键
	if (key == GLUT_KEY_RIGHT)
	{

		angle += 0.03f;
		orient(angle);
	}

	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Virtual gallery");
	init();

	glutIdleFunc(myidle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);

	glutMainLoop();
	return 0;
}
