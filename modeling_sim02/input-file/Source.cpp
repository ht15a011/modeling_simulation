#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ifstream fin;

int main()
{
	string filename_input = "./input.txt"; // 入力ファイル名
	double data[3];
	int count = 0;

	// ファイルを開く
	fin.open(filename_input.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
		cin.get();
		return -1;
	}

	while (!fin.eof() && !fin.fail()) {
		fin >> data[0] >> data[1] >> data[2]; // ファイル（fin）から一行読み込んで，data[0], data[1], data[2]に代入する．
		cout << count << " " << data[0] << " " << data[1] << " " << data[2] << endl;
		count++;
	}

	// ファイルを閉じる
	fin.close();

	cin.get();
	return 0;
}