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
		input_url = input; target_url = target; // 입력 이미지 및 타겟 이미지의 url을 저장
		input_img_row = input_row; target_img_row = target_row; // 입력 이미지 및 타겟 이미지의 행 개수를 저장
		input_img_col = input_col; target_img_col = target_col; // 입력 이미지 및 타겟 이미지의 열 개수를 저장

		// 아래는 입력 이미지, 타겟 이미지, 결과이미지를 저장할 2차원 배열을 이미지 크기만큼 동적할당함
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

	void Load_input_target_img(); // 입력 이미지와 타겟 이미지를 읽고 PDF를 저장하는 함수
	void Craete_Trans_Table(); // 변환 테이블을 생성하는 함수
	void Transformation(); // 입력이미지를 변환하는 함수

	void print_distribution(const char* type, const char* file_path1, const char* file_path2); // PDF, CDF를 파일로 저장하는 함수
	void Store_Transformation_result(); // 변환 결과를 raw 파일로 저장하는 함수
};

