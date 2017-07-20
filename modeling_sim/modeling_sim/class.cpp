#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Windows.h>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;

extern vector<BALL> ball;
extern ofstream fout;
extern ifstream fin;
vector< vector<double> > ball_val;

// �R���X�g���N�^�[
BALL::BALL() {}

// �f�X�g���N�^�[
BALL::~BALL() {}

void BALL::Set_ball() {
	ball.resize(ball_val.size());

	for (int i = 0; i < ball_val.size(); i++) {
		ball[i].m = ball_val[i][0]; // [g]
		ball[i].r = ball_val[i][1]; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
		ball[i].e = ball_val[i][2];

		ball[i].pos[0] = ball_val[i][3]; // x���W �����ʒu
		ball[i].pos[1] = ball_val[i][4]; // y���W �����ʒu
		ball[i].pos[2] = ball[i].r; // z���W �����ʒu

		ball[i].vel[0] = ball_val[i][6]; // x���� �������x
		ball[i].vel[1] = ball_val[i][7]; // y���� �������x
		ball[i].vel[2] = ball_val[i][8];   // z���� �������x

		ball[i].acc[0] = ball_val[i][9]; // x���� ���������x
		ball[i].acc[1] = ball_val[i][10]; // y���� ���������x
		ball[i].acc[2] = ball_val[i][11]; // z���� ���������x

		ball[i].col[0] = ball_val[i][12]; // �F R
		ball[i].col[1] = ball_val[i][13]; // �F G
		ball[i].col[2] = ball_val[i][14]; // �F B
		ball[i].col[3] = ball_val[i][15]; // �F A
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
	gluLookAt(0.0, 0.0, 400.0, // ���_�ʒu�ix���Ay���Az���j
		0.0, 0.0, 0.0,   // ���_�ڕW�ʒu �A�ǂ������邩�ڕW�����߂�ix���Ay���Az���j
		0.0, 1.0, 0.0);  // ������x�N�g�� �A���_�̌����ix���Ay���Az���j

	make_billiards_wall();  // �r�����[�h��̕ǂ̕`��
	make_balls();			// ����`��
	glutSwapBuffers();

	File_output();  // �t�@�C���ɋ��̈ʒu�⑬�x�Ȃǂ��o��

	for (int i = 0; i < ball.size(); i++) {
		BALL::ball_stop(i);  // ���̒�~����
		BALL::floor_friction(i);  // ���Ƃ̓����C�͂̌v�Z
		BALL::collision_ball(i);  // �����m�̏Փˏ���
		BALL::collision_wall(i);  // �����ǂɏՓ˂������̏���
	}
}

// �r�����[�h��̕ǂ�`���ÓI�����o�֐�
void BALL::make_billiards_wall() {
	const double height = 20.0;

	// �E�̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // ���F
		glTranslated(table_w / 2, 0.0, 0.0); // +x������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, height); // y������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ���̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // ���F
		glTranslated(-table_w / 2, 1.0, 0.0); // -x������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, height); // y������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ��̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // ���F
		glTranslated(0.0, table_h / 2, 0.0); // +y������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, height); // x������table_h�{�Ɉ������΂�
		glutSolidCube(1); // �T�C�Y1�̗����̂�`��
	}
	glPopMatrix();

	// ���̕�
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // ���F
		glTranslated(0.0, -table_h / 2, 0.0); // -y������table_w/2���炷
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, height); // x������table_h�{�Ɉ������΂�
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

// ���̕`�悷��ÓI�����o�֐�
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
void BALL::File_input() {
	while (!fin.eof() && !fin.fail()) {
		double a[16];
		vector<double> in;

		fin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]
			>> a[6] >> a[7] >> a[8] >> a[9] >> a[10] >> a[11]
			>> a[12] >> a[13] >> a[14] >> a[15];

		// vector�^�R���e�i�Ƀt�@�C������̏����l���i�[����
		for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
			in.push_back(a[i]);
		}
		ball_val.push_back(in);  // ��Ɋi�[���������l��}������
	}
}

void BALL::File_output() {
	double dt_sum = BALL::loopcounter * BALL::dt;

	for (int i = 0; i < ball.size(); i++) {
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

// ���̒�~�������s���ÓI�����o�֐�
void BALL::ball_stop(int i) {
	double vel = sqrt(ball[i].vel[0] * ball[i].vel[0] + ball[i].vel[1] * ball[i].vel[1]);

	if (vel <= v_min) {
		ball[i].vel[0] = 0.0;
		ball[i].vel[1] = 0.0;
		ball[i].acc[0] = 0.0;
		ball[i].acc[1] = 0.0;
	}
}

// �����C�͂��v�Z����ÓI�����o�֐�
void BALL::floor_friction(int i) {
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
}

// �{�[�����m�̏Փ˂̏������s���ÓI�����o�֐�
void BALL::collision_ball(int i) {
	for (int j = i + 1; j < ball.size(); j++) {
		double x = ball[j].pos[0] - ball[i].pos[0];
		double y = ball[j].pos[1] - ball[i].pos[1];

		double overlap = (ball[i].r + ball[j].r) - sqrt(pow(x, 2) + pow(y, 2));

		// �����m�̏Փ˔���
		if (overlap > 0) {
			double a = atan2(y, x);  // �Փˊp�x�̌v�Z

									 // �Փˎ��̂߂荞�ݖh�~����
			ball[i].pos[0] -= fabs(overlap / 2)*cos(a);
			ball[i].pos[1] -= fabs(overlap / 2)*sin(a);
			ball[j].pos[0] += fabs(overlap / 2)*cos(a);
			ball[j].pos[1] += fabs(overlap / 2)*sin(a);

			// ��[i]�̑��x���Փ˕����Ɛ��������ɕ�������
			double Vaxa = ball[i].vel[0] * cos(a) + ball[i].vel[1] * sin(a);
			double Vaya = -1 * ball[i].vel[0] * sin(a) + ball[i].vel[1] * cos(a);

			// ��[j]�̑��x���Փ˕����Ɛ��������ɕ�������
			double Vbxa = ball[j].vel[0] * cos(a) + ball[j].vel[1] * sin(a);
			double Vbya = -1 * ball[j].vel[0] * sin(a) + ball[j].vel[1] * cos(a);

			// �Փˌ�̑��x������ϐ�
			double afterVaxa, afterVbxa;

			// �Փˌ�̏Փ˕����ɂ����鑬�x���v�Z����
			if (ball[i].m == ball[j].m) {
				afterVaxa = Vbxa;
				afterVbxa = Vaxa;
			}
			else {
				afterVaxa = ((ball[i].m - ball[j].m)*Vaxa + 2 * ball[j].m*Vbxa) / (ball[i].m + ball[j].m);
				afterVbxa = (2 * ball[i].m*Vaxa + (ball[j].m - ball[i].m)*Vbxa) / (ball[i].m + ball[j].m);
			}
			ball[i].vel[0] = afterVaxa*cos(a) - Vaya*sin(a);
			ball[i].vel[1] = afterVaxa*sin(a) + Vaya*cos(a);
			ball[j].vel[0] = afterVbxa*cos(a) - Vbya*sin(a);
			ball[j].vel[1] = afterVbxa*sin(a) + Vbya*cos(a);
		}
	}
}

// �{�[�����ǂɏՓ˂������̏������s���ÓI�����o�֐�
void BALL::collision_wall(int i) {
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
