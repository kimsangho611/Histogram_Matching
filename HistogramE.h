#pragma once
#include<iostream>
#include<map>
#include<fstream>
#include<cmath>
using namespace std;

class HistogramE
{
private:
	const char* input_url, *target_url;
	map<int, int> input_distribution, target_distribution, result_distribution;
	unsigned char **input_image, **target_image, **result_image;
	int input_img_row, input_img_col, target_img_row, target_img_col;
	double input_cdf[256] = { 0, };
	int trans_list[256] = { 0, };

public:
	HistogramE(const char *input, const char *target, int input_row, int input_col, int target_row, int target_col) {
		input_url = input; target_url = target; // �Է� �̹��� �� Ÿ�� �̹����� url�� ����
		input_img_row = input_row; target_img_row = target_row; // �Է� �̹��� �� Ÿ�� �̹����� �� ������ ����
		input_img_col = input_col; target_img_col = target_col; // �Է� �̹��� �� Ÿ�� �̹����� �� ������ ����

		// �Ʒ��� �Է� �̹���, Ÿ�� �̹���, ����̹����� ������ 2���� �迭�� �̹��� ũ�⸸ŭ �����Ҵ���
		input_image = new unsigned char* [input_row];
		result_image = new unsigned char* [input_row];
		for (int i = 0; i < input_row; i++) {
			input_image[i] = new unsigned char[input_col];
			result_image[i] = new unsigned char[input_col];
		}
		target_image = new unsigned char* [target_row];
		for (int i = 0; i < target_row; i++)
			target_image[i] = new unsigned char[target_col];
	};
	~HistogramE() {};

	void Load_input_target_img(); // �Է� �̹����� Ÿ�� �̹����� �а� PDF�� �����ϴ� �Լ�
	void Craete_Trans_Table(); // ��ȯ ���̺��� �����ϴ� �Լ�
	void Transformation(); // �Է��̹����� ��ȯ�ϴ� �Լ�

	void print_distribution(const char* type, const char* file_path1, const char* file_path2); // PDF, CDF�� ���Ϸ� �����ϴ� �Լ�
	void Store_Transformation_result(); // ��ȯ ����� raw ���Ϸ� �����ϴ� �Լ�
};

