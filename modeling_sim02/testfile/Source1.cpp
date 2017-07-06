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

extern vector<BALL> ball;
extern ofstream fout;
extern ifstream fin;
vector< vector<double> > ball_val;

// コンストラクター
BALL::BALL() {}

// デストラクター
BALL::~BALL() {}

void BALL::Set_ball() {
	ball.resize(count_line);

	for (int i = 0; i < count_line; i++) {
		ball[i].m = ball_val[i][0]; // [g]
		ball[i].r = ball_val[i][1]; // [cm] 球体の大きさ ball.r = 120.71 / 2; // [cm] 球体の大きさ
		ball[i].e = ball_val[i][2];

		ball[i].pos[0] = ball_val[i][3]; // x座標 初期位置
		ball[i].pos[1] = ball_val[i][4]; // y座標 初期位置
		ball[i].pos[2] = ball_val[i][5]; // z座標 初期位置

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
	gluLookAt(0.0, 0.0, 500.0, // 視点位置（x軸、y軸、z軸）
		0.0, 0.0, 0.0,   // 視点目標位置 、どこを見るか目標を決める（x軸、y軸、z軸）
		0.0, 1.0, 0.0);  // 上方向ベクトル 、視点の向き（x軸、y軸、z軸）

						 // ビリヤード台の壁の描く
	make_billiards_wall();
	make_balls();
	glutSwapBuffers();

	File_output();  // ファイルに球の位置や速度などを出力

	// ボール同士が衝突した時の処理
	/*
	for (int i = 0; i < ball.size(); i++) {
		for (int j = i + 1; j < ball.size(); j++) {
			double x = ball[j].pos[0] - ball[i].pos[0];
			double y = ball[j].pos[1] - ball[i].pos[1];
			
			double overlap = (ball[i].r + ball[j].r) - sqrt(pow(x, 2) + pow(y, 2));

			if (overlap > 0) {
				double a = atan2(y, x);  // 衝突角度の計算

				ball[i].pos[0] = -fabs(overlap / 2)*cos(a);
				ball[i].pos[1] = -fabs(overlap / 2)*sin(a);
				ball[j].pos[0] = fabs(overlap / 2)*cos(a);
				ball[j].pos[1] = fabs(overlap / 2)*sin(a);
			}
		}
	}
	*/
	for (int i = 0; i < ball.size(); i++) {
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

		// ボール同士の衝突の処理
		for (int j = i + 1; j < ball.size(); j++) {
			double x = ball[j].pos[0] - ball[i].pos[0];
			double y = ball[j].pos[1] - ball[i].pos[1];

			double overlap = (ball[i].r + ball[j].r) - sqrt(pow(x, 2) + pow(y, 2));

			if (overlap > 0) {
				double a = atan2(y, x);  // 衝突角度の計算

				double Vaxa = ball[i].vel[0] * cos(a) + ball[i].vel[1] * sin(a);
				double Vaya = -1 * ball[i].vel[0] * sin(a) + ball[i].vel[1] * cos(a);

				double Vbxa = ball[j].vel[0] * cos(a) + ball[j].vel[1] * sin(a);
				double Vbya = -1 * ball[j].vel[0] * sin(a) + ball[j].vel[1] * cos(a);

				if (ball[i].m == ball[j].m) {
					double temp;
					temp = Vaxa; Vaxa = Vbxa; Vbxa = temp;
				}
				else {
					Vaxa = ((ball[i].m - ball[j].m)*Vaxa + 2 * ball[j].m*Vbxa) / (ball[i].m + ball[j].m);
					Vbxa = (2 * ball[i].m*Vaxa + (ball[j].m - ball[i].m)*Vbxa) / (ball[i].m + ball[j].m);
				}
				ball[i].vel[0] = Vaxa*cos(a) - Vaya*sin(a);
				ball[i].vel[1] = Vaxa*sin(a) + Vaya*cos(a);
				ball[i].pos[0] = -fabs(overlap / 2)*cos(a);
				ball[i].pos[1] = -fabs(overlap / 2)*sin(a);

				ball[j].vel[0] = Vbxa*cos(a) - Vbya*sin(a);
				ball[j].vel[1] = Vbxa*sin(a) + Vbya*cos(a);
				ball[j].pos[0] = fabs(overlap / 2)*cos(a);
				ball[j].pos[1] = fabs(overlap / 2)*sin(a);
			}
		}

		// ボールが壁に衝突した時の処理
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

// ファイルの行数をカウントするメンバ関数
void BALL::data_count(string filename) {
	//string filename_input = "input_4ball.txt"; // 入力ファイル名

	// ファイルを開く
	fin.open(filename.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
	}

	string line;
	while (!fin.eof() && !fin.fail()) {
		getline(fin, line);
		count_line++;
	}
	fin.close();
}

// ファイルからボールの初期値を読み込んでvector型変数に格納する静的メンバ関数
void BALL::File_input() {
	ball_val = vector<vector<double>>(count_line, vector<double>(16, 0));

	while (!fin.eof() && !fin.fail()) {
		double a[16];
		static int j = 0;

		fin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]
			>> a[6] >> a[7] >> a[8] >> a[9] >> a[10] >> a[11]
			>> a[12] >> a[13] >> a[14] >> a[15];

		for (int i = 0; i < 16; i++) {
			ball_val[j][i] = a[i];
		}
		j++;
	}
	fin.close();
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