#include<iostream>
#include<fstream>
#include<cmath>
#include "HistogramE.h"

int main(void) {
	// Histogram�� ��ȯ�ϱ� ���� Ŭ���� ��ü ����
	HistogramE RV("./gray/barbara(512x512).raw", "./gray/gAirplane256_256.raw", 512, 512, 256, 256);

	RV.Load_input_target_img(); // �Է� �̹����� Ÿ�� �̹����� �а� �� �̹����� PDF�� ������
	RV.print_distribution("input", "./input_dis_cdf.csv", "./input_dis_pdf.csv"); // �Է� �̹����� PDF, CDF�� ����
	RV.print_distribution("target", "./target_dis_cdf.csv", "./target_dis_pdf.csv"); // Ÿ�� �̹����� PDF, CDF�� ����

	RV.Craete_Trans_Table(); // ��ȯ ���̺� ����
	RV.Transformation(); // ���� ��ȯ ���̺��� ������ �Է� �̹����� ��ȯ�ϴ� �Լ�
	RV.print_distribution("result", "./result_dis_cdf.csv", "./result_dis_pdf.csv"); // ��� �̹����� PDF, CDF�� ����
	RV.Store_Transformation_result(); // ��� �̹����� raw ���Ϸ� ����
	return 0; 
}