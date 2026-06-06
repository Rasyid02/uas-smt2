# Swift Expedition - Panduan Lengkap

> Dokumentasi lengkap proyek UAS Algoritma Pemrograman & Struktur Data

---

## Daftar Isi

1. [Penjabaran Syarat UAS](#1-penjabaran-syarat-uas)
2. [Penggunaan Struktur Data](#2-penggunaan-struktur-data)
3. [Mengapa Menggunakan Header File](#3-mengapa-menggunakan-header-file)
4. [Mengapa Struktur File Seperti Itu](#4-mengapa-struktur-file-seperti-itu)
5. [Alur Kerja Program](#5-alur-kerja-program)
6. [Guide Edit Data CSV](#6-guide-edit-data-csv)
7. [Tutorial Simulasi Semua Role](#7-tutorial-simulasi-semua-role)

---

## 1. Penjabaran Syarat UAS

Berikut adalah penjabaran dari 16 syarat UAS yang wajib dipenuhi beserta alasan mengapa diimplementasikan dengan cara tersebut:

### 1.1 `struct` — Model Data (8 file di `models/`)

**Lokasi:** `models/Paket.h`, `models/Tracking.h`, `models/User.h`, `models/Kurir.h`, `models/Layanan.h`, `models/KlasifikasiBerat.h`, `models/Kota.h`, `models/Role.h`

**Mengapa `struct`:** Struct dipilih karena model-model ini berfungsi sebagai **Plain Data Objects (PDO)** — mereka hanya menyimpan data tanpa memerlukan enkapsulasi ketat (private members). Semua field bersifat public secara default pada struct, sehingga akses data menjadi lebih sederhana dan langsung. Ini sesuai dengan prinsip "data-first design" di mana model hanya bertugas membawa data antar layer aplikasi.

**Mengapa ditulis seperti itu:** Setiap struct merepresentasikan satu entitas bisnis nyata. Contoh `Paket` memiliki field seperti `id`, `resi`, `nama_penerima`, `berat`, `biaya`, `status`, dll. yang merupakan atribut alami dari sebuah paket pengiriman. Constructor dengan default argument disediakan agar objek dapat dibuat dengan fleksibel — bisa kosong (default) atau dengan nilai tertentu.

```cpp
struct Paket {
    int id;
    string resi;
    string nama_penerima;
    // ... field lainnya
    Paket(int id = 0, const string& resi = "", ...);  // default arguments
};
```

### 1.2 `pointer (*)` — Navigasi & Akses Data

**Lokasi:** `services/PaketService.cpp` (`findPaketById`, `findPaketByResi`), `services/AuthService.cpp` (`login`)

**Mengapa pointer:** Pointer digunakan untuk **return value opsional** — fungsi dapat mengembalikan `nullptr` jika data tidak ditemukan, atau pointer ke objek jika ditemukan. Ini lebih efisien daripada mengembalikan objek copy karena:
- Tidak ada overhead copying data
- Pemanggil bisa langsung memodifikasi data asli melalui pointer
- Null-check menjadi mekanisme error handling yang sederhana

```cpp
User* AuthService::login(const string& username, const string& password) {
    User* user = userTable.find(username);
    if (user != nullptr && user->password == password) {
        return user;   // login berhasil
    }
    return nullptr;    // login gagal
}
```

### 1.3 `reference (&)` — Pass by Reference

**Lokasi:** Parameter fungsi di seluruh service & menu

**Mengapa reference:** Reference (`&`) digunakan untuk **menghindari copying** objek besar saat passing parameter ke fungsi. Berbeda dengan pointer, reference:
- Tidak bisa null (lebih aman)
- Syntax pemanggilan lebih bersih (tidak perlu `*` atau `->`)
- Menjamin objek yang di-pass valid

```cpp
void loadUsers(const string& filename);   // string tidak di-copy
bool login(const string& username, const string& password);
bool show(User* user, PaketService& paket);  // service di-pass by reference
```

### 1.4 `namespace` — Organisasi Kode

**Lokasi:** `namespace SwiftExpedition { }` di seluruh file

**Mengapa namespace:** Namespace mencegah **name collision** (konflik nama) antar modul. Semua kode proyek dibungkus dalam `namespace SwiftExpedition` sehingga nama class/function seperti `User`, `Paket`, `Graph` tidak akan bentrok dengan library eksternal atau kode lain. Ini adalah best practice C++ untuk proyek skala menengah ke atas.

```cpp
namespace SwiftExpedition {
    class PaketService { ... };
    class AuthService { ... };
}
```

### 1.5 Callback Function — Lambda di Sorting

**Lokasi:** `services/PaketService.cpp` (lambda pada `std::sort`)

**Mengapa lambda:** Lambda expression digunakan sebagai **custom comparator** untuk sorting. Lambda memungkinkan definisi fungsi pembanding secara inline tanpa perlu membuat fungsi terpisah. Ini membuat kode lebih ringkas dan logika sorting langsung terlihat di tempat pemanggilan.

```cpp
sort(pakets.begin(), pakets.end(), [](const Paket& a, const Paket& b) {
    return a.biaya < b.biaya;  // ascending by biaya
});
```

### 1.6 Default Argument — Parameter Opsional

**Lokasi:** `storage/CSVManager.h` (`skipHeader = true`), `models/Paket.h` (constructor)

**Mengapa default argument:** Default argument memberikan **fleksibilitas** — pemanggil tidak wajib menyebutkan semua parameter. Pada `CSVManager`, parameter `skipHeader = true` berarti secara default baris pertama CSV (header) akan dilewati, yang merupakan perilaku paling umum. Pemanggil bisa override jika perlu membaca header.

```cpp
static vector<string> read(const string& filename, bool skipHeader = true);
```

### 1.7 Inline Function — Fungsi Kecil Cepat

**Lokasi:** `models/Role.h` (`roleToString`, `stringToRole`)

**Mengapa inline:** Fungsi `roleToString` dan `stringToRole` adalah fungsi **kecil dan sering dipanggil**. Keyword `inline` meminta compiler untuk menyisipkan (inline) body fungsi langsung di tempat pemanggilan alih-alih membuat function call, sehingga mengurangi overhead pemanggilan fungsi. Cocok untuk fungsi utility sederhana.

```cpp
inline string roleToString(RoleType r) {
    switch (r) {
        case RoleType::CEO: return "CEO";
        // ...
    }
}
```

### 1.8 Function Overloading — Nama Sama, Parameter Beda

**Lokasi:** `services/PaketService.h` (`sortByBiaya`, `sortByBerat`)

**Mengapa overloading:** Kedua fungsi memiliki tujuan yang sama (sorting) tetapi dengan kriteria berbeda. Overloading memungkinkan penggunaan nama yang mirip (`sortByBiaya` dan `sortByBerat`) sehingga API lebih intuitif. Masing-masing menerima parameter `bool ascending` yang juga merupakan default argument (`= true`).

```cpp
vector<Paket> sortByBiaya(bool ascending = true);
vector<Paket> sortByBerat(bool ascending = true);
```

### 1.9 Exception Handling — Try/Catch/Throw

**Lokasi:** `storage/CSVManager.cpp`, seluruh service `load*()` functions

**Mengapa exception handling:** Operasi file I/O rentan terhadap error (file tidak ditemukan, format salah, permission denied). Exception handling dengan `try/catch` memungkinkan program **menangani error dengan graceful** — menampilkan pesan error yang informatif tanpa crash. Setiap operasi loading data dibungkus dalam try/catch.

```cpp
try {
    vector<string> lines = CSVManager::read("data/" + filename);
    // proses data...
} catch (const exception& e) {
    cout << "  [ERROR] Gagal memuat: " << e.what() << "\n";
}
```

### 1.10 STL `vector` — Dynamic Array

**Lokasi:** `services/PaketService.h` (`vector<Paket>`, `vector<Kurir>`, `vector<Layanan>`)

**Mengapa `vector`:** `vector` dipilih sebagai kontainer utama karena:
- **Dynamic sizing** — jumlah data tidak diketahui saat kompilasi
- **Random access O(1)** — akses cepat berdasarkan indeks
- **Kompatibel dengan STL algorithm** — `std::sort`, `std::find_if`, `std::count_if`
- **Manajemen memori otomatis** — tidak perlu alokasi/dealokasi manual

### 1.11 Iterator — Range-based For Loop

**Lokasi:** `menus/AdminMenu.cpp`, seluruh file menu dan service

**Mengapa iterator:** Range-based for loop (`for (const auto& item : container)`) menggunakan iterator secara implisit. Ini adalah cara modern C++ untuk iterasi yang:
- Lebih aman (tidak ada index out-of-bounds)
- Lebih readable
- Bekerja dengan semua tipe kontainer STL

```cpp
for (const auto& l : layananList) {
    cout << "  ID: " << l.id << " | " << l.nama << "\n";
}
```

### 1.12 `std::sort` — Algoritma Sorting STL

**Lokasi:** `services/PaketService.cpp`

**Mengapa `std::sort`:** `std::sort` adalah implementasi **Introsort** (hybrid QuickSort/HeapSort/InsertionSort) dari STL yang memiliki kompleksitas **O(n log n)**. Dibandingkan menulis sorting manual, `std::sort` lebih efisien, teruji, dan mendukung custom comparator via lambda.

```cpp
sort(sortedByBiaya.begin(), sortedByBiaya.end(),
     [](const Paket& a, const Paket& b) { return a.biaya < b.biaya; });
```

### 1.13 `std::find_if` — Pencarian dengan Kondisi

**Lokasi:** `services/AuthService.cpp`, fungsi-fungsi pencarian

**Mengapa `std::find_if`:** `std::find_if` mencari elemen berdasarkan **predikat/kondisi kustom**, bukan hanya nilai eksak. Sangat berguna untuk mencari user berdasarkan username di HashTable atau mencari paket berdasarkan kriteria tertentu. Lebih fleksibel daripada `std::find`.

### 1.14 `std::count_if` — Menghitung dengan Kondisi

**Lokasi:** `services/ReportService.cpp`

**Mengapa `std::count_if`:** `std::count_if` menghitung jumlah elemen yang memenuhi kondisi tertentu dalam satu pass O(n). Digunakan untuk statistik seperti menghitung jumlah paket per status. Lebih efisien daripada loop manual dengan counter.

### 1.15 File Handling — Baca/Tulis CSV

**Lokasi:** `storage/CSVManager.cpp` (`ifstream`/`ofstream`)

**Mengapa file handling:** Data perlu **persisten** (bertahan setelah program ditutup). CSV dipilih karena:
- Format sederhana dan human-readable
- Mudah diedit manual dengan text editor atau Excel
- Tidak memerlukan library database eksternal
- `ifstream`/`ofstream` adalah standar C++ untuk file I/O

### 1.16 Lambda Expression — Fungsi Anonim

**Lokasi:** `services/PaketService.cpp` (lambda di `std::sort`)

**Mengapa lambda:** Lambda memungkinkan pembuatan fungsi **sekali pakai secara inline** tanpa perlu mendefinisikan functor atau function pointer terpisah. Pada sorting, lambda sebagai comparator membuat kode lebih ringkas dan logika perbandingan langsung terlihat.

```cpp
// Lambda sebagai comparator untuk sorting
auto sorted = paket.sortByBiaya([](const Paket& a, const Paket& b) {
    return a.biaya > b.biaya;  // descending
});
```

---

## 2. Penggunaan Struktur Data

Berikut penjelasan 10 struktur data yang digunakan, mengapa dipilih, dan bagaimana implementasinya:

### 2.1 Singly Linked List — Master Data Paket

**File:** `datastructures/SinglyLinkedList.h`  
**Fungsi:** Menyimpan seluruh data paket sebagai list utama

**Mengapa Singly Linked List:**
- **Insert/Delete O(1)** di awal/tengah list — efisien untuk operasi CRUD paket
- **Memory efisien** — hanya satu pointer `next` per node
- **Sequential access** — sesuai untuk menampilkan semua paket (traversal)
- Struktur node yang fleksibel memudahkan insert dan delete tanpa menggeser elemen seperti pada array

**Operasi:** `insertFirst`, `insertLast`, `deleteNode`, `find`, `display`, `toVector`

### 2.2 Circular Linked List — Rotasi Kurir

**File:** `datastructures/CircularLinkedList.h`  
**Fungsi:** Daftar kurir yang dirotasi untuk pembagian tugas merata

**Mengapa Circular Linked List:**
- **Rotasi natural** — node terakhir menunjuk ke node pertama, menciptakan siklus
- **Round-robin assignment** — setiap pemanggilan `getNextKurir()` mengembalikan kurir berikutnya secara siklik
- **Load balancing** — memastikan semua kurir mendapat giliran secara adil
- Tidak perlu reset ke awal secara manual; siklus berjalan terus

**Operasi:** `insertLast`, `getNext` (rotasi), `display`

### 2.3 Stack — Undo Tracking

**File:** `datastructures/Stack.h`  
**Fungsi:** Menyimpan riwayat status tracking untuk fitur undo (LIFO)

**Mengapa Stack:**
- **LIFO (Last-In-First-Out)** — undo harus mengembalikan ke status sebelumnya (yang terakhir ditambahkan)
- Operasi `push` (tambah tracking) dan `pop` (undo tracking) keduanya O(1)
- Secara konseptual cocok: setiap update tracking di-push, undo berarti pop status terakhir
- Membatasi akses hanya ke elemen teratas mencegah manipulasi tracking lama

**Operasi:** `push`, `pop`, `top`, `isEmpty`, `display`

### 2.4 Queue — Antrian Paket Masuk

**File:** `datastructures/Queue.h`  
**Fungsi:** Antrian FIFO paket yang menunggu diproses kurir

**Mengapa Queue:**
- **FIFO (First-In-First-Out)** — paket yang masuk lebih dulu harus diproses lebih dulu (keadilan)
- **Enqueue O(1)** — Admin menambah paket ke antrian
- **Dequeue O(1)** — Kurir mengambil paket dari depan antrian
- Mencerminkan proses bisnis nyata: paket diantar sesuai urutan masuk

**Operasi:** `enqueue`, `dequeue`, `front`, `isEmpty`, `size`, `display`

### 2.5 AVL Tree — Pencarian Resi

**File:** `datastructures/AVLTree.h`  
**Fungsi:** Index nomor resi untuk pencarian cepat O(log n)

**Mengapa AVL Tree:**
- **Self-balancing** — menjamin tinggi tree O(log n), mencegah degenerasi ke linked list
- **Pencarian O(log n)** — jauh lebih cepat daripada linear search O(n) untuk data banyak
- Resi adalah **key unik** yang ideal sebagai key tree
- Operasi insert otomatis menyeimbangkan tree (rotasi LL, RR, LR, RL)

**Operasi:** `insert`, `search`, `displayInOrder`, `displayStructure`

### 2.6 Graph — Jalur Antar Kota

**File:** `datastructures/Graph.h`  
**Fungsi:** Representasi jaringan kota dan jarak antar kota (undirected weighted graph)

**Mengapa Graph:**
- **Model alami** — kota = vertex, jalan = edge, jarak = weight
- Mendukung **BFS** (shortest path by edges) dan **DFS** (eksplorasi semua rute)
- **Undirected** — jalan antar kota dua arah
- **Weighted edges** — jarak dalam km sebagai bobot edge

**Representasi:** Adjacency list (map of string → list of pairs)

### 2.7 BFS — Jalur Terpendek (Minimal Transit)

**File:** `services/RoutingService.cpp` (method `bfsSearch`)  
**Fungsi:** Mencari jalur dengan jumlah transit paling sedikit

**Mengapa BFS:**
- BFS menjamin menemukan jalur dengan **jumlah edge paling sedikit** (level-order traversal)
- Cocok untuk mencari rute dengan **minimal transit kota** (bukan jarak terpendek)
- Menggunakan queue dalam implementasinya
- Kompleksitas O(V + E)

### 2.8 DFS — Semua Rute dari Kota

**File:** `services/RoutingService.cpp` (method `dfsSearch`)  
**Fungsi:** Menampilkan semua kota yang dapat dijangkau dari suatu kota

**Mengapa DFS:**
- DFS mengeksplorasi **sedalam mungkin** sebelum backtracking
- Cocok untuk **menemukan semua konektivitas** dari suatu titik
- Implementasi rekursif yang natural untuk traversal graph
- Dapat dimodifikasi untuk mencari semua jalur (backtracking)

### 2.9 Hash Table — Login User

**File:** `services/AuthService.cpp` (menggunakan `unordered_map`)  
**Fungsi:** Menyimpan data user dengan key username untuk login O(1)

**Mengapa Hash Table:**
- **Pencarian O(1) average case** — login harus cepat
- Username sebagai **key unik** yang di-hash
- `unordered_map` STL sudah teroptimasi dan menangani collision
- Insert dan lookup keduanya O(1)

### 2.10 Sorting — Laporan Terurut

**File:** `services/PaketService.cpp`  
**Fungsi:** Mengurutkan paket berdasarkan biaya atau berat untuk laporan

**Mengapa Sorting:**
- Manager/CEO perlu melihat data **terurut** untuk analisis
- `std::sort` = **O(n log n)** Introsort — algoritma paling efisien untuk general sorting
- Dua kriteria: biaya (finansial) dan berat (operasional)
- Dua arah: ascending dan descending

---

## 3. Mengapa Menggunakan Header File

Proyek ini menggunakan arsitektur **header + implementation file** (`.h` + `.cpp`) karena alasan berikut:

### 3.1 Separation of Concerns (Pemisahan Tanggung Jawab)

Header file (`.h`) berisi **deklarasi** (interface/API) — apa yang bisa dilakukan oleh sebuah modul.  
Implementation file (`.cpp`) berisi **definisi** (implementasi) — bagaimana cara kerjanya.

Pemisahan ini memungkinkan:
- **Information hiding** — pengguna class cukup membaca header untuk tahu API, tidak perlu baca implementasi
- **Independent development** — developer bisa bekerja paralel: satu orang menulis header (kontrak), yang lain menulis implementasi

### 3.2 Compilation Efficiency (Efisiensi Kompilasi)

C++ menggunakan model **separate compilation**:
- Setiap `.cpp` dikompilasi menjadi object file (`.o`/`.obj`) secara independen
- Jika hanya `.cpp` yang berubah, hanya file itu yang perlu dikompilasi ulang
- Header yang tidak berubah = tidak trigger rekompilasi file yang meng-include-nya
- Pada proyek besar, ini menghemat waktu kompilasi secara signifikan

### 3.3 Dependency Management (Manajemen Dependensi)

`#include` header file menciptakan **dependency graph** yang jelas:
- `main.cpp` → `#include "services/PaketService.h"` → `#include "models/Paket.h"`
- Setiap file hanya meng-include header yang benar-benar dibutuhkan
- Tidak ada include berlebihan yang memperlambat kompilasi

### 3.4 Reusability (Dapat Digunakan Ulang)

Header file bisa di-include oleh banyak file `.cpp` tanpa duplikasi kode. Contoh:
- `models/Paket.h` digunakan oleh `PaketService`, `TrackingService`, `AdminMenu`, `KurirMenu`, `ManagerMenu`, `CeoMenu`
- Tanpa header, definisi struct harus di-copy ke setiap file yang membutuhkan (DRY violation)

### 3.5 `#pragma once` — Include Guard Modern

Setiap header menggunakan `#pragma once` yang:
- Mencegah **double inclusion** (header di-include dua kali dalam satu unit kompilasi)
- Lebih ringkas daripada traditional include guard (`#ifndef`/`#define`/`#endif`)
- Didukung oleh semua compiler modern (GCC, Clang, MSVC)

---

## 4. Mengapa Struktur File Seperti Itu

Struktur folder proyek mengikuti pola arsitektur **Layered Architecture** (seperti MVC sederhana):

```
swift_expedition_uas/
├── main.cpp                    # Entry point (layer: Application)
├── models/                     # Data definitions (layer: Domain Model)
├── datastructures/             # Custom containers (layer: Infrastructure)
├── services/                   # Business logic (layer: Service/Business)
├── menus/                      # User interface (layer: Presentation/UI)
├── storage/                    # File I/O (layer: Persistence)
└── data/                       # CSV data files (layer: Data Storage)
```

### 4.1 `models/` — Domain Model Layer

**Isi:** 8 file struct (Role, User, Paket, Tracking, Kurir, Layanan, KlasifikasiBerat, Kota)

**Mengapa dipisah:** Setiap entitas bisnis memiliki file sendiri agar:
- Mudah ditemukan dan diedit
- Perubahan satu model tidak mempengaruhi model lain
- Mencerminkan prinsip **Single Responsibility**

### 4.2 `datastructures/` — Infrastructure Layer

**Isi:** 7 implementasi struktur data kustom (SinglyLinkedList, CircularLinkedList, Stack, Queue, AVLTree, Graph, HashTable) + LinkedList.h (base)

**Mengapa dipisah:** Struktur data adalah komponen **generic/reusable** yang tidak bergantung pada bisnis logic. Dengan memisahkannya:
- Bisa di-reuse untuk proyek lain
- Testing struktur data bisa dilakukan secara independen
- Memenuhi syarat UAS: implementasi struktur data sendiri (bukan STL)

### 4.3 `services/` — Business Logic Layer

**Isi:** 5 pasang `.h`/`.cpp` (AuthService, PaketService, TrackingService, ReportService, RoutingService)

**Mengapa dipisah:** Service layer berisi **aturan bisnis** dan mengorkestrasi penggunaan model + struktur data. Setiap service menangani satu domain:
- `AuthService` → login, autentikasi
- `PaketService` → CRUD paket, sorting, queue, AVL tree
- `TrackingService` → update status, undo stack, riwayat
- `ReportService` → laporan, statistik
- `RoutingService` → graph BFS/DFS, rute antar kota

### 4.4 `menus/` — Presentation Layer

**Isi:** 5 pasang `.h`/`.cpp` (LoginMenu, AdminMenu, KurirMenu, ManagerMenu, CeoMenu)

**Mengapa dipisah:** Setiap role memiliki menu sendiri. Pemisahan ini:
- Mencegah file menu menjadi terlalu besar (AdminMenu sendiri 180 baris)
- Memudahkan penambahan role baru tanpa mengubah menu lain
- Masing-masing menu hanya menerima service yang dibutuhkan role tersebut

### 4.5 `storage/` — Persistence Layer

**Isi:** `CSVManager.h`/`.cpp`

**Mengapa dipisah:** File handling diisolasi agar:
- Jika ingin ganti format (misal ke JSON/XML/database), hanya perlu ubah CSVManager
- Service layer tidak perlu tahu detail implementasi file I/O
- Method `read()` dan `write()` yang reusable

### 4.6 `data/` — Data Storage

**Isi:** 7 file CSV

**Mengapa dipisah:** Data adalah **external resource** yang harus terpisah dari kode:
- Bisa diedit tanpa menyentuh kode
- Bisa di-backup secara independen
- Format delimiter `;` (titik koma) untuk kompatibilitas dengan Excel Indonesia

### 4.7 `using namespace std;` — Kenapa Ditambahkan

Setiap file menggunakan `using namespace std;` setelah include untuk:
- **Readability** — `cout` lebih ringkas daripada `std::cout`
- **Produktivitas** — mengurangi ketikan berulang untuk objek STL umum (`string`, `vector`, `cout`, `cin`, `endl`)
- **Konsistensi** — semua file memiliki konvensi yang sama
- Cocok untuk project skala akademik di mana namespace conflict jarang terjadi

---

## 5. Alur Kerja Program

### 5.1 Diagram Alur

```
START
  │
  ▼
[Inisialisasi 5 Service]
  ├── AuthService
  ├── PaketService
  ├── TrackingService
  ├── ReportService
  └── RoutingService
  │
  ▼
[LOAD DATA dari CSV]
  ├── auth.loadUsers("anggota.csv")        → HashTable
  ├── paket.loadLayanan("layanan.csv")      → vector<Layanan>
  ├── paket.loadKlasifikasi("klasifikasi.csv") → vector<KlasifikasiBerat>
  ├── paket.loadPaket("paket.csv")          → SinglyLinkedList + AVLTree + Queue
  ├── paket.loadKurir("kurir.csv")          → CircularLinkedList
  ├── tracking.loadTracking("tracking.csv") → vector<Tracking> + Stack
  └── routing.loadKota("kota.csv")          → Graph + vector<Kota>
  │
  ▼
┌─────────────────────────────────┐
│        LOGIN LOOP               │
│  ┌───────────────────────────┐  │
│  │ LoginMenu.show(auth)      │  │
│  │  - Input username/pass    │  │
│  │  - Max 3 attempts         │  │
│  │  - Return User* / nullptr │  │
│  └───────────────────────────┘  │
│              │                   │
│     ┌────────┴────────┐         │
│     ▼                 ▼         │
│  [Login Gagal]    [Login OK]    │
│  return nullptr   return User*  │
│  → exit program         │       │
│                     ┌────┴────┐ │
│                     ▼         ▼ │
│              [Switch Role]  [Break] │
│               │    │    │    │  │
│               ▼    ▼    ▼    ▼  │
│            ADMIN KURIR MGR CEO  │
│               │    │    │    │  │
│               └────┴────┴────┘  │
│                      │          │
│               [Menu Loop]       │
│               user memilih      │
│               menu 1..N/0/99    │
│                      │          │
│              ┌───────┴───────┐ │
│              ▼               ▼ │
│         [Logout (0)]   [Exit (99)] │
│         return true     return false│
│              │               │ │
│              ▼               ▼ │
│         [Save CSV]      [Keluar] │
│         continue loop    break   │
└─────────────────────────────────┘
  │
  ▼
END
```

### 5.2 Penjelasan Detail Alur

#### Fase 1: Startup & Inisialisasi (`main.cpp` baris 39-45)

Program dimulai dengan membuat instance 5 service class. Service-service ini akan menangani seluruh logika bisnis:

| Service | Tanggung Jawab |
|---------|---------------|
| `AuthService` | Autentikasi user, manajemen session |
| `PaketService` | CRUD paket, sorting, queue, kurir |
| `TrackingService` | Update status, undo stack, riwayat |
| `ReportService` | Generate laporan, statistik |
| `RoutingService` | Graph kota, BFS, DFS |

#### Fase 2: Loading Data dari CSV (`main.cpp` baris 47-63)

Setiap service memiliki method `load*()` yang membaca file CSV dari folder `data/`. Urutan loading penting karena ada dependensi data:

1. **anggota.csv** — data user login (harus diload pertama karena dibutuhkan untuk autentikasi)
2. **layanan.csv & klasifikasi.csv** — data referensi (dibutuhkan sebelum paket karena paket mereferensi ID layanan/klasifikasi)
3. **paket.csv** — data paket utama (dimasukkan ke SinglyLinkedList, AVLTree, dan Queue)
4. **kurir.csv** — data kurir (dimasukkan ke CircularLinkedList)
5. **tracking.csv** — riwayat tracking (dimasukkan ke vector dan Stack)
6. **kota.csv** — data rute antar kota (dimasukkan ke Graph)

#### Fase 3: Login Loop (`main.cpp` baris 74-113)

Program masuk ke infinite loop login:

1. `LoginMenu.show(auth)` menampilkan form login
2. User memasukkan username dan password
3. `auth.login()` mencari user di HashTable (O(1))
4. Jika password cocok → return pointer User
5. Jika tidak cocok → increment attempt counter (max 3)
6. Jika 3x gagal → return nullptr → program exit

#### Fase 4: Role Routing (`main.cpp` baris 80-100)

Setelah login berhasil, program menggunakan `switch` untuk mengarahkan user ke menu sesuai role:

```
CEO     → CeoMenu.show(user, paket, report, tracking, routing)
Manager → ManagerMenu.show(user, paket, report, tracking)
Admin   → AdminMenu.show(user, paket)
Kurir   → KurirMenu.show(user, paket, tracking)
```

Setiap menu hanya menerima service yang **relevan** dengan role tersebut:
- CEO mendapat akses ke semua service (termasuk RoutingService untuk BFS/DFS)
- Admin hanya mendapat PaketService (fokus pada manajemen data)
- Kurir mendapat PaketService + TrackingService (fokus pada operasional)

#### Fase 5: Menu Loop (internal setiap menu)

Setiap menu berjalan dalam infinite loop dengan pola yang sama:

```
do {
    tampilkan_menu();
    input_pilihan();
    switch (pilihan) {
        case 1..N: jalankan_fitur();
        case 0:    return true;   // logout → kembali ke login
        case 99:   return false;  // exit program
    }
} while (true);
```

#### Fase 6: Save & Cleanup (`main.cpp` baris 102-111)

Setelah user logout (pilih 0) atau exit (pilih 99):

1. Data paket disimpan ke `paket.csv` via `paket.savePaket()`
2. Data tracking disimpan ke `tracking.csv` via `tracking.saveTracking()`
3. Jika user memilih exit (return false) → break dari login loop → program selesai
4. Jika user memilih logout (return true) → kembali ke login menu untuk user lain

---

## 6. Guide Edit Data CSV

Semua data disimpan dalam file CSV di folder `data/` dengan delimiter `;` (titik koma).

### 6.1 Aturan Umum

| Aturan | Keterangan |
|--------|-----------|
| Delimiter | `;` (titik koma) — **bukan koma!** |
| Encoding | UTF-8 |
| Baris pertama | Header (nama kolom) — **jangan dihapus** |
| ID | Harus unik dan auto-increment |
| String | Tanpa tanda kutip |
| Desimal | Menggunakan titik (.) contoh: `85000.00` |

### 6.2 Format Per File

#### anggota.csv — Data User Login

```
id;nama;username;password;role
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| id | int | ID unik user |
| nama | string | Nama lengkap |
| username | string | Username login (case-sensitive) |
| password | string | Password login (plain text) |
| role | string | CEO / Manager / Admin / Kurir |

**Contoh menambah user baru:**
```
11;Rina Amelia;rina;pass123;Admin
```

> ⚠️ Pastikan ID tidak duplikat dan role ditulis persis (CEO, Manager, Admin, Kurir)

#### paket.csv — Data Paket

```
id;resi;nama_penerima;alamat_tujuan;kota_asal;kota_tujuan;berat;biaya;status;id_layanan;id_klasifikasi;id_kurir
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| id | int | ID unik paket |
| resi | string | Nomor resi (format: SWFT-XXX) |
| nama_penerima | string | Nama penerima paket |
| alamat_tujuan | string | Alamat lengkap tujuan |
| kota_asal | string | Kota pengirim |
| kota_tujuan | string | Kota penerima |
| berat | double | Berat dalam kg |
| biaya | double | Biaya pengiriman (Rp) |
| status | string | menunggu / diproses / dalam_perjalanan / sampai_tujuan / terkirim |
| id_layanan | int | Referensi ke layanan.csv |
| id_klasifikasi | int | Referensi ke klasifikasi.csv |
| id_kurir | int | Referensi ke kurir.csv |

**Contoh menambah paket baru:**
```
11;SWFT-011;Ibu Ratna;Jl. Melati No.3;Jakarta Pusat;Depok;2.5;75000.00;menunggu;1;1;0
```

> ⚠️ `id_kurir = 0` artinya paket belum di-assign ke kurir

#### tracking.csv — Riwayat Tracking

```
id;id_paket;lokasi;status;timestamp
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| id | int | ID unik tracking entry |
| id_paket | int | Referensi ke paket.csv |
| lokasi | string | Lokasi saat status update |
| status | string | Status tracking |
| timestamp | string | Tanggal (format bebas: YYYY-MM-DD) |

**Contoh menambah tracking:**
```
21;5;Jakarta Pusat;diproses;2025-02-10
```

#### layanan.csv — Jenis Layanan

```
id;nama;tarif_per_kg
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| id | int | ID unik layanan |
| nama | string | Nama layanan (Reguler, Express, dll) |
| tarif_per_kg | double | Tarif per kg (Rp) |

#### klasifikasi.csv — Klasifikasi Berat

```
id;nama;biaya_tambahan
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| id | int | ID unik klasifikasi |
| nama | string | Nama klasifikasi (Ringan, Sedang, Berat) |
| biaya_tambahan | double | Biaya tambahan (Rp) |

#### kota.csv — Data Rute Kota

```
asal;tujuan;jarak
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| asal | string | Nama kota asal (case-sensitive!) |
| tujuan | string | Nama kota tujuan (case-sensitive!) |
| jarak | int | Jarak dalam km |

**Contoh menambah rute baru:**
```
Jakarta Timur;Depok;22
```

> ⚠️ **PENTING:** Nama kota bersifat **case-sensitive**. "Jakarta Pusat" ≠ "jakarta pusat". Jika ingin menambah kota baru, pastikan nama persis sama di semua tempat.

#### kurir.csv — Data Kurir

```
id;nama;status;total_paket
```

| Kolom | Tipe | Keterangan |
|-------|------|-----------|
| id | int | ID unik kurir |
| nama | string | Nama kurir |
| status | string | Status (Tersedia / Bertugas) |
| total_paket | int | Jumlah paket ditangani |

### 6.3 Tips Edit CSV

1. **Backup dulu** — copy file asli sebelum diedit
2. **Jangan hapus header** — baris pertama adalah nama kolom
3. **ID jangan duplikat** — setiap file memiliki ID auto-increment
4. **Gunakan text editor** — Notepad++, VS Code, atau sejenisnya (bukan Excel yang bisa mengubah format)
5. **Perhatikan delimiter** — selalu gunakan `;` bukan `,`
6. **Jangan ada spasi setelah `;`** — format: `data1;data2;data3` (bukan `data1; data2; data3`)
7. **Referensi ID valid** — pastikan `id_layanan`, `id_klasifikasi`, `id_kurir` di paket.csv merujuk ke ID yang ada di file referensi
8. **Restart program** — setelah edit CSV, restart program agar data baru diload

---

## 7. Tutorial Simulasi Semua Role

### 7.1 Persiapan

**Compile program:**
```bash
g++ main.cpp menus/*.cpp services/*.cpp storage/*.cpp -o main.exe
```

**Jalankan:**
```bash
./main.exe
```

### 7.2 Simulasi Role ADMIN (andi / pass123)

Admin bertanggung jawab atas manajemen data paket, kurir, layanan, dan klasifikasi.

```
============================================
=== SWIFT EXPEDITION - LOGIN ===
============================================
Username : andi
Password : pass123

============================================
Selamat datang, Andi Pratama!
Role: Admin
============================================

============================================
=== MENU ADMIN ===
User: Andi Pratama
============================================
1. Tambah Paket
2. Hapus Paket
3. Edit Paket
4. Cari Paket (by ID)
5. Cari Paket (by Resi)
6. Lihat Semua Paket
7. Lihat Antrean Paket
8. Lihat Daftar Kurir
9. Lihat Daftar Layanan
10. Lihat Klasifikasi Berat
11. Lihat AVL Tree (Resi)
0. Logout
99. Exit Program
----------------------------------------
Pilihan: _
```

#### Simulasi 1: Tambah Paket Baru

```
Pilihan: 1

--- Tambah Paket Baru ---
Resi          : SWFT-011
Nama Penerima : Ibu Ratna
Alamat Tujuan : Jl. Melati No.3
Kota Asal     : Jakarta Pusat
Kota Tujuan   : Depok
Berat (kg)    : 2.5
ID Layanan    : 1
ID Klasifikasi: 1
Biaya         : 75000

[SUCCESS] Paket berhasil ditambahkan dan dimasukkan ke antrean!
```

> Paket otomatis masuk ke SinglyLinkedList (master data) dan Queue (antrian kurir)

#### Simulasi 2: Cari Paket by ID

```
Pilihan: 4

Masukkan ID paket: 1

===== PAKET DITEMUKAN =====
ID       : 1
Resi     : SWFT-001
Penerima : Bapak Rahmat
Alamat   : Jl. Merdeka No.10
Dari     : Jakarta Pusat -> Bandung
Berat    : 5.5 kg
Biaya    : Rp 85000
Status   : terkirim
```

#### Simulasi 3: Cari Paket by Resi (AVL Tree)

```
Pilihan: 5

Masukkan nomor resi: SWFT-003

===== PAKET DITEMUKAN =====
ID       : 3
Resi     : SWFT-003
Penerima : PT Maju Jaya
Status   : sampai_tujuan
```

> Pencarian menggunakan AVL Tree → O(log n)

#### Simulasi 4: Lihat AVL Tree

```
Pilihan: 11

===== AVL TREE (Pencarian Resi) =====
[Menampilkan struktur AVL Tree dengan key resi]
```

#### Simulasi 5: Edit Paket

```
Pilihan: 3

Masukkan ID paket yang akan diedit: 11
--- Edit Paket (kosongkan untuk tidak mengubah) ---
Resi [SWFT-011]: 
Nama Penerima [Ibu Ratna]: Ibu Ratna Sari
Alamat Tujuan [Jl. Melati No.3]: 
Kota Asal [Jakarta Pusat]: 
Kota Tujuan [Depok]: 
Status [menunggu]: 
Berat [2.5]: 3.0
Biaya [75000]: 

[SUCCESS] Paket berhasil diupdate!
```

#### Simulasi 6: Logout

```
Pilihan: 0
Logout...

============================================
=== MENYIMPAN DATA... ===
============================================
```

> Kembali ke menu login

---

### 7.3 Simulasi Role KURIR (citra / pass123)

Kurir bertanggung jawab mengambil paket dari antrian dan mengupdate status pengiriman.

```
============================================
=== SWIFT EXPEDITION - LOGIN ===
============================================
Username : citra
Password : pass123

============================================
Selamat datang, Citra Dewi!
Role: Kurir
============================================

============================================
=== MENU KURIR ===
User: Citra Dewi
============================================
1. Ambil Paket dari Queue
2. Update Status Tracking
3. Undo Tracking (Stack)
4. Lihat Riwayat Tracking
5. Lihat Antrean Paket
6. Lihat Semua Paket
0. Logout
99. Exit Program
----------------------------------------
Pilihan: _
```

#### Simulasi 1: Lihat Antrean Paket

```
Pilihan: 5

===== ANTREAN PAKET (QUEUE) =====
[Menampilkan semua paket dalam antrian FIFO]
```

#### Simulasi 2: Ambil Paket dari Queue

```
Pilihan: 1

[BERHASIL] Mengambil paket SWFT-007
Penerima: Budi Utama
Tujuan  : Jakarta Selatan -> Tangerang
Kurir Citra Dewi ditugaskan ke paket 7
[TRACKING] Status paket 7: Dalam Perjalanan | Lokasi: Jakarta Selatan
```

> Paket di-dequeue dari Queue (FIFO), kurir di-assign via CircularLinkedList (rotasi)

#### Simulasi 3: Update Status Tracking

```
Pilihan: 2

ID Paket  : 7
Status Baru: sampai_tujuan
Lokasi     : Tangerang

[TRACKING] Status paket 7: sampai_tujuan | Lokasi: Tangerang
```

> Status baru di-push ke Stack (untuk fitur undo)

#### Simulasi 4: Undo Tracking (Stack)

```
Pilihan: 3

ID Paket untuk undo: 7

[UNDO] Kembali ke status: Dalam Perjalanan
```

> Pop dari Stack — rollback ke status sebelumnya (LIFO)

#### Simulasi 5: Lihat Riwayat Tracking

```
Pilihan: 4

1. Lihat per Paket (masukkan ID)
2. Lihat semua
Pilih: 1
ID Paket: 1

===== RIWAYAT TRACKING PAKET 1 =====
1 | diproses | Jakarta Pusat | 2025-01-10
2 | dalam_perjalanan | Jakarta Pusat | 2025-01-10
3 | sampai_tujuan | Bandung | 2025-01-11
4 | terkirim | Bandung | 2025-01-12
```

#### Simulasi 6: Logout

```
Pilihan: 0
Logout...
```

---

### 7.4 Simulasi Role MANAGER (budi / pass123)

Manager fokus pada laporan, sorting, dan statistik.

```
============================================
=== SWIFT EXPEDITION - LOGIN ===
============================================
Username : budi
Password : pass123

============================================
Selamat datang, Budi Santoso!
Role: Manager
============================================

============================================
=== MENU MANAGER ===
User: Budi Santoso
============================================
1. Laporan Semua Paket
2. Laporan Paket by Status
3. Sorting Paket Berdasarkan Biaya
4. Sorting Paket Berdasarkan Berat
5. Statistik Pengiriman
6. Lihat Riwayat Tracking
7. Lihat Semua Paket
0. Logout
99. Exit Program
----------------------------------------
Pilihan: _
```

#### Simulasi 1: Laporan Semua Paket

```
Pilihan: 1

===== LAPORAN SEMUA PAKET =====
1 | SWFT-001 | Bapak Rahmat | Jakarta Pusat -> Bandung | 5.5 kg | Rp 85000 | terkirim
2 | SWFT-002 | Ibu Sari | Jakarta Selatan -> Surabaya | 12 kg | Rp 345000 | terkirim
3 | SWFT-003 | PT Maju Jaya | Bandung -> Yogyakarta | 3 kg | Rp 60000 | sampai_tujuan
...
```

#### Simulasi 2: Laporan by Status

```
Pilihan: 2

===== PAKET BERDASARKAN STATUS =====

--- menunggu ---
7 | SWFT-007 | Budi Utama | 3.5 kg | Rp 110000
9 | SWFT-009 | Dina Lestari | 1.5 kg | Rp 80000

--- diproses ---
5 | SWFT-005 | Bapak Hadi | 45 kg | Rp 500000
8 | SWFT-008 | CV Nusantara | 8 kg | Rp 140000
...
```

#### Simulasi 3: Sorting Berdasarkan Biaya (Termahal)

```
Pilihan: 3

1. Ascending (termurah)
2. Descending (termahal)
Pilih: 2

===== PAKET DIURUTKAN BERDASARKAN BIAYA =====
10 | SWFT-010 | Koperasi Makmur | Rp 550000 | sampai_tujuan
5 | SWFT-005 | Bapak Hadi | Rp 500000 | diproses
2 | SWFT-002 | Ibu Sari | Rp 345000 | terkirim
8 | SWFT-008 | CV Nusantara | Rp 140000 | diproses
...
```

> Menggunakan `std::sort` + lambda expression (callback function)

#### Simulasi 4: Sorting Berdasarkan Berat (Terberat)

```
Pilihan: 4

1. Ascending (teringan)
2. Descending (terberat)
Pilih: 2

===== PAKET DIURUTKAN BERDASARKAN BERAT =====
10 | SWFT-010 | Koperasi Makmur | 50 kg | sampai_tujuan
5 | SWFT-005 | Bapak Hadi | 45 kg | diproses
2 | SWFT-002 | Ibu Sari | 12 kg | terkirim
...
```

#### Simulasi 5: Statistik Pengiriman

```
Pilihan: 5

===== STATISTIK PENGIRIMAN =====
Total Paket           : 10
Paket Terkirim        : 2
Paket Dalam Perjalanan: 2
Paket Diproses        : 2
Paket Menunggu        : 2
Paket Sampai Tujuan   : 2

Total Biaya Pengiriman: Rp 1590000
Rata-rata Biaya       : Rp 159000
```

> Menggunakan `std::count_if` untuk menghitung per status

#### Simulasi 6: Logout

```
Pilihan: 0
Logout...
```

---

### 7.5 Simulasi Role CEO (zaky / pass123)

CEO memiliki akses tertinggi termasuk BFS/DFS routing.

```
============================================
=== SWIFT EXPEDITION - LOGIN ===
============================================
Username : zaky
Password : pass123

============================================
Selamat datang, Zaky Ismail!
Role: CEO
============================================

============================================
=== MENU CEO ===
User: Zaky Ismail (CEO)
============================================
1. Laporan Keseluruhan (CEO Report)
2. Statistik Pengiriman
3. Lihat Semua Paket
4. Lihat Riwayat Tracking
5. Lihat Graph Rute Kota
6. BFS - Cari Jalur Terpendek Antar Kota
7. DFS - Tampilkan Semua Rute Dari Kota
8. Cari Semua Jalur Antar Kota
9. Lihat Daftar Kota
10. Simpan Data
0. Logout
99. Exit Program
----------------------------------------
Pilihan: _
```

#### Simulasi 1: CEO Report

```
Pilihan: 1

===== CEO REPORT =====
[Laporan komprehensif: total paket, revenue, performa kurir, dll]
```

#### Simulasi 2: Lihat Graph Rute Kota

```
Pilihan: 5

===== DAFTAR JALUR (KOTA.CSV) =====
Jakarta Pusat -> Jakarta Selatan (15 km)
Jakarta Pusat -> Jakarta Timur (20 km)
Jakarta Pusat -> Jakarta Barat (12 km)
Jakarta Selatan -> Jakarta Timur (18 km)
Jakarta Selatan -> Depok (25 km)
Jakarta Timur -> Bekasi (30 km)
Jakarta Barat -> Tangerang (25 km)
Jakarta Pusat -> Bogor (55 km)
...
```

#### Simulasi 3: BFS — Jalur Terpendek (Minimal Transit)

```
Pilihan: 6

Kota Asal  : Jakarta Pusat
Kota Tujuan: Surabaya

===== BFS: Mencari Transit Terdekat =====
Dari: Jakarta Pusat -> Tujuan: Surabaya
Jalur terpendek (minimal transit):
  Jakarta Pusat -> Surabaya
Total jarak: 780 km
Jumlah transit: 0
```

> BFS mencari jalur dengan **edge paling sedikit**, bukan jarak terpendek. Karena ada direct edge Jakarta Pusat→Surabaya (780 km), hasilnya langsung tanpa transit meskipun jaraknya mungkin bukan yang terpendek.

#### Simulasi 4: BFS — Dengan Transit

```
Pilihan: 6

Kota Asal  : Jakarta Pusat
Kota Tujuan: Yogyakarta

===== BFS: Mencari Transit Terdekat =====
Dari: Jakarta Pusat -> Tujuan: Yogyakarta
Jalur terpendek (minimal transit):
  Jakarta Pusat -> Bandung -> Yogyakarta
Total jarak: 580 km
Jumlah transit: 1
```

#### Simulasi 5: DFS — Semua Rute dari Kota

```
Pilihan: 7

Kota Awal: Jakarta Pusat

===== DFS: Menelusuri Semua Rute dari Jakarta Pusat =====
[Jalur DFS traversal:]
Jakarta Pusat -> Jakarta Selatan -> Jakarta Timur -> Bekasi -> Karawang
Jakarta Pusat -> Jakarta Selatan -> Jakarta Timur -> Bogor -> Semarang -> Surabaya
Jakarta Pusat -> Jakarta Selatan -> Jakarta Timur -> Bogor -> Semarang -> Yogyakarta
...
```

#### Simulasi 6: Cari Semua Jalur Antar Kota

```
Pilihan: 8

Kota Asal  : Jakarta Pusat
Kota Tujuan: Bandung

===== SEMUA JALUR DARI Jakarta Pusat KE Bandung =====
Jalur 1: Jakarta Pusat -> Bogor -> Bandung (235 km)
Jalur 2: Jakarta Pusat -> Bandung (150 km)
Total jalur ditemukan: 2
```

#### Simulasi 7: Lihat Daftar Kota

```
Pilihan: 9

===== DAFTAR KOTA =====
1. Jakarta Pusat
2. Jakarta Selatan
3. Jakarta Timur
4. Jakarta Barat
5. Depok
6. Bekasi
7. Tangerang
8. Bogor
9. Karawang
10. Bandung
11. Surabaya
12. Yogyakarta
13. Semarang
```

#### Simulasi 8: Simpan Data Manual

```
Pilihan: 10

[Menyimpan paket.csv dan tracking.csv]
```

#### Simulasi 9: Exit Program

```
Pilihan: 99

Keluar dari program...

============================================
=== MENYIMPAN DATA... ===
============================================
```

> Program menyimpan data dan keluar

---

### 7.6 Ringkasan Fitur Per Role

| Role | Fitur Utama | Struktur Data Digunakan |
|------|------------|------------------------|
| **Admin** | CRUD paket, lihat data master, AVL tree | SinglyLinkedList, Queue, AVLTree, CircularLinkedList |
| **Kurir** | Ambil paket (FIFO), update tracking, undo (LIFO) | Queue, Stack, SinglyLinkedList |
| **Manager** | Laporan, sorting, statistik | vector, std::sort, std::count_if |
| **CEO** | Laporan komprehensif, BFS/DFS routing, graph | Graph (BFS, DFS), semua struktur data |

### 7.7 Tips Simulasi

1. **Login sebagai Admin dulu** — tambahkan beberapa paket baru agar ada data untuk disimulasikan oleh role lain
2. **Login sebagai Kurir** — ambil paket dari queue, update statusnya bertahap (diproses → dalam_perjalanan → sampai_tujuan → terkirim)
3. **Login sebagai Manager** — lihat laporan dan statistik setelah data bertambah
4. **Login sebagai CEO** — gunakan BFS/DFS setelah memahami graph kota
5. **Edit CSV manual** — tambah kota baru di `kota.csv` dan lihat perubahannya di menu CEO
6. **Undo tracking** — coba update status beberapa kali lalu undo untuk melihat stack bekerja

---

> **Dokumentasi ini disusun untuk memenuhi pemahaman menyeluruh terhadap proyek Swift Expedition UAS Algoritma Pemrograman & Struktur Data.**