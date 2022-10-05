//  資訊四乙 10627207 陳柏佑
//  3-DES project

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include<sstream>
#include <string.h>
#include <bitset>
#include <vector>

using namespace std ;

/*
Input : 64-bit keys/ Plaintext
(1)Encryption with DES :
      The initial permutation(64-bit)
      The round function (repeated 16 times) :(32-bit)
        E Function(48-bit)
        Exclusive-OR(48-bit)
        The DES S-Boxes(32-bit)
        The P Function
      The final permutation
*/

int IP[] = {  // Initial Permutation : 在第一輪開始之前變換plaintext排列
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17,  9, 1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 15, 7
} ;

int E[] = {   // Expansion Function : 經IP過的輸入64bit會分成左半32-bit and 右半32-bit, 右半會做E Function => 32-bits -> 48-bit
  32, 1,  2,  3,  4,  5,
  4,  5,  6,  7,  8,  9,
  8,  9, 10, 11, 12, 13,
  12, 13, 14, 15, 16, 17,
  16, 17, 18, 19, 20, 21,
  20, 21, 22, 23, 24, 25,
  24, 25, 26, 27, 28, 29,
  28, 29, 30, 31, 32,  1
} ;

// ------------S-BOX-----------------

/*
將輸入分成8個6-bit
用6-bit的外部兩位與內部四位位查表以得出4-bit
最後變成8個4-bit ===> 32-bit(輸出)
*/

int S1[4][16] = {
  14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
   0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
   4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
  15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
} ;

int S2[4][16] = {
  15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
   3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
   0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
  13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
} ;

int S3[4][16] = {
  10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
  13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
  13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
   1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
} ;

int S4[4][16] = {
   7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
  13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
  10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
   3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
} ;

int S5[4][16] = {
   2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
  14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
   4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
  11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
} ;

int S6[4][16] = {
  12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
  10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
   9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
   4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
} ;

int S7[4][16]= {
   4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
  13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
   1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
   6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
} ;

int S8[4][16]= {
  13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
   1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
   7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
   2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
} ;

// ------------S-BOX-----------------

int P[] = {  // P function : 輸入為32-bit 輸出也為32-bit 主要做位移置換
  16,  7, 20, 21,
  29, 12, 28, 17,
  1,  15, 23, 26,
  5,  18, 31, 10,
  2,   8, 24, 14,
  32, 27,  3,  9,
  19, 13, 30,  6,
  22, 11,  4, 25
} ;

int FP[] = {  // Final Permutation
  40, 8, 48, 16, 56, 24, 64, 32,
  39, 7, 47, 15, 55, 23, 63, 31,
  38, 6, 46, 14, 54, 22, 62, 30,
  37, 5, 45, 13, 53, 21, 61, 29,
  36, 4, 44, 12, 52, 20, 60, 28,
  35, 3, 43, 11, 51, 19, 59, 27,
  34, 2, 42, 10, 50, 18, 58, 26,
  33, 1, 41,  9, 49, 17, 57, 25
} ;

// ----------Permuted Choice-------------

/*
(1)將輸入的key透過PC1刪減 64-bit -> 56-bit(只做一次)
(2)56-bit分成28-bit and 28-bit subkeys (當作C/D)

// -----以下動作重複16次-----
(3)分別對C and D 做向左移(左移多少單位看這是第幾輪)
(4)再讓C and D 做 PC2的刪減 56-bit -> 48-bit  round key
(5)回到(3)繼續做
*/

int PC1[] = {   // Permuted Choice 1 : 捨棄某一些bit(輸出為56-bit) 捨棄8的倍數的bit當做奇偶校驗位 共捨棄8個bit
  57, 49, 41, 33, 25, 17,  9,
  1,  58, 50, 42, 34, 26, 18,
  10,  2, 59, 51, 43, 35, 27,
  19, 11,  3, 60, 52, 44, 36,
  63, 55, 47, 39, 31, 23, 15,
  7,  62, 54, 46, 38, 30, 22,
  14,  6, 61, 53, 45, 37, 29,
  21, 13,  5, 28, 20, 12,  4
} ;

