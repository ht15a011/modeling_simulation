#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;

// �O���[�o���ϐ�
extern BALL ball;
extern ofstream fout;

// �{�[���̏����ݒ�
void BALL::ball_set() {
	ball.pos[0] = 0.0;  // x���W �����ʒu
	ball.pos[1] = 0.0;  // y���W �����ʒu
	ball.pos[2] = 0.0;  // z���W �����ʒu

	ball.vel[0] = 100.0;  // x���� �������x
	ball.vel[1] = 100.0;  // y���� �������x
	ball.vel[2] = 0.0;    // z���� �������x

	ball.acc[0] = 0.0;  // x���� ���������x
	ball.acc[1] = 0.0;  // y���� ���������x
	ball.acc[2] = 0.0;  // z���� ���������x

	ball.col[0] = 1.0;  // �F R
	ball.col[1] = 1.0;  // �F G
	ball.col[2] = 0.0;  // �F B
	ball.col[3] = 0.0;  // �F A

	ball.m = 170;      // [g]
	ball.r = 5.71 * 5; // ball.r = 120.71 / 2; // [cm] ���̂̑傫��
	ball.e = 1;        // �e���W��
}

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

// �L�[�{�[�h����̓��͂���������
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

// �f�B�X�v���C�ɕ\��������e�������i�ǁA�{�[���̈ʒu�E���x�E�����x�Ȃǁj
void BALL::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // ��ʂ��N���A�ɂ���
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // ���_�ʒu�ix���Ay���Az���j
		0.0, 0.0, 0.0,   // ���_�ڕW�ʒu �A�ǂ������邩�ڕW�����߂�ix���Ay���Az���j
		0.0, 1.0, 0.0);  // ������x�N�g�� �A���_�̌����ix���Ay���Az���j

	// �r�����[�h��̕ǂ̕`��

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
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 };  // ���F
		glTranslated(-table_w / 2, 1.0, 0.0); // -x������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, 1.0); // y������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ��̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 };  // ���F
		glTranslated(0.0, table_h / 2, 0.0);      // +y������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0);  // x������table_h�{�Ɉ������΂�
		glutSolidCube(1);  // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ���̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 };  // ���F
		glTranslated(0.0, -table_h / 2, 0.0);     // -y������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0);  // x������table_h�{�Ɉ������΂�
		glutSolidCube(1);  // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// �r�����[�h��̐F�t��
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.5, 0.0, 0.0 };  // �ΐF
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, table_h, 0.0);  // x,y������table_h�{�Ɉ������΂�
		glutSolidCube(1);  // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ���̕`��
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // ���̂̈ʒu
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.r, 100, 16);  // �T�C�Y�A�����Y�킳
	}
	glPopMatrix();

	glutSwapBuffers();

	double dt_sum = loopcounter * dt;

	// ��ԍX�V�E�E�E��ʂƃt�@�C���ɏo��(�{�[���̈ʒu�A���x�A�����x ��xyz����)
	cout << setw(8) << dt_sum << " " <<
		setw(8) << ball.pos[0] << " " <<
		setw(8) << ball.pos[1] << " " <<
		setw(8) << ball.pos[2] << " " <<
		setw(8) << ball.vel[0] << " " <<
		setw(8) << ball.vel[1] << " " <<
		setw(8) << ball.vel[2] << " " <<
		setw(8) << ball.acc[0] << " " <<
		setw(8) << ball.acc[1] << " " <<
		setw(8) << ball.acc[2] << " " <<
		endl;

	fout << setw(8) << dt_sum << " " <<
		setw(8) << ball.pos[0] << " " <<
		setw(8) << ball.pos[1] << " " <<
		setw(8) << ball.pos[2] << " " <<
		setw(8) << ball.vel[0] << " " <<
		setw(8) << ball.vel[1] << " " <<
		setw(8) << ball.vel[2] << " " <<
		setw(8) << ball.acc[0] << " " <<
		setw(8) << ball.acc[1] << " " <<
		setw(8) << ball.acc[2] << " " <<
		endl;

	fout.flush();
	loopcounter++;

	// �{�[�����ǂɏՓ˂����ۂ̏����i�߂荞�݉������������Ă���j
	if (ball.pos[0] + ball.r >= table_w / 2) {  // �E�̕�
		ball.pos[0] = table_w / 2 - ball.r;
		ball.vel[0] = -1 * mu_r * ball.vel[0];
	}
	else if (ball.pos[0] - ball.r <= -table_w / 2) {  // ���̕�
		ball.pos[0] = -table_w / 2 + ball.r;
		ball.vel[0] = -1 * mu_r * ball.vel[0];
	}

	if (ball.pos[1] + ball.r >= table_h / 2) {  // ��̕�
		ball.pos[1] = table_h / 2 - ball.r;
		ball.vel[1] = -1 * mu_r * ball.vel[1];
	}
	else if (ball.pos[1] - ball.r <= -table_h / 2) {  // ���̕�
		ball.pos[1] = -table_h / 2 + ball.r;
		ball.vel[1] = -1 * mu_r * ball.vel[1];
	}

	ball.vel[0] += ball.acc[0] * dt;
	ball.vel[1] += ball.acc[1] * dt;
	ball.vel[2] += ball.acc[2] * dt;

	ball.pos[0] += ball.vel[0] * dt;
	ball.pos[1] += ball.vel[1] * dt;
	ball.pos[2] += ball.vel[2] * dt;

}