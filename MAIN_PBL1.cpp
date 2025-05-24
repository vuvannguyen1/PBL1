#include <graphics.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <limits.h>
#include <conio.h>

#define MAXN 100
#define R 15

struct ToaDo {
    int x, y;
};

struct Canh {
    int a, b, w;
};

//===== BIEN TOAN CUC =====
int cur, da_ve = 0;
int n, m; // So canh va so dinh cua do thi
int adj[MAXN][MAXN]; // Ma tran ke
int used[MAXN]; // Mang danh dau dinh da su dung
int parent[MAXN]; // Mang phuc vu cau truc Union-Find
int so_canh = 0, tongTrongSo = 0;
ToaDo dinh[MAXN];
Canh ds_canh[MAXN * MAXN];
Canh MST[MAXN];
// ===== CAC HAM GIAO DIEN =====
void clearInputBuffer();
void setColor(int color);
void gotoxy(int x, int y);
void GioiThieu();
void Khung();
void MenuQuayLai(int x, int y);
void MenuNhapLieu();
void NhapDuLieuTuBanPhim();
void NhapDuLieuTuFile();
void NhapDuLieuTuDoThi();
void MenuChonThuatToan(int x, int y);
void xuatKetQua(Canh MST[], int so_canh, int tongTrongSo);
void mo_file(const char *tenfile);
//==== CAC HAM VE DO THI ====
float khoang_cach(int x1, int y1, int x2, int y2);
void ve_duong(int x1, int y1, int x2, int y2, int mau);
void in_trong_so(int x1, int y1, int x2, int y2, int w);
void nhap_dinh();
void nhap_canh();
void tu_dong_ve_dinh(int n, ToaDo ds[], int center_x, int center_y, int radius);
void tu_dong_ve_canh(int n, ToaDo ds[], int adj[MAXN][MAXN]);
// ===== CAC HAM XU LI CHINH =====
void Prim();
void Kruskal();
int KiemTraLienThong();
// ===== HAM HO TRO THUAT TOAN =====
int find(int x);
void unionSet(int x, int y);
int cmp(const void *a, const void *b);

// ==== HAM MAIN ======
int main() {
	SetConsoleOutputCP(CP_UTF8);
	GioiThieu();
    MenuNhapLieu();
    return 0;
}

// ===== CAC HAM GIAO DIEN =====
void clearInputBuffer() {
    int c; 
    while ((c = getchar()) != '\n' && c != EOF);
}

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void GioiThieu() {
	int i;
	gotoxy(10, 2);
	setColor(3);
	printf("╔");
	for (i = 0; i < 97; i++) printf("═");
	printf("╗");
	for (i = 1; i < 20; i++) {
		gotoxy(10, 2 + i);
		printf("║");
		gotoxy(108, 2 + i);
		printf("║");
	}
	gotoxy(10, 2 + i);
	printf("╚");
	for (i = 0; i < 97; i++) {
		if (i == 41) printf("╩");
		else printf("═");
	}
	printf("╝");
	gotoxy(10, 9);
	printf("╠");
	for (i = 0; i < 97; i++) printf("═");
	printf("╣");
	
	setColor(11);
	gotoxy(49, 3);
	printf("╔");
	for (i = 0; i < 15; i++) printf("═");
	printf("╗");
	gotoxy(49, 4);
	printf("║");
	gotoxy(65, 4);
	printf("║");
	gotoxy(49, 5);
	printf("╚");
	for (i = 0; i < 15; i++) printf("═");
	printf("╝");

	setColor(3);
	gotoxy(52, 9);
	printf("╦");
	for (i = 1; i <= 12; i++) {
		gotoxy(52, 9 + i);
		printf("║");
	}

	gotoxy(10, 13);
	printf("╠");
	for (i = 0; i < 97; i++) {
		if (i == 41) printf("╬");
		else printf("═");
	}
	printf("╣");
	
	setColor(11);
	gotoxy(25, 10);
	printf("╔");
	for (i = 0; i < 11; i++) printf("═");
	printf("╗");
	gotoxy(25, 11);
	printf("║");
	gotoxy(37, 11);
	printf("║");
	gotoxy(25, 12); 
	printf("╚");
	for (i = 0; i < 11; i++) printf("═");
	printf("╝");
	
	gotoxy(68, 10);
	printf("╔");
	for (i = 0; i < 23; i++) printf("═");
	printf("╗");
	gotoxy(68, 11);
	printf("║");
	gotoxy(92, 11);
	printf("║");
	gotoxy(68, 12); 
	printf("╚");
	for (i = 0; i < 23; i++) printf("═");
	printf("╝");
	
	setColor(3);
	gotoxy(52, 19);
	printf("╠");
	for (i = 0; i < 55; i++) printf("═");
	printf("╣");
	
	gotoxy(51, 4);
	printf("NHÓM 24.Nh15A");
	gotoxy(33, 6);
	setColor(10);
	printf("ĐỀ TÀI: XÂY DỰNG CHƯƠNG TRÌNH TÌM CÂY KHUNG NHỎ NHẤT");
	gotoxy(37, 7);
	printf("CỦA ĐỒ THỊ BẰNG THUẬT TOÁN KRUSKAL VÀ PRIM");
	setColor(3);
	gotoxy(29, 11);
	printf("Mô tả");
	gotoxy(73, 11);
	printf("Người thực hiện");
	setColor(10);
	gotoxy(58, 14);
	printf("1. NGUYỄN THỊ CẨM TUYỀN");
	gotoxy(59, 15);
	setColor(2);
	printf("LỚP: 24T_KHDL               MSSV: 102240286");
	setColor(10);
	gotoxy(58, 17);
	printf("2. NGUYỄN VĂN VŨ");
	gotoxy(59, 18);
	setColor(2);
	printf("LỚP: 24T_KHDL               MSSV: 102240290");
	setColor(12);
	gotoxy(60, 20);
	printf("GIẢNG VIÊN HƯỚNG DẪN: TS. NGUYỄN VĂN HIỆU");
	gotoxy(60, 21);
	printf("======= ĐẠI HỌC BÁCH KHOA ĐÀ NẴNG =======");
	
	setColor(6);
	gotoxy(12, 14);
	printf("- Nhập dữ liệu từ bàn phím, từ file");
	gotoxy(12, 15);
	printf("  hoặc từ đồ thị.");
	gotoxy(12, 16);
	printf("- Chọn thuật toán để tìm cây khung");
	gotoxy(12, 17);
	printf("  nhỏ nhất: ");
	gotoxy(13, 17);
	printf("+ KRUSKAL: Phù hợp với đồ thị thưa.");
	gotoxy(13, 18);
	printf("+ PRIM: Phù hợp với đồ thị dày,");
	gotoxy(13, 19);
	printf("cần chọn 1 đỉnh bắt đầu.");
	gotoxy(12, 20);
	printf("- Xuất kết quả ra màn hình,");
	gotoxy(12, 21);
	printf("  ra file hoặc đồ thị.");
	
	setColor(8);
	gotoxy(10, 24);
	printf("Nhấn phím bất kỳ để tiếp tục chương trình...");
	_getch();          
}

