#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Windows.h>
#include <GL/freeglut.h>
#include "class.h"
using namespace std;

extern vector<BALL> ball;
extern ofstream fout;
extern ifstream fin;
vector< vector<double> > ball_val;

// コンストラクター
BALL::BALL() {}

// デストラクター
BALL::~BALL() {}

void BALL::Set_ball() {
	ball.resize(ball_val.size());

	for (int i = 0; i < ball_val.size(); i++) {
		ball[i].m = ball_val[i][0]; // [g]
		ball[i].r = ball_val[i][1]; // [cm] 球体の大きさ ball.r = 120.71 / 2; // [cm] 球体の大きさ
		ball[i].e = ball_val[i][2];

		ball[i].pos[0] = ball_val[i][3]; // x座標 初期位置
		ball[i].pos[1] = ball_val[i][4]; // y座標 初期位置
		ball[i].pos[2] = ball[i].r; // z座標 初期位置

		ball[i].vel[0] = ball_val[i][6]; // x方向 初期速度
		ball[i].vel[1] = ball_val[i][7]; // y方向 初期速度
		ball[i].vel[2] = ball_val[i][8];   // z方向 初期速度

		ball[i].acc[0] = ball_val[i][9]; // x方向 初期加速度
		ball[i].acc[1] = ball_val[i][10]; // y方向 初期加速度
		ball[i].acc[2] = ball_val[i][11]; // z方向 初期加速度

		ball[i].col[0] = ball_val[i][12]; // 色 R
		ball[i].col[1] = ball_val[i][13]; // 色 G
		ball[i].col[2] = ball_val[i][14]; // 色 B
		ball[i].col[3] = ball_val[i][15]; // 色 A
	}
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
	gluLookAt(0.0, 0.0, 400.0, // 視点位置（x軸、y軸、z軸）
		0.0, 0.0, 0.0,   // 視点目標位置 、どこを見るか目標を決める（x軸、y軸、z軸）
		0.0, 1.0, 0.0);  // 上方向ベクトル 、視点の向き（x軸、y軸、z軸）

	make_billiards_wall();  // ビリヤード台の壁の描く
	make_balls();			// 球を描く
	glutSwapBuffers();

	File_output();  // ファイルに球の位置や速度などを出力

	for (int i = 0; i < ball.size(); i++) {
		BALL::ball_stop(i);  // 球の停止処理
		BALL::floor_friction(i);  // 床との動摩擦力の計算
		BALL::collision_ball(i);  // 球同士の衝突処理
		BALL::collision_wall(i);  // 球が壁に衝突した時の処理
	}
}

// ビリヤード台の壁を描く静的メンバ関数
void BALL::make_billiards_wall() {
	const double height = 20.0;

	// 右の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // 茶色
		glTranslated(table_w / 2, 0.0, 0.0); // +x方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, height); // y方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 左の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // 茶色
		glTranslated(-table_w / 2, 1.0, 0.0); // -x方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(1.0, table_h, height); // y方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 上の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // 茶色
		glTranslated(0.0, table_h / 2, 0.0); // +y方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, height); // x方向にtable_h倍に引き延ばす
		glutSolidCube(1); // サイズ1の立方体を描く
	}
	glPopMatrix();

	// 下の壁
	glPushMatrix();
	{
		GLfloat col[4] = { 0.6, 0.2, 0.0, 0.0 }; // 茶色
		glTranslated(0.0, -table_h / 2, 0.0); // -y方向にtable_w/2ずらす
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
		glScalef(table_w, 1.0, height); // x方向にtable_h倍に引き延ばす
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

// 球体描画する静的メンバ関数
void BALL::make_balls() {
	for (int i = 0; i < ball.size(); i++) {
		glPushMatrix();
		{
			glTranslated(ball[i].pos[0], ball[i].pos[1], ball[i].pos[2]);  // 物体の位置
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball[i].col);
			glutSolidSphere(ball[i].r, 100, 16);  // サイズ、球の綺麗さ、・・
		}
		glPopMatrix();
	}
}

