#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <GL/freeglut.h>
using namespace std;

class BALL {
public:
	double m;      // 質量
	double r;      // 半径
	double e;      // 弾性係数
	double pos[3]; // 位置
	double vel[3]; // 速度
	double acc[3]; // 加速度
	float col[4];  // 色

	static ofstream fout;
	
	static long int loopcounter;
	static const double mu_r;
	static const double dt;      // 単位は[sec]．この値は各自のPCのスペックに合わせて調整する．
	static const double table_w; // テーブル横幅 [cm]
	static const double table_h; // テーブル縦幅 [cm]
	static const double g;	      // 重力加速度 [cm/s^2]
	static const double mu_d;    // 動摩擦係数
	static const double v_min;	  // 速度の閾値

	//void set();
	BALL();
	BALL(double, float, float, float, float);
	~BALL();
	static void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void make_billiards_wall();
	static void make_balls();
};

ofstream BALL::fout;
long int BALL::loopcounter = 0;
const double BALL::mu_r = 0.9;
const double BALL::dt = 0.015;
const double BALL::table_w = 254;
const double BALL::table_h = 127;
const double BALL::g = 980.7;
const double BALL::mu_d = 0.01;
const double BALL::v_min = 0.1;

BALL ball;

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

	BALL::fout << setw(8) << dt_sum << " " <<
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

	BALL::fout.flush();
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
	BALL::fout.open(filename_output);
	//出力形式の調整 (小数点以下の3ケタに調整)
	cout << fixed << setprecision(3);
	BALL::fout << fixed << setprecision(3);

	glutDisplayFunc(display);
	glutKeyboardFunc(BALL::keyboard);  // キーボードからの入力を受け付ける
	glutReshapeFunc(BALL::resize);
	glutIdleFunc(BALL::idle);

	BALL::init();
	glutMainLoop();

	return 0;
}