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
	// OpenGL初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OSがフリーズする場合はこちらを使う
	glutInitWindowSize(800, 600);  // ディスプレイ画面の作成
	glutCreateWindow("simulation");

	BALL::data_count();  // ファイルの行数をカウント

	string filename_input = "input_2ball.txt"; // 入力ファイル名
	// ファイルを開く
	fin.open(filename_input.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
		cin.get();
		return -1;
	}

	BALL::File_input();  // ファイルのデータをvector型コンテナに入れる
	
	fin.close();  // ファイルを閉じる

	BALL::Set_ball();  // 球の初期値を設定

	// ファイルに出力
	string filename_output = "output.txt";
	fout.open(filename_output);

	//出力形式の調整 (小数点以下の3ケタに調整)
	//cout << fixed << setprecision(3);
	fout << fixed << setprecision(3);

	glutDisplayFunc(BALL::display);
	glutKeyboardFunc(BALL::keyboard);  // キーボードからの入力を受け付ける
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	BALL::init();
	glutMainLoop();

	return 0;
}