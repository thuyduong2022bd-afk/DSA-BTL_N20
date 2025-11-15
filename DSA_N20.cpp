#include <iostream>
#include <ctime>
#include <string>
#include <cstdio>
#include <iomanip> 
#include <algorithm> 
#include <vector>
#include <fstream> 
#include <sstream> 

#include <windows.h> 
#include <limits> 

using namespace std;

#define MAX_LICHKHAM 1000
#define MAX_CHUYENKHOA 5

// =================================================================================
//                 CÁC HÀM TIỆN ÍCH GIAO DIỆN CONSOLE MỚI
// =================================================================================

// ---------------- GIAO DIỆN CONSOLE ----------------
// Màu sắc trong Windows Console: 0: Đen, 1: Xanh dương, 2: Xanh lá, 3: Xanh ngọc, 
// 4: Đỏ, 5: Tím, 6: Vàng, 7: Trắng (mặc định), 8: Xám đậm, 9: Xanh sáng, 
// 10: Xanh lá sáng, 11: Xanh ngọc sáng, 12: Đỏ sáng, 13: Hồng sáng, 14: Vàng sáng, 15: Trắng sáng.
void thay_doi_kich_thuoc(int chieu_rong, int chieu_cao) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT khu_vuc_hien_thi = {0, 0, static_cast<short>(chieu_rong - 1), static_cast<short>(chieu_cao - 1)};
    SetConsoleWindowInfo(hOut, TRUE, &khu_vuc_hien_thi);
}
void doi_mau(int mau) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mau);
}

// ---------------- HELPERS HIỂN THỊ ----------------
static const string PHAN_CACH = "-----------------------------------------------------------------------------------------------------------------------";

void in_phan_cach(int mau = 8) { // Mặc định màu Xám đậm
    doi_mau(mau);
    cout << PHAN_CACH << "\n";
    doi_mau(7); // Trở lại màu mặc định
}

void in_tieu_de(const string& tieu_de, int mau_khung = 10, int do_rong = 110) { 
    int do_dai_tieu_de = tieu_de.length();
    int so_dau_bang = do_rong - do_dai_tieu_de - 2;
    int bang_trai = so_dau_bang / 2;
    int bang_phai = so_dau_bang - bang_trai;
    
    doi_mau(mau_khung);
    cout << string(bang_trai, '=') << " " << tieu_de << " " << string(bang_phai, '=') << "\n";
    doi_mau(7); // Trở lại màu mặc định
}

// ---------------- TIỆN ÍCH TẠM DỪNG ----------------
void tam_dung() {
    doi_mau(11); // Màu Xanh ngọc sáng
    cout << "\n\nNhan Enter de tiep tuc...";
    doi_mau(7);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


// =================================================================================
//                            KHAI BÁO CẤU TRÚC
// =================================================================================

// Thông tin bác sĩ
struct BacSi {
    int maSo;
    string hoTen;
    string chuyenKhoa;   // ví dụ: "RANG"
    int caLamViec;    // 1: Sang, 2: Chieu, 3: Ca ngay
    int phongKham;
};

// Thông tin bệnh nhân
struct BenhNhan {
    string hoTen;
    long long cccd;
    int namSinh;
    string sdt; 
    string diaChi; 
};

// Thông tin đặt lịch
struct LichKham {
    BenhNhan benhnhan;
    string tenBenh;      
    int chuyenKhoaBenh; // Mã số chuyên khoa (1-5)
    BacSi bacsi;
    int ngay, thang, nam;
    int caKham;
};

struct Node {
    LichKham data;
    Node* next;
};

// Tạo node mới
Node* createNode(LichKham lk) {
    Node* p = new Node;
    p->data = lk;
    p->next = NULL;
    return p;
}

// =================================================================================
//                            CÁC HÀM HỖ TRỢ CHUNG
// =================================================================================

// Kiểm tra ngày có hợp lệ không (Giữ nguyên)
bool NgayHopLe(int d, int m, int y) {
    if (m < 1 || m > 12 || d < 1 || d> 31) 
        return false;

    int ngayTrongThang[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
        if (d > 29) 
            return false;
    } 
    else {
        if (d > ngayTrongThang[m-1]) 
            return false;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);
    int dHienTai = ltm->tm_mday;
    int mHienTai = ltm->tm_mon + 1;
    int yHienTai = ltm->tm_year + 1900;
    
    if (y < yHienTai) 
        return false;
    if (y == yHienTai && m < mHienTai) 
        return false;
    if (y == yHienTai && m == mHienTai && d < dHienTai) 
        return false;
    return true;
}

// Các hàm hỗ trợ in
void printBacSi(const BacSi& bs) {
    const char* tenCaLamViec[] = {"", "Sang", "Chieu", "Ca ngay"};

    doi_mau(14); // Màu Vàng sáng cho Mã số
    cout << "[" << bs.maSo << "] ";
    doi_mau(7); // Trắng
    cout << left << setw(20) << bs.hoTen
         << "| Khoa: " << left << setw(6) << bs.chuyenKhoa 
         << "| Ca: " << left << setw(8) << tenCaLamViec[bs.caLamViec]
         << "| P.Kham: ";
    doi_mau(11); // Xanh ngọc sáng cho Phòng khám
    cout << bs.phongKham << endl;
    doi_mau(7);
}

void printLichKham(const LichKham& lk, int index = -1) {
    if (index != -1) {
        doi_mau(14); // Vàng sáng cho STT
        cout << "[" << index << "] ";
        doi_mau(7);
    }
    cout << "Ngay: ";
    doi_mau(15); // Trắng sáng cho ngày
    cout << lk.ngay << "/" << lk.thang << "/" << lk.nam;
    doi_mau(7);
    cout << " | Ca: " << (lk.caKham == 1 ? "Sang" : "Chieu"); // Chuyển ca số thành chữ
    cout << " | BS: ";
    doi_mau(9); // Xanh dương sáng cho tên BS
    cout << lk.bacsi.hoTen;
    doi_mau(7);
    cout << " (Ma: " << lk.bacsi.maSo << ")"
         << " | Khoa: " << lk.chuyenKhoaBenh
         << " | Benh: ";
    doi_mau(13); // Hồng sáng cho Tên bệnh
    cout << lk.tenBenh;
    doi_mau(7);
    cout << " | P.Kham: " << lk.bacsi.phongKham
         << endl; 
}

void printChiTietLichKham(const LichKham& lk) {
    in_phan_cach(15); // Trắng sáng
    doi_mau(10); // Xanh lá sáng
    cout << "THONG TIN DAT LICH KHAM" << endl;
    doi_mau(7);
    cout << "  - Ngay kham: ";
    doi_mau(15);
    cout << lk.ngay << "/" << lk.thang << "/" << lk.nam;
    doi_mau(7);
    cout << " | Ca kham: " << (lk.caKham == 1 ? "Sang" : "Chieu") << endl;
    
    doi_mau(14); // Vàng sáng
    cout << "  - Thong tin Benh nhan:" << endl;
    doi_mau(7);
    cout << "    | Ho ten: " << lk.benhnhan.hoTen 
         << " | Nam sinh: " << (lk.benhnhan.namSinh) << endl;
    cout << "    | CCCD: " << lk.benhnhan.cccd 
         << " | SDT: " << lk.benhnhan.sdt 
         << " | Dia chi: " << lk.benhnhan.diaChi << endl;
    
    doi_mau(9); // Xanh dương sáng
    cout << "  - Thong tin Bac si:" << endl;
    doi_mau(7);
    cout << "    | Ho ten: " << lk.bacsi.hoTen 
         << " | Ma so: " << lk.bacsi.maSo 
         << " | Phong kham: " << lk.bacsi.phongKham << endl;
    
    doi_mau(13); // Hồng sáng
    cout << "  - Thong tin Benh/Chuyen khoa:" << endl;
    doi_mau(7);
    cout << "    | Ten benh: " << lk.tenBenh             
         << " | Ma chuyen khoa: " << lk.chuyenKhoaBenh << endl;
    in_phan_cach(15); // Trắng sáng
}

// Hàm mới: Gợi ý tên bệnh
void printDiseaseSuggestions(int maKhoa) {
    doi_mau(11); // Xanh ngọc sáng
    cout << "  [GOI Y]: ";
    doi_mau(7);
    switch (maKhoa) {
        case 1: 
            cout << "RANG: Dau rang, Viem nuou, Tay trang, Nho rang khon, Chua tuy...\n"; 
            break;
        case 2: 
            cout << "MAT: Dau mat do, Can thi, Viem ket mac, Mo duc thuy tinh the...\n";
            break;
        case 3: 
            cout << "NOI: Dau da day, Tieu duong, Huyet ap cao, Benh tim mach...\n"; 
            break;
        case 4: 
            cout << "NHI: Sot, Ho, Cum, Benh tay chan mieng, Viem phoi...\n"; 
            break;
        case 5: 
            cout << "NGOAI: Chan thuong, Viem ruot thua, Thoat vi ben, Buou co...\n"; 
            break;
        default: 
            cout << "\n";
            break;
    }
}


class QuanLyLichKham {
public:

    Node* head;
    Node* tail;
    QuanLyLichKham() : head(NULL), tail(NULL) {}
    
    // Hàm hủy để giải phóng bộ nhớ
    ~QuanLyLichKham() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = NULL;
    }

    int demSoLich(int maSo, int ngay, int thang, int nam, int caKham) {
        int dem = 0;
        Node* p = head;
        while (p) {
            if (
                p->data.bacsi.maSo == maSo &&
                p->data.ngay == ngay &&
                p->data.thang == thang &&
                p->data.nam == nam &&
                p->data.caKham == caKham
            ) 
            dem++;
            p = p->next;
        }
        return dem;
    }

    // Khai báo các phương thức mới cho File
    void luuFile(); 
    void docFile(); 
    vector<BenhNhan> getDanhSachBenhNhan();
    void luuFileBenhNhan(); 

    // Các hàm khác
    void datLich(const vector<BacSi>& dsBacSi, int soBacSi);
    bool Empty();
    void huyLich(const vector<BacSi>& dsBacSi, int soBacSi);
    void suaLich(const vector<BacSi>& dsBacSi, int soBacSi);
    void timKiemLichKhamTheoCCCD(long long cccd);
    void hienThiTatCa();

    // Hàm mới: xóa lịch đã qua
    void xoaLichQuaKhu();
};

