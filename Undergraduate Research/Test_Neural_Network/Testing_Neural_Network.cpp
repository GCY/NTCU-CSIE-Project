// Testing_Neural_Network.cpp : 定義主控台應用程式的進入點。
#include <iostream>
#include <fstream>

#include "opennn.h"

int main(int argc, char* argv[])
{
	struct dataset_format{

		OpenNN::Vector<double> LElbow;
		OpenNN::Vector<double> RElbow;
		OpenNN::Vector<double> LShoulder;
		OpenNN::Vector<double> RShoulder;

		int totalCount;
		int correctCount;
		int wrongCount;
		int noneCount;
		double correctRate;
	};

	OpenNN::NeuralNetwork neural_network("data/neural_network.xml");
	OpenNN::Vector <dataset_format> test_data(9);
	OpenNN::Vector <double> output(9);

	for(int i=0; i<9; i++){
		test_data[i].totalCount=0;
		test_data[i].correctCount=0;
		test_data[i].wrongCount=0;
		test_data[i].noneCount=0;
		test_data[i].correctRate=0;
	}

	char* actionName[9] = {"雙手向下伸直",
						   "左手向上彎曲90度",
						   "右手向上彎曲90度",
						   "雙手向上彎曲90度",
						   "雙手向下彎曲90度",
						   "雙手水平伸直",
						   "雙手向上伸直",
						   "左上右下",
						   "右上左下"
	};

	double LShoulderTmp;
	double RShoulderTmp;
	double LElbowTmp;
	double RElbowTmp;

	bool actionNum[9];

	std::fstream file("data/dataset.dat", std::ios::in);
	std::fstream out("data/result.txt", std::ios::out); 

	if(!file)
	{
		std::cout << "無法開啟檔案" << std::endl;
		exit(1);
	}
	while(file >>LElbowTmp>>LShoulderTmp>>RElbowTmp>>RShoulderTmp
			   >>actionNum[0]>>actionNum[1]>>actionNum[2]>>actionNum[3]>>actionNum[4]
			   >>actionNum[5]>>actionNum[6]>>actionNum[7]>>actionNum[8])
	{
		for(int i=0; i<9; i++){
			if(actionNum[i]){
				// 總值
				test_data[i].LElbow.push_back(LElbowTmp);
				test_data[i].LShoulder.push_back(LShoulderTmp);
				test_data[i].RElbow.push_back(RElbowTmp);
				test_data[i].RShoulder.push_back(RShoulderTmp);
				// 資料筆數
				test_data[i].totalCount++;
			}
		}
	}
	
	for(int i=0; i<9; i++){
		int count = 0;
		for(int j=0; j<test_data[i].totalCount; j++){
			bool flag = true;
			count++;
			OpenNN::Vector<double> tmpInput(4);
			tmpInput[0] = test_data[i].LElbow[j];
			tmpInput[1] = test_data[i].LShoulder[j];
			tmpInput[2] = test_data[i].RElbow[j];
			tmpInput[3] = test_data[i].LShoulder[j];

			output = neural_network.calculate_outputs(tmpInput);
			for(int k=0; k<9; k++){
				if(std::floor(output[k]+0.5)==1){
					flag = false;
					std::cout << count << ": " << i << "=>" << k << std::endl;
					if(k==i){ // 計算結果正確
						test_data[i].correctCount++;
					}
					else{ // 計算結果錯誤
						test_data[i].wrongCount++;
					}
				}
			}
			if(flag){ // 無結果
				test_data[i].noneCount++;
			}
		}
		test_data[i].correctRate = ((double)test_data[i].correctCount/test_data[i].totalCount)*100;
	}

	for(int i=0; i<9; i++){
		out << "Action" << i << ": " << actionName[i] << std::endl
			<< "Total data count: " << test_data[i].totalCount << std::endl
			<< "Correct data count: " << test_data[i].correctCount << std::endl
			<< "Wrong data count: " << test_data[i].wrongCount << std::endl
			<< "Missing data count: " << test_data[i].noneCount << std::endl
			<< "Correct rate: " << test_data[i].correctRate << "%" << std::endl << std::endl;
	}


	return 0;
}

