#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>


typedef unsigned char BYTE;
#define funf(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define fung(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define funh(x, y, z) ((x) ^ (y) ^ (z))
#define funi(x, y, z) ((y) ^ ((x) | (~z)))
#define rotate_left(x, n) (((x) << (n)) | ((x) >> (32 - (n))))


using namespace std; 

uint32_t D = 0x10325476;
uint32_t C = 0x98badcfe;
uint32_t B = 0xefcdab89;
uint32_t A = 0x67452301;

//unsigned int T[64];
//unsigned int A = 0x01234567;
//unsigned int B = 0x89ABCDEF;
//unsigned int C = 0xFEDCBA98;
//unsigned int D = 0x76543210;

const int S[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };


const uint32_t T[64] = { 
 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

string to_string(int param)
{
    string str = "";
    for (str = ""; param; param /= 10)
        str += (char)('0' + param % 10);
    reverse(str.begin(), str.end());
    return str;
}

string Uint32ToHexForm(uint32_t a)
{
    string res;

    for (int i = 0; i < 4; i++)
    {
        int lowByte = a % 16;
        a /= 16;

        int highByte = a % 16;
        a /= 16;

        if (highByte > 9)
            res += 'a' + highByte - 10;
        else
            res += std::to_string(highByte);

        if (lowByte > 9)
            res += 'a' + lowByte - 10;
        else
            res += std::to_string(lowByte);
    }

    return res;
}

//выравнивание потока шаг 1
string TextPreparation(string text) 
{
    uint64_t len_text = text.length(); 
    if ((len_text * 8) % 512 < 448) {
        text += (unsigned char)0x80;
    }
    while ((text.length() * 8) % 512 != 448)
    {
        text += (unsigned char)0x00;
    }
    //добавление длины текста
    uint8_t result[8];
   // memcpy(result, &len_text, sizeof(len_text));
    /*text += (unsigned char)result[0];
    text += (unsigned char)result[7];
    text += (unsigned char)result[6];
    text += (unsigned char)result[5];
    text += (unsigned char)result[4];
    text += (unsigned char)result[1];
    text += (unsigned char)result[2];
    text += (unsigned char)result[3];*/
   /* 
    for (int i = 0; i < 8; i++)
        text += (char)result[i];*/

    len_text *= 8; 
    string len_add; 
    while (len_text / 256 > 0)
    {
        len_add += (char)(len_text % 256);
        len_text /= 256;
    }
    len_add += (char)(len_text % 256);
    for (int i = (len_add.length() - 1); i != -1; i--)
        text += len_add[len_add.length() - 1 - i];

    for (int i = 0; i < 8 - len_add.length(); i++)
        text += '\0';
    //cout << text.length() << endl;
    //cout << text << endl;
    //reverse(text.begin(), text.end());
    
    cout << text << endl;
    return text;
}

uint32_t FunF(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t m, int s, uint32_t t)
{
    a = b + (rotate_left((a + funf(b, c, d) + m + t), s));
    return a;
}

uint32_t FunG(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t m, int s, uint32_t t)
{
    a = b + (rotate_left((a + fung(b, c, d) + m + t), s));
    return a;
}

uint32_t FunH(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t m, int s, uint32_t t)
{
    a = b + (rotate_left((a + funh(b, c, d) + m + t), s));
    return a;
}

uint32_t FunI(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t m, int s, uint32_t t)
{
    a = b + (rotate_left((a + funi(b, c, d) + m + t), s));
    return a;
}

//void TPreparation()
//{
//    for (int i = 0; i < 64; ++i)
//        T[i] = floor((4294967296 * abs(sin(i))));
//}

unsigned int CreateWord(unsigned char* input)
{
    unsigned int result = 0;
    result = ((uint32_t)input[0]) | (((uint32_t)input[1]) << 8) |
        (((uint32_t)input[2]) << 16) | (((uint32_t)input[3]) << 24);
    return result;
}

unsigned int* CreateBlock(string text)
{
    uint32_t* words = new uint32_t[16];
    unsigned char word[4];
    uint32_t tmp = 0;
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 4; ++j)
            word[j] = text[j + (i * 4)];
        tmp = CreateWord(word);
        words[i] = tmp;
    }
    return words;
}

void MainCycle(string mes)
{
    uint32_t m[16];
    for (int i = mes.length() - 1; i > -1; i -= 4)
    {
        m[i / 4] = 0;
        m[i / 4] |= (unsigned char)mes[i] << 24;
        m[i / 4] |= (unsigned char)mes[i - 1] << 16;
        m[i / 4] |= (unsigned char)mes[i - 2] << 8;
        m[i / 4] |= (unsigned char)mes[i - 3];
    }
    uint32_t a = A;
    uint32_t b = B;
    uint32_t c = C;
    uint32_t d = D;
    uint32_t buffer[4] = { a, b, c, d }; // 
    //std::cout << "F" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunF(buffer[(4 - (i % 4)) % 4], buffer[(4 - ((i + 3) % 4)) % 4],
            buffer[(4 - ((i + 2) % 4)) % 4], buffer[(4 - ((i + 1) % 4)) % 4], m[i], S[i], T[i]);
       
    }
    cout << endl;
    //std::cout << "G" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunG(buffer[(4 - (i % 4)) % 4], buffer[(4 - ((i + 3) % 4)) % 4],
            buffer[(4 - ((i + 2) % 4)) % 4], buffer[(4 - ((i + 1) % 4)) % 4], m[(1 + (i * 5)) % 16], S[i + 16], T[i + 16]);
        
    }
    //std::cout << "H" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunH(buffer[(4 - (i % 4)) % 4], buffer[(4 - ((i + 3) % 4)) % 4],
            buffer[(4 - ((i + 2) % 4)) % 4], buffer[(4 - ((i + 1) % 4)) % 4], m[(5 + (i * 3)) % 16], S[i + 32], T[i + 32]);
    }
    //std::cout << "I" << std::endl;
    for (int i = 0; i < 16; ++i)
    {
        buffer[(4 - (i % 4)) % 4] = FunI(buffer[(4 - (i % 4)) % 4], buffer[(4 - ((i + 3) % 4)) % 4],
            buffer[(4 - ((i + 2) % 4)) % 4], buffer[(4 - ((i + 1) % 4)) % 4], m[(i * 7) % 16], S[i + 48], T[i + 48]);
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
    //TPreparation();
    for (int i = 0; i < (text.length() * 8) / 512; ++i)
    {
        substring = text.substr(i*64, 64);
        //MainCycle(CreateBlock(substring));
        MainCycle(substring);
    }
    result = Uint32ToHexForm(A) + Uint32ToHexForm(B) + Uint32ToHexForm(C) + Uint32ToHexForm(D);
    return result;

}


int main()
{
    string text = "message digest";
    cout << Hash(text) << endl; 
}

