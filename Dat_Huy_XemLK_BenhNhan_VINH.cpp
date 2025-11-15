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