int PC2[] = {   // Permuted Choice 2 : 捨棄某一些bit(輸出為48-bit) 共捨棄8個bit
  14, 17, 11, 24,  1,  5,
  3,  28, 15,  6, 21, 10,
  23, 19, 12,  4, 26,  8,
  16,  7, 27, 20, 13,  2,
  41, 52, 31, 37, 47, 55,
  30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53,
  46, 42, 50, 36, 29, 32
} ;

int SHIFTS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 } ;  // 每一輪都需要將subkeys左移shift單位

// ----------Permuted Choice-------------

string TextToBinaryString(string words) {
    string binaryString = "";
    for ( int i = 0 ; i < words.length() ; i++ ) {
        bitset<8> bs4(words[i]);
        binaryString = binaryString + bs4.to_string() ;
    }
    return binaryString;
}

string convertToString(char* a, int size) {
  int i;
  string s = "";
  for (i = 0; i < size; i++) {
    s = s + a[i];
  }
  return s;
}


string BinaryStringToText(string binaryString) {
    string text = "";
    stringstream sstream(binaryString);
    while (sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        text += char(bits.to_ulong());
    }
    return text;
}

string HextoBit( string input ) { // 16bit->2-bit
  string key48hex ;
  for ( int i = 0 ; i < input.length() ; i++ ) {
    string temp = input.substr( i, 1 ) ;
    if ( temp.compare("0") == 0 ) key48hex = key48hex + "0000" ;
    else if ( temp.compare("1") == 0 ) key48hex = key48hex + "0001" ;
    else if ( temp.compare("2") == 0 ) key48hex = key48hex + "0010" ;
    else if ( temp.compare("3") == 0 ) key48hex = key48hex + "0011" ;
    else if ( temp.compare("4") == 0 ) key48hex = key48hex + "0100" ;
    else if ( temp.compare("5") == 0 ) key48hex = key48hex + "0101" ;
    else if ( temp.compare("6") == 0 ) key48hex = key48hex + "0110" ;
    else if ( temp.compare("7") == 0 ) key48hex = key48hex + "0111" ;
    else if ( temp.compare("8") == 0 ) key48hex = key48hex + "1000" ;
    else if ( temp.compare("9") == 0 ) key48hex = key48hex + "1001" ;
    else if ( temp.compare("A") == 0 ) key48hex = key48hex + "1010" ;
    else if ( temp.compare("B") == 0 ) key48hex = key48hex + "1011" ;
    else if ( temp.compare("C") == 0 ) key48hex = key48hex + "1100" ;
    else if ( temp.compare("D") == 0 ) key48hex = key48hex + "1101" ;
    else if ( temp.compare("E") == 0 ) key48hex = key48hex + "1110" ;
    else if ( temp.compare("F") == 0 ) key48hex = key48hex + "1111" ;
  } // for()
  
  return key48hex ;
  
} // HextoBit()

string permute( string key, int* table, int size ) { // 置換函數
    string newkey = "";
    for ( int i = 0 ; i < size ; i++ ) {
        newkey = newkey + key[table[i] - 1] ;
    }
    return newkey;
} // permute()

string ShiftLeft( string key28, int shift ) { // 向左移表中位置
    string newkey = "";
    for ( int i = 0; i < shift ; i++ ) {
        for ( int j = 1 ; j < 28 ; j++ ) {
            newkey = newkey + key28[j] ;
        }
        newkey = newkey + key28[0] ;
        key28 = newkey;
        newkey = "" ;
    }
    return key28 ;
} // ShiftLeft()

