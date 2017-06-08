#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <GL/freeglut.h>
using namespace std;

class BALL {
private:
	double m; // ����
	double r; // ���a
	double e; // �e���W��
public:
	double pos[3]; // �ʒu
	double vel[3]; // ���x
	double acc[3]; // �����x
	float col[4]; // �F
	BALL();
	~BALL();
	double return_r() { return r; }
};

BALL ball;

BALL::BALL() {
	ball.col[0] = 1.0; // �F R
	ball.col[1] = 1.0; // �F G
	ball.col[2] = 0.0; // �F B
	ball.col[3] = 0.0; // �F A

	ball.m = 170; // [g]
	ball.r = 5.71 / 2; // [cm] ���̂̑傫��
	ball.e = 1;
}

BALL::~BALL() {}

// �{�[���̏����ݒ�
void ball_set() {
	ball.pos[0] = 0.0; // x���W �����ʒu
	ball.pos[1] = 0.0; // y���W �����ʒu
	ball.pos[2] = 0.0; // z���W �����ʒu

	ball.vel[0] = 0.1; // x���� �������x
	ball.vel[1] = 0.0; // y���� �������x
	ball.vel[2] = 0.0; // z���� �������x

	ball.acc[0] = 0.0; // x���� ���������x
	ball.acc[1] = 0.0; // y���� ���������x
	ball.acc[2] = 0.0; // z���� ���������x
}

void idle() {
	glutPostRedisplay();
}

void init() {
	// �����̐ݒ�
	GLfloat lpos0[4] = { 0.0, 100.0, 100.0, 1.0 };
	GLfloat lcol0[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lcol0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// �w�i�F
	glClearColor(0.0, 0.4, 0.0, 0.0);  // RGB

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char k, int x, int y) {
	switch (k) {
	case 'q': // q�ŏI��
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

						 // ���̕`��
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // ���̂̈ʒu
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.return_r(), 100, 16);  // �T�C�Y�A�����Y�킳�A�E�E
	}
	glPopMatrix();

	//cout << x << " " << y << " " << z << endl;
	glutSwapBuffers();

	// ��ԍX�V

	//cout << ball.pos[0] << " " << ball.pos[1] << " " << ball.pos[2] << endl;
	cout << ball.pos[2] << endl;

	/*if (ball.pos[0] > 50)
	ball.pos[0] += 0.01;
	else
	ball.pos[0]+= 0.05;*/
	//ball.pos[1]+= 0.01;
	ball.pos[2] += 0.05;

}

int main(int argc, char *argv[]) {
	// OpenGL������
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OS���t���[�Y����ꍇ�͂�������g��
	glutInitWindowSize(800, 600);  // �f�B�X�v���C��ʂ̍쐬
	glutCreateWindow("simulation");

	ball_set();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);  // �L�[�{�[�h����̓��͂��󂯕t����
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	init();
	glutMainLoop();

	return 0;
}