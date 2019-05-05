
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


//�����˵ĸ����ֲ���
#define FEELER_RADIUS 0.1
#define FEELER_HEIGHT 1.5

//ͷ�İ뾶
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

//�����˸����ֵ�id
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

//ÿһ��������ת�Ƕ�
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

//�������Ƿ���ת
GLboolean robot_move = false;
//���ƻ����˸����Ƕ�
static float angle = 0.0, ratio;
//�������λ��
static float x = 0.0f, y = 8.0f, z = 5.0f;
//�����λ������
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;  //���߷���,��ʼֵ��z����
GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };//ָ��ȫ�ֻ�����  wΪ1.0ʱ����ԴΪ��λ��Դ��
GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };//������
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };//���淴��
GLfloat positionLight[] = { 0.0f, 20.0f, 0.0f, 1.0f };//��Դλ��

GLfloat model_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

//************************���淴��****************************
GLfloat mat_spec1[] = { 0.6f, 0.6f, 0.6f, 1.0f };//������ɫ

GLfloat mat_spec16[] = { 0.6f, 0.6f, 0.6f, 1.0 };//������
//**************************������*******************************//
GLfloat mat_amb_diff1[] = { 0.6f, 0.6f, 0.6f, 1.0 }; //������ɫ

GLfloat mat_amb_diff16[] = { 0.6f, 0.6f, 0.6f, 1.0 };//������



//���û�������ת�Ƕ�
GLfloat theta[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0 };

//���û�������������
GLfloat bendingDirection[] = { 0, -1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1 };

//set states
GLboolean bending_state = GLU_TRUE;
GLboolean rotate_state = GLU_TRUE;

GLUquadricObj *p;   //���ƶ�����������Ҫ���������������

//���ʽṹ��
typedef struct materialStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} materialStruct;

//Ϊ�����˲�ͬ���۾�����
materialStruct chocolateCakeEyeMaterials = { { 0.21, 0.07, 0.11 }, { 0.41, 0.17, 0.21 }, { 0.61, 0.37, 0.41 }, 15 };
//Ϊ�����˲�ͬ��ͷ������
materialStruct chocolateCakeHeadMaterials = { { 0.675, 0.425, 0.05 }, { 0.775, 0.525, 0.15 }, { 1.0, 0.85, 0.45 }, 50 };
materialStruct chocolateCakeFeelerMaterials = { { 0.0, 0.0, 0.0 }, { 0.4, 0.4, 0.4 }, { 0.85, 0.65, 0.49 }, 5 };
//Ϊ�����˲�ͬ�Ĳ��Ӳ���
materialStruct chocolateCakeNeckMaterials = { { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, 20 };
//Ϊ�����˲�ͬ���������
materialStruct chocolateCakeBodyMaterials = { { 0.25, 0.05, 0.00 }, { 0.45, 0.25, 0.10 }, { 0.85, 0.65, 0.49 }, 100 };
//Ϊ�����˲�ͬ�ĸ첲����
materialStruct chocolateCakeUpperArmMaterials = { { 0.45, 0.25, 0.09 }, { 0.65, 0.45, 0.29 }, { 0.85, 0.65, 0.49 }, 20 };
materialStruct chocolateCakeLowerArmMaterials = { { 0.71, 0.57, 0.52 }, { 0.91, 0.77, 0.50 }, { 0.9, 0.65, 0.39 }, 20 };
//Ϊ�����˲�ͬ���Ȳ�����
materialStruct chocolateCakeUpperLegMaterials = { { 0.30, 0.17, 0.0 }, { 0.55, 0.35, 0.39 }, { 0.85, 0.65, 0.49 }, 20 };
materialStruct chocolateCakeLowerLegMaterials = { { 0.45, 0.25, 0.09 }, { 0.65, 0.45, 0.29 }, { 0.85, 0.65, 0.49 }, 20 };


//�����˸����ֲ���ָ��
materialStruct *eyeMaterials;
materialStruct *headMaterials;
materialStruct *feelerMaterials;
materialStruct *neckMaterials;
materialStruct *bodyMaterials;
materialStruct *upperArmMaterials;
materialStruct *lowerArmMaterials;
materialStruct *upperLegMaterials;
materialStruct *lowerLegMaterials;

//���սṹ
typedef struct lightingStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
} lightingStruct;