string BinarytoHex( string key48 ) {  // string
  string key48hex ;
  for ( int i = 0 ; i < key48.length() ; i = i + 4 ) {
    string temp = key48.substr( i, 4 ) ;
    if ( temp.compare("0000") == 0 ) key48hex = key48hex + "0" ;
    else if ( temp.compare("0001") == 0 ) key48hex = key48hex + "1" ;
    else if ( temp.compare("0010") == 0 ) key48hex = key48hex + "2" ;
    else if ( temp.compare("0011") == 0 ) key48hex = key48hex + "3" ;
    else if ( temp.compare("0100") == 0 ) key48hex = key48hex + "4" ;
    else if ( temp.compare("0101") == 0 ) key48hex = key48hex + "5" ;
    else if ( temp.compare("0110") == 0 ) key48hex = key48hex + "6" ;
    else if ( temp.compare("0111") == 0 ) key48hex = key48hex+ "7" ;
    else if ( temp.compare("1000") == 0 ) key48hex = key48hex + "8" ;
    else if ( temp.compare("1001") == 0 ) key48hex = key48hex + "9" ;
    else if ( temp.compare("1010") == 0 ) key48hex = key48hex + "A" ;
    else if ( temp.compare("1011") == 0 ) key48hex = key48hex + "B" ;
    else if ( temp.compare("1100") == 0 ) key48hex = key48hex + "C" ;
    else if ( temp.compare("1101") == 0 ) key48hex = key48hex + "D" ;
    else if ( temp.compare("1110") == 0 ) key48hex = key48hex + "E" ;
    else if ( temp.compare("1111") == 0 ) key48hex = key48hex + "F" ;
  } // for()
  
  return key48hex ;
} // BinarytoHex()

string Xor( string key48, string text48 ) { // key and text 做xor
    string output = "" ;
    for ( int i = 0 ; i < key48.length() ; i++ ) {
        if ( key48[i] == text48[i] ) output = output + "0" ;
        else output = output + "1" ;
    } // for()
    
    return output ;
} // Xor()

int S_boxF( int round, int box[8][6] ) {  // S-BOX Function
    int row, column ;
    int b6[6] ;
    for ( int i = 0 ; i < 6 ; i++ ) { // 處理一列6-bit
        b6[i] = box[round][i] ;
    } // for()

    row = b6[0] * 2 + b6[5] ;  // 列
    column = 8 * b6[1] + 4 * b6[2] + 2 * b6[3] + b6[4] ; // 行

    if ( round == 0 ) return S1[row][column] ;
    else if ( round == 1 ) return S2[row][column] ;
    else if ( round == 2 ) return S3[row][column] ;
    else if ( round == 3 ) return S4[row][column] ;
    else if ( round == 4 ) return S5[row][column] ;
    else if ( round == 5 ) return S6[row][column] ;
    else if ( round == 6 ) return S7[row][column] ;
    else if ( round == 7 ) return S8[row][column] ;
} // S_boxF()

