# Dữ liệu bài toán tối ưu cấu hình PC

Toàn bộ số liệu giá và điểm hiệu năng trong thư mục này là **số liệu thật**, lấy trực tiếp
từ trang sản phẩm và trang benchmark. Ngày truy cập: **2026-11-17**.

Mỗi linh kiện có một dòng trong `sources.csv` chứa link để đối chiếu.
Tổng: **50 linh kiện (10 mỗi nhóm), 70 link nguồn**.

## 1. Cấu trúc

| File | Nội dung |
|---|---|
| `cpu.csv`, `vga.csv`, `ram.csv`, `mb.csv`, `ssd.csv` | Dữ liệu cho thuật toán: giá, công suất, điểm, loại RAM. Mỗi file 10 dòng |
| `sources.csv` | Sổ nguồn: 70 dòng, mỗi dòng 1 link + giá trị đã ghi + ngày truy cập |
| `demo_75rows.csv` | Dữ liệu **minh hoạ** (`data_type=demo`, nguồn `MINH-HOA`). Chỉ dùng test thuật toán, **không dùng trong báo cáo thị trường** |

Cột của 5 file nhóm (11 cột, code đọc 10 cột đầu):

```
group;id;name;price_vnd;power_w;score_raw;ram_type;benchmark;source;date;note
```

## 2. Nguồn dữ liệu

| Nhóm | Giá | Điểm hiệu năng | Công suất |
|---|---|---|---|
| CPU | GearVN | PassMark CPU Mark | TDP theo trang GearVN (Intel công bố) |
| VGA | GearVN | PassMark G3D Mark | Max TDP theo PassMark |
| RAM | GearVN | Công thức bandwidth | **Giả định 5 W/thanh** (kit 2 thanh = 10 W) |
| MB | GearVN | Không có (trọng số 0) | **Giả định 30 W** |
| SSD | GearVN | Tốc độ đọc tuần tự (MB/s) | **Giả định 5 W** |

Băng thông RAM: `MT/s × 8 byte × số kênh`. Đây là **công thức**, không phải kết quả đo,
nên không được ghi là "benchmark".

Ba chỗ **giả định** (RAM, MB, SSD) phải ghi rõ trong báo cáo: các trang bán lẻ và trang
benchmark không công bố mức tiêu thụ của ba nhóm này. Vì công suất trong cùng một nhóm là
như nhau (trừ kit 2 thanh RAM), giá trị giả định chỉ làm dịch tổng công suất, **không làm
sai thứ tự lựa chọn** giữa các linh kiện trong cùng nhóm.

## 3. Mô hình tính điểm

```
score_norm = 10000 × score_raw / max(score_raw của nhóm)
perf       = round(trọng số × score_norm × 100)
P100       = ngưỡng hiệu năng × 100
```

Trọng số (Mainboard = 0, 4 nhóm còn lại chuẩn hoá lại cho tổng = 1):

| Nhóm | Trọng số cũ | Trọng số dùng |
|---|---|---|
| CPU | 0,25 | 0,2632 |
| VGA | 0,45 | 0,4737 |
| RAM | 0,10 | 0,1053 |
| MB | 0,05 | **0** |
| SSD | 0,15 | 0,1579 |

Điểm tối đa lý thuyết = 1.000.100 (do làm tròn từng nhóm; xấp xỉ 100%).

### Điểm `perf` của từng linh kiện (để đối chiếu)

