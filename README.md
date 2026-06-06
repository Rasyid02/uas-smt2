# Swift Expedition - C++

> Project UAS matakuliah Algoritma Pemrograman & Struktur Data  
> CLI program dengan bahasa C++

---

## Overview

**Swift Expedition** adalah aplikasi cli pengelolaan sistem ekspedisi dengan multi-role yang memenuhi persyaratan uas. Program ini memiliki 4 role dummy: **CEO**, **Manager**, **Admin**, dan **Kurir**,.Fitur pada program ini: manajemen paket, tracking pengiriman, dan visualisasi rute antar kota.

### Daftar Syarat uas yang Wajib dipenuhi

| No | Konsep | Lokasi Implementasi |
|----|--------|-------------------|
| 1 | `struct` | `models/Paket.h`, `models/Tracking.h`, `models/User.h`, dll |
| 2 | `pointer (*)` | `services/PaketService.cpp` (findPaketById, dll) |
| 3 | `reference (&)` | Parameter fungsi service & menu |
| 4 | `namespace` | `namespace SwiftExpedition` di seluruh file |
| 5 | callback function | `services/PaketService.cpp` (lambda pada sorting) |
| 6 | default argument | `storage/CSVManager.h` (`skipHeader = true`) |
| 7 | inline function | `models/Role.h` (`roleToString`) |
| 8 | function overloading / template | `services/PaketService.h` (`sortByBiaya`, `sortByBerat`) |
| 9 | exception handling | `storage/CSVManager.cpp` (try/catch/throw) |
| 10 | STL vector/list | `services/PaketService.h` (vector\<Paket\>, vector\<Kurir\>) |
| 11 | iterator | `menus/AdminMenu.cpp` (range-for loop) |
| 12 | sort | `services/PaketService.cpp` (std::sort) |
| 13 | find | `services/AuthService.cpp` (std::find_if) |
| 14 | count | `services/ReportService.cpp` (std::count_if) |
| 15 | file handling | `storage/CSVManager.cpp` (ifstream/ofstream) |
| 16 | lambda expression | `services/PaketService.cpp` (lambda di std::sort) |

### Penggunaan Struktur Data

| No | Struktur Data | File | Fungsi |
|----|--------------|------|--------|
| 1 | Singly Linked List | `datastructures/SinglyLinkedList.h` | Master data paket |
| 2 | Circular Linked List | `datastructures/CircularLinkedList.h` | Rotasi kurir |
| 3 | Stack | `datastructures/Stack.h` | Undo status tracking |
| 4 | Queue | `datastructures/Queue.h` | Antrian paket masuk |
| 5 | AVL Tree | `datastructures/AVLTree.h` | Pencarian nomor resi |
| 6 | Graph | `datastructures/Graph.h` | Jalur antar kota |
| 7 | BFS | `services/RoutingService.cpp` | Jalur transit terdekat |
| 8 | DFS | `services/RoutingService.cpp` | Semua jalur dari kota |
| 9 | Hash Table | `services/AuthService.cpp` | Login user (unordered_map) |
| 10 | Sorting | `services/PaketService.cpp` | Laporan paket |

---

## Struktur Folder

