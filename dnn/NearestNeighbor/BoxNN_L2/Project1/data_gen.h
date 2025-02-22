#pragma once
#include<vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>


void generate_data(int d, int n, int m, std::string fname) {
	double max_bound = 20.;
	std::vector<std::vector<std::vector<int>>> Boxes;
	std::vector<std::vector<double>> sites;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> u_dis_double(0.,max_bound);
	int size_bnd = max_bound / 3;
	std::uniform_int_distribution<> u_dis_int(1, size_bnd);
	std::uniform_int_distribution<> u_dis_int2(0, max_bound - size_bnd);
	for (size_t i = 0; i < m; i++) {
		std::vector<std::vector<int>> temp_box;
		bool flag = false;
		do {
			temp_box.assign(2, std::vector<int>(d,0));
			for (size_t k = 0; k < d; k++) {
				int shift = u_dis_int2(gen);
				for (size_t j = 0; j < 2; j++) {
					temp_box[j][k] = u_dis_int(gen) + shift;
					while (j == 1 && temp_box[0][k] == temp_box[1][k]) {
						temp_box[j][k] = u_dis_int(gen) + shift;
					}
				}
				if (temp_box[0][k] > temp_box[1][k]) std::swap(temp_box[0][k], temp_box[1][k]);
			}
			for (auto box : Boxes) {
				bool flag1 = true;
				for (size_t j = 0; j < d; j++) {
					if (std::max(box[0][j], temp_box[0][j]) > std::min(box[1][j], temp_box[1][j])) {
						flag1 = false;
						break;
					}
				}
				if (flag1) {
					flag = flag1;
					break;
				}
			}
		} while (flag);
		Boxes.push_back(temp_box);
	}
	for (size_t i = 0; i < n + 1; i++) {
		std::vector<double> pt;
		bool flag = false;
		do {
			pt.clear();
			for (size_t j = 0; j < d; j++) {
				pt.push_back(u_dis_double(gen));
			}
			for (auto box : Boxes) {
				bool flag1 = true;
				for (size_t k = 0; k < d; k++) {
					if (box[0][k] > pt[k] || pt[k] > box[1][k]) {
						flag1 = false;
						break;
					}
				}
				if (flag1) {
					flag = flag1;
					break;
				}
			}
		} while (flag);
		sites.emplace_back(pt);
	}

	std::ofstream file(fname);
	file << n << " " << m << " " << d << std::endl;
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < d; j++) {
			file << sites[i][j] << " ";
		}
		file << std::endl;
	}
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < 2; j++) {
			for (size_t k = 0; k < d; k++) {
				file << Boxes[i][j][k] << " ";
			}
			file << std::endl;
		}
	}
	for (size_t j = 0; j < d; j++) {
		file << sites[n][j] << " ";
	}
	file << std::endl;
	file.close();
}