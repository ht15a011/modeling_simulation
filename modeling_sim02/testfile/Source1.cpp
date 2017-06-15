#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
#include <random>
#include "Header.h"
using namespace std;

const int ball_num = 2;
extern BALL ball[ball_num];
extern ofstream fout;

// コンストラクター
BALL::BALL() {
	random_device rd;  // 非決定的な乱数生成器
	mt19937_64 mt(rd());  // メルセンヌ・ツイスタの64ビット版、引数は初期シード

	// 色の乱数生成
	uniform_real_distribution<double> color0(0.0, 1.0);
	uniform_real_distribution<double> color1(0.0, 1.0);
	uniform_real_distribution<double> color2(0.0, 1.0);
	uniform_real_distribution<double> color3(0.0, 1.0);

	// 初期速度の乱数生成
	uniform_real_distribution<double> xvel(-100.0, 100.0);
	uniform_real_distribution<double> yvel(-100.0, 100.0);

	for (int i = 0; i < ball_num; i++) {
		ball[i].m = 170; // [g]
		ball[i].r = 5.71 / 2; // [cm] 球体の大きさ ball.r = 120.71 / 2; // [cm] 球体の大きさ
		ball[i].e = 1;

		ball[i].col[0] = color0(mt); // 色 R
		ball[i].col[1] = color1(mt); // 色 G
		ball[i].col[2] = color2(mt); // 色 B
		ball[i].col[3] = color3(mt); // 色 A

		ball[i].pos[0] = 0.0; // x座標 初期位置
		ball[i].pos[1] = 0.0; // y座標 初期位置
		ball[i].pos[2] = 0.0; // z座標 初期位置

		ball[i].vel[0] = xvel(mt); // x方向 初期速度
		ball[i].vel[1] = yvel(mt); // y方向 初期速度
		ball[i].vel[2] = 0.0;   // z方向 初期速度

		ball[i].acc[0] = 0.0; // x方向 初期加速度
		ball[i].acc[1] = 0.0; // y方向 初期加速度
		ball[i].acc[2] = 0.0; // z方向 初期加速度
	}
}

// デストラクター
BALL::~BALL() {}

void BALL::idle() {
	glutPostRedisplay();
}

void BALL::init() {
	// 光源の設定
	GLfloat lpos0[4] = { 0.0, 100.0, 100.0, 1.0 };
	GLfloat lcol0[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lcol0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 背景色 灰色
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
	case 'q': // qで終了
		fout.close();
		exit(0);
		break;
	default:
		return;
	}
}

void BALL::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 画面をクリアにする
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // 視点位置（x軸、y軸、z軸）
		0.0, 0.0, 0.0,   // 視点目標位置 、どこを見るか目標を決める（x軸、y軸、z軸）
		0.0, 1.0, 0.0);  // 上方向ベクトル 、視点の向き（x軸、y軸、z軸）

						 // ビリヤード台の壁の描く
	make_billiards_wall();
	make_balls();
	glutSwapBuffers();

	File_output();  // ファイルに球の位置や速度などを出力

	for (int i = 0; i < ball_num; i++) {
		double vel = sqrt(ball[i].vel[0] * ball[i].vel[0] + ball[i].vel[1] * ball[i].vel[1]);
		if (vel <= v_min) {
			ball[i].vel[0] = 0.0;
			ball[i].vel[1] = 0.0;
			ball[i].acc[0] = 0.0;
			ball[i].acc[1] = 0.0;
		}

		// 動摩擦力の実装
		if (ball[i].vel[0] > 0) {  // x方向
			ball[i].acc[0] = -mu_d * g;
		}
		else if (0 > ball[i].vel[0]) {
			ball[i].acc[0] = mu_d * g;
		}

		if (ball[i].vel[1] > 0) {  // y方向
			ball[i].acc[1] = -mu_d * g;
		}
		else if (0 > ball[i].vel[1]) {
			ball[i].acc[1] = mu_d * g;
		}

		// ボールが衝突した時の処理
		if (ball[i].pos[0] + ball[i].r >= table_w / 2) {  // 右の壁
			ball[i].pos[0] = table_w / 2 - ball[i].r;
			ball[i].vel[0] = -1 * mu_r * ball[i].vel[0];
		}
		else if (ball[i].pos[0] - ball[i].r <= -table_w / 2) {  // 左の壁
			ball[i].pos[0] = -table_w / 2 + ball[i].r;
			ball[i].vel[0] = -1 * mu_r * ball[i].vel[0];
		}

		if (ball[i].pos[1] + ball[i].r >= table_h / 2) {  // 上の壁
			ball[i].pos[1] = table_h / 2 - ball[i].r;
			ball[i].vel[1] = -1 * mu_r * ball[i].vel[1];
		}
		else if (ball[i].pos[1] - ball[i].r <= -table_h / 2) {  // 下の壁
			ball[i].pos[1] = -table_h / 2 + ball[i].r;
			ball[i].vel[1] = -1 * mu_r * ball[i].vel[1];
		}

		ball[i].vel[0] += ball[i].acc[0] * dt;
		ball[i].vel[1] += ball[i].acc[1] * dt;
		ball[i].vel[2] += ball[i].acc[2] * dt;

		ball[i].pos[0] += ball[i].vel[0] * dt;
		ball[i].pos[1] += ball[i].vel[1] * dt;
		ball[i].pos[2] += ball[i].vel[2] * dt;
	}
}

