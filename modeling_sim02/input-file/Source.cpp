#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ifstream fin;

int main()
{
	string filename_input = "./input.txt"; // ���̓t�@�C����
	double data[3];
	int count = 0;

	// �t�@�C�����J��
	fin.open(filename_input.c_str());
	if (!fin.is_open()) {
		cout << "fin error" << endl;
		cin.get();
		return -1;
	}

	while (!fin.eof() && !fin.fail()) {
		fin >> data[0] >> data[1] >> data[2]; // �t�@�C���ifin�j�����s�ǂݍ���ŁCdata[0], data[1], data[2]�ɑ������D
		cout << count << " " << data[0] << " " << data[1] << " " << data[2] << endl;
		count++;
	}

	// �t�@�C�������
	fin.close();

	cin.get();
	return 0;
}