void Khung() {	
    setColor(9);
    gotoxy(10, 1);
    printf("╔");
    for (int i = 0; i < 80; i++) printf("═");
    printf("╗");
    for (int i = 2; i < 8; i++) {
        gotoxy(10, i);
        printf("║");
        gotoxy(91, i);
        printf("║");
    }
    gotoxy(10, 8);
    printf("╚");
    for (int i = 0; i < 80; i++) printf("═");
    printf("╝");

    gotoxy(37, 2);
    setColor(12);
    printf("ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN");
    gotoxy(37, 3);
    printf("ĐỀ TÀI: Cây khung nhỏ nhất");
    gotoxy(19, 5);
    setColor(14);
    printf("Sinh viên thực hiện");
    gotoxy(20, 6);
    printf("- Nguyễn Thị Cẩm Tuyền");
    gotoxy(20, 7);
    printf("- Nguyễn Văn Vũ");
    gotoxy(59, 5);
    printf("Giảng viên hướng dẫn:");
    gotoxy(60, 6);
    printf("TS. Nguyễn Văn Hiệu");
    gotoxy(10, 10);
}

void MenuQuayLai(int x, int y) {
    char check;
    int choice;
    char input[100];

    setColor(11);
    gotoxy(30 + x, 15 + y);
    printf("╔");
    for (int i = 0; i < 40; i++) printf("═");
    printf("╗");
    for (int i = 1; i < 5; i++) {
        gotoxy(30 + x, i + 15 + y);
        printf("║");
        gotoxy(71 + x, i + 15 + y);
        printf("║");
    }
    gotoxy(30 + x, 20 + y);
    printf("╚");
    for (int i = 0; i < 40; i++) printf("═");
    printf("╝");

    gotoxy(40 + x, 17 + y);
    printf("1. Quay lại menu chính");
    gotoxy(40 + x, 18 + y);
    printf("0. Thoát chương trình");
    gotoxy(10 + x, 21 + y);

    setColor(8);
    printf("Nhập yêu cầu:                                            ");
    gotoxy(24 + x, 21 + y);
    fgets(input, sizeof(input), stdin);
    while (sscanf(input, "%d %c", &choice, &check) != 1) {
        setColor(4);
        gotoxy(10 + x, 22 + y);
        printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
        Sleep(1000);
        gotoxy(10 + x, 21 + y);
        setColor(8);
        printf("Nhập yêu cầu:                                            ");
        setColor(0);
        gotoxy(10 + x, 22 + y);
        printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
        gotoxy(24 + x, 21 + y);
        setColor(8);
        fgets(input, sizeof(input), stdin);
    }

    switch (choice) {
        case 1:
            system("cls");
            Khung();
            MenuNhapLieu();
            break;
        case 0:
            system("cls");
            Khung();
            setColor(10);
            printf("Nhấn phím bất kỳ để thoát chương trình!");
            setColor(8);
            gotoxy(10, 11);
            exit(0);
        default:
            setColor(4);
            gotoxy(10, 22 + y);
            printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
            Sleep(1000);
            setColor(0);
            gotoxy(10, 22 + y);
            printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
            MenuQuayLai(x, y);
    }
}

