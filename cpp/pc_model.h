#pragma once

#include <string>
#include <vector>

struct LinhKien {
    std::string ma, ten, loaiRam;
    long long gia = 0;
    int congSuat = 0;
    double hieuNang = 0; // Thang diem 0 - 100%
};

void docDuLieu(const std::string& duongDan);
void vetCan();
void nhanhCan();
