#ifndef CLASS_H_
#define CLASS_H_

class BALL {
private:
	double m;      // ����
	double r;      // ���a
	double e;      // �e���W��
	float col[4];  // �F
	double pos[3];  // �ʒu
	double vel[3];  // ���x
	double acc[3];  // �����x
	static long int loopcounter;  // ���[�v����
	static const double mu_r;     // �����W��
	static const double dt;       // �P�ʂ�[sec]�D���̒l�͊e����PC�̃X�y�b�N�ɍ��킹�Ē�������D
	static const double table_w;  // �e�[�u������ [cm]
	static const double table_h;  // �e�[�u���c�� [cm]
	static const double g;	      // �d�͉����x [cm/s^2]
	static const double mu_d;     // �����C�W��
	static const double v_min;	  // ���x��臒l
public:
	BALL();
	~BALL();
	static void Set_ball();  // ���̏����l��ݒ肷��ÓI�����o�֐�
	static void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
	static void make_billiards_wall();  // �r�����[�h��̕ǂ�`���ÓI�����o�֐�
	static void make_balls();			// ����`���ÓI�����o�֐�
	static void File_input();			// �t�@�C�����狅�̏����l��ǂݍ��ސÓI�����o�֐�
	static void File_output();			// �t�@�C���ɋ��̈ʒu�⑬�x�Ȃǂ��o�͂���ÓI�����o�֐�
	static void ball_stop(int);			// ���̒�~�������s���ÓI�����o�֐�
	static void floor_friction(int);	// �����C�͂��v�Z����ÓI�����o�֐�
	static void collision_ball(int);	// �����m�̏Փˏ���������ÓI�����o�֐�
	static void collision_wall(int);	// �����ǂɏՓ˂������̏���������ÓI�����o�֐�
};

#endif // CLASS_H_
