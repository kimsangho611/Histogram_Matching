#include "HistogramE.h"

void HistogramE::Load_input_target_img() {
	FILE* input, *target;
	fopen_s(&input, input_url, "rb"); // 입력 이미지를 binary로 읽음
	fopen_s(&target, target_url, "rb"); // 타겟 이미지를 binary로 열음

	// 입력 이미지를 unsigned char 형식으로 문자 1개씩 읽고 이를 2차원 배열에 저장
	for (int i = 0; i < input_img_row; i++) {
		for (int j = 0; j < input_img_col; j++) {
			unsigned char a = fgetc(input);
			input_image[i][j] = a;
			++input_distribution[(int)a]; // PDF를 저장하는 input_distribution 변수
		}
	}
	// 타겟 이미지를 unsigned char 형식으로 문자 1개씩 읽고 이를 2차원 배열에 저장
	for (int i = 0; i < target_img_row; i++) {
		for (int j = 0; j < target_img_col; j++) {
			unsigned char a = fgetc(target);
			target_image[i][j] = a;
			++target_distribution[(int)a]; // PDF를 저장하는 target_distribution 변수
		}
	}
	fclose(input); // input 파일을 닫음
	fclose(target); // target 파일을 닫음
}

void HistogramE::Craete_Trans_Table() {
	for (auto k = input_distribution.begin(); k != input_distribution.end(); k++) {
		double icdf = input_cdf[k->first]; // input_distribution의 cdf를 저장
		double cdf = 0.0;
		int result_pixel;
		double result_cdf = INT_MAX;
		for (auto p = target_distribution.begin(); p != target_distribution.end(); p++) {
			cdf += (double)p->second / ((double)target_img_row * (double)target_img_col); // target distribution에서 cdf를 구함
			if (abs(icdf - cdf) < result_cdf) { // input_distribution의 cdf와 target distribution의 cdf 차가 가장 작으면 
				result_cdf = abs(icdf - cdf); result_pixel = p->first; // 현재 pixel 값을 저장
			}
		}
		trans_list[k->first] = result_pixel; // 변환결과를 저장하는 배열에 변환 결과 pixel 값을 저장
	}
}

void HistogramE::Transformation() {
	for (int i = 0; i < input_img_row; i++) { // 입력 이미지의 행 개수만큼 반복
		for (int j = 0; j < input_img_col; j++) { // 입력 이미지의 열 개수만큼 반복
			unsigned char im = input_image[i][j]; // 입력 이미지의 pixel 값을 저장
			int trans = trans_list[(int)im]; // 해당 pixel 값의 변환 값을 저장
			result_image[i][j] = (unsigned char)trans; // 해당 저장 값을 결과 이미지 배열에 저장
			++result_distribution[trans]; // 결과 이미지의 PDF 값을 저장
		}
	}
}

void HistogramE::print_distribution(const char* type, const char* cdf_file_path, const char* pdf_file_path) {
	int row, col;
	map<int, int> distribution;
	if (!strcmp(type, "input")) { // type이 input이면
		row = input_img_row; col = input_img_col;
		distribution = input_distribution; // input distribution을 사용
	}
	else if (!strcmp(type, "target")) { // type이 target이면 
		row = target_img_row; col = target_img_col;
		distribution = target_distribution; // target distribution을 사용
	}
	else if (!strcmp(type, "result")) { // type이 result이면 
		row = input_img_row; col = input_img_col;
		distribution = result_distribution; // result distribution을 사용
	}
	std::ofstream fout_pdf, fout_cdf;

	fout_pdf.open(pdf_file_path); // pdf file path에 파일을 open
	fout_cdf.open(cdf_file_path); // cdf file path에 파일을 open

	int rgb = -1;
	double cdf = 0.0;
	for (auto p = distribution.begin(); p != distribution.end(); p++) {
		// distribution에 저장한 첫번째 값이 0보다 큰 경우 그 값까지 파일에 저장
		while (rgb++ != p->first - 1) { 
			fout_pdf << rgb << "," << 0 << std::endl;
			fout_cdf << rgb << "," << cdf << std::endl;
		}
		cdf += (double)p->second / ((double)row * (double)col); // 이미지의 CDF를 계산
		fout_pdf << p->first << "," << p->second << std::endl; // 파일에 pdf값 출력
		fout_cdf << p->first << "," << cdf << std::endl; // 파일에 cdf 값 출력

		if (!strcmp(type, "input")) 
			input_cdf[p->first] = cdf; // input_cdf를 저장하는 배열에 cdf 값 저장
	}
	// 0~255까지의 값을 저장하기 위해 이미지의 distribution 저장 값이 255 보다 작으면 그 뒷 부분도 출력
	while (rgb++ != 255) {
		fout_pdf << rgb << "," << 0 << std::endl;
		fout_cdf << rgb << "," << cdf << std::endl;

		if (!strcmp(type, "input")) input_cdf[rgb] = cdf; // input_cdf를 저장하는 배열에 cdf 값 저장
	}
	fout_pdf.close(); // 파일을 닫음
	fout_cdf.close(); // 파일을 닫음
}

void HistogramE::Store_Transformation_result() {
	FILE* result;
	fopen_s(&result, "./result.raw", "wb"); // raw 파일을 저장하기 위해 binary write로 파일을 open

	// 결과 이미지를 저장한 2차원 배열에 있는 값을 하나씩 파일에 저장
	for (int i = 0; i < input_img_row; i++)
		for (int j = 0; j < input_img_col; j++)
			fputc(result_image[i][j], result);
	fclose(result); // 파일을 닫음
}