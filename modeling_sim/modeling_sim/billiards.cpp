#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;


// �O���[�o���ϐ�
BALL ball;
ofstream fout;

// �ÓI�ȃ����o�ϐ��̍쐬 ���t�@�C����������ۂ�cpp�t�@�C���ɏ���
const double BALL::dt = 0.015;  // ���[�v����
const double BALL::mu_r = 0.9;  // 
const double BALL::table_w = 254;
const double BALL::table_h = 127;
long int BALL::loopcounter = 0;

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