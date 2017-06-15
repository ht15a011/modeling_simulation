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

long int BALL::loopcounter = 0;
const double BALL::mu_r = 0.9;
const double BALL::dt = 0.001;
const double BALL::table_w = 254;
const double BALL::table_h = 127;
const double BALL::g = 980.7;
const double BALL::mu_d = 0.01;
const double BALL::v_min = 0.1;

const int ball_num = 2;
BALL ball[ball_num];
ofstream fout;

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

	glutDisplayFunc(BALL::display);
	glutKeyboardFunc(BALL::keyboard);  // �L�[�{�[�h����̓��͂��󂯕t����
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	BALL::init();
	glutMainLoop();

	return 0;
}