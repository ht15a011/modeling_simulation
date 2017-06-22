#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<iterator>
using namespace std;

ifstream fin;
vector< vector<double> > ball_val;

int main()
{
	string filename_input = "./input.txt"; // ���̓t�@�C����
	int count_line = 10;

	// �t�@�C�����J��
	fin.open(filename_input.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
		cin.get();
		return -1;
	}

	ball_val.resize(count_line);
	for (int i = 0; i < count_line; i++) {
		ball_val[i].resize(16);
	}

	while (!fin.eof() && !fin.fail()) {
		vector<double> in;
		double a[16];

		fin >> a[0] >> a[1] >> a2 >> a3 >> a4 >> a5
			>> a6 >> a[7] >> a[8] >> a9 >> a10 >> a11 >> a12 >> a13 >> a14 >> a15;

		for (int i = 0; i < 16; i++) {
			in.push_back(a[i]);
		}

		//ball_val.pop_back(in);
	}
	/*
	for (int i = 0; i < ball_val.size(); i++) {
		cout << ball_val[i][0] << " "
			<< ball_val[i][1] << " "
			<< ball_val[i][2] << " "
			<< ball_val[i][3] << " "
			<< ball_val[i][4] << " "
			<< ball_val[i][5] << " "
			<< ball_val[i][6] << " "
			<< ball_val[i][7] << " "
			<< ball_val[i][8] << " "
			<< ball_val[i][9] << " "
			<< ball_val[i][10] << " "
			<< ball_val[i][11] << " "
			<< ball_val[i][12] << " "
			<< ball_val[i][13] << " "
			<< ball_val[i][14] << " "
			<< ball_val[i][15];
	}
	*/
	// �t�@�C�������
	fin.close();

	cin.get();
	return 0;
}