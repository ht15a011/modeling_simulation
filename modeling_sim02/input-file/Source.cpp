#include <iostream>
#include <fstream>
#include <string>
#include<vector>
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
		for (int i = 0; i < count_line; i++) {
			for (int j = 0; i < 16; j++) {
				fin >> ball_val[i][j];
			}
		}
	}

	// �t�@�C�������
	fin.close();

	cin.get();
	return 0;
}