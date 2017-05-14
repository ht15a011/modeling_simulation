#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
using namespace std;


// �O���[�o���ϐ�
ofstream fout;
long int loopcounter = 0;
const double mu_r = 1;

// ���[�v����
const double dt = 0.015;  // �P�ʂ�[sec]�D���̒l�͊e����PC�̃X�y�b�N�ɍ��킹�Ē�������D

						  // �r�����[�h�̃e�[�u��
const double table_w = 254; // �e�[�u������ [cm]
const double table_h = 127; // �e�[�u���c�� [cm]

class BALL {
public:
	double m; // ����
	double r; // ���a
	double e; // �e���W��
	float col[4]; // �F
	double pos[3]; // �ʒu
	double vel[3]; // ���x
	double acc[3]; // �����x
	void ball_set();
	void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
};

BALL ball;

// �{�[���̏����ݒ�
void BALL::ball_set() {
	ball.pos[0] = 0.0; // x���W �����ʒu
	ball.pos[1] = 0.0; // y���W �����ʒu
	ball.pos[2] = 0.0; // z���W �����ʒu

	ball.vel[0] = 100.0; // x���� �������x
	ball.vel[1] = 100.0; // y���� �������x
	ball.vel[2] = 0.0; // z���� �������x

	ball.acc[0] = 0.0; // x���� ���������x
	ball.acc[1] = 0.0; // y���� ���������x
	ball.acc[2] = 0.0; // z���� ���������x

	ball.col[0] = 1.0; // �F R
	ball.col[1] = 1.0; // �F G
	ball.col[2] = 0.0; // �F B
	ball.col[3] = 0.0; // �F A

	ball.m = 170; // [g]
	ball.r = 5.71 / 2; // [cm] ���̂̑傫��
					   //ball.r = 120.71 / 2; // [cm] ���̂̑傫��
	ball.e = 1;
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

						 //�r�����[�h��̕ǂ̕`��

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

	// ���̕`��
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // ���̂̈ʒu
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.r, 100, 16);  // �T�C�Y�A�����Y�킳�A�E�E
	}
	glPopMatrix();

	glutSwapBuffers();

	double dt_sum = loopcounter*dt;

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
	loopcounter++;

	// �{�[�����Փ˂������̏���
	if (ball.pos[0] + ball.r >= table_w / 2) {  // �E�̕ǂɏՓ˂������̏���
		ball.vel[0] = -1 * mu_r * ball.vel[0];
	}
	if (ball.pos[0] - ball.r <= -table_w / 2) {  // ���̕ǂɏՓ˂������̏���
		ball.vel[0] = -1 * mu_r * ball.vel[0];
	}
	if (ball.pos[1] + ball.r >= table_h / 2) {  // ��̕ǂɏՓ˂������̏���
		ball.vel[1] = -1 * mu_r * ball.vel[1];
	}
	if (ball.pos[1] - ball.r <= -table_h / 2) {  // ���̕ǂɏՓ˂������̏���
		ball.vel[1] = -1 * mu_r * ball.vel[1];
	}

	ball.vel[0] += ball.acc[0] * dt;
	ball.vel[1] += ball.acc[1] * dt;
	ball.vel[2] += ball.acc[2] * dt;

	ball.pos[0] += ball.vel[0] * dt;
	ball.pos[1] += ball.vel[1] * dt;
	ball.pos[2] += ball.vel[2] * dt;

}

int main(int argc, char *argv[]) {
	// OpenGL������
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OS���t���[�Y����ꍇ�͂�������g��
	glutInitWindowSize(800, 600);  // �f�B�X�v���C��ʂ̍쐬
	glutCreateWindow("simulation");

	ball.ball_set();

	// �t�@�C���ɏo��
	string filename_output = "output.txt";
	fout.open(filename_output);
	//�o�͌`���̒��� (�����_�ȉ���3�P�^�ɒ���)
	cout << fixed << setprecision(3);
	fout << fixed << setprecision(3);

	glutDisplayFunc(BALL::display);
	glutKeyboardFunc(BALL::keyboard);  // �L�[�{�[�h����̓��͂��󂯕t����
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	ball.init();
	glutMainLoop();

	return 0;
}