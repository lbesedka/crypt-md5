#include <iostream>
#include <cmath>
#include <iomanip>


typedef unsigned char BYTE;
#define funf(x, y, z) ((x & y) | (~x & z))
#define fung(x, y, z) ((x & z) | (~z & y))
#define funh(x, y, z) (x ^ y ^ z)
#define funi(x, y, z) (y ^ (~z | x))


using namespace std; 
unsigned int A = 0x01234567;
unsigned int B = 0x89ABCDEF;
unsigned int C = 0xFEDCBA98;
unsigned int D = 0x76543210;
unsigned int T[64];

unsigned int S[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };


//выравнивание потока шаг 1
string TextPreparation(string text) 
{
    uint64_t len_text = text.length(); 
    if ((len_text * 8) % 512 < 448) {
        text += (char)0x80;
    }
    while ((text.length() * 8) % 512 != 448)
    {
        text += (char)0x00;
    }
    //добавление длины текста
    uint8_t result[8];
    memcpy(result, &len_text, sizeof(len_text));
    for (int i = 0; i < 8; ++i)
        text += (char)result[i];
    cout << text.length() << endl;
    return text;
}

unsigned int FunF(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int m, int s, unsigned int t)
{
    a = b + ((a + funf(b, c, d) + m + t) << s);
    return a;
}

unsigned int FunG(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int m, int s, unsigned int t)
{
    a = b + ((a + fung(b, c, d) + m + t) << s);
    return a;
}

unsigned int FunH(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int m, int s, unsigned int t)
{
    a = b + ((a + funh(b, c, d) + m + t) << s);
    return a;
}

unsigned int FunI(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int m, int s, unsigned int t)
{
    a = b + ((a + funi(b, c, d) + m + t) << s);
    return a;
}

void TPreparation()
{
    for (int i = 0; i < 64; ++i)
        T[i] = (int)(4294967296 * abs(sin(i+1)));
}

unsigned int* CreateBlock(string text)
{
    unsigned int* words = new unsigned int[16];
    unsigned char word[4];
    unsigned int tmp = 0;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 4; ++j)
            word[j] = text[j + (i * 4)];
        memcpy(&tmp, word, sizeof(word));
        words[i] = tmp;
    }
    return words;
}

void MainCycle(unsigned int* m)
{
    unsigned int a = A;
    unsigned int b = B;
    unsigned int c = C;
    unsigned int d = D;
    unsigned int buffer[4] = { a, b, c, d };
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunF(buffer[(4 - (i % 4)) % 4], buffer[(4 - (i + 1 % 4) % 4)],
            buffer[(4 - (i + 2 % 4) % 4)], buffer[(4 - (i + 3 % 4) % 4)], m[i], S[i], T[i]);
    }
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunG(buffer[(4 - (i % 4)) % 4], buffer[(4 - (i + 1 % 4) % 4)],
            buffer[(4 - (i + 2 % 4) % 4)], buffer[(4 - (i + 3 % 4) % 4)], m[(1 + (i * 5)) % 16], S[i + 16], T[i + 16]);
    }
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunH(buffer[(4 - (i % 4)) % 4], buffer[(4 - (i + 1 % 4) % 4)],
            buffer[(4 - (i + 2 % 4) % 4)], buffer[(4 - (i + 3 % 4) % 4)], m[(5 + (i * 3)) % 16], S[i + 32], T[i + 32]);
    }
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunI(buffer[(4 - (i % 4)) % 4], buffer[(4 - (i + 1 % 4) % 4)],
            buffer[(4 - (i + 2 % 4) % 4)], buffer[(4 - (i + 3 % 4) % 4)], m[(0 + (i * 7)) % 16], S[i + 48], T[i + 48]);
    }
    A += buffer[0];
    B += buffer[1];
    C += buffer[2];
    D += buffer[3];
}

string Hash(string text)
{
    string substring;
    string result = "";
    text = TextPreparation(text);
    TPreparation();
    for (int i = 0; i < (text.length() * 8) / 512; ++i)
    {
        substring = text.substr(i * 64, (i + 1) * 64);
        MainCycle(CreateBlock(substring));
    }
    unsigned int buffer[4] = {A, B, C, D};
    const int siz_ar = sizeof(buffer) / sizeof(unsigned int);
    for (int i = 0; i < siz_ar; ++i)
        cout << buffer[i] << " ";
    cout << endl;

    for (int i = 0; i < siz_ar; ++i)
        cout << hex << setfill('0') << setw(2) << buffer[i] << " ";
    cout << endl;

    return result;

}


int main()
{
    string text = "md4";
    cout << Hash(text) << endl; 
}

