#include "HistogramE.h"

void HistogramE::Load_input_target_img() {
	FILE* input, *target;
	fopen_s(&input, input_url, "rb"); // �Է� �̹����� binary�� ����
	fopen_s(&target, target_url, "rb"); // Ÿ�� �̹����� binary�� ����

	// �Է� �̹����� unsigned char �������� ���� 1���� �а� �̸� 2���� �迭�� ����
	for (int i = 0; i < input_img_row; i++) {
		for (int j = 0; j < input_img_col; j++) {
			unsigned char a = fgetc(input);
			input_image[i][j] = a;
			++input_distribution[(int)a]; // PDF�� �����ϴ� input_distribution ����
		}
	}
	// Ÿ�� �̹����� unsigned char �������� ���� 1���� �а� �̸� 2���� �迭�� ����
	for (int i = 0; i < target_img_row; i++) {
		for (int j = 0; j < target_img_col; j++) {
			unsigned char a = fgetc(target);
			target_image[i][j] = a;
			++target_distribution[(int)a]; // PDF�� �����ϴ� target_distribution ����
		}
	}
	fclose(input); // input ������ ����
	fclose(target); // target ������ ����
}

void HistogramE::Craete_Trans_Table() {
	for (auto k = input_distribution.begin(); k != input_distribution.end(); k++) {
		double icdf = input_cdf[k->first]; // input_distribution�� cdf�� ����
		double cdf = 0.0;
		int result_pixel;
		double result_cdf = INT_MAX;
		for (auto p = target_distribution.begin(); p != target_distribution.end(); p++) {
			cdf += (double)p->second / ((double)target_img_row * (double)target_img_col); // target distribution���� cdf�� ����
			if (abs(icdf - cdf) < result_cdf) { // input_distribution�� cdf�� target distribution�� cdf ���� ���� ������ 
				result_cdf = abs(icdf - cdf); result_pixel = p->first; // ���� pixel ���� ����
			}
		}
		trans_list[k->first] = result_pixel; // ��ȯ����� �����ϴ� �迭�� ��ȯ ��� pixel ���� ����
	}
}

void HistogramE::Transformation() {
	for (int i = 0; i < input_img_row; i++) { // �Է� �̹����� �� ������ŭ �ݺ�
		for (int j = 0; j < input_img_col; j++) { // �Է� �̹����� �� ������ŭ �ݺ�
			unsigned char im = input_image[i][j]; // �Է� �̹����� pixel ���� ����
			int trans = trans_list[(int)im]; // �ش� pixel ���� ��ȯ ���� ����
			result_image[i][j] = (unsigned char)trans; // �ش� ���� ���� ��� �̹��� �迭�� ����
			++result_distribution[trans]; // ��� �̹����� PDF ���� ����
		}
	}
}

void HistogramE::print_distribution(const char* type, const char* cdf_file_path, const char* pdf_file_path) {
	int row, col;
	map<int, int> distribution;
	if (!strcmp(type, "input")) { // type�� input�̸�
		row = input_img_row; col = input_img_col;
		distribution = input_distribution; // input distribution�� ���
	}
	else if (!strcmp(type, "target")) { // type�� target�̸� 
		row = target_img_row; col = target_img_col;
		distribution = target_distribution; // target distribution�� ���
	}
	else if (!strcmp(type, "result")) { // type�� result�̸� 
		row = input_img_row; col = input_img_col;
		distribution = result_distribution; // result distribution�� ���
	}
	std::ofstream fout_pdf, fout_cdf;

	fout_pdf.open(pdf_file_path); // pdf file path�� ������ open
	fout_cdf.open(cdf_file_path); // cdf file path�� ������ open

	int rgb = -1;
	double cdf = 0.0;
	for (auto p = distribution.begin(); p != distribution.end(); p++) {
		// distribution�� ������ ù��° ���� 0���� ū ��� �� ������ ���Ͽ� ����
		while (rgb++ != p->first - 1) { 
			fout_pdf << rgb << "," << 0 << std::endl;
			fout_cdf << rgb << "," << cdf << std::endl;
		}
		cdf += (double)p->second / ((double)row * (double)col); // �̹����� CDF�� ���
		fout_pdf << p->first << "," << p->second << std::endl; // ���Ͽ� pdf�� ���
		fout_cdf << p->first << "," << cdf << std::endl; // ���Ͽ� cdf �� ���

		if (!strcmp(type, "input")) 
			input_cdf[p->first] = cdf; // input_cdf�� �����ϴ� �迭�� cdf �� ����
	}
	// 0~255������ ���� �����ϱ� ���� �̹����� distribution ���� ���� 255 ���� ������ �� �� �κе� ���
	while (rgb++ != 255) {
		fout_pdf << rgb << "," << 0 << std::endl;
		fout_cdf << rgb << "," << cdf << std::endl;

		if (!strcmp(type, "input")) input_cdf[rgb] = cdf; // input_cdf�� �����ϴ� �迭�� cdf �� ����
	}
	fout_pdf.close(); // ������ ����
	fout_cdf.close(); // ������ ����
}

void HistogramE::Store_Transformation_result() {
	FILE* result;
	fopen_s(&result, "./result.raw", "wb"); // raw ������ �����ϱ� ���� binary write�� ������ open

	// ��� �̹����� ������ 2���� �迭�� �ִ� ���� �ϳ��� ���Ͽ� ����
	for (int i = 0; i < input_img_row; i++)
		for (int j = 0; j < input_img_col; j++)
			fputc(result_image[i][j], result);
	fclose(result); // ������ ����
}