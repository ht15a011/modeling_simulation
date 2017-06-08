#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;


// グローバル変数
BALL ball;
ofstream fout;

// 静的なメンバ変数の作成 ＊ファイル分割する際はcppファイルに書く
const double BALL::dt = 0.01;     // ループ時間
const double BALL::mu_r = 0.9;     // 反発係数
const double BALL::table_w = 254;  // ビリヤードのテーブル横幅 [cm]
const double BALL::table_h = 127;  // ビリヤードのテーブル縦幅 [cm]
long int BALL::loopcounter = 0;    // ループ時間
const double BALL::g = 980.7;	   // 重力加速度 [cm/s^2]
const double BALL::mu_d = 0.01;    // 動摩擦係数
const double BALL::v_min = 0.1;	   // 速度の閾値

int main(int argc, char *argv[]) {
	// OpenGL初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OSがフリーズする場合はこちらを使う
	glutInitWindowSize(800, 600);  // ディスプレイ画面の作成
	glutCreateWindow("simulation");

	ball.ball_set();

	// ファイルに出力
	string filename_output = "output.txt";
	fout.open(filename_output);

	//出力形式の調整 (小数点以下の3ケタに調整)
	cout << fixed << setprecision(3);
	fout << fixed << setprecision(3);

	glutDisplayFunc(BALL::display);
	glutKeyboardFunc(BALL::keyboard);  // キーボードからの入力を受け付ける
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	ball.init();
	glutMainLoop();

	return 0;
}