```
swift_expedition_uas/
│
├── main.cpp                          # Entry point program
├── main.exe                          # Executable hasil kompilasi
│
├── models/                           # Model data (struct)
│   ├── Role.h                        # Enum RoleType + inline roleToString
│   ├── User.h                        # Struct User (login)
│   ├── Paket.h                       # Struct Paket (data pengiriman)
│   ├── Tracking.h                    # Struct Tracking (riwayat pengiriman)
│   ├── Kurir.h                       # Struct Kurir
│   ├── Layanan.h                     # Struct Layanan (jenis pengiriman)
│   ├── KlasifikasiBerat.h           # Struct KlasifikasiBerat
│   └── Kota.h                        # Struct Kota (untuk graph)
│
├── datastructures/                   # Implementasi struktur data
│   ├── SinglyLinkedList.h            # Singly Linked List
│   ├── CircularLinkedList.h          # Circular Linked List
│   ├── Stack.h                       # Stack
│   ├── Queue.h                       # Queue
│   ├── AVLTree.h                     # AVL Tree (pencarian resi)
│   ├── Graph.h                       # Graph (jalur antar kota)
│   └── HashTable.h                   # Hash Table utility
│
├── services/                         # Business logic
│   ├── AuthService.h/.cpp            # Autentikasi & login
│   ├── PaketService.h/.cpp           # CRUD paket, sorting, queue, AVL
│   ├── TrackingService.h/.cpp        # Tracking & undo stack
│   ├── ReportService.h/.cpp          # Laporan & statistik
│   └── RoutingService.h/.cpp         # Graph BFS/DFS routing
│
├── menus/                            # Menu UI per role
│   ├── LoginMenu.h/.cpp              # Menu login
│   ├── AdminMenu.h/.cpp              # Menu Admin
│   ├── KurirMenu.h/.cpp              # Menu Kurir
│   ├── ManagerMenu.h/.cpp            # Menu Manager
│   └── CeoMenu.h/.cpp                # Menu CEO
│
├── storage/                          # File handling
│   └── CSVManager.h/.cpp             # Baca/tulis file CSV
│
└── data/                             # Data CSV
    ├── anggota.csv                    # Data user login
    ├── paket.csv                      # Data paket
    ├── tracking.csv                   # Riwayat tracking
    ├── layanan.csv                    # Jenis layanan
    ├── klasifikasi.csv               # Klasifikasi berat
    ├── kota.csv                       # Jalur antar kota (graph)
    └── kurir.csv                      # Data kurir
```

---

## Cara Penggunaan
_*Compile program dengan command ini sebelum melakukan execution_:
```bash
g++ main.cpp menus/*.cpp services/*.cpp storage/*.cpp -o main.exe
```

### 1. Start Program

```bash
./main.exe
```

### 2. Login

Gunakan salah satu data dummy:

| Username | Password | Role    |
|----------|----------|---------|
| zaky     | pass123  | CEO     |
| budi     | pass123  | Manager |
| andi     | pass123  | Admin   |
| citra    | pass123  | Kurir   |
| doni     | pass123  | Kurir   |
| eka      | pass123  | Kurir   |
| admin    | pass1    | Admin   |
| manager  | pass1    | Manager |
| ceo      | pass1    | CEO     |

### 3. Menu Per Role

#### Menu Admin
- Tambah paket baru (otomatis masuk queue)
- Hapus / Edit paket
- Cari paket by ID / Resi
- Lihat semua paket, antrean, kurir, layanan, klasifikasi
- Lihat AVL Tree (pencarian resi terstruktur)

#### Menu Kurir
- Ambil paket dari queue (FIFO)
- Update status tracking paket
- Undo tracking (Stack — rollback ke status sebelumnya)
- Lihat riwayat tracking

#### Menu Manager
- Laporan semua paket
- Laporan paket berdasarkan status
- Sorting paket berdasarkan biaya (asc/desc)
- Sorting paket berdasarkan berat (asc/desc)
- Statistik pengiriman

#### Menu CEO
- Laporan keseluruhan (CEO Report)
- Statistik pengiriman
- Lihat Graph rute antar kota
- **BFS** — Cari jalur terpendek antar kota
- **DFS** — Tampilkan semua rute dari suatu kota
- Cari semua jalur antar kota
- Simpan data ke CSV

### 4. Exit Program

Pilih opsi **99** di dalam menu role mana pun, atau **0** untuk logout dan kembali ke login.

---

## Format CSV

### anggota.csv
```
username;password;nama;role
```

### paket.csv
```
id;resi;nama_penerima;alamat_tujuan;kota_asal;kota_tujuan;berat;id_layanan;id_klasifikasi;biaya;status;id_kurir
```

### tracking.csv
```
id;id_paket;status;lokasi;waktu
```

### layanan.csv
```
id;nama;tarif_per_kg
```

### klasifikasi.csv
```
id;nama;biaya_tambahan
```

### kota.csv
```
id;nama
<kota1>;<kota2>;<jarak>
```

### kurir.csv
```
id;nama;no_hp;kendaraan;status
```

---

## _Notes_

- Semua data disave pada folder `data/` dan dibaca saat program dimulai
- Data otomatis tersimpan ke CSV saat user logout
- Program menggunakan delimiter `;` (titik koma) untuk file CSV
- Kompatibel dengan Windows (g++ MinGW-w64)