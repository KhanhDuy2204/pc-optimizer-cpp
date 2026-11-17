#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "pc_model.h"

using namespace std;

const long long VO_CUNG = 1000000000000000000LL;
const char* TEN_NHOM[5] = {"CPU", "VGA", "RAM", "MB", "SSD"};
const double TRONG_SO[5] = {0.2632, 0.4737, 0.1053, 0.0, 0.1579};

vector<LinhKien> ds[5]; // 0: CPU, 1: VGA, 2: RAM, 3: MB, 4: SSD
int congSuatMax = 300;
double hieuNangMin = 70.0;

// Bien luu ket qua tot nhat
long long giaTotNhat;
int csTotNhat, chonTotNhat[5], dangChon[5];
double hnTotNhat;
long long demBuoc;

// Can hau to cho Nhanh can
long long giaMinSau[6];
int csMinSau[6];
double hnMaxSau[6];

int timNhom(const string& ten) {
    for (int g = 0; g < 5; g++)
        if (ten == TEN_NHOM[g]) return g;
    return -1;
}

bool reHon(const LinhKien& a, const LinhKien& b) {
    return a.gia < b.gia;
}

void docMotFile(const string& duongDanFile) {
    ifstream fin(duongDanFile);
    if (!fin) {
        cout << "Khong mo duoc file: " << duongDanFile << "\n";
        return;
    }

    string dong;
    getline(fin, dong); // Bo qua dong tieu de
    while (getline(fin, dong)) {
        if (dong.empty()) continue;
        stringstream ss(dong);
        string cot[7];
        for (int i = 0; i < 7; i++) getline(ss, cot[i], ';');

        int g = timNhom(cot[0]);
        if (g == -1 || cot[3].empty()) continue;

        LinhKien lk;
        lk.ma       = cot[1];
        lk.ten      = cot[2];
        lk.gia      = stoll(cot[3]);
        lk.congSuat = stoi(cot[4]);
        lk.hieuNang = stod(cot[5]); // Tam luu diem goc, se chuan hoa ben duoi
        lk.loaiRam  = cot[6];
        ds[g].push_back(lk);
    }
}

void docDuLieu(const string& duongDan) {
    if (duongDan.size() >= 4 && duongDan.substr(duongDan.size() - 4) == ".csv") {
        docMotFile(duongDan);
    } else {
        const char* tenFile[5] = {"cpu.csv", "vga.csv", "ram.csv", "mb.csv", "ssd.csv"};
        for (int g = 0; g < 5; g++) docMotFile(duongDan + "/" + tenFile[g]);
    }

    // Chuan hoa hieu nang ve thang % va sap xep gia tang dan
    for (int g = 0; g < 5; g++) {
        double maxGoc = 0;
        for (int i = 0; i < (int)ds[g].size(); i++)
            if (ds[g][i].hieuNang > maxGoc) maxGoc = ds[g][i].hieuNang;

        for (int i = 0; i < (int)ds[g].size(); i++) {
            if (maxGoc > 0) ds[g][i].hieuNang = 100.0 * TRONG_SO[g] * ds[g][i].hieuNang / maxGoc;
            else ds[g][i].hieuNang = 0;
        }
        sort(ds[g].begin(), ds[g].end(), reHon);
    }
}

void inKetQua(const string& tieuDe, double thoiGianMs) {
    cout << "\n=== " << tieuDe << " ===\n";
    if (giaTotNhat == VO_CUNG) {
        cout << "Khong tim thay cau hinh hop le.\n";
    } else {
        for (int g = 0; g < 5; g++) {
            const LinhKien& lk = ds[g][chonTotNhat[g]];
            cout << "  " << TEN_NHOM[g] << ": " << lk.ma << " - " << lk.ten
                 << " (" << lk.gia << " VND, " << lk.congSuat << " W)\n";
        }
        cout << "Tong gia  : " << giaTotNhat << " VND\n";
        cout << "Cong suat : " << csTotNhat << " W / " << congSuatMax << " W\n";
        cout << "Hieu nang : " << hnTotNhat << "% / " << hieuNangMin << "%\n";
    }
    cout << "So buoc   : " << demBuoc << "\n";
    cout << "Thoi gian : " << thoiGianMs << " ms\n";
}

// ---------- THUAT TOAN 1: VET CAN ----------
void vetCan() {
    clock_t batDau = clock();
    giaTotNhat = VO_CUNG;
    demBuoc = 0;

    for (int i0 = 0; i0 < (int)ds[0].size(); i0++)
        for (int i1 = 0; i1 < (int)ds[1].size(); i1++)
            for (int i2 = 0; i2 < (int)ds[2].size(); i2++)
                for (int i3 = 0; i3 < (int)ds[3].size(); i3++)
                    for (int i4 = 0; i4 < (int)ds[4].size(); i4++) {
                        demBuoc++;
                        if (ds[2][i2].loaiRam != ds[3][i3].loaiRam) continue;

                        int cs = ds[0][i0].congSuat + ds[1][i1].congSuat + ds[2][i2].congSuat
                               + ds[3][i3].congSuat + ds[4][i4].congSuat;
                        if (cs > congSuatMax) continue;

                        double hn = ds[0][i0].hieuNang + ds[1][i1].hieuNang + ds[2][i2].hieuNang
                                  + ds[3][i3].hieuNang + ds[4][i4].hieuNang;
                        if (hn + 1e-9 < hieuNangMin) continue;

                        long long gia = ds[0][i0].gia + ds[1][i1].gia + ds[2][i2].gia
                                      + ds[3][i3].gia + ds[4][i4].gia;
                        if (gia < giaTotNhat) {
                            giaTotNhat = gia;
                            csTotNhat = cs;
                            hnTotNhat = hn;
                            chonTotNhat[0] = i0; chonTotNhat[1] = i1; chonTotNhat[2] = i2;
                            chonTotNhat[3] = i3; chonTotNhat[4] = i4;
                        }
                    }

    inKetQua("VET CAN", 1000.0 * (clock() - batDau) / CLOCKS_PER_SEC);
}

