# Triple-DES

![image](https://user-images.githubusercontent.com/91272874/194062509-8cd79223-f0b7-4bc0-9be2-72eb209e7e88.png)

Source from: https://www.tutorialspoint.com/cryptography/triple_des.htm

執行結果:

![image](https://user-images.githubusercontent.com/91272874/194062862-c173689a-1f14-4bd5-9ace-49ebddafd64b.png)

金鑰與文本輸入一律使用十六進位，且大小都為64-bit。
分別有三支不同的金鑰: key1, key2, key3。

輸出包含加密內做三次加密解密:

(1)金鑰1對文本做加密 -> 密文1

(2)金鑰2對密文1做解密 -> 密文2

(3)金鑰3對密文2作加密 -> 密文3(加密密文)

與解密內的三次解密加密:

(1)金鑰3對密文3做解密 -> 密文4

(2)金鑰2對密文4做加密 -> 密文5

(3)金鑰1對密文5作解密 -> 原始密文
