#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;

// グローバル変数
extern BALL ball;
extern ofstream fout;

// ボールの初期設定
void BALL::ball_set() {
	ball.pos[0] = 0.0;  // x座標 初期位置
	ball.pos[1] = 0.0;  // y座標 初期位置
	ball.pos[2] = 0.0;  // z座標 初期位置

	ball.vel[0] = 100.0;  // x方向 初期速度
	ball.vel[1] = 100.0;  // y方向 初期速度
	ball.vel[2] = 0.0;    // z方向 初期速度

	ball.acc[0] = 0.0;  // x方向 初期加速度
	ball.acc[1] = 0.0;  // y方向 初期加速度
	ball.acc[2] = 0.0;  // z方向 初期加速度

	ball.col[0] = 1.0;  // 色 R
	ball.col[1] = 1.0;  // 色 G
	ball.col[2] = 0.0;  // 色 B
	ball.col[3] = 0.0;  // 色 A

	ball.m = 170;      // [g]
	ball.r = 5.71 * 5; // ball.r = 120.71 / 2; // [cm] 球体の大きさ
	ball.e = 1;        // 弾性係数
}

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

// キーボードからの入力を処理する
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

// ディスプレイに表示する内容を処理（壁、ボールの位置・速度・加速度など）
void BALL::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 画面をクリアにする
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // 視点位置（x軸、y軸、z軸）
		0.0, 0.0, 0.0,   // 視点目標位置 、どこを見るか目標を決める（x軸、y軸、z軸）
		0.0, 1.0, 0.0);  // 上方向ベクトル 、視点の向き（x軸、y軸、z軸）

	// ビリヤード台の壁の描く

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
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 };  // 黒色
		glTranslated(-table_w / 2, 1.0, 0.0); // -x方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, 1.0); // y方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 上の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 };  // 黒色
		glTranslated(0.0, table_h / 2, 0.0);      // +y方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0);  // x方向にtable_h倍に引き延ばす
		glutSolidCube(1);  // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 下の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.0, 0.0, 0.0 };  // 黒色
		glTranslated(0.0, -table_h / 2, 0.0);     // -y方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, 1.0);  // x方向にtable_h倍に引き延ばす
		glutSolidCube(1);  // サイズ1の立方体を描く
	}
	glPopMatrix();

	// ビリヤード台の色付け
	glPushMatrix();
	{
		GLfloat col[4] = { 0.0, 0.5, 0.0, 0.0 };  // 緑色
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, table_h, 0.0);  // x,y方向にtable_h倍に引き延ばす
		glutSolidCube(1);  // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 球体描画
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // 物体の位置
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.r, 100, 16);  // サイズ、球の綺麗さ
	}
	glPopMatrix();

	glutSwapBuffers();

	double dt_sum = loopcounter * dt;

	// 状態更新・・・画面とファイルに出力(ボールの位置、速度、加速度 ＊xyz方向)
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

	// ボールが壁に衝突した際の処理（めり込み解消処理もしている）
	if (ball.pos[0] + ball.r >= table_w / 2) {  // 右の壁
		ball.pos[0] = table_w / 2 - ball.r;
		ball.vel[0] = -1 * mu_r * ball.vel[0];
	}
	else if (ball.pos[0] - ball.r <= -table_w / 2) {  // 左の壁
		ball.pos[0] = -table_w / 2 + ball.r;
		ball.vel[0] = -1 * mu_r * ball.vel[0];
	}

	if (ball.pos[1] + ball.r >= table_h / 2) {  // 上の壁
		ball.pos[1] = table_h / 2 - ball.r;
		ball.vel[1] = -1 * mu_r * ball.vel[1];
	}
	else if (ball.pos[1] - ball.r <= -table_h / 2) {  // 下の壁
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