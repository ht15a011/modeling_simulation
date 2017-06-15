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

extern BALL ball;
extern ofstream fout;

// コンストラクター
BALL::BALL() {
	m = 170; // [g]
	r = 5.71; // [cm] 球体の大きさ ball.r = 120.71 / 2; // [cm] 球体の大きさ
	e = 1;

	col[0] = 1.0; // 色 R
	col[1] = 1.0; // 色 G
	col[2] = 0.0; // 色 B
	col[3] = 0.0; // 色 A

	pos[0] = 0.0; // x座標 初期位置
	pos[1] = 0.0; // y座標 初期位置
	pos[2] = 0.0; // z座標 初期位置

	vel[0] = 100.0; // x方向 初期速度
	vel[1] = 100.0; // y方向 初期速度
	vel[2] = 0.0;   // z方向 初期速度

	acc[0] = 0.0; // x方向 初期加速度
	acc[1] = 0.0; // y方向 初期加速度
	acc[2] = 0.0; // z方向 初期加速度
}

BALL::BALL(double size, float col0, float col1, float col2, float col3) {
	static double pos0, pos1, pos2;
	pos0 = 0.0;
	pos1 = 0.0;
	pos2 = 0.0;

	m = 170; // [g]
	r = size; // [cm] 球体の大きさ ball.r = 120.71 / 2; // [cm] 球体の大きさ
	e = 1;

	col[0] = col0; // 色 R
	col[1] = col1; // 色 G
	col[2] = col2; // 色 B
	col[3] = col3; // 色 A

	pos[0] = pos0; // x座標 初期位置
	pos[1] = pos1; // y座標 初期位置
	pos[2] = pos2; // z座標 初期位置

	vel[0] = 100.0; // x方向 初期速度
	vel[1] = 100.0; // y方向 初期速度
	vel[2] = 0.0;   // z方向 初期速度

	acc[0] = 0.0; // x方向 初期加速度
	acc[1] = 0.0; // y方向 初期加速度
	acc[2] = 0.0; // z方向 初期加速度
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
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // 物体の位置
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.r, 100, 16);  // サイズ、球の綺麗さ、・・
	}
	glPopMatrix();
}