// ---------- THUAT TOAN 2: NHANH CAN ----------
void thu(int tang, long long gia, int cs, double hn, const string& loaiRam) {
    for (int i = 0; i < (int)ds[tang].size(); i++) {
        const LinhKien& x = ds[tang][i];
        demBuoc++;

        long long giaMoi = gia + x.gia;
        if (giaMoi + giaMinSau[tang + 1] >= giaTotNhat) break; // Da sap xep gia tang dan
        if (tang == 3 && x.loaiRam != loaiRam) continue;        // RAM va Mainboard phai cung loai

        int csMoi = cs + x.congSuat;
        if (csMoi + csMinSau[tang + 1] > congSuatMax) continue;

        double hnMoi = hn + x.hieuNang;
        if (hnMoi + hnMaxSau[tang + 1] + 1e-9 < hieuNangMin) continue;

        dangChon[tang] = i;
        if (tang == 4) {
            giaTotNhat = giaMoi;
            csTotNhat = csMoi;
            hnTotNhat = hnMoi;
            for (int g = 0; g < 5; g++) chonTotNhat[g] = dangChon[g];
        } else {
            thu(tang + 1, giaMoi, csMoi, hnMoi, (tang == 2) ? x.loaiRam : loaiRam);
        }
    }
}

void nhanhCan() {
    clock_t batDau = clock();
    giaTotNhat = VO_CUNG;
    demBuoc = 0;

    giaMinSau[5] = 0; csMinSau[5] = 0; hnMaxSau[5] = 0;
    for (int g = 4; g >= 0; g--) {
        long long gMin = VO_CUNG;
        int cMin = 1000000;
        double hMax = 0;
        for (int i = 0; i < (int)ds[g].size(); i++) {
            if (ds[g][i].gia < gMin) gMin = ds[g][i].gia;
            if (ds[g][i].congSuat < cMin) cMin = ds[g][i].congSuat;
            if (ds[g][i].hieuNang > hMax) hMax = ds[g][i].hieuNang;
        }
        giaMinSau[g] = giaMinSau[g + 1] + gMin;
        csMinSau[g]  = csMinSau[g + 1] + cMin;
        hnMaxSau[g]  = hnMaxSau[g + 1] + hMax;
    }

    thu(0, 0, 0, 0, "");
    inKetQua("NHANH CAN", 1000.0 * (clock() - batDau) / CLOCKS_PER_SEC);
}

void inDanhSach() {
    for (int g = 0; g < 5; g++) {
        cout << "\n[" << TEN_NHOM[g] << "] " << ds[g].size() << " lua chon\n";
        for (int i = 0; i < (int)ds[g].size(); i++) {
            const LinhKien& lk = ds[g][i];
            cout << "  " << lk.ma << " | " << lk.ten << " | " << lk.gia
                 << " VND | " << lk.congSuat << " W | " << lk.hieuNang << "%";
            if (!lk.loaiRam.empty()) cout << " | " << lk.loaiRam;
            cout << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    bool xemDanhSach = false;
    string thamSo[3];
    int dem = 0;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--list") xemDanhSach = true;
        else if (dem < 3) thamSo[dem++] = arg;
    }

    if (dem >= 1) congSuatMax = stoi(thamSo[0]);
    if (dem >= 2) hieuNangMin = stod(thamSo[1]);
    if (hieuNangMin > 100.0) hieuNangMin /= 10000.0; // Ho tro ca P100 (700000 = 70%)
    string duongDan = (dem >= 3) ? thamSo[2]
        : (ifstream("data/cpu.csv").good() ? "data" : "cpp/data");

    docDuLieu(duongDan);
    for (int g = 0; g < 5; g++) {
        if (ds[g].empty()) {
            cout << "Thieu du lieu nhom " << TEN_NHOM[g] << " tai: " << duongDan << "\n";
            return 1;
        }
    }

    cout << "CHON CAU HINH PC TOI UU (Cong suat <= " << congSuatMax
         << " W, Hieu nang >= " << hieuNangMin << "%)\n";
    if (xemDanhSach) inDanhSach();

    vetCan();
    long long giaVC = giaTotNhat;

    nhanhCan();
    long long giaNC = giaTotNhat;

    cout << "\nKiem chung: " << (giaVC == giaNC ? "KHOP 100%" : "KHONG KHOP") << "\n";
    return 0;
}
