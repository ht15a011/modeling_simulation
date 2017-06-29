#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;

//const int ball_num = 10;
//extern BALL ball[ball_num];
extern vector<BALL> ball;
extern ofstream fout;
extern ifstream fin;
vector< vector<double> > ball_val;

BALL::BALL(){}

BALL::~BALL() {}

void BALL::Set_ball() {
	for (int i = 0; i < count_line; i++) {
		for (int j = 0; j < 16; j++) {
			ball[i].m = ball_val[i][j]; // [g]
			ball[i].r = ball_val[i][j]; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
			ball[i].e = ball_val[i][j];

			ball[i].col[0] = ball_val[i][j]; // �F R
			ball[i].col[1] = ball_val[i][j]; // �F G
			ball[i].col[2] = ball_val[i][j]; // �F B
			ball[i].col[3] = ball_val[i][j]; // �F A

			ball[i].pos[0] = ball_val[i][j]; // x���W �����ʒu
			ball[i].pos[1] = ball_val[i][j]; // y���W �����ʒu
			ball[i].pos[2] = ball_val[i][j]; // z���W �����ʒu

			ball[i].vel[0] = ball_val[i][j]; // x���� �������x
			ball[i].vel[1] = ball_val[i][j]; // y���� �������x
			ball[i].vel[2] = ball_val[i][j];   // z���� �������x

			ball[i].acc[0] = ball_val[i][j]; // x���� ���������x
			ball[i].acc[1] = ball_val[i][j]; // y���� ���������x
			ball[i].acc[2] = ball_val[i][j]; // z���� ���������x
		}
	}
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

	for (int i = 0; i < ball.size(); i++) {
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
	for (int i = 0; i < ball.size(); i++) {
		glPushMatrix();
		{
			glTranslated(ball[i].pos[0], ball[i].pos[1], ball[i].pos[2]);  // ���̂̈ʒu
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball[i].col);
			glutSolidSphere(ball[i].r, 100, 16);  // �T�C�Y�A�����Y�킳�A�E�E
		}
		glPopMatrix();
	}
}

// �t�@�C������{�[���̏����l��ǂݍ����vector�^�ϐ��Ɋi�[����ÓI�����o�֐�
void BALL::Input_file() {

	string line;
	while (!fin.eof() && !fin.fail()) {
		getline(fin, line);
		count_line++;
	}

	ball.resize(count_line);
	ball_val = vector<vector<double>>(count_line, vector<double>(16, 0));

	while (!fin.eof() && !fin.fail()) {
		double a[16];
		static int j = 0;

		fin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]
			>> a[6] >> a[7] >> a[8] >> a[9] >> a[10] >> a[11]
			>> a[12] >> a[13] >> a[14] >> a[15];

		for (int i = 0; i < 16; i++) {
			ball_val[j][i] = a[i];
		}
		j++;
	}

}

void BALL::File_output() {
	double dt_sum = BALL::loopcounter * BALL::dt;

	for (int i = 0; i < ball.size(); i++) {
		// ��ԍX�V�E�E�E��ʂƃt�@�C���ɏo��
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