| CPU | perf | VGA | perf | RAM | perf | SSD | perf |
|---|---|---|---|---|---|---|---|
| C1 | 70.746 | V1 | 124.394 | R1 | 28.080 | S1 | 52.987 |
| C2 | 74.307 | V2 | 200.214 | R2 | 105.300 | S2 | 75.771 |
| C3 | 98.996 | V3 | 237.823 | R3 | 52.650 | S3 | 78.950 |
| C4 | 126.140 | V4 | 169.898 | R4 | 52.650 | S4 | 63.584 |
| C5 | 128.742 | V5 | 259.895 | R5 | 28.080 | S5 | 78.420 |
| C6 | 98.996 | V6 | 308.168 | R6 | 28.080 | S6 | 38.150 |
| C7 | 154.385 | V7 | 344.496 | R7 | 49.140 | S7 | 155.781 |
| C8 | 193.798 | V8 | 325.589 | R8 | 31.590 | S8 | 74.181 |
| C9 | 194.442 | V9 | 473.700 | R9 | 105.300 | S9 | 78.950 |
| C10 | 263.200 | V10 | 453.812 | R10 | 105.300 | S10 | 157.900 |

Mainboard M1–M10: perf = 0.

## 4. Tham số

| Tham số | Giá trị | Ý nghĩa |
|---|---|---|
| `W` | 300 W | Trần **tổng công suất 5 linh kiện** (CPU + VGA + RAM + MB + SSD) |
| `P100` | 700.000 | Hiệu năng tối thiểu, tương đương 70% |

`W` và `P100` là **tham số kịch bản**, không phải số liệu thị trường.

### W = 300 W ứng với bộ nguồn nào

`W` **không phải** công suất định mức của nguồn. Nó là trần công suất của 5 linh kiện.
Phần dư còn lại dành cho quạt, ổ phụ, đèn, màn hình cắm thêm, và hao hụt của nguồn.

| Nguồn | Cấp liên tục (~80%) | Trừ phần còn lại | Ngân sách 5 linh kiện |
|---|---|---|---|
| 450 W (case mini, SFX) | 360 W | −60 W | **~300 W** ← dùng trong báo cáo |
| 550 W (NVIDIA khuyến nghị cho RTX 4060) | 440 W | −70 W | ~370 W |
| 700 W | 560 W | −80 W | ~480 W |

Tổng công suất của 4 mốc cấu hình trong bộ dữ liệu:

| Cấu hình | CPU | VGA | RAM+MB+SSD | Tổng |
|---|---|---|---|---|
| C1 + V1 + R6 + M7 + S6 (rẻ nhất) | 58 | 75 | 40 | 173 W |
| C8 + V6 + R4 + M3 + S7 (tối ưu) | 125 | 115 | 40 | **280 W** |
| C10 + V6 + R2 + M3 + S10 (mạnh nhất trong W) | 125 | 115 | 45 | 285 W |
| C8 + V10 + R2 + M3 + S1 (nặng nhất) | 125 | 250 | 45 | 420 W |

Nguồn 700 W gánh cấu hình nặng nhất (420 W) chạy 60% tải — dư nhiều.

## 5. Kết quả tham chiếu (để đối chiếu khi chạy code)

Tổng tổ hợp duyệt: `10 × 10 × 10 × 10 × 10 = 100.000`, trong đó **48.000** hợp lệ vì
có 48 cặp RAM–MB cùng loại (24 cặp DDR4 + 24 cặp DDR5), mỗi cặp × 10 CPU × 10 VGA × 10 SSD.

Số cấu hình thỏa `W ≤ 300`: **33.720**. Số cấu hình thỏa cả `W` và `P100 = 700.000`: **612**.

| Trường hợp | Cấu hình | Giá | Công suất | perf |
|---|---|---|---|---|
| Rẻ nhất (bỏ qua P100) | C1 + V1 + R6 + M7 + S6 | 12.750.000 | 173 W | 261.370 |
| **Tối ưu (đạt P100)** | **C8 + V6 + R4 + M3 + S7** | **28.150.000** | **280 W** | **710.397** |
| Mạnh nhất trong W | C10 + V6 + R2 + M3 + S10 | 55.750.000 | 285 W | 834.568 |
| Mạnh nhất bỏ qua W | C10 + V9 + R2 + M3 + S10 | 67.250.000 | 390 W | 1.000.100 |

Nhận xét dùng được trong báo cáo:

