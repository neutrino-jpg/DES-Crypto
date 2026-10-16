#include <iostream>
#include <cmath>
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <vector>

#define minus "---------------------"
#define ravno "=========================="

using namespace std;

//ВНИМАНИЕ!!!
//63 - СЛЕВА
//0 - СПРАВА

//Sbox | 6 -> 4
static const int S[8][4][16] = {
    // S1
    {
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
        { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
        { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
        {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
    },
    // S2
    {
        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
        { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
        { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
        {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
    },
    // S3
    {
        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
        {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
        {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
        { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
    },
    // S4
    {
        { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
        {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
        {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
        { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
    },
    // S5
    {
        { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
        {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
        { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
        {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
    },
    // S6
    {
        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
        {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
        { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
        { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
    },
    // S7
    {
        { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
        {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
        { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
        { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
    },
    // S8
    {
        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
        { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
        { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
        { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
    }
};

// Таблица 1: Матрица начальной перестановки
    static const int IP[64] = {
        57, 49, 41, 33, 25, 17,  9,  1,
        59, 51, 43, 35, 27, 19, 11,  3,
        61, 53, 45, 37, 29, 21, 13,  5,
        63, 55, 47, 39, 31, 23, 15,  7,
        56, 48, 40, 32, 24, 16,  8,  0,
        58, 50, 42, 34, 26, 18, 10,  2,
        60, 52, 44, 36, 28, 20, 12,  4,
        62, 54, 46, 38, 30, 22, 14,  6
    };

// Таблица 2: Матрица обратной перестановки
static const int invIP[64] = {
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25,
    32,  0, 40,  8, 48, 16, 56, 24
};

// Таблица 3: Подготовка ключа | 64 -> 56
static const int PC1[56] = {
    56, 48, 40, 32, 24, 16,  8,
     0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26,
    18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,
     6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28,
    20, 12,  4, 27, 19, 11,  3
};

// Таблица 4: Формирование раундового ключа | 56 -> 48
static const int PC2[48] = {
    13, 16, 10, 23,  0,  4,
     2, 27, 14,  5, 20,  9,
    22, 18, 11,  3, 25,  7,
    15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54,
    29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52,
    45, 41, 49, 35, 28, 31
};

//Таблица 5: Расширение для суммирования с ключом | 32 -> 48
static const int E[48] = {
    31,  0,  1,  2,  3,  4,
     3,  4,  5,  6,  7,  8,
     7,  8,  9, 10, 11, 12,
    11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20,
    19, 20, 21, 22, 23, 24,
    23, 24, 25, 26, 27, 28,
    27, 28, 29, 30, 31,  0
};

//Таблица 6: Перемешивание после преобразования Sbox 
static const int P[32] = {
    15,  6, 19, 20,
    28, 11, 27, 16,
     0, 14, 22, 25,
     4, 17, 30,  9,
     1,  7, 23, 13,
    31, 26,  2,  8,
    18, 12, 29,  5,
    21, 10,  3, 24
};
//Таблица 7: Таблица сдвигов 
static const int SHIFTS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};


uint32_t Sub(uint64_t in48) {
    uint32_t out32 =0;
    for (int i =0; i<8; i++) {
        int shift = 42 -6*i;
        int six = (in48 >> shift) & 0x3F;

        int row = ((six & 0x20) >> 4) | (six & 1); //байты 1 и 6
        int col = (six >> 1) & 0x0F; //байты 2,3,4,5
        
        int val = S[i][row][col];
        out32 = (out32 << 4) | val;
    } 
    return out32;
}

//Функция перестановки бит
uint64_t permute(uint64_t in, int in_bits,
                         const int* table, int out_bits) {
    uint64_t out = 0;
    for (int i = 0; i < out_bits; ++i) {
        int src = table[i];
        uint64_t bit = (in >> (in_bits - 1 - src)) & 1ULL;
        out |= bit << (out_bits - 1 - i);
    }
    return out;
}

inline uint32_t rotl28(uint32_t x, int n) {
    x &= 0x0FFFFFFF;
    return ((x << n) | (x >> (28 - n))) & 0x0FFFFFFF;
}

void RKeysGen(uint64_t CipherKey, uint64_t KeyArr[16]) {
  
    uint64_t pc1_result = permute(CipherKey, 64, PC1, 56);
    uint32_t C = (uint32_t)(pc1_result >> 28) & 0x0FFFFFFF;
    uint32_t D = (uint32_t)(pc1_result) & 0x0FFFFFFF;
    
    for (int i =0; i<16; i++) {
        C = rotl28(C, SHIFTS[i]);
        D = rotl28(D, SHIFTS[i]);
        uint64_t CD = ((uint64_t)C << 28) | D;    // 56 бит
        KeyArr[i] = permute(CD, 56, PC2, 48);     //48 бит
    }
}

uint32_t FeystelFunc(uint32_t half, uint64_t rkey) {
    uint64_t tmp = permute(half, 32, E, 48);
    tmp ^= rkey;
    tmp = Sub(tmp);
    uint32_t out = permute(tmp, 32, P, 32); 
    return out;
}

void printWord(uint64_t w) {
    ios oldState(nullptr);
    oldState.copyfmt(cout);   // запомнили текущее состояние
    cout << hex << uppercase
              << setw(8) << setfill('0') << w;
              //<< static_cast<int>(w);

    cout.copyfmt(oldState);   // вернули как было
}

void printByte(uint8_t b) {
    ios oldState(nullptr);
    oldState.copyfmt(cout);   // запомнили текущее состояние
    cout << hex << uppercase
              << setw(2) << setfill('0') << b;
              //<< static_cast<int>(b);

    cout.copyfmt(oldState);   // вернули как было
}

uint64_t text_to_u64(const std::string& s) {
    uint64_t x = 0;
    for (int i = 0; i < 8; ++i)
        x = (x << 8) | (uint8_t)s[i];
    return x;
}

void HelloBanner() {
    cout << ravno << endl;
    cout << "           DES" << endl;
    cout << ravno << endl;
}


//ПРОГА
int main() {
    system("cls");
    HelloBanner();

 //   string text = "Now is t";
    uint64_t key = 0x0101010101010101; 
    uint64_t text_hex = 0x95F8A5E5DD31D900; 
    uint64_t encrypted_text = 0;
    uint64_t decrypted_text = 0;

   // cout << "Input text: " << text << endl;
	cout << "Bytes: "; printWord(text_hex); 
    cout << endl << endl;

    //ГЕНЕРАЦИЯ И ВЫВОД РАУНДОВЫХ КЛЮЧЕЙ
    uint64_t RoundKeys[16];
    RKeysGen(key, RoundKeys);
    for (int i =0; i < 16; i++) {
        cout << "Key[" << i << "]: "; printWord(RoundKeys[i]);
        cout << endl;
    }
    
    uint64_t ip = permute(text_hex, 64, IP, 64);
    printWord(ip);
    uint32_t L = 0; L |= ((ip & 0xFFFFFFFF00000000) >> 32) ;
    uint32_t R = 0; R |= ip & 0xFFFFFFFF;
    cout << endl << "L0: "; printWord(L); 
    cout << endl << "R0: "; printWord(R);
    
    //ШИФРОВАНИЕ
    for (int i = 0; i < 16; i++) {
        uint32_t tmp = R;
        uint32_t Fout= FeystelFunc(R, RoundKeys[i]);
        R = L ^ Fout;
        L = tmp;
    }
    
    ip = 0;
    ip += R; cout << endl << "R: "; printWord(R);
    ip = ip << 32; 
    ip |= L; cout << endl << "L: "; printWord(L);
    encrypted_text = permute(ip, 64, invIP, 64); 
    cout << endl << "Encrypted Text: "; printWord(encrypted_text); cout<<endl;


    //ДЕШИФРОВКА
    cout << ravno << endl << "        DECRYPT"<< endl << ravno << endl;
    cout << "Encrypted Text: "; printWord(encrypted_text); cout<<endl;

    ip = permute(encrypted_text, 64, IP, 64);
    R = 0; R |= ((ip & 0xFFFFFFFF00000000) >> 32) ;
    L = 0; L |= ip & 0xFFFFFFFF;
    cout << endl << "R15: "; printWord(R);
    cout << endl << "L15: "; printWord(L); 
    
    for (int i = 15; i >= 0; i--) {
        uint32_t tmp = L;
        uint32_t Fout= FeystelFunc(L, RoundKeys[i]);
        L = R ^ Fout;
        R = tmp;
    }
    ip = 0;
    ip += L; cout << endl << "R: "; printWord(R);
    ip = ip << 32; 
    ip |= R; cout << endl << "L: "; printWord(L);
    decrypted_text = permute(ip, 64, invIP, 64);
    cout << endl << "Decrypted Text: "; printWord(decrypted_text); cout<<endl;
    cout << "Correct? "<<(text_hex == decrypted_text ? "TRUE" : "FALSE") << endl;
    system("pause");
    return 0;
}