string Encryption( string plaintext, string key64 ) { // 加密
  string key56 = "" ;
  string key28C = "" ;
  string key28D = "" ;
  string newkey56 = "" ;
  string key48 = "" ;
  string roundkey48b = "" ;
  string roundkey48h = "" ;
  string text64 = "" ;
  string text32L = "" ;
  string text32R = "" ;
  string text48 = "" ;
  string xoutput48 = "" ;
  string output32 = "" ;
  stringstream ss ;
  
  // --------------處理key--------------------
  key56 = permute( key64, PC1, 56 ) ; // 做Permuted Choice 1(只做一次) = 64-bit-> 56-bit
  // cout << key56 << endl ;
  key28C = key56.substr( 0, 28 ) ; // 28-bit
  key28D = key56.substr( 28, 28 ) ; // 28-bit
  // cout << key28C << endl ;
  // cout << key28D << endl ;
  
  vector<string> roundKey48hex ;
  vector<string> roundKey48binary ;
  for ( int i = 0 ; i < 16 ; i++ ) { // 做16輪
    int shift = SHIFTS[i] ;
    key28C = ShiftLeft( key28C, shift ) ; // 向左移
    key28D = ShiftLeft( key28D, shift ) ; // 向左移
    
    newkey56 = key28C + key28D ; // 56-bit
    // cout << newkey56 << endl ;

    roundkey48b = permute( newkey56, PC2, 48 ) ; // 做Permuted Choice 2(會做16次)
    // cout << roundkey48b << endl ;
    roundkey48h = BinarytoHex( roundkey48b ) ;
    // cout << roundkey48h << endl ;
    
    roundKey48binary.push_back( roundkey48b ) ;
    roundKey48hex.push_back( roundkey48h ) ;
  } // for()
  // --------------處理key--------------------
  
  // --------------處理text-------------------
  text64 = permute( plaintext, IP, 64 ) ; // The Initial Permutation
  // cout << text64 << endl ;
  text32L = text64.substr( 0, 32 ) ;      // 左半
  text32R = text64.substr( 32, 32 ) ;     // 右半 --> 拿去計算
  // --------------處理text-------------------
  
  for ( int i = 0 ; i < 16 ; i++ ) {
    output32 = "" ;
    text48 = permute( text32R, E, 48 ) ; // Expansion (32-bit -> 48-bit)
    xoutput48 = Xor( roundKey48binary[i], text48 ) ;  // key48 and text48 做xor
    // cout << "XOR後的48-bit: " << xoutput48 << endl ;
    
    // --------S-BOX----------
    char xop48[50] ;
    strcpy( xop48 , xoutput48.c_str() ) ;
    int box[8][6] ;  // 裝8份6-bit
    int count = 0 ;
    for ( int j = 0 ; j < 8 ; j++ ) {     // 將48-bit分成8份, 一份有6-bit
      for ( int k = 0 ; k < 6 ; k++ ) {
        int temp = int(xop48[count]) - 48 ;
        box[j][k] = temp ;
        count++ ;
      } // for()
      
    } // for()
    
    
    for ( int i = 0 ; i < 8 ; i++ ) { // 跑8輪
      int num = S_boxF( i, box ) ;
      // cout << num << endl;
      bitset<4>bit(num) ;
      output32 = output32 + bit.to_string() ;
    } // for()
    
    // cout << "S-BOX之後: " << output32.length() << endl ;
    // --------S-BOX----------
    
    string p32 = permute( output32, P, 32 ) ; // P function
    // cout << "after sxor: " << output32 << endl ;
    string x = Xor( p32, text32L ) ;  // 做xor
    text32L = x ;
    if ( i != 15 ) swap( text32L, text32R ) ; // L and R 交換
  } // for()
  
  string combine64 = text32L + text32R ;
  combine64 = permute( combine64, FP, 64 ) ;  // FP function
  // cout << BinarytoHex( combine64, 64 ) ;
  return combine64 ;
  
} // Encryption()

