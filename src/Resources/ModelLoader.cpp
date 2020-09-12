#include "ModelLoader.h"

#include <fstream>
#include <sstream>

#include <iostream>

namespace glGame {

	void ModelLoader::loadModel(const char* filePath, std::vector<ModelData>* data, std::vector<unsigned int>* indicies) {
		loadObjModel(filePath, data, indicies);

	}

	void ModelLoader::loadObjModel(const char* filePath, std::vector<ModelData>* data, std::vector<unsigned int>* indicies) {
		std::ifstream file(filePath);

		if(!file.is_open()) {
			std::cout << "Could not open file: " << filePath << std::endl;
			return;
		}

		std::vector<ModelVec3> v;
		std::vector<ModelVec2> vt;
		std::vector<ModelVec3> vn;

		std::string line;
		while(std::getline(file, line)) {
			if(line[0] == 'v') {
				std::stringstream lineBuffer;
				lineBuffer << line;
				std::string number;
				std::getline(lineBuffer, number, ' '); // Jump over begining of line

				float lineNumbers[3];
				for(int i = 0; i < 3 && std::getline(lineBuffer, number, ' '); ++i) {
					lineNumbers[i] = std::stof(number);
				}

				if(line[1] == ' ') {
					ModelVec3 mv3(lineNumbers[0], lineNumbers[1], lineNumbers[2]);
					v.push_back(mv3);
				}
				else if(line[1] == 't') {
					ModelVec2 mv2(lineNumbers[0], lineNumbers[1]);
					vt.push_back(mv2);
				}
				else if(line[1] == 'n') {
					ModelVec3 mv3(lineNumbers[0], lineNumbers[1], lineNumbers[2]);
					vn.push_back(mv3);
				}
			}
			else if(line[0] == 'f') {
				std::stringstream lineBuffer;
				lineBuffer << line;
				std::string vertex;
				std::getline(lineBuffer, vertex, ' '); // Jump over begining of line

				for(int i = 0; i < 3; ++i) {
					std::getline(lineBuffer, vertex, ' ');
					
					std::stringstream vertexBuffer;
					vertexBuffer << vertex;
					std::string numberString;

					int numbers[3];
					for(int j = 0; j < 3; ++j) {
						std::getline(vertexBuffer, numberString, '/');

						bool isDigit = true;
						for(char& c : numberString) {
							if(!std::isdigit(c)) {
								isDigit = false;
								break;
							}
						}

						if(isDigit && numberString.size() > 0) {
							numbers[j] = std::stoi(numberString) - 1;
						}
						else {
							numbers[j] = 0;
						}
					}

					ModelVec3 modelVert = numbers[0] >=  v.size() ? ModelVec3(0.0, 0.0, 0.0) : v[numbers[0]];
					ModelVec2 modelTex  = numbers[1] >= vt.size() ? ModelVec2(0.0, 0.0)      : vt[numbers[1]];
					ModelVec3 modelNorm = numbers[2] >= vn.size() ? ModelVec3(0.0, 0.0, 0.0) : vn[numbers[2]];

					data->push_back(ModelData(modelVert, modelTex, modelNorm));
					//TODO parse indicies
				}
			}
		}

		file.close();
	}

}