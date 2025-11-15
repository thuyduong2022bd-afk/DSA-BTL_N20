 //Xem danh sách bác sĩ
 cout << "[Ma] " << left << setw(20) << "Ho Ten" << "| Khoa  | Ca        | P.Kham" << endl;
                in_phan_cach();
                for (size_t i = 0; i < dsBacSi.size(); ++i) { 
                    printBacSi(dsBacSi[i]);
                }
                in_tieu_de("KET THUC DANH SACH", 14, 80);
                tam_dung();
                break;
//Xóa Bác sĩ theo mã số
cout << "\n>> XOA BAC SI:\n"; doi_mau(7);
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
//Xóa Bác sĩ theo chuyên khoa
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