// ファイルからボールの初期値を読み込んでvector型変数に格納する静的メンバ関数
void BALL::File_input() {
	while (!fin.eof() && !fin.fail()) {
		double a[16];
		vector<double> in;

		fin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]
			>> a[6] >> a[7] >> a[8] >> a[9] >> a[10] >> a[11]
			>> a[12] >> a[13] >> a[14] >> a[15];

		// vector型コンテナにファイルからの初期値を格納する
		for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
			in.push_back(a[i]);
		}
		ball_val.push_back(in);  // 先に格納した初期値を挿入する
	}
}

void BALL::File_output() {
	double dt_sum = BALL::loopcounter * BALL::dt;

	for (int i = 0; i < ball.size(); i++) {
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

// 球の停止処理を行う静的メンバ関数
void BALL::ball_stop(int i) {
	double vel = sqrt(ball[i].vel[0] * ball[i].vel[0] + ball[i].vel[1] * ball[i].vel[1]);

	if (vel <= v_min) {
		ball[i].vel[0] = 0.0;
		ball[i].vel[1] = 0.0;
		ball[i].acc[0] = 0.0;
		ball[i].acc[1] = 0.0;
	}
}

// 動摩擦力を計算する静的メンバ関数
void BALL::floor_friction(int i) {
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
}

// ボール同士の衝突の処理を行う静的メンバ関数
void BALL::collision_ball(int i) {
	for (int j = i + 1; j < ball.size(); j++) {
		double x = ball[j].pos[0] - ball[i].pos[0];
		double y = ball[j].pos[1] - ball[i].pos[1];

		double overlap = (ball[i].r + ball[j].r) - sqrt(pow(x, 2) + pow(y, 2));

		// 球同士の衝突判定
		if (overlap > 0) {
			double a = atan2(y, x);  // 衝突角度の計算

									 // 衝突時のめり込み防止処理
			ball[i].pos[0] -= fabs(overlap / 2)*cos(a);
			ball[i].pos[1] -= fabs(overlap / 2)*sin(a);
			ball[j].pos[0] += fabs(overlap / 2)*cos(a);
			ball[j].pos[1] += fabs(overlap / 2)*sin(a);

			// 球[i]の速度を衝突方向と垂直方向に分解する
			double Vaxa = ball[i].vel[0] * cos(a) + ball[i].vel[1] * sin(a);
			double Vaya = -1 * ball[i].vel[0] * sin(a) + ball[i].vel[1] * cos(a);

			// 球[j]の速度を衝突方向と垂直方向に分解する
			double Vbxa = ball[j].vel[0] * cos(a) + ball[j].vel[1] * sin(a);
			double Vbya = -1 * ball[j].vel[0] * sin(a) + ball[j].vel[1] * cos(a);

			// 衝突後の速度を入れる変数
			double afterVaxa, afterVbxa;

			// 衝突後の衝突方向における速度を計算する
			if (ball[i].m == ball[j].m) {
				afterVaxa = Vbxa;
				afterVbxa = Vaxa;
			}
			else {
				afterVaxa = ((ball[i].m - ball[j].m)*Vaxa + 2 * ball[j].m*Vbxa) / (ball[i].m + ball[j].m);
				afterVbxa = (2 * ball[i].m*Vaxa + (ball[j].m - ball[i].m)*Vbxa) / (ball[i].m + ball[j].m);
			}
			ball[i].vel[0] = afterVaxa*cos(a) - Vaya*sin(a);
			ball[i].vel[1] = afterVaxa*sin(a) + Vaya*cos(a);
			ball[j].vel[0] = afterVbxa*cos(a) - Vbya*sin(a);
			ball[j].vel[1] = afterVbxa*sin(a) + Vbya*cos(a);
		}
	}
}

// ボールが壁に衝突した時の処理を行う静的メンバ関数
void BALL::collision_wall(int i) {
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