- Cấu hình tối ưu chọn **i5-14600KF (C8) chứ không phải i5-14600K (C9)**: C8 rẻ hơn
  500.000 mà CPU Mark chỉ thấp hơn 127 điểm (0,3%).
- Chọn **RTX 4060 (V6) chứ không phải RTX 5060 (V8)**: V8 đắt hơn 6 triệu nhưng
  G3D Mark chỉ cao hơn 1.101 điểm (5,7%) — không đáng trong ngân sách 28 triệu.
- Chọn **RAM DDR5 16GB (R4)**: rẻ nhất trong nhóm DDR5, cùng bandwidth với R3 đắt gấp 3.
- Chọn **SSD Gen5 (S7)**: đọc 14.700 MB/s, cùng giá với các SSD Gen4 chậm hơn một nửa.

### So sánh 2 kịch bản công suất

| Ngưỡng hiệu năng | W = 300 (nguồn 450 W) | W = 480 (nguồn 700 W) |
|---|---|---|
| P100 = 700.000 (70%) | 28,15M — C8+V6+R4+M3+S7 | 28,15M — **giống nhau** |
| P100 = 800.000 (80%) | 45,45M — C10+V6+R2+M3+S7 | 36,35M — C4+V9+R4+M3+S7 |
| P100 = 850.000 (85%) | **không có nghiệm** | 38,85M — C8+V9+R6+M7+S7 |

Hai kết luận dùng được trong báo cáo:

1. Ngưỡng 70%: công suất **không ảnh hưởng** kết quả — cùng một cấu hình, cùng một giá.
2. Ngưỡng 80–85%: nguồn nhỏ **buộc đổi cấu hình hoặc vô nghiệm**, chênh 9,1 triệu
   (80%) và mất hoàn toàn khả thi (85%). Đây là bằng chứng ràng buộc công suất có tác dụng thật.

Lệnh chạy lại kịch bản thứ hai:

```
pc_main.exe 480 850000 data
```

## 6. Lưu ý khi bảo vệ

- Giá GearVN là giá đang bán, **thay đổi theo ngày**; đã ghi ngày truy cập cho từng dòng.
- CPU dùng **TDP trên trang GearVN** (giá trị Intel công bố), không dùng `TDP Up`.
  PassMark ghi i5-13400F tới 148 W (PL2) — nếu dùng PL2 thì kết quả tối ưu sẽ khác.
  Đã chọn một nguồn công suất duy nhất cho toàn bộ CPU để tránh trộn hai thang đo.
- Mainboard không tạo FPS trực tiếp nên trọng số = 0; nó chỉ tham gia
  ràng buộc `RAM.ramType == MB.ramType` và giá.
- SSD dùng tốc độ đọc tuần tự do hãng công bố trên trang GearVN, không phải điểm benchmark độc lập.
- RAM dùng bandwidth lý thuyết, không phải latency đo được. Hai thanh 16GB cho bandwidth
  gấp đôi một thanh vì chạy dual-channel.
- Công suất RAM/MB/SSD là **giả định mô hình**, không phải số liệu hãng.
- Chỉ chọn **CPU Intel LGA1700** và **mainboard B660/B760** để mọi tổ hợp CPU–MB đều
  lắp được; ràng buộc socket vì thế không cần đưa vào mô hình. Nếu trộn AMD thì phải
  thêm ràng buộc socket.
- Bộ 10 lựa chọn/nhóm đủ cho phần thực nghiệm. Nếu muốn test quy mô lớn hơn
  (n = 20–50) thì sinh dữ liệu `generated`, không lấy giá giả ghi là giá thật.

## 7. Cách chạy

```
g++ -std=c++17 -O2 pc_main.cpp -o pc_main.exe
pc_main.exe 300 700000 data
pc_main.exe 300 700000 data --list
```

Tham số: `W`, `P100`, thư mục dữ liệu. Bỏ trống thì dùng mặc định trong `pc_model.h`.
