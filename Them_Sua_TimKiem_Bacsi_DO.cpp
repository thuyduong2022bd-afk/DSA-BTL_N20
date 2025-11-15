void QuanLyBacSi(vector<BacSi>& dsBacSi) {
    int chon;
    do {
        cout << "\n\n";
        in_tieu_de("MENU QUAN LI BAC SI (ADMIN)", 13, 80); // H?ng sáng
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Dùng b?n m?nh hon
            chon = -1;
            doi_mau(12); cout << "    [LOI] Vui long nhap 1 so tu 0->6.\n"; doi_mau(7);
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I

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
                cout << "  - Ma so: "; cin >> bs.maSo; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
                
                for (const auto& existingBs : dsBacSi) {
                    if (existingBs.maSo == bs.maSo) {
                        doi_mau(12); cout << "    [LOI] Ma so bac si [" << bs.maSo << "] da ton tai. Them that bai.\n"; doi_mau(7);
                        goto skip_add;
                    }
                }
                
                cout << "  - Ho ten: "; getline(cin, bs.hoTen);
                while(true) {
                    int maKhoa;
                    cout << "  - Chuyen khoa (Ma 1:RANG, 2:MAT,3:NOI,4:NHI,5:NGOAI): "; cin >> maKhoa; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
                    const char* tenChuyenKhoa[] = {"", "RANG", "MAT", "NOI", "NHI", "NGOAI"};
                    if (maKhoa >= 1 && maKhoa <= MAX_CHUYENKHOA) {
                        bs.chuyenKhoa = tenChuyenKhoa[maKhoa]; 
                        break;
                    }
                    doi_mau(12); cout << "    [LOI] Ma chuyen khoa khong hop le. Nhap lai.\n"; doi_mau(7);
                }
                while(true) {
                    cout << "  - Ca lam viec (1:Sang, 2:Chieu, 3:Ca ngay): "; cin >> bs.caLamViec; 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
                    if (bs.caLamViec >= 1 && bs.caLamViec <= 3) break;
                    doi_mau(12); cout << "    [LOI] Ma ca lam viec khong hop le. Nhap lai.\n"; doi_mau(7);
                }
                cout << "  - Phong kham: "; cin >> bs.phongKham; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I

                dsBacSi.push_back(bs);
                doi_mau(10); cout << "\n>>> THEM BAC SI [" << bs.hoTen << "] THANH CONG. So luong hien tai: " << dsBacSi.size() << endl; doi_mau(7);
                skip_add:;
                tam_dung();
                break;
            }
            case 3: { 
                doi_mau(11); cout << "\n>> XOA BAC SI:\n"; doi_mau(7);
                cout << "  - Nhap MA SO Bac si can xoa: ";
                int maXoa; cin >> maXoa; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
                
                auto it = dsBacSi.begin();
                bool timDuoc = false;
                while (it != dsBacSi.end()) {
                    if (it->maSo == maXoa) {
                        timDuoc = true;
                        doi_mau(14); cout << "  - Xac nhan xoa BS: " << it->hoTen << " (y/n)? "; doi_mau(7);
                        char confirm; cin >> confirm; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
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
                int maSua; cin >> maSua; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
                
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
                        cout << "  - Nhap Chuyen khoa moi (Ma 1:RANG, 2:MAT,3:NOI,4:NHI,5:NGOAI) | Hien tai: " << bs.chuyenKhoa << "): "; cin >> maKhoaMoi; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
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
                    cout << "  - Nhap phong kham moi (" << bs.phongKham << "): "; cin >> bs.phongKham; cin.ignore(numeric_limits<streamsize>::max(), '\n'); // S?A L?I
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

                // Ð?m tru?c d? h?i xác nh?n
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