void MenuNhapLieu() {
	char check;
    int choice;
    char input[100];

    do {
        system("cls");
        Khung();
        setColor(11);
        gotoxy(38, 10);
        printf("<   NHẬP DỮ LIỆU   >");
        gotoxy(41, 11);
        printf("1. Từ bàn phím");
        gotoxy(41, 12);
        printf("2. Từ file");
        gotoxy(41, 13);
        printf("3. Từ đồ thị");
        gotoxy(41, 14);
        printf("0. Thoát");
        gotoxy(39, 15);
        printf("------------------");

        gotoxy(10, 17);
        setColor(8);
        printf("Nhập yêu cầu: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d %c", &choice, &check) != 1) {
        	gotoxy(10, 18);
        	setColor(4);
        	printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
        	Sleep(1000);
        	continue;
		}

        switch (choice) {
            case 1: {
                system("cls");
                Khung();
                setColor(10);
                printf("Bạn đã chọn nhập dữ liệu từ bàn phím!\n");
                NhapDuLieuTuBanPhim();
                MenuChonThuatToan(0, -4);
                break;
            }
            case 2: {
                system("cls");
                Khung();
                setColor(10);
                printf("Bạn đã chọn nhập dữ liệu từ file!\n");
                NhapDuLieuTuFile();
                MenuChonThuatToan(0, -4);
                break;
            }
            case 3: {
                system("cls");
                Khung();
                setColor(10);
                printf("Bạn đã chọn nhập dữ liệu từ đồ thị!\n");
                NhapDuLieuTuDoThi();
                da_ve = 1;
                MenuChonThuatToan(0, -4);
                break;
			}
            case 0: {
                system("cls");
                Khung();
                setColor(10);
                printf("Nhấn phím bất kỳ để thoát chương trình!");
                setColor(8);
                exit(0);
            }
            default: {
                setColor(4);
                gotoxy(10, 18);
                printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
                Sleep(1000);
            }
        }
    } while (1);
}

void NhapDuLieuTuBanPhim() {
    gotoxy(10, 12);
    setColor(14);
    char check;

    int i, j;
    char inputV[100];
    char inputE[100];
    printf("Nhập số đỉnh: ");
    fgets(inputV, sizeof(inputV), stdin);

    if (sscanf(inputV, "%d %c", &n, &check) != 1 || n <= 0 || n > MAXN) {
        setColor(4);
        gotoxy(10, 13);
        printf("Dữ liệu nhập vào không hợp lệ, vui lòng kiểm tra lại!!!\n");
        MenuQuayLai(0, 0);
        return;
    }
    gotoxy(10, 13);
    printf("Nhập số cạnh: ");
    fgets(inputE, sizeof(inputE), stdin);

    if (sscanf(inputE, "%d %c", &m, &check) != 1 || m <= 0 || m > n * (n - 1) / 2) {
        setColor(4);
        gotoxy(10, 14);
        printf("Dữ liệu nhập vào không hợp lệ, vui lòng kiểm tra lại!!!\n");
        MenuQuayLai(0, 1);
        return;
    }

    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            adj[i][j] = INT_MAX;
        }
        used[i] = FALSE;
    }

    gotoxy(10, 14);
    printf("NHẬP DANH SÁCH CẠNH (Đỉnh 1 - Đỉnh 2 - Trọng số):\n");
    gotoxy(10, 15);
    int cur = 15;
    char temp[100];
    for (i = 0; i < m; i++) {
    	fgets(temp, sizeof(temp), stdin);
        int x, y, w;
        
        if (sscanf(temp, "%d %d %d %c", &x, &y, &w, &check) != 3 || x <= 0 || y <= 0 || w < 0) {
        	setColor(4);
            gotoxy(10, ++cur);
            printf("Dữ liệu nhập vào không hợp lệ, vui lòng kiểm tra lại!!!\n");
            Sleep(1000);
            system("cls");
            Khung();
            MenuQuayLai(0, -5);
            return;
		}

        if (w < adj[x][y]) {
            adj[x][y] = w;
            adj[y][x] = w;
        }

        gotoxy(10, ++cur);
    }

    setColor(10);
    printf("Dữ liệu đã được nhập thành công!\n");
    Sleep(1000);
    gotoxy(10, 16 + i);
    setColor(14);
    printf("KẾT QUẢ KIỂM TRA LIÊN THÔNG: ");
    if (KiemTraLienThong()) {
        setColor(10);
        printf("Đồ thị liên thông!\n");
    } 
    else {
        setColor(4);
        printf("Đồ thị không liên thông!\n");
        gotoxy(10, 17 + i);
        printf("Không thể tạo cây khung nhỏ nhất!");
        MenuQuayLai(0, m + 4);
    }
    gotoxy(10, 17 + i);
    setColor(8);
    system("pause");
}   

