# CaesarCipher
Crypt, Decrypt and crack Caesar Cipher with keyword
## Crypt and Decrypt
The Caesar keyword encryption system is a single-alphabetic substitution system. The peculiarity of this system is the use of the keyword to shift and change the order of characters in the substitution alphabet
Example:
If k = 5 and keyword = "DIPLOMAT" we get encrypt alphabet
![image](https://user-images.githubusercontent.com/61086315/140827808-d57aba34-38eb-4882-b6c8-5f62feb4cf5a.png)
So, if we input **SEND MORE MONEY** we'll get encrypt text **HZBY TCGZ TCBZS**
## Crack system
The hacking system uses frequency analysis and dictionary words. First the n-grams are analyzed, then the words from the resulting text are matched with words from the dictionary that differ by one letter
## About Input and Output
You can only specify the path to the file with the text, the output will be produced in the file "output.txt". 
In order to use the hack, you must create a file "dictionary.txt" with any text, preferably more than 10000 characters.
