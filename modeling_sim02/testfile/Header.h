#ifndef HEADER_H_
#define HEADER_H_

class BALL {
private:
	double m;      // ����
	double r;      // ���a
	double e;      // �e���W��
	float col[4];  // �F
	static long int loopcounter;  // ���[�v����
	static const double mu_r;     // �����W��
	static const double dt;       // �P�ʂ�[sec]�D���̒l�͊e����PC�̃X�y�b�N�ɍ��킹�Ē�������D
	static const double table_w;  // �e�[�u������ [cm]
	static const double table_h;  // �e�[�u���c�� [cm]
	static const double g;	      // �d�͉����x [cm/s^2]
	static const double mu_d;     // �����C�W��
	static const double v_min;	  // ���x��臒l
public:
	double pos[3];  // �ʒu
	double vel[3];  // ���x
	double acc[3];  // �����x
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