void NhapDuLieuTuFile() {
    char check;
    int choice;
    char input[100];
    char filename[100];
    FILE *f;

    while (1) {
        system("cls");
        Khung();
        setColor(11);
        gotoxy(35, 10);
        printf("<   CHỌN FILE ĐỂ LẤY DỮ LIỆU   >");
        gotoxy(32, 11);
        printf("1. CHỌN TEST1                6. CHỌN TEST6");
        gotoxy(32, 12);
        printf("2. CHỌN TEST2                7. CHỌN TEST7");
        gotoxy(32, 13);
        printf("3. CHỌN TEST3                8. CHỌN TEST8");
        gotoxy(32, 14);
        printf("4. CHỌN TEST4                9. CHỌN TEST9");
        gotoxy(32, 15); 
        printf("5. CHỌN TEST5                10. CHỌN TEST10");
        gotoxy(32, 16);
        printf("11. NHẬP FILE MỚI            0. QUAY LẠI MENU CHÍNH");

        gotoxy(10, 18);
        setColor(8);
        printf("Nhập yêu cầu: ");
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d %c", &choice, &check) != 1) {
        	setColor(4);
        	gotoxy(10, 19);
            printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
            Sleep(1000);
            continue;
		}

        if (choice == 0) {
            system("cls");
            Khung();
            MenuNhapLieu();
            return;
        }

        if (choice < 0 || choice > 11) {
            setColor(4);
            gotoxy(10, 19);
            printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
            Sleep(1000);
            continue;
        }

		char dulieu[100];
        int check = 0;

		if (choice == 11) {
			setColor(14);
            gotoxy(10, 20);
			printf("Nhập tên file: ");
			scanf("%s", dulieu);
			sprintf(filename, "%s.txt", dulieu);
            clearInputBuffer();
		}
		else sprintf(filename, "test%d.txt", choice);

        f = fopen(filename, "r");
        if (!f) {
            setColor(12);
            gotoxy(10, 22);
            printf("LỖI: Không thể mở file %s!", filename);
            Sleep(1000);
            continue;
        }
        
        mo_file(filename);

        if (fscanf(f, "%d %d", &n, &m) != 2 || n <= 0 || m <= 0 || n > MAXN || m > n * (n - 1) / 2) 
            check = 1;
	
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                adj[i][j] = INT_MAX;
            }
            used[i] = FALSE;
        }
        
        for (int i = 0; i < m; i++) {
            int u, v, w;
            if (fscanf(f, "%d %d %d", &u, &v, &w) != 3 || u <= 0 || v <= 0 || w < 0 || u > n || v > n) {
            	check = 1;
                continue;
			}
                
        	if (w < adj[u][v]) {
            	adj[u][v] = w;
            	adj[v][u] = w;
            }
        }
        
        fclose(f);

        system("cls");
        Khung();
        gotoxy(10, 10);
        setColor(10);
        int y = 15;
        if (!check) {
            printf("Đọc file thành công! Dữ liệu đồ thị như sau:");
            gotoxy(10, 12);
            setColor(14);
            printf("Số đỉnh: %d, Số cạnh: %d", n, m);
            gotoxy(10, 14);
            printf("Danh sách cạnh:");
            for (int i = 1; i <= n; i++) {
                for (int j = i + 1; j <= n; j++) {
                    if (adj[i][j] != INT_MAX) {
                        gotoxy(10, y++);
                        printf("%3d - %-3d : %5d", i, j, adj[i][j]);
                    }
                }
            }

            gotoxy(10, y + 1);
            Sleep(1000);
            setColor(14);
            printf("KẾT QUẢ KIẾM TRA LIÊN THÔNG: ");
            if (KiemTraLienThong()) {
                setColor(10);
                printf("Đồ thị liên thông!");
            } 
            else {
                setColor(4);
                printf("Đồ thị không liên thông!");
                gotoxy(10, y + 2);
                printf("Không thể tạo cây khung nhỏ nhất!");
                MenuQuayLai(0, y - 11);
            }

        }

        else {
            setColor(4);
            gotoxy(10, y - 5);
            printf("Dữ liệu trong file không hợp lệ!");
            gotoxy(10, y - 4);
            printf("Vui lòng kiếm tra lại!!!");
            MenuQuayLai(0, y - 16);
            Sleep(1000);
        }

        setColor(8);
        gotoxy(10, y + 2);
        system("pause");
        MenuChonThuatToan(0, -4);
        break;
    }
}

void NhapDuLieuTuDoThi() {
    char check;
	initwindow(800, 1200);
	settextstyle(10, HORIZ_DIR, 1);	
  	char vertex[100];
    char edge[100];
    setColor(14);
    gotoxy(10, 12);
    printf("Nhập số đỉnh: ");
    fgets(vertex, sizeof(vertex), stdin);
    if (sscanf(vertex, "%d %c", &n, &check) != 1 || n <= 0 || n > MAXN) {
    	closegraph();
    	setColor(4);
    	gotoxy(10, 13);
    	printf("Dữ liệu nhập vào không hợp lệ, vui lòng kiếm tra lại!!!\n");
    	Sleep(1000);
    	system("cls");
    	Khung();
        MenuQuayLai(0, -5);
        return;
	}
    nhap_dinh();
    gotoxy(10, 13);
    printf("Nhập số cạnh: ");
    fgets(edge, sizeof(edge), stdin);
    if (sscanf(edge, "%d %c", &m, &check) != 1 || m <= 0 || m > n * (n - 1) / 2) {
    	closegraph();
    	setColor(4);
    	gotoxy(10, 14);
    	printf("Dữ liệu nhập vào không hợp lệ, vui lòng kiếm tra lại!!!\n");
    	Sleep(1000);
    	system("cls");
    	Khung();
        MenuQuayLai(0, -5);
        return;
	}
    nhap_canh();
    
    setColor(10);
    printf("Đồ thị đã được nhập thành công!!!");
    Sleep(1000);
    
    gotoxy(10, ++cur);
    setColor(14);
    printf("KẾT QUẢ KIẾM TRA LIÊN THÔNG: ");
    if (KiemTraLienThong()) {
        setColor(10);
        printf("Đồ thị liên thông!\n");
    } 
    else {
        setColor(4);
        printf("Đồ thị không liên thông!\n");
        gotoxy(10, ++cur);
        printf("Không thể tạo cây khung nhỏ nhất!");
        Sleep(2000);
        closegraph();
        system("cls");
        Khung();
        MenuQuayLai(0, -5);
    }
    gotoxy(10, ++cur);
    setColor(8);
    system("pause");
}

