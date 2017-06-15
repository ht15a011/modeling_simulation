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

extern const int ball_num = 2;;
extern BALL ball[ball_num];
extern ofstream fout;

// �R���X�g���N�^�[
BALL::BALL() {
	for (int i = 0; i < ball_num; i++) {
		m = 170; // [g]
		r = 5.71 / 2; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
		e = 1;

		ball[i].col[0] = 1.0; // �F R
		ball[i].col[1] = 1.0; // �F G
		ball[i].col[2] = 0.0; // �F B
		ball[i].col[3] = 0.0; // �F A

		ball[i].pos[0] = 0.0; // x���W �����ʒu
		ball[i].pos[1] = 0.0; // y���W �����ʒu
		ball[i].pos[2] = 0.0; // z���W �����ʒu

		ball[i].vel[0] = 100.0; // x���� �������x
		ball[i].vel[1] = 100.0; // y���� �������x
		ball[i].vel[2] = 0.0;   // z���� �������x

		ball[i].acc[0] = 0.0; // x���� ���������x
		ball[i].acc[1] = 0.0; // y���� ���������x
		ball[i].acc[2] = 0.0; // z���� ���������x
	}
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
	for (int i = 0; i < ball_num; i++) {
		glPushMatrix();
		{
			glTranslated(ball[i].pos[0], ball[i].pos[1], ball[i].pos[2]);  // ���̂̈ʒu
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball[i].col);
			glutSolidSphere(ball[i].r, 100, 16);  // �T�C�Y�A�����Y�킳�A�E�E
		}
		glPopMatrix();
	}
}