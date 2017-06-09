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
const double mu_r = 0.9;
const double dt = 0.015;  // �P�ʂ�[sec]�D���̒l�͊e����PC�̃X�y�b�N�ɍ��킹�Ē�������D
const double table_w = 254; // �e�[�u������ [cm]
const double table_h = 127; // �e�[�u���c�� [cm]
const double g = 980.7;	   // �d�͉����x [cm/s^2]
const double mu_d = 0.01;    // �����C�W��
const double v_min = 0.1;	   // ���x��臒l

class BALL {
public:
	double m; // ����
	double r; // ���a
	double e; // �e���W��
	double pos[3]; // �ʒu
	double vel[3]; // ���x
	double acc[3]; // �����x
	float col[4]; // �F
	//void set();
	BALL();
	~BALL();
	static void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
};

BALL ball;

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

BALL::~BALL(){}

/*
// �{�[���̏����ݒ�
void BALL::set() {
	pos[0] = 0.0; // x���W �����ʒu
	pos[1] = 0.0; // y���W �����ʒu
	pos[2] = 0.0; // z���W �����ʒu

	vel[0] = 127.0; // x���� �������x
	vel[1] = 0.0; // y���� �������x
	vel[2] = 0.0; // z���� �������x

	acc[0] = 0.0; // x���� ���������x
	acc[1] = 0.0; // y���� ���������x
	acc[2] = 0.0; // z���� ���������x

	col[0] = 1.0; // �F R
	col[1] = 1.0; // �F G
	col[2] = 0.0; // �F B
	col[3] = 0.0; // �F A

	m = 170; // [g]
	r = 5.71 / 2; // [cm] ���̂̑傫�� ball.r = 120.71 / 2; // [cm] ���̂̑傫��
	e = 1;
}
*/

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

void display() {
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

	// ���̕`�� (�����̋��̂�`���Ƃ��ɂ��̊֐��𕡐���������Ǝv����)
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
	
	double vel = sqrt(ball.vel[0] * ball.vel[0] + ball.vel[1] * ball.vel[1]);
	if (vel <= v_min) {
		ball.vel[0] = 0.0;
		ball.vel[1] = 0.0;
		ball.acc[0] = 0.0;
		ball.acc[1] = 0.0;
	}

	// �����C�͂̎���
	if (ball.vel[0] > 0) {  // x����
		ball.acc[0] = -mu_d * g;
	}
	else if (0 > ball.vel[0]) {
		ball.acc[0] = mu_d * g;
	}

	if (ball.vel[1] > 0) {  // y����
		ball.acc[1] = -mu_d * g;
	}
	else if (0 > ball.vel[1]) {
		ball.acc[1] = mu_d * g;
	}

	// �{�[�����Փ˂������̏���
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