void MenuChonThuatToan(int x, int y) {
    char check;
    int choice;
    char input[100];
    
    do {
        system("cls");
        Khung();
        
        setColor(11);
        gotoxy(30 + x, 15 + y);
        printf("╔");
        for (int i = 0; i < 40; i++) printf("═");
        printf("╗");
        
        for (int i = 1; i < 6; i++) {
            gotoxy(30 + x, i + 15 + y);
            printf("║");
            gotoxy(71 + x, i + 15 + y);
            printf("║");
        }
        
        gotoxy(30 + x, 21 + y);
        printf("╚");
        for (int i = 0; i < 40; i++) printf("═");
        printf("╝");
        
        gotoxy(28 + x, 14 + y);
        printf("< CHỌN THUẬT TOÁN ĐỂ TÌM CÂY KHUNG NHỎ NHẤT >");
        
        gotoxy(37 + x, 17 + y);
        printf("1. Chọn thuật toán Prim");
        gotoxy(37 + x, 18 + y);
        printf("2. Chọn thuật toán Kruskal");
        gotoxy(37 + x, 19 + y);
        printf("0. Quay lại menu chính");
        
        gotoxy(10 + x, 22 + y);
        setColor(8);
        printf("Nhập yêu cầu: ");
        
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d %c", &choice, &check) != 1) {
        	setColor(4);
            gotoxy(10 + x, 23 + y);
            printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
            Sleep(1000);
            continue;
        }
        
        switch (choice) {
            case 1: {
                system("cls");
                Khung();
                setColor(10);
                gotoxy(10, 10);
                printf("Bạn đã chọn thuật toán Prim!");
                Sleep(1000);
                Prim();
                break;
            }
            case 2: {
                system("cls");
                Khung();
                setColor(10);
                gotoxy(10, 10);
                printf("Bạn đã chọn thuật toán Kruskal!");
                Sleep(1000);
                Kruskal();
                break;
            }
            case 0: {
            	if (da_ve) {
                    closegraph();
                    da_ve = 0;
                }
                system("cls");
                Khung();
                MenuNhapLieu();
                return;
            }
            default: {
                setColor(4);
                gotoxy(10 + x, 23 + y);
                printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
                Sleep(1000);
            }
        }
    } while (1);
}

