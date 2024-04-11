#include <fstream>
#include <sstream>
#include <iostream>

#include "ObjLoader.h"

Object::Object(const std::string &filepath)
{
    std::ifstream stream(filepath);
    if (!stream) {
        std::cout << "failed to read file." << std::endl;
        return;
    }

    std::string line;
    while (getline(stream, line)) {

        std::istringstream iss(line);

        std::string lineHeader;
        iss >> lineHeader;
        
        if (lineHeader == "v") {

            float tmp;
            while (iss >> tmp)
                v_buffer.push_back(tmp);

        } else if (lineHeader == "vt") {

            float tmp;
            while (iss >> tmp)
                vt_buffer.push_back(tmp);
            
        } else if (lineHeader == "vn") {

            float tmp;
            while (iss >> tmp)
                vn_buffer.push_back(tmp);

        } else if (lineHeader == "f") {

            int tmp[12] = {0};
            char rawIndex[100];
            int i = 0;
            while (iss >> rawIndex) {
                sscanf(rawIndex, "%d/%d/%d", &tmp[i], &tmp[i+1], &tmp[i+2]);
                i += 3;
            }
            for (int i = 0; i < 12; i++)
                f_buffer.push_back(tmp[i]);
        }
    }
}

void Object::Show()
{
    std::cout << "index" << std::endl;
    for (int i = 0; i < f_buffer.size(); i += 12) {
        for (int j = i; j < 12; j += 3)
            std::cout << f_buffer[j] << "/" << f_buffer[j + 1] << "/" << f_buffer[j + 2] << " ";
        std::cout << '\n';
    }
}