// ビリヤード台の壁を描く静的メンバ関数
void BALL::make_billiards_wall() {
	// 右の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // 黒色
		glTranslated(table_w / 2, 0.0, 0.0); // +x方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, 1.0); // y方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 左の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // 黒色
		glTranslated(-table_w / 2, 1.0, 0.0); // -x方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, 1.0); // y方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 上の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // 黒色
		glTranslated(0.0, table_h / 2, 0.0); // +y方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0); // x方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 下の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 }; // 黒色
		glTranslated(0.0, -table_h / 2, 0.0); // -y方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0); // x方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// ビリヤード台の色付け
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.5, 0.0, 0.0 }; // 緑色
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, table_h, 0.0); // x,y方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();
}

// 球体描画
void BALL::make_balls() {
	for (int i = 0; i < ball_num; i++) {
		glPushMatrix();
		{
			glTranslated(ball[i].pos[0], ball[i].pos[1], ball[i].pos[2]);  // 物体の位置
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball[i].col);
			glutSolidSphere(ball[i].r, 100, 16);  // サイズ、球の綺麗さ、・・
		}
		glPopMatrix();
	}
}

void BALL::File_output() {
	double dt_sum = BALL::loopcounter * BALL::dt;

	for (int i = 0; i < ball_num; i++) {
		// 状態更新・・・画面とファイルに出力
		/*
		cout << setw(8) << dt_sum << " " <<
			setw(8) << ball[i].pos[0] << " " <<
			setw(8) << ball[i].pos[1] << " " <<
			setw(8) << ball[i].pos[2] << " " <<
			setw(8) << ball[i].vel[0] << " " <<
			setw(8) << ball[i].vel[1] << " " <<
			setw(8) << ball[i].vel[2] << " " <<
			setw(8) << ball[i].acc[0] << " " <<
			setw(8) << ball[i].acc[1] << " " <<
			setw(8) << ball[i].acc[2] << " " <<
			endl;
			*/
		fout << setw(8) << dt_sum << " " <<
			setw(8) << ball[i].pos[0] << " " <<
			setw(8) << ball[i].pos[1] << " " <<
			setw(8) << ball[i].pos[2] << " " <<
			setw(8) << ball[i].vel[0] << " " <<
			setw(8) << ball[i].vel[1] << " " <<
			setw(8) << ball[i].vel[2] << " " <<
			setw(8) << ball[i].acc[0] << " " <<
			setw(8) << ball[i].acc[1] << " " <<
			setw(8) << ball[i].acc[2] << " " <<
			endl;

		fout.flush();
	}
	BALL::loopcounter++;
}