string Decryption( string ciphertext, string key64 ) { // 解密
  string key56 = "" ;
  string key28C = "" ;
  string key28D = "" ;
  string newkey56 = "" ;
  string key48 = "" ;
  string roundkey48b = "" ;
  string roundkey48h = "" ;
  string text64 = "" ;
  string text32L = "" ;
  string text32R = "" ;
  string text48 = "" ;
  string xoutput48 = "" ;
  string output32 = "" ;
  stringstream ss ;

  // --------------處理key--------------------
  key56 = permute( key64, PC1, 56 ) ; // 做Permuted Choice 1(只做一次) = 64-bit-> 56-bit
  // cout << key56 << endl ;
  key28C = key56.substr( 0, 28 ) ; // 28-bit
  key28D = key56.substr( 28, 28 ) ; // 28-bit
  // cout << key28C << endl ;
  // cout << key28D << endl ;

  vector<string> roundKey48hex ;
  vector<string> roundKey48binary ;
  for ( int i = 0 ; i < 16 ; i++ ) { // 做16輪
    int shift = SHIFTS[i] ;
    key28C = ShiftLeft( key28C, shift ) ; // 向左移
    key28D = ShiftLeft( key28D, shift ) ; // 向左移

    newkey56 = key28C + key28D ; // 56-bit
    // cout << newkey56 << endl ;

    roundkey48b = permute( newkey56, PC2, 48 ) ; // 做Permuted Choice 2(會做16次)
    // cout << roundkey48b << endl ;
    roundkey48h = BinarytoHex( roundkey48b ) ;
    // cout << roundkey48h << endl ;

    roundKey48binary.push_back( roundkey48b ) ;
    roundKey48hex.push_back( roundkey48h ) ;
  } // for()
  // --------------處理key--------------------


  // --------------處理ciphertext-------------------
  text64 = permute( ciphertext, IP, 64 ) ; // The Initial Permutation
  // cout << text64 << endl ;
  text32L = text64.substr( 0, 32 ) ;      // 左半
  text32R = text64.substr( 32, 32 ) ;     // 右半 --> 拿去計算
  
  for ( int i = 15 ; i >= 0 ; i-- ) {
    output32 = "" ;
    text48 = permute( text32R, E, 48 ) ; // Expansion (32-bit -> 48-bit)
    xoutput48 = Xor( roundKey48binary[i], text48 ) ;  // key48 and text48 做xor
    // --------S-BOX----------
    char xop48[50] ;
    strcpy( xop48 , xoutput48.c_str() ) ;
    int box[8][6] ;  // 裝8份6-bit
    int count = 0 ;
    for ( int j = 0 ; j < 8 ; j++ ) {     // 將48-bit分成8份, 一份有6-bit
      for ( int k = 0 ; k < 6 ; k++ ) {
        int temp = int(xop48[count]) - 48 ;
        box[j][k] = temp ;
        count++ ;
      } // for()

    } // for()


    for ( int i = 0 ; i < 8 ; i++ ) { // 跑8輪
      int num = S_boxF( i, box ) ;
      // cout << num << endl;
      bitset<4>bit(num) ;
      output32 = output32 + bit.to_string() ;
    } // for()

    // cout << "S-BOX之後: " << output32.length() << endl ;
    // --------S-BOX----------
    string p32 = permute( output32, P, 32 ) ; // P function
    string x = Xor( p32, text32L ) ;  // 做xor
    text32L = x ;
    if ( i != 0 ) swap( text32L, text32R ) ; // L and R 交換
         
  } // for()
  
  string combine64 = text32L + text32R ;
  combine64 = permute( combine64, FP, 64 ) ;  // FP function
  // cout << BinarytoHex( combine64, 64 ) ;
  return combine64 ;
  
  
  
  // --------------處理ciphertext-------------------

} // Decryption()

string Text2BinaryString( char inputtext[255] ) {
  string text64 = "" ;
  for ( int i = 0 ; i < 8 ; i++ ) {
    bitset<8>sub(inputtext[i]) ;
    text64 = text64 + sub.to_string() ;
    if ( text64.length() == 64 ) break ;
  } // for()
  
  if ( text64.length() < 64 ) { // 湊成64-bit
    bitset<8>ch(text64) ;
    text64 = ch.to_string() ;
  } // if()

  return text64 ;
  
} // Text2BinaryString()

