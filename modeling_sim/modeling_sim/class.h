#ifndef CLASS_H_
#define CLASS_H_

class BALL {
private:
	double m;      // 質量
	double r;      // 半径
	double e;      // 弾性係数
	float col[4];  // 色
	double pos[3];  // 位置
	double vel[3];  // 速度
	double acc[3];  // 加速度
	static long int loopcounter;  // ループ時間
	static const double mu_r;     // 反発係数
	static const double dt;       // 単位は[sec]．この値は各自のPCのスペックに合わせて調整する．
	static const double table_w;  // テーブル横幅 [cm]
	static const double table_h;  // テーブル縦幅 [cm]
	static const double g;	      // 重力加速度 [cm/s^2]
	static const double mu_d;     // 動摩擦係数
	static const double v_min;	  // 速度の閾値
public:
	BALL();
	~BALL();
	static void Set_ball();  // 球の初期値を設定する静的メンバ関数
	static void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
	static void make_billiards_wall();  // ビリヤード台の壁を描く静的メンバ関数
	static void make_balls();			// 球を描く静的メンバ関数
	static void File_input();			// ファイルから球の初期値を読み込む静的メンバ関数
	static void File_output();			// ファイルに球の位置や速度などを出力する静的メンバ関数
	static void ball_stop(int);			// 球の停止処理を行う静的メンバ関数
	static void floor_friction(int);	// 動摩擦力を計算する静的メンバ関数
	static void collision_ball(int);	// 球同士の衝突処理をする静的メンバ関数
	static void collision_wall(int);	// 球が壁に衝突した時の処理をする静的メンバ関数
};

#endif // CLASS_H_
