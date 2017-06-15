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

extern const int ball_num = 2;;
extern BALL ball[ball_num];
extern ofstream fout;

// コンストラクター
BALL::BALL() {
	for (int i = 0; i < ball_num; i++) {
		m = 170; // [g]
		r = 5.71 / 2; // [cm] 球体の大きさ ball.r = 120.71 / 2; // [cm] 球体の大きさ
		e = 1;

		ball[i].col[0] = 1.0; // 色 R
		ball[i].col[1] = 1.0; // 色 G
		ball[i].col[2] = 0.0; // 色 B
		ball[i].col[3] = 0.0; // 色 A

		ball[i].pos[0] = 0.0; // x座標 初期位置
		ball[i].pos[1] = 0.0; // y座標 初期位置
		ball[i].pos[2] = 0.0; // z座標 初期位置

		ball[i].vel[0] = 100.0; // x方向 初期速度
		ball[i].vel[1] = 100.0; // y方向 初期速度
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

void BALL::make_balls() {
	// 球体描画 (複数の球体を描くときにこの関数を複数生成すると思われる)
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