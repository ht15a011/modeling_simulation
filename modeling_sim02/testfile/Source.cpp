#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <GL/freeglut.h>
using namespace std;

class BALL {
private:
	double m; // 質量
	double r; // 半径
	double e; // 弾性係数
public:
	double pos[3]; // 位置
	double vel[3]; // 速度
	double acc[3]; // 加速度
	float col[4]; // 色
	BALL();
	~BALL();
	double return_r() { return r; }
};

BALL ball;

BALL::BALL() {
	ball.col[0] = 1.0; // 色 R
	ball.col[1] = 1.0; // 色 G
	ball.col[2] = 0.0; // 色 B
	ball.col[3] = 0.0; // 色 A

	ball.m = 170; // [g]
	ball.r = 5.71 / 2; // [cm] 球体の大きさ
	ball.e = 1;
}

BALL::~BALL() {}

// ボールの初期設定
void ball_set() {
	ball.pos[0] = 0.0; // x座標 初期位置
	ball.pos[1] = 0.0; // y座標 初期位置
	ball.pos[2] = 0.0; // z座標 初期位置

	ball.vel[0] = 0.1; // x方向 初期速度
	ball.vel[1] = 0.0; // y方向 初期速度
	ball.vel[2] = 0.0; // z方向 初期速度

	ball.acc[0] = 0.0; // x方向 初期加速度
	ball.acc[1] = 0.0; // y方向 初期加速度
	ball.acc[2] = 0.0; // z方向 初期加速度
}

void idle() {
	glutPostRedisplay();
}

void init() {
	// 光源の設定
	GLfloat lpos0[4] = { 0.0, 100.0, 100.0, 1.0 };
	GLfloat lcol0[4] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lcol0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 背景色
	glClearColor(0.0, 0.4, 0.0, 0.0);  // RGB

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
}

void resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char k, int x, int y) {
	switch (k) {
	case 'q': // qで終了
		exit(0);
		break;
	default:
		return;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 画面をクリアにする
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 500.0, // 視点位置（x軸、y軸、z軸）
		0.0, 0.0, 0.0,   // 視点目標位置 、どこを見るか目標を決める（x軸、y軸、z軸）
		0.0, 1.0, 0.0);  // 上方向ベクトル 、視点の向き（x軸、y軸、z軸）

						 // 球体描画
	glPushMatrix();
	{
		glTranslated(ball.pos[0], ball.pos[1], ball.pos[2]);  // 物体の位置
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball.col);
		glutSolidSphere(ball.return_r(), 100, 16);  // サイズ、球の綺麗さ、・・
	}
	glPopMatrix();

	//cout << x << " " << y << " " << z << endl;
	glutSwapBuffers();

	// 状態更新

	//cout << ball.pos[0] << " " << ball.pos[1] << " " << ball.pos[2] << endl;
	cout << ball.pos[2] << endl;

	/*if (ball.pos[0] > 50)
	ball.pos[0] += 0.01;
	else
	ball.pos[0]+= 0.05;*/
	//ball.pos[1]+= 0.01;
	ball.pos[2] += 0.05;

}

int main(int argc, char *argv[]) {
	// OpenGL初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH ); // OSがフリーズする場合はこちらを使う
	glutInitWindowSize(800, 600);  // ディスプレイ画面の作成
	glutCreateWindow("simulation");

	ball_set();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);  // キーボードからの入力を受け付ける
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	init();
	glutMainLoop();

	return 0;
}