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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 画面をクリアにする
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // 視点位置（x軸、y軸、z軸）
		0.0, 0.0, 0.0,   // 視点目標位置 、どこを見るか目標を決める（x軸、y軸、z軸）
		0.0, 1.0, 0.0);  // 上方向ベクトル 、視点の向き（x軸、y軸、z軸）

	// ビリヤード台の壁の描く
	BALL::make_billiards_wall();
	BALL::make_balls();

	glutSwapBuffers();

	double dt_sum = BALL::loopcounter * BALL::dt;

	// 状態更新・・・画面とファイルに出力
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

	// 動摩擦力の実装
	if (ball.vel[0] > 0) {  // x方向
		ball.acc[0] = -BALL::mu_d * BALL::g;
	}
	else if (0 > ball.vel[0]) {
		ball.acc[0] = BALL::mu_d * BALL::g;
	}

	if (ball.vel[1] > 0) {  // y方向
		ball.acc[1] = -BALL::mu_d * BALL::g;
	}
	else if (0 > ball.vel[1]) {
		ball.acc[1] = BALL::mu_d * BALL::g;
	}

	// ボールが衝突した時の処理
	if (ball.pos[0] + ball.r >= BALL::table_w / 2) {  // 右の壁
		ball.pos[0] = BALL::table_w / 2 - ball.r;
		ball.vel[0] = -1 * BALL::mu_r * ball.vel[0];
	}
	else if (ball.pos[0] - ball.r <= -BALL::table_w / 2) {  // 左の壁
		ball.pos[0] = -BALL::table_w / 2 + ball.r;
		ball.vel[0] = -1 * BALL::mu_r * ball.vel[0];
	}

	if (ball.pos[1] + ball.r >= BALL::table_h / 2) {  // 上の壁
		ball.pos[1] = BALL::table_h / 2 - ball.r;
		ball.vel[1] = -1 * BALL::mu_r * ball.vel[1];
	}
	else if (ball.pos[1] - ball.r <= -BALL::table_h / 2) {  // 下の壁
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
	// OpenGL初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OSがフリーズする場合はこちらを使う
	glutInitWindowSize(800, 600);  // ディスプレイ画面の作成
	glutCreateWindow("simulation");

	// ファイルに出力
	string filename_output = "output.txt";
	fout.open(filename_output);
	//出力形式の調整 (小数点以下の3ケタに調整)
	cout << fixed << setprecision(3);
	fout << fixed << setprecision(3);

	glutDisplayFunc(display);
	glutKeyboardFunc(BALL::keyboard);  // キーボードからの入力を受け付ける
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	BALL::init();
	glutMainLoop();

	return 0;
}