// =================================================================================
//       ĐỊNH NGHĨA CÁC HÀM XỬ LÝ FILE CHO LỊCH KHÁM (LINKED LIST)
// =================================================================================

void QuanLyLichKham::luuFile() {
    ofstream file("LichKham.txt");
    if (!file.is_open()) {
        doi_mau(12); // Đỏ sáng
        cout << "[LOI] Khong mo duoc file LichKham.txt de luu." << endl;
        doi_mau(7);
        return;
    }

    int count = 0;
    Node* p = head;
    while (p) {
        count++;
        p = p->next;
    }
    file << count << "\n"; 

    p = head;

    while (p) {
        // Ghi BenhNhan
        file << p->data.benhnhan.hoTen << "\n";
        file << p->data.benhnhan.cccd << "\n";
        file << p->data.benhnhan.namSinh << "\n";
        file << p->data.benhnhan.sdt << "\n";
        file << p->data.benhnhan.diaChi << "\n";

        // Ghi LichKham details
        file << p->data.tenBenh << "\n";          
        file << p->data.chuyenKhoaBenh << "\n";

        // Ghi BacSi
        file << p->data.bacsi.maSo << "\n"; 
        file << p->data.bacsi.hoTen << "\n"; 
        file << p->data.bacsi.chuyenKhoa << "\n"; 
        file << p->data.bacsi.caLamViec << "\n";
        file << p->data.bacsi.phongKham << "\n";
        
        // Ghi Date/Ca 
        file << p->data.ngay << "\n";
        file << p->data.thang << "\n";
        file << p->data.nam << "\n";
        file << p->data.caKham << "\n";

        p = p->next;
    }
    file.close();
    doi_mau(10); // Xanh lá sáng
    cout << "\n>>> [Luu File] Lich kham thanh cong. Tong so: " << count << endl;
    doi_mau(7);
}

void QuanLyLichKham::docFile() {
    ifstream file("LichKham.txt");
    if (!file.is_open()) {
        cout << "[Doc File] Khong tim thay file LichKham.txt. Khoi tao danh sach rong." << endl;
        return;
    }

    int count = 0;
    string temp;
    if (file >> count) {
        getline(file, temp);
        
        while (head) {
            Node* tempNode = head;
            head = head->next;
            delete tempNode;
        }
        tail = NULL;

        for (int i = 0; i < count; ++i) {
            LichKham lk;

            // Đọc BenhNhan
            getline(file, lk.benhnhan.hoTen);
            file >> lk.benhnhan.cccd; getline(file, temp);
            file >> lk.benhnhan.namSinh; getline(file, temp);
            getline(file, lk.benhnhan.sdt);
            getline(file, lk.benhnhan.diaChi);

            // Đọc LichKham details
            getline(file, lk.tenBenh); 
            file >> lk.chuyenKhoaBenh; getline(file, temp);

            // Đọc BacSi
            file >> lk.bacsi.maSo; getline(file, temp);
            getline(file, lk.bacsi.hoTen);
            getline(file, lk.bacsi.chuyenKhoa); 
            file >> lk.bacsi.caLamViec; getline(file, temp);
            file >> lk.bacsi.phongKham; getline(file, temp);

            // Đọc Date/Ca
            file >> lk.ngay; getline(file, temp);
            file >> lk.thang; getline(file, temp);
            file >> lk.nam; getline(file, temp);
            file >> lk.caKham; getline(file, temp);

            // Thêm vào linked list
            Node* node = createNode(lk);
            if (Empty()) 
                head = tail = node;
            else {
                tail->next = node; 
                tail = node;
            }
        }
    }
    file.close();
    cout << ">>> [Doc File] Lich kham thanh cong. Tong so: " << count << endl;
}


// =================================================================================
//        ĐỊNH NGHĨA CÁC HÀM XỬ LÝ FILE CHO BỆNH NHÂN (ĐỘC LẬP)
// =================================================================================

vector<BenhNhan> QuanLyLichKham::getDanhSachBenhNhan() {
    vector<BenhNhan> dsBenhNhan;
    Node* p = head;
    
    while (p) {
        long long cccdMoi = p->data.benhnhan.cccd;
        bool daTonTai = false;

        for (const auto& bn : dsBenhNhan) {
            if (bn.cccd == cccdMoi) {
                daTonTai = true;
                break;
            }
        }

        if (!daTonTai) {
            dsBenhNhan.push_back(p->data.benhnhan);
        }
        p = p->next;
    }
    return dsBenhNhan;
}

