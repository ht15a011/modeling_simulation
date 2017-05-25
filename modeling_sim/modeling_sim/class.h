#ifndef CLASS_H_
#define CLASS_H_

class BALL {
private:
	double m; // ����
	double r; // ���a
	double e; // �e���W��
	float col[4]; // �F
	static const double table_w; // �r�����[�h�̃e�[�u������ [cm]
	static const double table_h; // �r�����[�h�̃e�[�u���c�� [cm]
	static const double dt;  // �P�ʂ�[sec]�D���̒l�͊e����PC�̃X�y�b�N�ɍ��킹�Ē�������D
	static const double mu_r;  // �����W��(0 < mu_r < 1)
	static long int loopcounter;
public:
	double pos[3]; // �ʒu
	double vel[3]; // ���x
	double acc[3]; // �����x
	void ball_set();
	void init();
	static void idle();
	static void resize(int, int);
	static void keyboard(unsigned char, int, int);
	static void display();
};

#endif // CLASS_H_
