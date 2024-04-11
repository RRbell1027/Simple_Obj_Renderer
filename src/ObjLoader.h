#pragma once

#include <string>
#include <vector>

class Object {
    public:
    Object(const std::string &filepath);
    void Show();

    std::vector<float> v_buffer;
    std::vector<float> vt_buffer;
    std::vector<float> vn_buffer;
    std::vector<int> f_buffer;
};