//̫����
lightingStruct whiteLighting = { { 1.0, 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0, 1.0 } };

//���õ�ǰ����
lightingStruct *currentLighting;

//���ù���λ��
GLfloat light_pos0[] = { 15, 15, 15, 1.0 };

//�������˵�����
void body()
{
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);  //angleΪ��ת�ĽǶ�,x��ת
	gluCylinder(p, BODY_RADIUS, BODY_RADIUS, BODY_HEIGHT, 100, 100); //��Բ��
	gluDisk(p, 0, BODY_RADIUS, 100, 100);//����һ����Բ��
	glTranslatef(0, 0, BODY_HEIGHT);
	gluDisk(p, 0, BODY_RADIUS, 100, 100);
	glPopMatrix();

}
//�������˵�ͷ��
void head()
{
	glPushMatrix();
	glutSolidSphere(HEAD_RADIUS, 100, 100); //��Ⱦһ�����壬r,���ߣ�γ��
	glPopMatrix();
}
//�������˵Ĳ���
void neck()
{
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);//angleΪ��ת�ĽǶ�,x��ת
	gluCylinder(p, NECK_RADIUS, NECK_RADIUS, NECK_HEIGHT, 100, 100); //��Բ��
	glPopMatrix();
}
//�������˵����崥��
void feeler()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(p, FEELER_RADIUS, FEELER_RADIUS, FEELER_HEIGHT, 100, 100);
	glTranslatef(0.0, 0.0, FEELER_HEIGHT);
	gluDisk(p, 0, FEELER_RADIUS, 100, 100);// ����һ����Բ��
	glPopMatrix();

}
//�������˵����Ȳ�
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
//�������˵����Ȳ�
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
//�������˵ĸ첲
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
//�������˵ļ��
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
//�������˵��۾�
void eye()
{
	glPushMatrix();
	glutSolidSphere(EYE_RADIUS, 100, 100);
	glPopMatrix();

}

//���趨�����ֲ��ϵ�����
void chooseMaterial(materialStruct *material)
{
	//����Ĺ���������Ҫ����������ϵ�����������ϵ�����������ϵ���;���ָ���ĸ�
	glMaterialfv(GL_FRONT, GL_AMBIENT, material->ambient);//������ϵ��
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material->diffuse);//�������ϵ��
	glMaterialfv(GL_FRONT, GL_SPECULAR, material->specular);//�������ϵ��
	glMaterialfv(GL_FRONT, GL_SHININESS, &material->shininess);//����ָ��

}

//��һ������������
void object()
{

	glRotatef(theta[ALL], 0, 1, 0);//all part should rotate some angles

	//�󴥽�
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(feelerMaterials);
	glTranslatef(BODY_RADIUS / 2, BODY_HEIGHT + NECK_HEIGHT + (HEAD_RADIUS*sqrt(3.0) / 2), 0.0);
	glRotatef(theta[LF], 1, 0, 0);
	glRotatef(-30.0, 0.0, 0.0, 1.0);
	feeler();
	glPopMatrix();

	//�Ҵ���
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(feelerMaterials);
	glTranslatef(-BODY_RADIUS / 2, BODY_HEIGHT + NECK_HEIGHT + (HEAD_RADIUS*sqrt(3.0) / 2), 0.0);
	glRotatef(theta[RF], 1, 0, 0);
	glRotatef(30.0, 0.0, 0.0, 1.0);
	feeler();
	glPopMatrix();

	//��ͷ��
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(headMaterials);
	glTranslatef(0, BODY_HEIGHT + NECK_HEIGHT, 0);
	head();
	glPopMatrix();

	//������
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(neckMaterials);
	glTranslatef(0, BODY_HEIGHT + NECK_HEIGHT, 0);
	neck();
	glPopMatrix();

	//������
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(bodyMaterials);
	body();
	glPopMatrix();

	//�����±�
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

	//�����±�
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

	//����С�� 
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

	//����С��
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

	//����
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(eyeMaterials);
	glTranslatef(-HEAD_RADIUS * 2 / 5, BODY_HEIGHT + NECK_HEIGHT + HEAD_RADIUS * 3 / 5 - EYE_RADIUS, HEAD_RADIUS * 17 / 24);
	eye();
	glPopMatrix();

	//����
	glPushMatrix();
	glRotatef(theta[BODY], 1, 0, 0);
	chooseMaterial(eyeMaterials);
	glTranslatef(HEAD_RADIUS * 2 / 5, BODY_HEIGHT + NECK_HEIGHT + HEAD_RADIUS * 3 / 5 - EYE_RADIUS, HEAD_RADIUS * 17 / 24);
	eye();
	glPopMatrix();
	return;
}


