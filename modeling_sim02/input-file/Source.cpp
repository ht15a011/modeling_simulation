#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include<iterator>
using namespace std;

ifstream fin;

int main()
{
	string filename_input = "input.txt"; // 入力ファイル名
	int count_line = 10;
	vector< vector<double> > ball_val;

	ball_val = vector<vector<double>>(count_line, vector<double>(16, 0));

	// ファイルを開く
	fin.open(filename_input.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
		cin.get();
		return -1;
	}

	while (!fin.eof() && !fin.fail()) {
		double a[16];
		static int j = 0;

		fin >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]
			>> a[6] >> a[7] >> a[8] >> a[9] >> a[10] >> a[11]
			>> a[12] >> a[13] >> a[14] >> a[15];

		for (int i = 0; i < 16; i++) {
			ball_val[j][i] = a[i];
		}
		j++;
	}
	
	for (int i = 0; i < count_line; i++) {
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
			<< ball_val[i][15] << endl;
	}
	
	// ファイルを閉じる
	fin.close();

	cin.get();
	return 0;
}