void xuatKetQua(Canh MST[], int so_canh, int tongTrongSo) {
	char check;
    char input[100];
    int choice;

    do {
        setColor(11);
        gotoxy(28, 15);
        printf("╔");
        for (int i = 0; i < 46; i++) printf("═");
        printf("╗");
        for (int i = 1; i < 6; i++) {
            gotoxy(28, i + 15);
            printf("║");
            gotoxy(75, i + 15);
            printf("║");
        }
        gotoxy(28, 21);
        printf("╚");
        for (int i = 0; i < 46; i++) printf("═");
        printf("╝");
        gotoxy(38, 14);
        printf("< CHỌN CÁCH XUẤT KẾT QUẢ >");
        gotoxy(33, 16);
        printf("1. Xuất kết quả trực tiếp ra màn hình");
        gotoxy(33, 17);
        printf("2. Xuất kết quả theo từng bước");
        gotoxy(33, 18);
        printf("3. Xuất kết quả vào file output.txt");
        gotoxy(33, 19);
        printf("4. Xuất kết quả ra đồ thị");
        gotoxy(33, 20);
        printf("0. Quay lại menu chính");

        gotoxy(10, 23);
        setColor(8);
        printf("Nhập yêu cầu:                                      ");
        gotoxy(24, 23);
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d %c", &choice, &check) != 1) {
        	setColor(4);
            gotoxy(10, 24);
            printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
            Sleep(1000);
            setColor(0);
            gotoxy(10, 24);
            printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
            continue;
        }

        int y = 16;
        switch (choice) {
            case 1: {
                system("cls");
                Khung();
                setColor(8);
                printf("DỮ LIỆU ĐỒ THỊ ĐÃ NHẬP VÀO:");
                gotoxy(10, 12);
                printf("Số đỉnh: %d, Số cạnh: %d", n, m);
                gotoxy(10, 14);
                printf("Danh sách cạnh:");

                for (int i = 1; i <= n; i++) {
                    for (int j = i + 1; j <= n; j++) {
                        if (adj[i][j] != INT_MAX) {
                            gotoxy(10, y++);
                            printf("%3d - %-3d : %5d", i, j, adj[i][j]);
                        }
                    }
                }
                gotoxy(10, y + 1);
                Sleep(1000);
                setColor(11);
                printf("CÂY KHUNG NHỎ NHẤT:");
                gotoxy(10, y + 3);
                printf("Danh sách cạnh:");
                for (int i = 0; i < so_canh; i++) {
                    gotoxy(10, y + 5 + i);
                    printf("%3d - %-3d : %5d", MST[i].a, MST[i].b, MST[i].w);
                }
                gotoxy(10, y + 6 + so_canh);
                printf("Tổng trọng số: %d", tongTrongSo);
                
                gotoxy(10, y + 8 + so_canh);
                setColor(8);
                system("pause");
                return;
            }
            case 2: {
                system("cls");
                Khung();
                setColor(8);
                printf("DỮ LIỆU ĐỒ THỊ ĐÃ NHẬP VÀO:");
                gotoxy(10, 12);
                printf("Số đỉnh: %d, Số cạnh: %d", n, m);
                gotoxy(10, 14);
                printf("Danh sách cạnh:");

                for (int i = 1; i <= n; i++) {
                    for (int j = i + 1; j <= n; j++) {
                        if (adj[i][j] != INT_MAX) {
                            gotoxy(10, y++);
                            printf("%3d - %-3d : %5d", i, j, adj[i][j]);
                        }
                    }
                }
                gotoxy(10, y + 1);
                Sleep(1000);
                setColor(11);
                printf("BẮT ĐẦU XÂY DỰNG CÂY KHUNG NHỎ NHẤT:");
                for (int i = 0; i < so_canh; i++) {
                    gotoxy(10, y + 3 + i);
                    system("cmd /c \"pause >nul\"");
                    printf(" + Chọn cạnh %3d - %-3d : %5d", MST[i].a, MST[i].b, MST[i].w);
                }
                gotoxy(10, y + 4 + so_canh);
                system("cmd /c \"pause >nul\"");
                printf("Tổng trọng số: %d", tongTrongSo);
                
                gotoxy(10, y + 6 + so_canh);
                setColor(8);
                system("pause");
                return;
            }
            case 3: {
                FILE *f = fopen("output.txt", "w");
                if (!f) {
                    setColor(4);
                    gotoxy(10, 25);
                    printf("Không thể tạo file output.txt!");
                    Sleep(1000);
                    continue;
                }
                
                fprintf(f, "DỮ LIỆU ĐỒ THỊ ĐÃ NHẬP VÀO:\n");
                gotoxy(10, 12);
                fprintf(f, "Số đỉnh: %d, Số cạnh: %d\n", n, m);
                gotoxy(10, 14);
                fprintf(f, "Danh sách cạnh:\n");

                for (int i = 1; i <= n; i++) {
                    for (int j = i + 1; j <= n; j++) {
                        if (adj[i][j] != INT_MAX) {
                            gotoxy(10, y++);
                            fprintf(f, "%3d - %-3d : %5d\n", i, j, adj[i][j]);
                        }
                    }
                }

                fprintf(f, "\nCây khung nhỏ nhất:\n");
                for (int i = 0; i < so_canh; i++) {
                    fprintf(f, "%3d - %-3d : %5d\n", MST[i].a, MST[i].b, MST[i].w);
                }
                fprintf(f, "Tổng trọng số: %d\n", tongTrongSo);
                fclose(f);

                setColor(10);
                gotoxy(10, 25);
                printf("Đã xuất kết quả vào file output.txt thành công!");
                Sleep(1000);
                mo_file("output.txt");
                gotoxy(10, 26);
                setColor(8);
                system("pause");
                return;
            }
            case 4: {
            	system("cls");
                Khung();
                setColor(8);
                printf("DỮ LIỆU ĐỒ THỊ ĐÃ NHẬP VÀO:");
                gotoxy(10, 12);
                printf("Số đỉnh: %d, Số cạnh: %d", n, m);
                gotoxy(10, 14);
                printf("Danh sách cạnh:");

                for (int i = 1; i <= n; i++) {
                    for (int j = i + 1; j <= n; j++) {
                        if (adj[i][j] != INT_MAX) {
                            gotoxy(10, y++);
                            printf("%3d - %-3d : %5d", i, j, adj[i][j]);
                        }
                    }
                }
                y++;
                setColor(11);
                gotoxy(10, y++);
                printf("CÂY KHUNG NHỎ NHẤT:");
                gotoxy(10, y++);
                printf("Danh sách cạnh:");
                for (int i = 0; i < so_canh; i++) {
                    gotoxy(10, y++);
                    printf("%3d - %-3d : %5d", MST[i].a, MST[i].b, MST[i].w);
                }
                y++;
                gotoxy(10, y++);
                printf("Tổng trọng số: %d", tongTrongSo);
            	if (da_ve){
            		y++;
            		gotoxy(10, y++);
            		setColor(8);
	            	printf("Nhấn phím bất kỳ để thực hiện vẽ cây khung nhỏ nhất...");
				    while (!_kbhit()) {
				        delay(100);
				    }
				    _getch();
                	for (int i = 0; i < so_canh; i++) {
        				int u = MST[i].a;
       		 			int v = MST[i].b;
        				int w = MST[i].w;
       					ve_duong(dinh[u].x, dinh[u].y, dinh[v].x, dinh[v].y, 12);
        				in_trong_so(dinh[u].x, dinh[u].y, dinh[v].x, dinh[v].y, w);
   					}

    				char s[50];

	   				sprintf(s, "Tong trong so nho nhat: %d", tongTrongSo);
	   				outtextxy(30, 500, s);
	  				setColor(10);
	                gotoxy(10, y++);
	                printf("Đã xuất kết quả vào đồ thị thành công!");
	                Sleep(1000);
	                gotoxy(10, y++);
	    			setColor(8);
	            	printf("Nhấn phím bất kỳ để tiếp tục...");
	            	_getch();
		            for (int i = 0; i < so_canh; i++) {
		        		int u = MST[i].a;
		       		 	int v = MST[i].b;
		        		int w = MST[i].w;
		       			ve_duong(dinh[u].x, dinh[u].y, dinh[v].x, dinh[v].y, 15);
		        		in_trong_so(dinh[u].x, dinh[u].y, dinh[v].x, dinh[v].y, w);
		   			}	 
	   				setcolor(0);
	   				sprintf(s, "Tong trong so nho nhat: %d", tongTrongSo);
	   				outtextxy(30, 500, s);
	   				return;
            	}
            	else {
	                initwindow(800, 1200);
				    tu_dong_ve_dinh(n, dinh, 400, 300, 200);
				    tu_dong_ve_canh(n, dinh, adj);
				    setColor(8);
				    y++;
				    gotoxy(10, y++);
				    printf("Nhấn phím bất kỳ để thực hiện vẽ cây khung nhỏ nhất...");
				    while (!_kbhit()) {
				        delay(100);
				    }
				    _getch();

				    for (int i = 0; i < so_canh; i++) {
				        int u = MST[i].a;
				        int v = MST[i].b;
				        int w = MST[i].w;
				        ve_duong(dinh[u].x, dinh[u].y, dinh[v].x, dinh[v].y, 12);
				        in_trong_so(dinh[u].x, dinh[u].y, dinh[v].x, dinh[v].y, w);
				    }

				    char s[50];
				    sprintf(s, "Tong trong so nho nhat: %d", tongTrongSo);
				    outtextxy(10, 580, s);
            	}
            	setColor(10);
            	gotoxy(10, y++);
            	printf("Đã thực hiện vẽ cây khung nhỏ nhất!!!");
            	gotoxy(10, y++);
    			setColor(8);
            	printf("Nhấn phím bất kỳ để tiếp tục...");
            	_getch();
            	closegraph();
            	return;
            }
            case 0: {
            	if (da_ve) {
                    closegraph();
                    da_ve = 0;
                }
                system("cls");
                Khung();
                MenuNhapLieu();
                return;
            }
            default:
                setColor(4);
                gotoxy(10, 24);
                printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
                Sleep(1000);
                setColor(0);
	            gotoxy(10, 24);
	            printf("Lựa chọn không hợp lệ! Vui lòng nhập lại!!!");
        }
    } while (1);
}