//������������Ĵ���
void chuang(float x, float y, float z)
{
	glPushMatrix();
	glScalef(0.2, 0.5, 0.2);
	glTranslatef(x, y, z);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec1);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  //����

	//��ǰ��	
	glBegin(GL_QUADS);  //�������ĸ�������ɵ�һ�鵥�����ı���
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-37.0f, -13.0f, 10.0f); //��һ����������X���ꡣ 0.0f ���������ࡣ 0.5f ��������е㣬 1.0f ��������Ҳࡣ
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-37.0f, -20.0f, 10.0f);// �ڶ�����������Y���ꡣ 0.0f ������ĵײ��� 0.5f ��������е㣬 1.0f ������Ķ�����
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -20.0f, 10.0f); //glTexCoord2f()�������������� glVertex3f()������ͼ�����ꡣ
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, 10.0f);
	glEnd();

	//������
	glBegin(GL_POLYGON);  //������һ��͹����Ρ�����1��n�������������Ρ�
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-37.0f, -13.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-37.0f, -20.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -20.0f, -30.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, -30.0f);
	glEnd();

	//������
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15.0f, -13.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0f, -20.0f, -30.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -20.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, 10.0f);
	glEnd();



	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff16);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec16);
	//������
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-37.0f, -13.0f, -30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-37.0f, -13.0f, 10.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0f, -13.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0f, -13.0f, -30.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);//�Ի�Ĳ��ʵ�Ԫ���ж�ά��ͼ����glVertex3f��Ч
	glPopMatrix();
}

