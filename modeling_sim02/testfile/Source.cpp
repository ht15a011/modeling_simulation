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

BALL ball;
ofstream fout;

long int BALL::loopcounter = 0;
const double BALL::mu_r = 0.9;
const double BALL::dt = 0.015;
const double BALL::table_w = 254;
const double BALL::table_h = 127;
const double BALL::g = 980.7;
const double BALL::mu_d = 0.01;
const double BALL::v_min = 0.1;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // ��ʂ��N���A�ɂ���
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // ���_�ʒu�ix���Ay���Az���j
		0.0, 0.0, 0.0,   // ���_�ڕW�ʒu �A�ǂ������邩�ڕW�����߂�ix���Ay���Az���j
		0.0, 1.0, 0.0);  // ������x�N�g�� �A���_�̌����ix���Ay���Az���j

	// �r�����[�h��̕ǂ̕`��
	BALL::make_billiards_wall();
	BALL::make_balls();

	glutSwapBuffers();

	double dt_sum = BALL::loopcounter * BALL::dt;

	// ��ԍX�V�E�E�E��ʂƃt�@�C���ɏo��
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
	BALL::loopcounter++;

	double vel = sqrt(ball.vel[0] * ball.vel[0] + ball.vel[1] * ball.vel[1]);
	if (vel <= BALL::v_min) {
		ball.vel[0] = 0.0;
		ball.vel[1] = 0.0;
		ball.acc[0] = 0.0;
		ball.acc[1] = 0.0;
	}

	// �����C�͂̎���
	if (ball.vel[0] > 0) {  // x����
		ball.acc[0] = -BALL::mu_d * BALL::g;
	}
	else if (0 > ball.vel[0]) {
		ball.acc[0] = BALL::mu_d * BALL::g;
	}

	if (ball.vel[1] > 0) {  // y����
		ball.acc[1] = -BALL::mu_d * BALL::g;
	}
	else if (0 > ball.vel[1]) {
		ball.acc[1] = BALL::mu_d * BALL::g;
	}

	// �{�[�����Փ˂������̏���
	if (ball.pos[0] + ball.r >= BALL::table_w / 2) {  // �E�̕�
		ball.pos[0] = BALL::table_w / 2 - ball.r;
		ball.vel[0] = -1 * BALL::mu_r * ball.vel[0];
	}
	else if (ball.pos[0] - ball.r <= -BALL::table_w / 2) {  // ���̕�
		ball.pos[0] = -BALL::table_w / 2 + ball.r;
		ball.vel[0] = -1 * BALL::mu_r * ball.vel[0];
	}

	if (ball.pos[1] + ball.r >= BALL::table_h / 2) {  // ��̕�
		ball.pos[1] = BALL::table_h / 2 - ball.r;
		ball.vel[1] = -1 * BALL::mu_r * ball.vel[1];
	}
	else if (ball.pos[1] - ball.r <= -BALL::table_h / 2) {  // ���̕�
		ball.pos[1] = -BALL::table_h / 2 + ball.r;
		ball.vel[1] = -1 * BALL::mu_r * ball.vel[1];
	}

	ball.vel[0] += ball.acc[0] * BALL::dt;
	ball.vel[1] += ball.acc[1] * BALL::dt;
	ball.vel[2] += ball.acc[2] * BALL::dt;

	ball.pos[0] += ball.vel[0] * BALL::dt;
	ball.pos[1] += ball.vel[1] * BALL::dt;
	ball.pos[2] += ball.vel[2] * BALL::dt;
}

int main(int argc, char *argv[]) {
	// OpenGL������
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OS���t���[�Y����ꍇ�͂�������g��
	glutInitWindowSize(800, 600);  // �f�B�X�v���C��ʂ̍쐬
	glutCreateWindow("simulation");

	// �t�@�C���ɏo��
	string filename_output = "output.txt";
	fout.open(filename_output);
	//�o�͌`���̒��� (�����_�ȉ���3�P�^�ɒ���)
	cout << fixed << setprecision(3);
	fout << fixed << setprecision(3);

	glutDisplayFunc(display);
	glutKeyboardFunc(BALL::keyboard);  // �L�[�{�[�h����̓��͂��󂯕t����
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	BALL::init();
	glutMainLoop();

	return 0;
}