void mo_file(const char *tenfile) {
    ShellExecute(NULL, "open", tenfile, NULL, NULL, SW_SHOW);
}

// ===== CAC HAM VE DO THI =====
float khoang_cach(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

void ve_duong(int x1, int y1, int x2, int y2, int mau) {
	setlinestyle(0, 0, 2);
    setcolor(mau);
    line(x1, y1, x2, y2);
}

void in_trong_so(int x1, int y1, int x2, int y2, int w) {
	settextstyle(10, HORIZ_DIR, 1);	
    char s[100];
    sprintf(s, "%d", w);
    outtextxy((x1 + x2) / 2, (y1 + y2) / 2, s);
}

void nhap_dinh() {
    cleardevice();
    outtextxy(250, 20, "CLICK DE THEM DINH");

    char s[5];
    int x, y, dem = 1;
    setcolor(10);
    while (dem <= n) {
        delay(1);
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            dinh[dem].x = x;
            dinh[dem].y = y;
            circle(x, y, R);
            floodfill(x, y, 10);
            sprintf(s, "%d", dem);
            outtextxy(x - 5, y - 10, s);
            dem++;
        }
    }
}

void nhap_canh() {
    char check;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            adj[i][j] = INT_MAX;

    int x, y, d1 = 0, d2 = 0, w;
	cur = 14;
    for (int i = 1; i <= m; i++) {
        d1 = d2 = 0;
        while (d2 == 0) {
            delay(1);
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, x, y);
                for (int j = 1; j <= n; j++) {
                    if (khoang_cach(x, y, dinh[j].x, dinh[j].y) <= R) {
                        if (d1 == 0) d1 = j;
                        else if (d1 != j) d2 = j;
                    }
                }
            }
        }
		ve_duong(dinh[d1].x, dinh[d1].y, dinh[d2].x, dinh[d2].y, 15);
		gotoxy(10, cur++);
        printf("Nhập trọng số cạnh %d - %d: ", d1, d2);
        char inputW[100];
        fgets(inputW, sizeof(inputW), stdin);
        if (sscanf(inputW, "%d %c", &w, &check) != 1 || w <= 0) {
        	setColor(4);
            gotoxy(10, cur++);
            closegraph();
            printf("Dữ liệu nhập vào không hợp lệ, vui lòng kiểm tra lại!!!\n");
            Sleep(1000);
            system("cls");
            Khung();
            MenuQuayLai(0, -5);
            return;
		}
        adj[d1][d2] = adj[d2][d1] = w;
        in_trong_so(dinh[d1].x, dinh[d1].y, dinh[d2].x, dinh[d2].y, w);
    }
    gotoxy(10, cur);
}