int main() {
  bool error = false ;

  while ( !error ) {
    string inputkey1 = "" ;
    string inputkey2 = "";
    string inputkey3 = "" ;
    string key1 = "" ;
    string key2 = "" ;
    string key3 = "" ;
    char temp[255] ;
    string inputtext = "" ;
    string plaintext = "" ;
    
     // key => 16個字元
    cout << "-----------------3-DES--------------------" << endl ;
    cout << "請輸入文本: " ; // plaintext
    getline( cin, inputtext ) ;
    // inputtext = convertToString( temp, strlen(temp) ) ;
    
    cout << "請輸入金鑰1: " ; // K1-> 16個字
    cin >> inputkey1 ;
    cout << "請輸入金鑰2: " ; // K2-> 16個字
    cin >> inputkey2 ;
    cout << "請輸入金鑰3: " ; // K3-> 16個字
    cin >> inputkey3 ;

    cout << "-----------------3-DES--------------------" << endl ;
    
      // -----------處理KEY----------------
      key1 = TextToBinaryString( inputkey1 ) ; // 64-bit 金鑰
      key2 = TextToBinaryString( inputkey2 ) ; // 64-bit
      key3 = TextToBinaryString( inputkey3 ) ; // 64-bit
      // -----------處理KEY----------------
      
      // cout << key1 << endl ;
      // cout << key2 << endl ;
      // cout << key3 << endl ;
      cout << "\n" ;
      cout << "原始密文: " ;
      cout << inputtext << endl ;
      
      
      char c = 0;
	    while ( inputtext.length()% 8 != 0) { //Plain text less than 8 digits will automatically fill in 0
		    inputtext += c;
		    // cout << "in" << endl ;
	    } // while()
      // cout << inputtext << endl ;

      // -----------處理TEXT----------------
      plaintext = TextToBinaryString( inputtext ) ; // 文本(char array to binary)(64-bit)
      // cout << "文本(in bit): " << plaintext << endl ;
      // cout << "文本(in bit)長度: " <<plaintext.length() << endl ;
      // -----------處理TEXT----------------
      
      cout << "\n" ;

    string result = "" ;
    cout << "3-DES(加密): " << endl ;
    for ( int a = 0 ; a < plaintext.length() ; a++ ) {
      string temp = plaintext.substr(a*64,64) ;
      if ( temp.length() == 0 ) break ;
      
      string cipher1 = Encryption( plaintext.substr(a*64,64), key1 ) ; // 第一層加密  k1
      cout << "第一層加密: " << BinarytoHex( cipher1 ) << endl ;
      string  cipher2 = Decryption( cipher1, key2 ) ;  // 第二層解密  k2
      cout << "第二層解密: " << BinarytoHex( cipher2 ) << endl ;
      string cipher3 = Encryption( cipher2, key3 ) ; // 第三層加密  k3
      cout << "第三層加密: " << BinarytoHex( cipher3 ) << endl ;
      result = result + cipher3 ;
    } // for()
    
    cout << "\n" ;

    cout << "======> 加密密文: " << BinarytoHex( result ) << endl ;

    cout << "\n" ;
    
    cin.ignore() ;
    
    cout << "請輸入密文: " ;
    string rr = "" ;
    string ss = "" ;
    getline( cin, rr ) ;
    ss = HextoBit( rr ) ;
    
    
    string result1 = "" ;
    cout << "3-DES(解密): " << endl ;
    for ( int a = 0 ; a < ss.length() ; a++ ) {
      string temp = ss.substr(a*64,64) ;
      if ( temp.length() == 0 ) break ;
      string cipher4 = Decryption( ss.substr(a*64,64), key3 ) ; // 第一層解密  k1
      cout << "第一層解密: " << BinarytoHex( cipher4 ) << endl ;
      string  cipher5 = Encryption( cipher4, key2 ) ;  // 第二層加密  k2
      cout << "第二層加密: " << BinarytoHex( cipher5 ) << endl ;
      string retext = Decryption( cipher5, key1 ) ; // 第三層解密  k3
      cout << "第三層解密: " << BinarytoHex( retext ) << endl ;
      result1 = result1 + retext ;
    } // for()
      cout << "\n" ;

      cout << "======> 解密後文本: " << BinaryStringToText( result1 ) << endl ;
      
      cout << "\n" ;
      
      
      
  } // while()
  
} // main()
