#include<iostream>
#include<fstream>
#include<cmath>
#include "HistogramE.h"

int main(void) {
	// Histogram을 변환하기 위한 클래스 객체 생성
	HistogramE RV("./gray/barbara(512x512).raw", "./gray/gAirplane256_256.raw", 512, 512, 256, 256);

	RV.Load_input_target_img(); // 입력 이미지와 타겟 이미지를 읽고 각 이미지의 PDF를 저장함
	RV.print_distribution("input", "./input_dis_cdf.csv", "./input_dis_pdf.csv"); // 입력 이미지의 PDF, CDF를 저장
	RV.print_distribution("target", "./target_dis_cdf.csv", "./target_dis_pdf.csv"); // 타겟 이미지의 PDF, CDF를 저장

	RV.Craete_Trans_Table(); // 변환 테이블 생성
	RV.Transformation(); // 위의 변환 테이블을 가지고 입력 이미지를 변환하는 함수
	RV.print_distribution("result", "./result_dis_cdf.csv", "./result_dis_pdf.csv"); // 결과 이미지의 PDF, CDF를 저장
	RV.Store_Transformation_result(); // 결과 이미지를 raw 파일로 저장
	return 0; 
}