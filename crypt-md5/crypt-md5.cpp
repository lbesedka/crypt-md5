#include <iostream>


typedef unsigned char BYTE;
#define funf(x, y, z) ((x & y) | (~x & z))
#define fung(x, y, z) ((x & z) | (~z & y))
#define funh(x, y, z) (x ^ y ^ z)
#define funi(x, y, z) (y ^ (~z | x))


using namespace std; 

BYTE A[4] = { 0x01, 0x23, 0x45, 0x67 };
BYTE B[4] = { 0x89, 0xAB, 0xCD, 0xEF };
BYTE C[4] = { 0xFE, 0xDC, 0xBA, 0x98 };
BYTE D[4] = { 0x76, 0x54, 0x32, 0x10 };


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


int main()
{
    string text = "dshkdfghkfdgjdfshksdhksdfsdfgsdsmjgjkdflsp[[";
    cout << TextPreparation(text) << endl; 
}

