#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
#include <random>
#include "Header.h"
using namespace std;

const int ball_num = 2;
extern BALL ball[ball_num];
extern ofstream fout;

// �R���X�g���N�^�[
BALL::BALL() {
	random_device rd;  // �񌈒�I�ȗ���������
	mt19937_64 mt(rd());  // �����Z���k�E�c�C�X�^��64�r�b�g�ŁA�����͏����V�[�h

	// �F�̗�������
	uniform_real_distribution<double> color0(0.0, 1.0);
	uniform_real_distribution<double> color1(0.0, 1.0);
	uniform_real_distribution<double> color2(0.0, 1.0);
	uniform_real_distribution<double> color3(0.0, 1.0);

	// �������x�̗�������
	uniform_real_distribution<double> xvel(-100.0, 100.0);
	uniform_real_distribution<double> yvel(-100.0, 100.0);

	for (int i = 0; i < ball_num; i++) {
		ball[i].m = 170; // [g]
		ball[i].r = 5.71 / 2; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
		ball[i].e = 1;

		ball[i].col[0] = color0(mt); // �F R
		ball[i].col[1] = color1(mt); // �F G
		ball[i].col[2] = color2(mt); // �F B
		ball[i].col[3] = color3(mt); // �F A

		ball[i].pos[0] = 0.0; // x���W �����ʒu
		ball[i].pos[1] = 0.0; // y���W �����ʒu
		ball[i].pos[2] = 0.0; // z���W �����ʒu

		ball[i].vel[0] = xvel(mt); // x���� �������x
		ball[i].vel[1] = yvel(mt); // y���� �������x
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

void BALL::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // ��ʂ��N���A�ɂ���
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // ���_�ʒu�ix���Ay���Az���j
		0.0, 0.0, 0.0,   // ���_�ڕW�ʒu �A�ǂ������邩�ڕW�����߂�ix���Ay���Az���j
		0.0, 1.0, 0.0);  // ������x�N�g�� �A���_�̌����ix���Ay���Az���j

						 // �r�����[�h��̕ǂ̕`��
	make_billiards_wall();
	make_balls();
	glutSwapBuffers();

	File_output();  // �t�@�C���ɋ��̈ʒu�⑬�x�Ȃǂ��o��

	for (int i = 0; i < ball_num; i++) {
		double vel = sqrt(ball[i].vel[0] * ball[i].vel[0] + ball[i].vel[1] * ball[i].vel[1]);
		if (vel <= v_min) {
			ball[i].vel[0] = 0.0;
			ball[i].vel[1] = 0.0;
			ball[i].acc[0] = 0.0;
			ball[i].acc[1] = 0.0;
		}

		// �����C�͂̎���
		if (ball[i].vel[0] > 0) {  // x����
			ball[i].acc[0] = -mu_d * g;
		}
		else if (0 > ball[i].vel[0]) {
			ball[i].acc[0] = mu_d * g;
		}

		if (ball[i].vel[1] > 0) {  // y����
			ball[i].acc[1] = -mu_d * g;
		}
		else if (0 > ball[i].vel[1]) {
			ball[i].acc[1] = mu_d * g;
		}

		// �{�[�����Փ˂������̏���
		if (ball[i].pos[0] + ball[i].r >= table_w / 2) {  // �E�̕�
			ball[i].pos[0] = table_w / 2 - ball[i].r;
			ball[i].vel[0] = -1 * mu_r * ball[i].vel[0];
		}
		else if (ball[i].pos[0] - ball[i].r <= -table_w / 2) {  // ���̕�
			ball[i].pos[0] = -table_w / 2 + ball[i].r;
			ball[i].vel[0] = -1 * mu_r * ball[i].vel[0];
		}

		if (ball[i].pos[1] + ball[i].r >= table_h / 2) {  // ��̕�
			ball[i].pos[1] = table_h / 2 - ball[i].r;
			ball[i].vel[1] = -1 * mu_r * ball[i].vel[1];
		}
		else if (ball[i].pos[1] - ball[i].r <= -table_h / 2) {  // ���̕�
			ball[i].pos[1] = -table_h / 2 + ball[i].r;
			ball[i].vel[1] = -1 * mu_r * ball[i].vel[1];
		}

		ball[i].vel[0] += ball[i].acc[0] * dt;
		ball[i].vel[1] += ball[i].acc[1] * dt;
		ball[i].vel[2] += ball[i].acc[2] * dt;

		ball[i].pos[0] += ball[i].vel[0] * dt;
		ball[i].pos[1] += ball[i].vel[1] * dt;
		ball[i].pos[2] += ball[i].vel[2] * dt;
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

// ���̕`��
void BALL::make_balls() {
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

void BALL::File_output() {
	double dt_sum = BALL::loopcounter * BALL::dt;

	for (int i = 0; i < ball_num; i++) {
		// ��ԍX�V�E�E�E��ʂƃt�@�C���ɏo��
		/*
		cout << setw(8) << dt_sum << " " <<
			setw(8) << ball[i].pos[0] << " " <<
			setw(8) << ball[i].pos[1] << " " <<
			setw(8) << ball[i].pos[2] << " " <<
			setw(8) << ball[i].vel[0] << " " <<
			setw(8) << ball[i].vel[1] << " " <<
			setw(8) << ball[i].vel[2] << " " <<
			setw(8) << ball[i].acc[0] << " " <<
			setw(8) << ball[i].acc[1] << " " <<
			setw(8) << ball[i].acc[2] << " " <<
			endl;
			*/
		fout << setw(8) << dt_sum << " " <<
			setw(8) << ball[i].pos[0] << " " <<
			setw(8) << ball[i].pos[1] << " " <<
			setw(8) << ball[i].pos[2] << " " <<
			setw(8) << ball[i].vel[0] << " " <<
			setw(8) << ball[i].vel[1] << " " <<
			setw(8) << ball[i].vel[2] << " " <<
			setw(8) << ball[i].acc[0] << " " <<
			setw(8) << ball[i].acc[1] << " " <<
			setw(8) << ball[i].acc[2] << " " <<
			endl;

		fout.flush();
	}
	BALL::loopcounter++;
}