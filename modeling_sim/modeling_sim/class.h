#ifndef CLASS_H_
#define CLASS_H_

// クラス定義部
class BALL {
private:
	double m;      // 質量
	double r;      // 半径
	double e;      // 弾性係数
	float col[4];  // ボールの色
	static const double table_w;  // ビリヤードのテーブル横幅 [cm]
	static const double table_h;  // ビリヤードのテーブル縦幅 [cm]
	static const double dt;       // 単位は[sec]．この値は各自のPCのスペックに合わせて調整する．
	static const double mu_r;     // 反発係数(0 < mu_r < 1)
	static long int loopcounter;  // ループ時間
	static const double g;		  // 重力加速度 [cm/s^2]
	static const double mu_d;     // 動摩擦係数
	static const double v_min;			  // 速度の閾値
public:
	double pos[3];  // 位置
	double vel[3];  // 速度
	double acc[3];  // 加速度
	void ball_set();
	void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
};

#endif // CLASS_H_