void tu_dong_ve_dinh(int n, ToaDo ds[], int center_x, int center_y, int radius) {
    settextstyle(10, HORIZ_DIR, 1);
    outtextxy(250, 20, "NHAN PHIM BAT KY DE VE CAY KHUNG");
    float goc;
    char s[5];
    setcolor(10);
    for (int i = 1; i <= n; i++) {
        goc = 2 * M_PI * (i - 1) / n;
        ds[i].x = center_x + radius * cos(goc);
        ds[i].y = center_y + radius * sin(goc);

        circle(ds[i].x, ds[i].y, R);
        floodfill(ds[i].x, ds[i].y, 10);
        sprintf(s, "%d", i);
        outtextxy(ds[i].x - 5, ds[i].y - 10, s);
    }
}

void tu_dong_ve_canh(int n, ToaDo ds[], int adj[MAXN][MAXN]) {
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (adj[i][j] != INT_MAX) {
                ve_duong(ds[i].x, ds[i].y, ds[j].x, ds[j].y, 15);
                in_trong_so(ds[i].x, ds[i].y, ds[j].x, ds[j].y, adj[i][j]);
            }
        }
    }
}

// ===== CAC HAM XU LI CHINH =====
void Prim() {
	char check;
    int Dinh;
    char priminput[100];

    while (1) {
    	setColor(8);
    	gotoxy(10, 11);
    	printf("Chọn đỉnh bắt đầu thuật toán Prim (1 - %d):                          ", n);
    	gotoxy(53 ,11);
        fgets(priminput, sizeof(priminput), stdin);
        if (sscanf(priminput, "%d %c", &Dinh, &check) != 1 || Dinh < 1 || Dinh > n) {
        	gotoxy(10, 12);
        	setColor(4);
        	printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
        	Sleep(1000);
        	gotoxy(10, 12);
        	setColor(0);
        	printf("Lựa chọn không hợp lệ, vui lòng nhập lại!!!");
        	continue;
		}
        if (Dinh >= 1 && Dinh <= n) 
			break;
    }
    printf("\n");
    setColor(7);

    Canh MST[MAXN];
    int tongTrongSo = 0, so_canh = 0;
    
    for (int i = 1; i <= n; i++) {
        used[i] = FALSE;
    }
    
    used[Dinh] = TRUE;

    while (so_canh < n - 1) {
        int min_w = INT_MAX, X = -1, Y = -1;

        // Tim canh co trong so nho nhat noi giua dinh da dung va chua dung
        for (int i = 1; i <= n; i++) {
            if (used[i]) {
                for (int j = 1; j <= n; j++) {
                    if (!used[j] && adj[i][j] < min_w) {
                        min_w = adj[i][j];
                        X = i;
                        Y = j;
                    }
                }
            }
        }

        MST[so_canh].a = X;
        MST[so_canh].b = Y;
        MST[so_canh].w = min_w;
        so_canh++;
        tongTrongSo += min_w;
        used[Y] = TRUE;
    }
    gotoxy(10, 12);
    setColor(10);
    printf("Đã thực hiện thuật toán Prim!");
    Sleep(1000);
    xuatKetQua(MST, so_canh, tongTrongSo);
}

void Kruskal() {
    Canh ds_canh[MAXN];
    Canh MST[MAXN];
    int tongTrongSo = 0, so_canh = 0;

    // Tao danh sach tat ca cac canh
    int k = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (adj[i][j] != INT_MAX) {
                ds_canh[k].a = i;
                ds_canh[k].b = j;
                ds_canh[k].w = adj[i][j];
                k++;
            }
        }
    }

    // Sap xep cac canh theo trong so tang dan
    qsort(ds_canh, k, sizeof(Canh), cmp);

    // Khoi tao cau truc Union-Find
    for (int i = 1; i <= n; i++)
        parent[i] = i;

    // Duyet qua cac canh da sap xep
    for (int i = 0; i < k && so_canh < n - 1; i++) {
        int u = ds_canh[i].a;
        int v = ds_canh[i].b;
        int w = ds_canh[i].w;

        // Kiem tra co tao chu trinh khong
        if (find(u) != find(v)) {
            MST[so_canh++] = ds_canh[i];
            tongTrongSo += w;
            unionSet(u, v);
        }
    }
    gotoxy(10, 12);
    setColor(10);
    printf("Đã thực hiện thuật toán Kruskal!");
    xuatKetQua(MST, so_canh, tongTrongSo);
}

int KiemTraLienThong() {
    if (n <= 1) 
		return 1;
    int visited[MAXN] = {0};
    int stack[MAXN], top = -1;
    
    stack[++top] = 1;
    visited[1] = 1;

    while (top != -1) {
        int u = stack[top--];
        for (int v = 1; v <= n; v++) {
            if (adj[u][v] != INT_MAX && !visited[v]) {
                stack[++top] = v;
                visited[v] = 1;
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        if (!visited[i]) 
            return 0;
    }
    return 1;
}

// ===== HAM HO TRO THUAT TOAN =====
int find(int x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}

void unionSet(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY)
        parent[rootX] = rootY;
}

int cmp(const void *a, const void *b) {
    Canh *ca = (Canh *)a;
    Canh *cb = (Canh *)b;
    return ca->w - cb->w;
}