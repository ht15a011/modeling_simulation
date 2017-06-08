#ifndef HEADER_H_
#define HEADER_H_

// �N���X��`��
class BALL {
private:
	double m;      // ����
	double r;      // ���a
	double e;      // �e���W��
	static const double table_w;  // �r�����[�h�̃e�[�u������ [cm]
	static const double table_h;  // �r�����[�h�̃e�[�u���c�� [cm]
	static const double dt;       // �P�ʂ�[sec]�D���̒l�͊e����PC�̃X�y�b�N�ɍ��킹�Ē�������D
	static const double mu_r;     // �����W��(0 < mu_r < 1)
	static long int loopcounter;  // ���[�v����
	static const double g;		  // �d�͉����x [cm/s^2]
	static const double mu_d;     // �����C�W��
	static const double v_min;			  // ���x��臒l
public:
	float col[4];  // �{�[���̐F
	double pos[3];  // �ʒu
	double vel[3];  // ���x
	double acc[3];  // �����x
	void ball_set();
	void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
};

#endif // HEADER_H_