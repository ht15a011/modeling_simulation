#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
#include "Header.h"
using namespace std;

extern BALL ball;
extern ofstream fout;

// �R���X�g���N�^�[
BALL::BALL() {
	m = 170; // [g]
	r = 5.71; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
	e = 1;

	col[0] = 1.0; // �F R
	col[1] = 1.0; // �F G
	col[2] = 0.0; // �F B
	col[3] = 0.0; // �F A

	pos[0] = 0.0; // x���W �����ʒu
	pos[1] = 0.0; // y���W �����ʒu
	pos[2] = 0.0; // z���W �����ʒu

	vel[0] = 100.0; // x���� �������x
	vel[1] = 100.0; // y���� �������x
	vel[2] = 0.0;   // z���� �������x

	acc[0] = 0.0; // x���� ���������x
	acc[1] = 0.0; // y���� ���������x
	acc[2] = 0.0; // z���� ���������x
}

BALL::BALL(double size, float col0, float col1, float col2, float col3) {
	static double pos0, pos1, pos2;
	pos0 = 0.0;
	pos1 = 0.0;
	pos2 = 0.0;

	m = 170; // [g]
	r = size; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
	e = 1;

	col[0] = col0; // �F R
	col[1] = col1; // �F G
	col[2] = col2; // �F B
	col[3] = col3; // �F A

	pos[0] = pos0; // x���W �����ʒu
	pos[1] = pos1; // y���W �����ʒu
	pos[2] = pos2; // z���W �����ʒu

	vel[0] = 100.0; // x���� �������x
	vel[1] = 100.0; // y���� �������x
	vel[2] = 0.0;   // z���� �������x

	acc[0] = 0.0; // x���� ���������x
	acc[1] = 0.0; // y���� ���������x
	acc[2] = 0.0; // z���� ���������x
}

// �f�X�g���N�^�[
BALL::~BALL() {}

void BALL::idle() {
	glutPostRedisplay();
}

void BALL::init() {
	// �����̐ݒ�
	GLfloat lpos0[4] = { 0.0, 100.0, 100.0, 1.0 };
	GLfloat lcol0[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lcol0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// �w�i�F �D�F
	glClearColor(0.7, 0.7, 0.7, 0.0);  // RGB

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
}

void BALL::resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void BALL::keyboard(unsigned char k, int x, int y) {
	switch (k) {
	case 'q': // q�ŏI��
		fout.close();
		exit(0);
		break;
	default:
		return;
	}
}


// �r�����[�h��̕ǂ�`���ÓI�����o�֐�
void BALL::make_billiards_wall() {
	// �E�̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // ���F
		glTranslated(table_w / 2, 0.0, 0.0); // +x������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, 1.0); // y������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ���̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // ���F
		glTranslated(-table_w / 2, 1.0, 0.0); // -x������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, 1.0); // y������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ��̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // ���F
		glTranslated(0.0, table_h / 2, 0.0); // +y������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0); // x������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ���̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // ���F
		glTranslated(0.0, -table_h / 2, 0.0); // -y������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0); // x������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// �r�����[�h��̐F�t��
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.5, 0.0, 0.0 }; // �ΐF
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, table_h, 0.0); // x,y������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();
}

void BALL::make_balls() {
	// ���̕`�� (�����̋��̂�`���Ƃ��ɂ��̊֐��𕡐���������Ǝv����)
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // ���̂̈ʒu
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.r, 100, 16);  // �T�C�Y�A�����Y�킳�A�E�E
	}
	glPopMatrix();
}