/**********************��ʾ*******************/
void display(void)
{
	//��Ԥ�Ƶ�ֵ����ջ�����
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //��ֹ��������ԭ�е���ɫ��ϢӰ�챾�λ�ͼ

	//�ָ���ʼ����ϵ
	glLoadIdentity();
	glPushMatrix();
	gluLookAt(x, y, z, //��һ�����ݾ����Դ���λ�ã��ڶ������ݾ����۾����������λ�ã����������ͷ������ķ�����Ϊ���������ͷ��ͬһ�����壩
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
	glEnable(GL_LIGHTING); //�������ø��ֹ��ܡ������ɲ���������
	glEnable(GL_LIGHT0);
	glPushMatrix();
	glTranslated(20, -10, -20);
	//��ʾ������
	object();
	//��ʾ ����
	chuang(0.0f, 0.0f, 0.0f);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	//������ĵذ�
	glBegin(GL_QUADS);
	glColor3f(0.5, 0, 0); //�����glBegin()��glEnd()����֮��������������ɫ��������ô��ֻ����ʾ�����һ�ε���ɫ
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -20.0f, 60.0f);
	glTexCoord2f(0.0f, 4.0f); glVertex3f(40.0f, -20.0f, 60.0f);
	glTexCoord2f(1.0f, 4.0f); glVertex3f(40.0f, -20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -20.0f, -60.0f);
	glEnd();


	//��������컨��
	glBegin(GL_QUADS);
	//����
	glColor3f(0.5, 0.5, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, 20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, 20.0f, -60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, 20.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40.0f, 20.0f, 60.0f);
	glEnd();

	//������ĺ���

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, -20.0f, 59.95f);			//����
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -20.0f, 59.95f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, 20.0f, 59.95f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40.0f, 20.0f, 59.95f);
	glEnd();

	//�����������
	glBegin(GL_QUADS);												//����
	glColor3f(0, 0.5, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40.0f, -20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40.0f, -20.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40.0f, 20.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40.0f, 20.0f, -60.0f);
	glEnd();

	//�����������
	glBegin(GL_QUADS);
	glColor3f(0.2, 0, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -20.0f, -60.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -20.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, 20.0f, 60.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f, 20.0f, -60.0f);
	glEnd();

	//������������ǽ

	glBegin(GL_QUADS);
	glColor3f(0.2, 0.5, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -20.0f, -59.9f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40.0f, -20.0f, -59.9f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40.0f, 20.0f, -59.9f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f, 20.0f, -59.9f);
	glEnd();

	//���������
	glBegin(GL_QUADS);
	glColor3f(0.2, 0.8, 0.1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(39.95f, -19.95f, 35.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(39.95f, -19.95f, 55.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(39.95f, 17.00f, 55.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(39.95f, 17.00f, 35.0f);
	glEnd();

	//��������ͻ�
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(39.89f, 0.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(39.89f, 0.0f, -52.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(39.89f, 12.0f, -52.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(39.89f, 12.0f, -40.0f);
	glEnd();

	//��������ͻ�
	glBegin(GL_QUADS);
	glColor3f(0.6, 0.8, 0.6);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-39.89f, 0.0f, -40.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-39.89f, 0.0f, -52.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-39.89f, 12.0f, -52.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-39.89f, 12.0f, -40.0f);
	glEnd();

	//��������ͻ�
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.1, 0.8);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(39.89f, 0.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(39.89f, 0.0f, -32.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(39.89f, 12.0f, -32.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(39.89f, 12.0f, 30.0f);
	glEnd();
	//��������ͻ�
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.4, 0.8);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-39.89f, 0.0f, 30.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-39.89f, 0.0f, -32.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-39.89f, 12.0f, -32.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-39.89f, 12.0f, 30.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers(); //��������������ָ�롣

}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec1);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* �������� */

	glDisable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);//���ù������Ե�ģ�͡������õĹ�������ģ�͵���������pnameָ���ģ�param��ʾpname���������õ�ֵ
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight);

	//��ʼ���ӽ�λ��
	x = 6.88;
	y = 8;
	z = 46;
	//��ʼ������λ�õĲ���
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
	//���õ�ǰ����
	currentLighting = &whiteLighting;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT0, GL_AMBIENT, currentLighting->ambient);//����ָ���Ĺ�Դ,light������GL_LIGHT0��GL_LIGHT1��pname������ǹ�Դ�����ԣ�
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

	// ������̫Сʱ����ֹ����
	if (h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// ���޸�ǰ��������ϵ
	glMatrixMode(GL_PROJECTION);
	//
	glLoadIdentity();

	// �����Ӵ�Ϊ��������
	glViewport(0, 0, w, h);

	//gluPerspective����ľ�����������ھ�ͷ����
	gluPerspective(45, ratio, 1, 150);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// gluLookAt�������������ڲ������������λ�á���ת�Ƕ�
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);


}
//���·����ƶ�������ӽ�
void orient(float ang) {


	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	// gluLookAt�������������ڲ������������λ�á���ת�Ƕȣ�
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
}

//ˮƽ�������ƶ�������ӽ�
void moveflat(int i) {
	x = x + i * (lx)*0.1;
	z = z + i * (lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
}
//��Ӧ���̺���
void keyboard(int key, int x, int y)
{

	//F4���ƻ����˿�ʼ��ת
	if (key == GLUT_KEY_F4)
	{
		robot_move = true;
	}
	//F5���ƻ�����ֹͣ��ת
	if (key == GLUT_KEY_F5)
	{
		robot_move = false;
	}
	//pageup��
	if (key == GLUT_KEY_PAGE_UP)
	{
		ly += 0.05f;
	}
	//pagedown��
	if (key == GLUT_KEY_PAGE_DOWN)
	{
		ly -= 0.05f;

	}
	//������ϼ�
	if (key == GLUT_KEY_UP)
	{

		moveflat(5);
	}
	//������¼�
	if (key == GLUT_KEY_DOWN)
	{

		moveflat(-5);
	}
	//��������
	if (key == GLUT_KEY_LEFT)
	{

		angle -= 0.03f;
		orient(angle);
	}
	//������Ҽ�
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
