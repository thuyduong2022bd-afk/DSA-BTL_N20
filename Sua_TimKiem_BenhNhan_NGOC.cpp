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