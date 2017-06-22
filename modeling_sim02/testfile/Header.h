#ifndef HEADER_H_
#define HEADER_H_

class BALL {
private:
	double m;      // 質量
	double r;      // 半径
	double e;      // 弾性係数
	float col[4];  // 色
	static long int loopcounter;  // ループ時間
	static const double mu_r;     // 反発係数
	static const double dt;       // 単位は[sec]．この値は各自のPCのスペックに合わせて調整する．
	static const double table_w;  // テーブル横幅 [cm]
	static const double table_h;  // テーブル縦幅 [cm]
	static const double g;	      // 重力加速度 [cm/s^2]
	static const double mu_d;     // 動摩擦係数
	static const double v_min;	  // 速度の閾値
public:
	double pos[3];  // 位置
	double vel[3];  // 速度
	double acc[3];  // 加速度
	//vector< vector<double> > ball_val;
	static int count_line;
	BALL();
	~BALL();
	static void Set_ball_val();
	static void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
	static void make_billiards_wall();
	static void make_balls();
	static void File_output();
};

#endif // HEADER_H_