void QuanLyLichKham::luuFileBenhNhan() {
    vector<BenhNhan> dsBenhNhan = getDanhSachBenhNhan();
    ofstream file("BenhNhan.txt");
    if (!file.is_open()) {
        doi_mau(12); // Đỏ sáng
        cout << "[LOI] Khong mo duoc file BenhNhan.txt de luu." << endl;
        doi_mau(7);
        return;
    }

    size_t count = dsBenhNhan.size();
    file << count << "\n"; 

    for (const auto& bn : dsBenhNhan) {
        file << bn.hoTen << "\n";
        file << bn.cccd << "\n";
        file << bn.namSinh << "\n";
        file << bn.sdt << "\n";
        file << bn.diaChi << "\n";
    }
    file.close();
    doi_mau(10); // Xanh lá sáng
    cout << ">>> [Luu File] Benh nhan thanh cong. Tong so: " << count << endl;
    doi_mau(7);
}


// =================================================================================
//            ĐỊNH NGHĨA CÁC HÀM XỬ LÝ FILE CHO BÁC SĨ (VECTOR)
// =================================================================================

void luuFileBacSi(const vector<BacSi>& dsBacSi) {
    ofstream file("BacSi.txt");
    if (!file.is_open()) {
        doi_mau(12); // Đỏ sáng
        cout << "[LOI] Khong mo duoc file BacSi.txt de luu." << endl;
        doi_mau(7);
        return;
    }

    size_t count = dsBacSi.size();
    file << count << "\n"; 

    for (const auto& bs : dsBacSi) {
        file << bs.maSo << "\n";
        file << bs.hoTen << "\n";
        file << bs.chuyenKhoa << "\n"; 
        file << bs.caLamViec << "\n";
        file << bs.phongKham << "\n";
    }
    file.close();
    doi_mau(10); // Xanh lá sáng
    cout << ">>> [Luu File] Bac si thanh cong. Tong so: " << count << endl;
    doi_mau(7);
}

void docFileBacSi(vector<BacSi>& dsBacSi) {
    ifstream file("BacSi.txt");
    if (!file.is_open()) {
        cout << "[Doc File] Khong tim thay file BacSi.txt. Khoi tao du lieu mac dinh." << endl;
        return;
    }

    size_t count;
    string temp;
    if (file >> count) {
        getline(file, temp); 
        
        dsBacSi.clear(); 
        for (size_t i = 0; i < count; ++i) {
            BacSi bs;
            file >> bs.maSo; getline(file, temp);
            getline(file, bs.hoTen);
            getline(file, bs.chuyenKhoa); 
            file >> bs.caLamViec; getline(file, temp);
            file >> bs.phongKham; getline(file, temp);
            dsBacSi.push_back(bs);
        }
    }
    file.close();
    cout << ">>> [Doc File] Bac si thanh cong. Tong so: " << dsBacSi.size() << endl;
}


// =================================================================================
//               ĐỊNH NGHĨA CÁC PHƯƠNG THỨC CÒN LẠI (CÓ MÀU)
// =================================================================================

bool QuanLyLichKham::Empty() {
    return head == NULL; 
}

