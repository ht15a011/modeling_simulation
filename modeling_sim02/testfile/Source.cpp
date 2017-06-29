#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <GL/freeglut.h>
#include "Header.h"
using namespace std;

int BALL::count_line = 0;
long int BALL::loopcounter = 0;
const double BALL::mu_r = 0.9;
const double BALL::dt = 0.0022;
const double BALL::table_w = 254;
const double BALL::table_h = 127;
const double BALL::g = 980.7;
const double BALL::mu_d = 0.01;
const double BALL::v_min = 0.1;

vector<BALL> ball;
ofstream fout;
ifstream fin;

int main(int argc, char *argv[]) {
	// OpenGL������
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OS���t���[�Y����ꍇ�͂�������g��
	glutInitWindowSize(800, 600);  // �f�B�X�v���C��ʂ̍쐬
	glutCreateWindow("simulation");

	BALL::data_count();  // �t�@�C���̍s�����J�E���g

	string filename_input = "input_2ball.txt"; // ���̓t�@�C����
	// �t�@�C�����J��
	fin.open(filename_input.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
		cin.get();
		return -1;
	}

	BALL::File_input();  // �t�@�C���̃f�[�^��vector�^�R���e�i�ɓ����
	
	fin.close();  // �t�@�C�������

	BALL::Set_ball();  // ���̏����l��ݒ�

	// �t�@�C���ɏo��
	string filename_output = "output.txt";
	fout.open(filename_output);

	//�o�͌`���̒��� (�����_�ȉ���3�P�^�ɒ���)
	//cout << fixed << setprecision(3);
	fout << fixed << setprecision(3);

	glutDisplayFunc(BALL::display);
	glutKeyboardFunc(BALL::keyboard);  // �L�[�{�[�h����̓��͂��󂯕t����
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	BALL::init();
	glutMainLoop();

	return 0;
}