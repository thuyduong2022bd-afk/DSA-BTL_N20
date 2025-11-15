//Lưu file Lịch khám
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
//Đọc file lịch khám
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
//Lưu file bệnh nhân
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
//Lưu file bác sĩ
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
//Đọc file bác sĩ
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