void QuanLyLichKham::datLich(const vector<BacSi>& dsBacSi, int soBacSi) {
    in_tieu_de("CHUC NANG: DAT LICH KHAM BENH", 11, 80); // Xanh ngọc sáng
    
    BenhNhan bn;
    doi_mau(14); // Vàng sáng
    cout << ">> NHAP THONG TIN BENH NHAN:" << endl;
    doi_mau(7);
    cout << "  - Ho ten: ";
    getline(cin,bn.hoTen);
    cout << "  - Nam sinh: ";
    cin >> bn.namSinh; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    cout << "  - So CCCD: ";
    cin >> bn.cccd; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    cout << "  - So dien thoai (sdt): ";
    getline(cin, bn.sdt);
    cout << "  - Dia chi (diaChi): ";
    getline(cin, bn.diaChi);

    doi_mau(14); // Vàng sáng
    cout << "\n>> NHAP THONG TIN LICH KHAM:" << endl;
    doi_mau(7);
    
    int d, m, y;
    while (true) {
        string ngayStr;
        cout << "  - Nhap ngay kham (dd/mm/yyyy): ";
        getline(cin, ngayStr);
        if (sscanf(ngayStr.c_str(), "%d/%d/%d", &d, &m, &y) != 3) {
            doi_mau(12); // Đỏ sáng
            cout << "    [LOI] Dinh dang khong hop le. Vui long nhap lai!\n";
            doi_mau(7);
            continue;
        }
        if (!NgayHopLe(d, m, y)) {
            doi_mau(12); // Đỏ sáng
            cout << "    [LOI] Ngay kham khong hop le (qua khu hoac sai cu phap). Vui long nhap lai!\n";
            doi_mau(7);
            continue;
        }
        break;
    }

    int caKham;
    while (true) {
        cout << "  - Nhap ca kham (1: Sang, 2: Chieu): ";
        cin >> caKham;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        if (caKham == 1 || caKham == 2) 
        break;
        doi_mau(12); // Đỏ sáng
        cout << "    [LOI] Chi duoc nhap 1 (sang) hoac 2 (chieu)! Vui long nhap lai.\n";
        doi_mau(7);
    }
    
    LichKham lk;
    lk.benhnhan = bn;
    lk.ngay = d; lk.thang = m; lk.nam = y;
    lk.caKham = caKham;
    
    cout << "  - Chuyen khoa can kham (Ma so 1:Rang, 2:Mat, 3:Noi, 4:Nhi, 5:Ngoai): ";
    while (true) {
        if (!(cin >> lk.chuyenKhoaBenh)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
            doi_mau(12); // Đỏ sáng
            cout << "    [LOI] Nhap sai dinh dang. Nhap lai.\n";
            doi_mau(7);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        if (lk.chuyenKhoaBenh >= 1 && lk.chuyenKhoaBenh <= MAX_CHUYENKHOA)
            break;
        doi_mau(12); // Đỏ sáng
        cout << "    [LOI] Chuyen khoa khong hop le! Nhap lai.\n";
        doi_mau(7);
    }
    
    printDiseaseSuggestions(lk.chuyenKhoaBenh);
    
    cout << "  - Nhap TEN BENH (vi du: Dau rang, Sot, Viem da day, ...): ";
    getline(cin, lk.tenBenh); 
    
    const char* tenChuyenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};
    string tenKhoaCanTim = tenChuyenKhoa[lk.chuyenKhoaBenh]; 

    in_tieu_de("DANH SACH BAC SI PHU HOP (Khoa " + tenKhoaCanTim + " | Ca " + (caKham==1 ? "SANG" : "CHIEU") + " | Ngay " 
         + to_string(d) + "/" + to_string(m) + "/" + to_string(y) + "):", 9, 110); // Xanh dương sáng
    
    vector<int> indexBacSiPhuHop; 
    
    for (int i = 0; i < soBacSi; ++i) {
        if (dsBacSi[i].chuyenKhoa == tenKhoaCanTim && 
           (dsBacSi[i].caLamViec == caKham || dsBacSi[i].caLamViec == 3)) {
            int soLuot = demSoLich(dsBacSi[i].maSo, d, m, y, caKham);
            doi_mau(14); // Vàng sáng
            cout << "[" << dsBacSi[i].maSo << "] ";
            doi_mau(7);
            cout << "BS: " << left << setw(20) << dsBacSi[i].hoTen
                 << "| P.Kham: " << dsBacSi[i].phongKham;
            doi_mau(11); // Xanh ngọc sáng
            cout << " | Da dat: " << soLuot << "/25\n";
            doi_mau(7);
            indexBacSiPhuHop.push_back(i);
        }
    }
    in_phan_cach();
    
    if (indexBacSiPhuHop.empty()) {
        doi_mau(12); // Đỏ sáng
        cout << "KHONG CO BAC SI nao thuoc chuyen khoa va ca nay, vui long chon ca kham khac!\n";
        in_tieu_de("DAT LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }
    
    BacSi bsChon;
    while (true) {
        cout << ">> Nhap **MA SO** bac si de chon: ";
        int maBS;
        cin >> maBS; 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        bool timDuoc = false;
        
        for (int index : indexBacSiPhuHop) {
            if (dsBacSi[index].maSo == maBS) {
                bsChon = dsBacSi[index];
                timDuoc = true;
                break;
            }
        }
        if (timDuoc==false) {
            doi_mau(12); // Đỏ sáng
            cout << "    [LOI] Khong tim thay bac si phu hop. Vui long nhap lai!\n";
            doi_mau(7);
        }
        else 
            break;
    }
    
    int luotDaDat = demSoLich(bsChon.maSo, d, m, y, caKham);
    if (luotDaDat >= 25) {
        doi_mau(12); // Đỏ sáng
        cout << "    [LOI] Bac si da dat toi da 25 luot/ca/ngay. Vui long chon bac si hoac ca khac!\n";
        in_tieu_de("DAT LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }

    lk.bacsi = bsChon;

    Node* node = createNode(lk);
    if (Empty()) 
        head = tail = node;
    else {
        tail->next = node; 
        tail = node;
    }
    
    in_tieu_de("DAT LICH THANH CONG!", 10, 80); // Xanh lá sáng
    cout << "Ho ten benh nhan: "; doi_mau(14); cout << lk.benhnhan.hoTen << endl; doi_mau(7);
    cout << "Kham vao ngay: "; doi_mau(15); cout << lk.ngay << "/" << lk.thang << "/" << lk.nam; doi_mau(7);
    cout << " | Ca: " << (lk.caKham == 1 ? "Sang" : "Chieu") << endl;
    cout << "Benh: "; doi_mau(13); cout << lk.tenBenh; doi_mau(7);
    cout << " | Khoa: " << tenKhoaCanTim << endl;
    cout << "Bac si: "; doi_mau(9); cout << lk.bacsi.hoTen; doi_mau(7);
    cout << " | Phong: " << lk.bacsi.phongKham << endl;
    in_phan_cach(10);
}

void QuanLyLichKham::huyLich(const vector<BacSi>& dsBacSi, int soBacSi) {
    in_tieu_de("CHUC NANG: HUY LICH KHAM BENH", 11, 80);

    if (Empty()) {
        doi_mau(12);
        cout << "Danh sach lich kham dang rong. Khong the huy.\n";
        in_tieu_de("HUY LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }

    cout << ">> Nhap CCCD benh nhan can huy lich: ";
    long long cccd;
    cin >> cccd; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    
    Node* a[MAX_LICHKHAM]; 
    int n = 0;
    Node* p = head;
    while (p) {
        if (p->data.benhnhan.cccd == cccd) {
             a[n] = p;
             n++;
        }
        p = p->next;
    }
    if (n == 0) {
        doi_mau(12);
        cout << "    [LOI] Khong tim thay benh nhan voi CCCD [" << cccd << "].\n";
        in_tieu_de("HUY LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }

    in_tieu_de("DANH SACH LICH KHAM CUA BENH NHAN (CCCD: " + to_string(cccd) + ")", 14, 80); // Vàng sáng
    for (int i = 0; i < n; ++i) {
        printLichKham(a[i]->data, i + 1);
    }
    in_phan_cach();

    cout << ">> Nhap so thu tu lich kham muon HUY: ";
    int m;
    cin >> m; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    if (m < 1 || m > n) {
        doi_mau(12);
        cout << "    [LOI] So thu tu khong hop le.\n";
        in_tieu_de("HUY LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }
    
    Node* nodeHuy = a[m - 1];
    if (nodeHuy == head) {
        head = nodeHuy->next;
        if (head == NULL) tail = NULL;
    }
    else {
        Node* prev = head;
        while (prev->next != nodeHuy){
            prev = prev->next;
        }
            prev->next = nodeHuy->next;
        if (nodeHuy == tail) 
            tail = prev;
    }
    delete nodeHuy; 
    doi_mau(10);
    cout << "\n>>> DA HUY LICH KHAM [STT: " << m << "] THANH CONG.\n";
    in_tieu_de("HUY LICH THANH CONG", 10, 80);
    doi_mau(7);
}

void QuanLyLichKham::suaLich(const vector<BacSi>& dsBacSi, int soBacSi) {
    in_tieu_de("CHUC NANG: SUA LICH KHAM BENH", 11, 80);
    
    if (Empty()) {
        doi_mau(12);
        cout << "Danh sach lich kham dang rong. Khong the sua.\n";
        in_tieu_de("SUA LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }
    
    cout << ">> Nhap CCCD benh nhan can sua lich: ";
    long long cccd;
    cin >> cccd; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    
    Node* a[MAX_LICHKHAM]; 
    int n = 0;
    Node* p = head;
    while (p) {
        if (p->data.benhnhan.cccd == cccd) {
            a[n] = p;
            n++;
        }
        p = p->next;
    }
    if (n == 0) {
        doi_mau(12);
        cout << "    [LOI] Khong tim thay benh nhan voi CCCD [" << cccd << "].\n";
        in_tieu_de("SUA LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }

    in_tieu_de("DANH SACH LICH KHAM CUA BENH NHAN (CCCD: " + to_string(cccd) + ")", 14, 80);
    for (int i = 0; i < n; ++i) {
        printLichKham(a[i]->data, i + 1);
    }
    in_phan_cach();

    cout << ">> Nhap so thu tu lich kham muon SUA: ";
    int m;
    cin >> m; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    if (m < 1 || m > n) {
        doi_mau(12);
        cout << "    [LOI] So thu tu khong hop le.\n";
        in_tieu_de("SUA LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }
    Node* nodeSua = a[m - 1];
    LichKham& lk = nodeSua->data;
    
    int maBsCu = lk.bacsi.maSo;

    doi_mau(11);
    cout << "\nBAT DAU SUA LICH KHAM [STT: " << m << "]:" << endl;
    doi_mau(7);
    
    cout << ">> Ban co muon sua THONG TIN BENH NHAN (ho ten, CCCD, nam sinh, sdt, dia chi)? (y/n): ";
    char ch;
    cin >> ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
    if (ch == 'y' || ch == 'Y') {
        cout << "  - Nhap ho ten benh nhan moi (" << lk.benhnhan.hoTen << "): ";
        getline(cin,lk.benhnhan.hoTen);
        cout << "  - Nhap nam sinh moi (" << lk.benhnhan.namSinh << "): ";
        cin >> lk.benhnhan.namSinh;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        cout << "  - Nhap so CCCD moi (" << lk.benhnhan.cccd << "): ";
        cin >> lk.benhnhan.cccd;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        cout << "  - Nhap so dien thoai (sdt) moi (" << lk.benhnhan.sdt << "): ";
        getline(cin, lk.benhnhan.sdt);
        cout << "  - Nhap dia chi (diaChi) moi (" << lk.benhnhan.diaChi << "): ";
        getline(cin, lk.benhnhan.diaChi);
        doi_mau(10);
        cout << "  [INFO] Cap nhat THONG TIN BENH NHAN thanh cong." << endl;
        doi_mau(7);
    }
    
    int d, mth, y;
    while (true) {
        string ngayStr;
        cout << ">> Nhap NGAY KHAM moi (dd/mm/yyyy | Hien tai: " << lk.ngay << "/" << lk.thang << "/" << lk.nam << "): ";
        getline(cin, ngayStr);
        if (sscanf(ngayStr.c_str(), "%d/%d/%d", &d, &mth, &y) != 3) {
            doi_mau(12); cout << "    [LOI] Dinh dang khong hop le. Vui long nhap lai!\n"; doi_mau(7);
            continue;
        }
        if (!NgayHopLe(d, mth, y)) {
            doi_mau(12); cout << "    [LOI] Ngay kham khong hop le. Vui long nhap lai!\n"; doi_mau(7);
            continue;
        }
        break;
    }
    lk.ngay = d; lk.thang = mth; lk.nam = y;
    
     while (true) {
        cout << ">> Nhap CA KHAM moi (1: Sang, 2: Chieu | Hien tai: " << lk.caKham << "): ";
        cin >> lk.caKham; 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        if (lk.caKham == 1 || lk.caKham == 2) 
            break;
        doi_mau(12); cout << "    [LOI] Chi duoc nhap 1 (sang) hoac 2 (chieu)! Vui long nhap lai.\n"; doi_mau(7);
    }

    cout << "\n>> Nhap THONG TIN BENH/CHUYEN KHOA moi:" << endl;
    cout << "  - Chuyen khoa (Ma so 1:Rang, 2:Mat, 3:Noi, 4:Nhi, 5:Ngoai | Hien tai: " << lk.chuyenKhoaBenh << "): ";
    while (true) {
        if (!(cin >> lk.chuyenKhoaBenh)) {
             cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
             doi_mau(12); cout << "    [LOI] Nhap sai dinh dang. Nhap lai.\n"; doi_mau(7);
             continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        if (lk.chuyenKhoaBenh >= 1 && lk.chuyenKhoaBenh <= MAX_CHUYENKHOA)
            break;
        doi_mau(12); cout << "    [LOI] Chuyen khoa khong hop le! Nhap lai.\n"; doi_mau(7);
    }

    printDiseaseSuggestions(lk.chuyenKhoaBenh);
    
    cout << "  - Nhap Ten benh moi (Hien tai: " << lk.tenBenh << "): ";
    getline(cin, lk.tenBenh); 
    
    const char* tenChuyenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};
    string tenKhoaCanTim = tenChuyenKhoa[lk.chuyenKhoaBenh]; 

    in_tieu_de("DANH SACH BAC SI PHU HOP VOI SUA DOI (Khoa " + tenKhoaCanTim
         + " | Ca " + (lk.caKham==1 ? "SANG" : "CHIEU") + " | Ngay " + to_string(lk.ngay) + "/" + to_string(lk.thang) + "/" + to_string(lk.nam) + "):", 9, 110); 
    
    vector<int> indexBacSiPhuHopMoi; 
    
    for (int i = 0; i < soBacSi; ++i) {
        if (dsBacSi[i].chuyenKhoa == tenKhoaCanTim && 
           (dsBacSi[i].caLamViec == lk.caKham || dsBacSi[i].caLamViec == 3)) {
            int soLuot = demSoLich(dsBacSi[i].maSo, lk.ngay, lk.thang, lk.nam, lk.caKham);
            
            int luotHienTai = soLuot;
            if (dsBacSi[i].maSo == maBsCu) {
                luotHienTai--; 
            }
                             
            doi_mau(14); cout << "[" << dsBacSi[i].maSo << "] "; doi_mau(7);
            cout << "BS: " << left << setw(20) << dsBacSi[i].hoTen
                 << "| P.Kham: " << dsBacSi[i].phongKham;
            doi_mau(11);
            cout << " | Da dat: " << luotHienTai << "/25\n"; 
            doi_mau(7);
            indexBacSiPhuHopMoi.push_back(i);
        }
    }
    in_phan_cach();
    if (indexBacSiPhuHopMoi.empty()) { 
        doi_mau(12); cout << "KHONG CO BAC SI nao thuoc chuyen khoa va ca kham nay.\n";
        in_tieu_de("SUA LICH THAT BAI", 12, 80);
        doi_mau(7);
        return;
    }

    BacSi bsChon;
    while (true) {
        cout << ">> Nhap **MA SO** bac si de chon: ";
        int maBS;
        cin >> maBS; 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        bool timDuoc = false;
        
        for (int index : indexBacSiPhuHopMoi) {
            if (dsBacSi[index].maSo == maBS) {
                bsChon = dsBacSi[index];
                timDuoc = true;
                break;
            }
        }
        if (timDuoc==false) {
            doi_mau(12); cout << "    [LOI] Khong tim thay bac si phu hop. Vui long nhap lai!\n"; doi_mau(7);
        }
        else {
            int luotDaDatMoi = demSoLich(bsChon.maSo, lk.ngay, lk.thang, lk.nam, lk.caKham);
            
            if (bsChon.maSo == maBsCu) { 
                luotDaDatMoi--; 
            }
            if (luotDaDatMoi >= 25) {
                doi_mau(12); cout << "    [LOI] Bac si da dat toi da 25 luot/ca/ngay. Vui long chon bac si khac!\n"; doi_mau(7);
                continue;
            }
            break;
        }
    }
    lk.bacsi = bsChon;

    doi_mau(10);
    cout << "\n>>> DA SUA LICH KHAM THANH CONG!\n";
    in_tieu_de("SUA LICH KHAM THANH CONG", 10, 80);
    doi_mau(7);
}

void QuanLyLichKham::timKiemLichKhamTheoCCCD(long long cccd) {
    in_tieu_de("CHUC NANG: TIM KIEM LICH KHAM THEO CCCD (CCCD: " + to_string(cccd) + ")", 11, 110);
    
    LichKham ds[MAX_LICHKHAM];
    int n = 0;
    Node* p = head;
    while (p) {
        if (p->data.benhnhan.cccd == cccd) {
             ds[n] = p->data;
             n++;
        }
        p = p->next;
    }
    if (n == 0) {
        doi_mau(12);
        cout << "KHONG CO LICH KHAM nao voi CCCD [" << cccd << "]." << endl;
        in_tieu_de("TIM KIEM KHONG THANH CONG", 12, 110);
        doi_mau(7);
        return;
    }
    
    // Sắp xếp
    for (int i = 0; i < n-1; ++i)
        for (int j = i+1; j < n; ++j) {
            LichKham &a = ds[i], &b = ds[j];
            if (
                a.nam > b.nam ||
                (a.nam == b.nam && a.thang > b.thang) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay > b.ngay) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham > b.caKham) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham == b.caKham && a.bacsi.maSo > b.bacsi.maSo)
            ) {
                LichKham t = ds[i]; 
                         ds[i] = ds[j]; 
                         ds[j] = t;
            }
        }
        
    doi_mau(10);
    cout << "\nHIEN THI " << n << " LICH KHAM CUA BENH NHAN (Da sap xep):" << endl;
    doi_mau(7);
    for (int k = 0; k < n; ++k) {
        printChiTietLichKham(ds[k]);
    }
    in_tieu_de("TIM KIEM THANH CONG", 10, 110);
}

void QuanLyLichKham::hienThiTatCa() {
    in_tieu_de("CHUC NANG: XEM TAT CA LICH KHAM (SAP XEP)", 11, 110);
    
    LichKham ds[MAX_LICHKHAM];
    int n = 0;
    Node* p = head;
    if(Empty()){
        doi_mau(12);
        cout<<"Danh sach khong co benh nhan."<<endl;
        in_tieu_de("HIEN THI THAT BAI", 12, 110);
        doi_mau(7);
        return;
    }
    while (p) {
        ds[n] = p->data;
        n++;
        p = p->next;
    }

    // Sắp xếp
    for (int i = 0; i < n-1; ++i)
        for (int j = i+1; j < n; ++j) {
            LichKham &a = ds[i], &b = ds[j];
            if (
                a.nam > b.nam ||
                (a.nam == b.nam && a.thang > b.thang) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay > b.ngay) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham > b.caKham) ||
                (a.nam == b.nam && a.thang == b.thang && a.ngay == b.ngay && a.caKham == b.caKham && a.bacsi.maSo > b.bacsi.maSo)
            ) {
                LichKham t = ds[i]; 
                         ds[i] = ds[j]; 
                         ds[j] = t;
            }
        }
    
    doi_mau(10);
    cout << "TONG CONG: " << n << " LICH KHAM (Da sap xep theo thoi gian)" << endl;
    doi_mau(7);
    for (int k = 0; k < n; ++k) {
        printChiTietLichKham(ds[k]);
    }
    in_tieu_de("HIEN THI THANH CONG", 10, 110);
}

// =================================================================================
//           ĐỊNH NGHĨA HÀM MỚI: XÓA LỊCH QUÁ KHỨ
// =================================================================================

void QuanLyLichKham::xoaLichQuaKhu() {
    in_tieu_de("CHUC NANG: XOA TAT CA LICH KHAM DA QUA KHU", 13, 100);

    if (Empty()) {
        doi_mau(12);
        cout << "Danh sach lich kham rong. Khong co gi de xoa.\n";
        doi_mau(7);
        return;
    }

    // Lấy ngày hiện tại
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int dNow = ltm->tm_mday;
    int mNow = ltm->tm_mon + 1;
    int yNow = ltm->tm_year + 1900;

    int countXoa = 0;

    // Xóa các node đầu tiên nếu nó quá khứ
    while (head != NULL) {
        LichKham& lk = head->data;

        bool quaKhu =
            (lk.nam < yNow) ||
            (lk.nam == yNow && lk.thang < mNow) ||
            (lk.nam == yNow && lk.thang == mNow && lk.ngay < dNow);

        if (!quaKhu) break;

        Node* temp = head;
        head = head->next;
        delete temp;
        countXoa++;
    }

    if (head == NULL) {
        tail = NULL;
        doi_mau(10);
        cout << ">>> Da xoa tat ca " << countXoa << " lich qua khu.\n";
        doi_mau(7);
        return;
    }

    // Xóa các node còn lại
    Node* p = head;
    while (p->next != NULL) {
        LichKham& lk = p->next->data;

        bool quaKhu =
            (lk.nam < yNow) ||
            (lk.nam == yNow && lk.thang < mNow) ||
            (lk.nam == yNow && lk.thang == mNow && lk.ngay < dNow);

        if (quaKhu) {
            Node* temp = p->next;
            p->next = temp->next;
            if (temp == tail) tail = p;
            delete temp;
            countXoa++;
        } else {
            p = p->next;
        }
    }

    doi_mau(10);
    cout << ">>> Da xoa " << countXoa << " lich kham qua khu.\n";
    doi_mau(7);
}

// =================================================================================
//                          HÀM KHÁC (BÁC SĨ, MENU...)
// =================================================================================

void QuanLyBacSi(vector<BacSi>& dsBacSi) {
    int chon;
    do {
        cout << "\n\n";
        in_tieu_de("MENU QUAN LI BAC SI (ADMIN)", 13, 80); // Hồng sáng
        cout << "1. Xem danh sach bac si\n";
        cout << "2. Them bac si\n";
        cout << "3. Xoa bac si (theo Ma so)\n";
        cout << "4. Sua thong tin bac si (theo Ma so)\n";
        cout << "5. Tim kiem bac si theo chuyen khoa\n";
        cout << "6. Xoa bac si theo chuyen khoa\n";
        cout << "0. Quay lai Menu Admin\n";
        in_phan_cach(13);
        cout << ">> Chon chuc nang: ";
        if (!(cin >> chon)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
            chon = -1;
            doi_mau(12); cout << "    [LOI] Vui long nhap 1 so tu 0->6.\n"; doi_mau(7);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI

        switch (chon) {
            case 1: { 
                in_tieu_de("DANH SACH BAC SI HIEN CO (Tong: " + to_string(dsBacSi.size()) + ")", 14, 80); // Vàng sáng
                in_phan_cach();
                cout << "[Ma] " << left << setw(20) << "Ho Ten" << "| Khoa  | Ca        | P.Kham" << endl;
                in_phan_cach();
                for (size_t i = 0; i < dsBacSi.size(); ++i) { 
                    printBacSi(dsBacSi[i]);
                }
                in_tieu_de("KET THUC DANH SACH", 14, 80);
                tam_dung();
                break;
            }
            case 2: { 
                BacSi bs;
                doi_mau(11); cout << "\n>> THEM BAC SI MOI:\n"; doi_mau(7);
                cout << "  - Ma so: "; cin >> bs.maSo; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                
                for (const auto& existingBs : dsBacSi) {
                    if (existingBs.maSo == bs.maSo) {
                        doi_mau(12); cout << "    [LOI] Ma so bac si [" << bs.maSo << "] da ton tai. Them that bai.\n"; doi_mau(7);
                        goto skip_add;
                    }
                }
                
                cout << "  - Ho ten: "; getline(cin, bs.hoTen);
                while(true) {
                    int maKhoa;
                    cout << "  - Chuyen khoa (Ma 1:RANG, 2:MAT,3:NOI,4:NHI,5:NGOAI): "; cin >> maKhoa; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                    const char* tenChuyenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};
                    if (maKhoa >= 1 && maKhoa <= MAX_CHUYENKHOA) {
                        bs.chuyenKhoa = tenChuyenKhoa[maKhoa]; 
                        break;
                    }
                    doi_mau(12); cout << "    [LOI] Ma chuyen khoa khong hop le. Nhap lai.\n"; doi_mau(7);
                }
                while(true) {
                    cout << "  - Ca lam viec (1:Sang, 2:Chieu, 3:Ca ngay): "; cin >> bs.caLamViec; 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                    if (bs.caLamViec >= 1 && bs.caLamViec <= 3) break;
                    doi_mau(12); cout << "    [LOI] Ma ca lam viec khong hop le. Nhap lai.\n"; doi_mau(7);
                }
                cout << "  - Phong kham: "; cin >> bs.phongKham; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI

                dsBacSi.push_back(bs);
                doi_mau(10); cout << "\n>>> THEM BAC SI [" << bs.hoTen << "] THANH CONG. So luong hien tai: " << dsBacSi.size() << endl; doi_mau(7);
                skip_add:;
                tam_dung();
                break;
            }
            case 3: { 
                doi_mau(11); cout << "\n>> XOA BAC SI:\n"; doi_mau(7);
                cout << "  - Nhap MA SO Bac si can xoa: ";
                int maXoa; cin >> maXoa; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                
                auto it = dsBacSi.begin();
                bool timDuoc = false;
                while (it != dsBacSi.end()) {
                    if (it->maSo == maXoa) {
                        timDuoc = true;
                        doi_mau(14); cout << "  - Xac nhan xoa BS: " << it->hoTen << " (y/n)? "; doi_mau(7);
                        char confirm; cin >> confirm; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                        if (confirm == 'y' || confirm == 'Y') {
                            it = dsBacSi.erase(it);
                            doi_mau(10); cout << "\n>>> XOA BAC SI MA SO [" << maXoa << "] THANH CONG.\n"; doi_mau(7);
                        } else {
                            doi_mau(9); cout << "    [INFO] Da huy thao tac xoa.\n"; doi_mau(7);
                            ++it;
                        }
                        break; 
                    } else {
                        ++it;
                    }
                }
                if (!timDuoc) {
                    doi_mau(12); cout << "    [LOI] Khong tim thay Bac si voi MA SO [" << maXoa << "].\n"; doi_mau(7);
                }
                tam_dung();
                break;
            }
            case 4: { 
                doi_mau(11); cout << "\n>> SUA THONG TIN BAC SI:\n"; doi_mau(7);
                cout << "  - Nhap MA SO Bac si can sua: ";
                int maSua; cin >> maSua; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                
                BacSi* bsSua = nullptr;
                for (BacSi& bs : dsBacSi) {
                    if (bs.maSo == maSua) {
                        bsSua = &bs;
                        break;
                    }
                }
                
                if (bsSua != nullptr) {
                    BacSi& bs = *bsSua;
                    doi_mau(14); cout << "  - Dang sua thong tin BS: " << bs.hoTen << endl; doi_mau(7);
                    cout << "  - Nhap Ho ten moi (" << bs.hoTen << "): "; 
                    getline(cin, bs.hoTen);
                    while(true) {
                        int maKhoaMoi;
                        cout << "  - Nhap Chuyen khoa moi (Ma 1:RANG, 2:MAT,3:NOI,4:NHI,5:NGOAI) | Hien tai: " << bs.chuyenKhoa << "): "; cin >> maKhoaMoi; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                        const char* tenChuyenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};
                        if (maKhoaMoi >= 1 && maKhoaMoi <= MAX_CHUYENKHOA) {
                            bs.chuyenKhoa = tenChuyenKhoa[maKhoaMoi]; 
                            break;
                        }
                        doi_mau(12); cout << "    [LOI] Ma chuyen khoa khong hop le. Nhap lai.\n"; doi_mau(7);
                    }
                    while(true) {
                        cout << "  - Nhap Ca lam viec moi (" << bs.caLamViec << ", 1:Sang, 2:Chieu, 3:Ca ngay): "; cin >> bs.caLamViec; 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // THÊM CIN.IGNORE
                        if (bs.caLamViec >= 1 && bs.caLamViec <= 3) break;
                        doi_mau(12); cout << "    [LOI] Ma ca lam viec khong hop le. Nhap lai.\n"; doi_mau(7);
                    }
                    cout << "  - Nhap phong kham moi (" << bs.phongKham << "): "; cin >> bs.phongKham; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                    doi_mau(10); cout << "\n>>> SUA THONG TIN BAC SI [" << maSua << "] THANH CONG.\n"; doi_mau(7);
                } else {
                    doi_mau(12); cout << "    [LOI] Khong tim thay Bac si voi MA SO [" << maSua << "].\n"; doi_mau(7);
                }
                tam_dung();
                break;
            }
            case 5: {
                int maKhoa;
                const char* tenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};

                cout << "\n>> Nhap ma chuyen khoa can tim (1:RANG,2:MAT,3:NOI,4:NHI,5:NGOAI): ";
                if (!(cin >> maKhoa)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    doi_mau(12); cout << "[LOI] Nhap sai dinh dang.\n"; doi_mau(7);
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (maKhoa < 1 || maKhoa > 5) {
                    doi_mau(12);
                    cout << "[LOI] Ma khoa khong hop le!\n";
                    doi_mau(7);
                    break;
                }

                string ten = tenKhoa[maKhoa];
                
                in_tieu_de("DANH SACH BAC SI THUOC KHOA " + ten, 14, 80);

                bool found = false;
                for (auto& bs : dsBacSi) {
                    if (bs.chuyenKhoa == ten) {
                        printBacSi(bs);
                        found = true;
                    }
                }

                if (!found) {
                    doi_mau(12);
                    cout << "Khong co bac si nao thuoc khoa " << ten << ".\n";
                    doi_mau(7);
                }

                tam_dung();
                break;
            }
            case 6: {
                int maKhoa;
                const char* tenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};

                cout << "\n>> Nhap ma chuyen khoa muon xoa (1:RANG,2:MAT,3:NOI,4:NHI,5:NGOAI): ";
                if (!(cin >> maKhoa)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    doi_mau(12); cout << "[LOI] Nhap sai dinh dang.\n"; doi_mau(7);
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (maKhoa < 1 || maKhoa > 5) {
                    doi_mau(12);
                    cout << "[LOI] Ma khoa khong hop le!\n";
                    doi_mau(7);
                    break;
                }

                string ten = tenKhoa[maKhoa];

                // Đếm trước để hỏi xác nhận
                int soLuong = 0;
                for (auto& bs : dsBacSi) {
                    if (bs.chuyenKhoa == ten) soLuong++;
                }

                if (soLuong == 0) {
                    doi_mau(12);
                    cout << "Khong co bac si nao thuoc khoa " << ten << ".\n";
                    doi_mau(7);
                    break;
                }

                doi_mau(14);
                cout << "Ban co chac muon xoa TAT CA " << soLuong
                     << " bac si thuoc khoa " << ten << " ? (y/n): ";
                doi_mau(7);

                char confirm;
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (confirm != 'y' && confirm != 'Y') {
                    doi_mau(9);
                    cout << "Huy thao tac xoa.\n";
                    doi_mau(7);
                    break;
                }

                // Xóa
                dsBacSi.erase(
                    remove_if(
                        dsBacSi.begin(),
                        dsBacSi.end(),
                        [&](const BacSi& bs) { return bs.chuyenKhoa == ten; }
                    ),
                    dsBacSi.end()
                );

                doi_mau(10);
                cout << ">>> Da xoa thanh cong tat ca bac si thuoc khoa " << ten << ".\n";
                doi_mau(7);

                tam_dung();
                break;
            }
            case 0:
                doi_mau(9); cout << "    [INFO] Quay lai Menu Admin.\n"; doi_mau(7);
                break;
            default:
                doi_mau(12); cout << "\n    [LOI] Chuc nang khong hop le! Vui long chon lai.\n"; doi_mau(7);
                tam_dung();
        }
    } while (chon != 0);
}

bool DangNhapAdmin() {
    string user, pass;
    in_tieu_de("DANG NHAP ADMIN", 13, 80);
    
    for (int i = 0; i < 3; ++i) { 
        cout << "  - Ten dang nhap: ";
        getline(cin, user);
        cout << "  - Mat khau: ";
        getline(cin, pass);
        
        if (user == "admin" && pass == "123") {
            doi_mau(10); cout << "\n>>> DANG NHAP ADMIN THANH CONG!\n"; doi_mau(7);
            in_phan_cach(10);
            return true;
        }
        doi_mau(12); cout << "    [LOI] Ten dang nhap hoac mat khau khong dung. Con lai " << 2 - i << " lan thu.\n"; doi_mau(7);
    }
    doi_mau(12); cout << "\n    [LOI] Da het so lan thu. Quay lai Menu chinh.\n"; doi_mau(7);
    in_phan_cach(12);
    return false;
}

// =================================================================================
//                              HÀM MENU BỆNH NHÂN ĐÃ SỬA
// =================================================================================

void MenuBenhNhan(QuanLyLichKham& ql, vector<BacSi>& dsBacSi) {
    int chon;
    int soBacSi = dsBacSi.size();
    do {
        cout << "\n\n";
        in_tieu_de("MENU NGUOI KHAM BENH", 10, 80); // Xanh lá sáng
        cout << "1. Dat lich kham\n";
        cout << "2. Huy lich kham\n";
        cout << "3. Sua lich kham\n";
        cout << "4. Tim kiem lich kham cua minh (theo CCCD)\n"; // Đổi tên rõ ràng hơn
        cout << "7. Luu du lieu (Lich kham, Benh nhan) vao file\n";
        cout << "0. Quay lai Menu Chon Vai Tro\n";
        in_phan_cach(10);
        cout << ">> Chon chuc nang: ";
        if (!(cin >> chon)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
            chon = -1;
            doi_mau(12); cout << "    [LOI] Vui long nhap mot so.\n"; doi_mau(7);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        
        switch (chon) {
            case 1:
                ql.datLich(dsBacSi, soBacSi);
                break;
            case 2:
                ql.huyLich(dsBacSi, soBacSi);
                break;
            case 3:
                ql.suaLich(dsBacSi, soBacSi);
                break;
            case 4: {
                cout << "\n";
                cout << ">> Nhap CCCD cua ban can tim kiem: ";
                long long cccd;
                if (!(cin >> cccd)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
                    doi_mau(12); cout << "    [LOI] Nhap sai dinh dang CCCD.\n"; doi_mau(7);
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                ql.timKiemLichKhamTheoCCCD(cccd);
                break;
            }
            case 7: // Lưu file (Chỉ lưu file liên quan Bệnh nhân và Lịch khám)
                ql.luuFile();
                ql.luuFileBenhNhan();
                break;
            case 0:
                doi_mau(9); cout << "    [INFO] Quay lai Menu Chon Vai Tro.\n"; doi_mau(7);
                break;
            default:
                doi_mau(12); cout << "\n    [LOI] Chuc nang khong hop le! Vui long chon lai.\n"; doi_mau(7);
        }
        if (chon != 0) tam_dung();
    } while (chon != 0);
}

// =================================================================================
//                              HÀM MENU ADMIN MỚI
// =================================================================================

void MenuAdmin(QuanLyLichKham& ql, vector<BacSi>& dsBacSi) {
    int chon;
    do {
        cout << "\n\n";
        in_tieu_de("MENU QUAN TRI VIEN (ADMIN)", 13, 80); // Hồng sáng
        cout << "1. Quan ly Bac si (Them/Xoa/Sua/Xem DS)\n";
        cout << "2. Xem tat ca Lich kham (Da sap xep)\n";
        cout << "3. Tim kiem Lich kham theo CCCD Benh nhan\n";
        cout << "4. Xoa tat ca Lich kham da qua (qua khu)\n";
        cout << "7. Luu du lieu Bac si vao file\n";
        cout << "0. Quay lai Menu Chon Vai Tro\n";
        in_phan_cach(13);
        cout << ">> Chon chuc nang: ";
        if (!(cin >> chon)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
            chon = -1;
            doi_mau(12); cout << "    [LOI] Vui long nhap mot so.\n"; doi_mau(7);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        
        switch (chon) {
            case 1:
                QuanLyBacSi(dsBacSi);
                break;
            case 2:
                ql.hienThiTatCa();
                break;
            case 3: {
                cout << "\n";
                cout << ">> Nhap CCCD benh nhan can tim kiem: ";
                long long cccd;
                if (!(cin >> cccd)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
                    doi_mau(12); cout << "    [LOI] Nhap sai dinh dang CCCD.\n"; doi_mau(7);
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
                ql.timKiemLichKhamTheoCCCD(cccd);
                break;
            }
            case 4: // Xóa lịch quá khứ
                ql.xoaLichQuaKhu();
                // tự động lưu file sau khi xóa
                ql.luuFile();
                ql.luuFileBenhNhan();
                break;
            case 7: // Lưu file Bác sĩ
                luuFileBacSi(dsBacSi);
                break;
            case 0:
                doi_mau(9); cout << "    [INFO] Quay lai Menu Chon Vai Tro.\n"; doi_mau(7);
                break;
            default:
                doi_mau(12); cout << "\n    [LOI] Chuc nang khong hop le! Vui long chon lai.\n"; doi_mau(7);
        }
        if (chon != 0) tam_dung();
    } while (chon != 0);
}


// =================================================================================
//                              HÀM MAIN (ĐÃ SỬA ĐỔI)
// =================================================================================

int main() {
    // Thay đổi kích thước cửa sổ console
    thay_doi_kich_thuoc(120, 40); 
    
    vector<BacSi> dsBacSi = {
        {101, "Nguyen Van An", "RANG", 1, 101},
        {102, "Tran Thi Thao Vy", "RANG", 2, 101},
        {103, "Le Van Dung", "MAT", 3, 201},
        {104, "Pham Thi Thu Duyen", "NOI", 1, 301},
        {105, "Hoang Van Viet", "NOI", 3, 302},
        {106, "Ly Hoang Anh", "NOI", 1, 303},
        {107, "Trieu Minh Tien", "NOI", 2, 301},
        {108, "Ho Viet Quoc", "NHI", 1, 401},
        {109, "Ngo Trong Nghia", "NHI", 2, 401},
        {110, "Phuong Nguyen", "NGOAI", 3, 501},
        {111, "Vo Thao Nguyen", "NGOAI", 1, 502},
        {112, "Bui Huyen Huyen", "NGOAI", 2, 503}
    };

    QuanLyLichKham ql;

    doi_mau(15);
    cout << "--- Khoi dong he thong va Doc File ---" << endl;
    doi_mau(7);
    docFileBacSi(dsBacSi);
    ql.docFile();
    doi_mau(15);
    cout << "--------------------------------------" << endl;
    doi_mau(7);


    int chon_vai_tro;
    do {
        cout << "\n\n";
        in_tieu_de("HE THONG DAT LICH KHAM ONLINE", 11, 80); // Xanh ngọc sáng
        doi_mau(14); cout << "1. NGUOI KHAM (Benh nhan) - Dat/Sua/Huy lich, Tra cuu lich ca nhan\n"; doi_mau(7);
        doi_mau(13); cout << "2. ADMIN (Quan ly) - Quan ly Bac si, Xem tat ca lich kham\n"; doi_mau(7);
        cout << "0. Thoat\n";
        in_phan_cach(11);
        cout << ">> Chon vai tro: ";
        
        if (!(cin >> chon_vai_tro)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng bản mạnh hơn
            chon_vai_tro = -1;
            doi_mau(12); cout << "    [LOI] Vui long nhap mot so.\n"; doi_mau(7);
            tam_dung();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // SỬA LỖI
        
        switch (chon_vai_tro) {
            case 1: // NGUOI KHAM
                MenuBenhNhan(ql, dsBacSi);
                break;
            
            case 2: // ADMIN
                if (DangNhapAdmin()) {
                    MenuAdmin(ql, dsBacSi);
                }
                break;

            case 0:
                // Tự động lưu file khi thoát
                doi_mau(15);
                cout << "\n--- Dang Luu du lieu truoc khi thoat ---" << endl;
                doi_mau(7);
                luuFileBacSi(dsBacSi); // Lưu BS
                ql.luuFile(); // Lưu Lịch khám
                ql.luuFileBenhNhan(); // Lưu BN
                doi_mau(15);
                cout << "--------------------------------------" << endl;
                doi_mau(7);

                in_tieu_de("THOAT CHUONG TRINH", 10, 80);
                return 0;

            default:
                doi_mau(12);
                cout << "\n    [LOI] Chuc nang khong hop le! Vui long chon lai.\n";
                doi_mau(7);
                tam_dung();
        }
    } while (true);
    return 0;
}