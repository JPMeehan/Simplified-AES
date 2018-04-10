/*
Simply run program once per encryption/decryption need.
Asks at start for enc or dec to decide which to run
Inputs are 16 char bit strings
Error checking for CT/PT/key length, but no error checking for content of CT/PT/key length.
*/

#include <iostream>

using namespace std;

string keyExpand(string key);
string sBox(string s);
string XOR(string a, string b, int len); // to XOR a string, for use with step AKi
string sAESenc(string PT, string k);
string mixColumn(string top, string bot);
char XORc(char a, char b); // to XOR a single character
string sAESdec(string CT, string k);
string sBoxinv(string s);

int main()
{
    string mode, PT, CT, k;
    cout << "If you wish to encrypt, type 'enc'. If you wish to decrypt, type 'dec' (without the quotes)" << endl;
    cin >> mode;
    cout << endl;
    if (mode != "enc" && mode != "dec"){
        cout << "Invalid input, run the program again" << endl;
        return 0;
    }
    cout << "Please input the key as a 16 character bit string eg '0001000100010001' " << endl;
    cin >> k;
    if (k.length() != 16){
        cout << "Invalid key length, run the program again" << endl;
        return 0;
    }
    if (mode == "enc"){
        cout << "Please input the plaintext as a 16 character bit string" << endl;
        cin >> PT;
        if (PT.length() != 16){
            cout << "Invalid PT length, run the program again" << endl;
            return 0;
        }
        cout << "The CT is" << sAESenc(PT, k) << endl;
    }
    if (mode == "dec"){
        cout << "Please input the ciphertext as a 16 character bit string" << endl;
        cin >> CT;
        if (CT.length() != 16){
            cout << "Invalid CT length, run the program again" << endl;
            return 0;
        }
        cout << "The PT is " << sAESdec(CT, k) << endl;
    }
    return 0;
}

string sBox(string s){
    if (s=="0000"){
        return "1001";
    }
    else if (s=="0001") {
        return "0100";
    }
    else if (s=="0010") {
        return "1010";
    }
    else if (s=="0011") {
        return "1011";
    }
    else if (s=="0100") {
        return "1101";
    }
    else if (s=="0101") {
        return "0001";
    }
    else if (s=="0110") {
        return "1000";
    }
    else if (s=="0111") {
        return "0101";
    }
    else if (s=="1000") {
        return "0110";
    }
    else if (s=="1001") {
        return "0010";
    }
    else if (s=="1010") {
        return "0000";
    }
    else if (s=="1011") {
        return "0011";
    }
    else if (s=="1100") {
        return "1100";
    }
    else if (s=="1101") {
        return "1110";
    }
    else if (s=="1110") {
        return "1111";
    }
    else if (s=="1111") {
        return "0111";
    }
    cout << "Sbox error" << endl;
    return "0000";
}

string sBoxinv(string s){
    if (s=="1001"){
        return "0000";
    }
    else if (s=="0100") {
        return "0001";
    }
    else if (s=="1010") {
        return "0010";
    }
    else if (s=="1011") {
        return "0011";
    }
    else if (s=="1101") {
        return "0100";
    }
    else if (s=="0001") {
        return "0101";
    }
    else if (s=="1000") {
        return "0110";
    }
    else if (s=="0101") {
        return "0111";
    }
    else if (s=="0110") {
        return "1000";
    }
    else if (s=="0010") {
        return "1001";
    }
    else if (s=="0000") {
        return "1010";
    }
    else if (s=="0011") {
        return "1011";
    }
    else if (s=="1100") {
        return "1100";
    }
    else if (s=="1110") {
        return "1101";
    }
    else if (s=="1111") {
        return "1110";
    }
    else if (s=="0111") {
        return "1111";
    }
    cout << "Sbox error" << endl;
    return "0000";
}

string XOR(string a, string b, int len){
    string out;
    for (int i=0;i<len;i++){
        if (a[i]!=b[i])
            out+='1';
        else
            out+='0';
    }
    return out;
}

char XORc(char a, char b){
    if (a==b)
        return '0';
    else
        return '1';
}

string keyExpand(string key){
    string out, tempA, tempB, tempC, tempD, W1, W2, W3, W4;
    out+=key;
    //cout << out << endl;
    tempA=key.substr(8,4);
    tempB=key.substr(12,4);
    W1=tempA+tempB;
    //cout << W1 << endl;
    tempA=sBox(tempA);
    tempB=sBox(tempB);
    tempB=XOR(tempB, "1000", 4); // XOR with RCON[1]
    tempC=tempB+tempA; //
    W2=XOR(key, tempC, 8); // XOR first 8 bits of key with RCON[1] XOR SubNimb(RotNib(W[i-1]))
    out+=W2; // w[2] assembled
    //cout << W2 << endl;
    W3 = XOR(W1,W2,8);
    //cout << W3 << endl;
    out+=W3;
    tempA=W3.substr(0,4);
    tempB=W3.substr(4,4);
    tempA=sBox(tempA); //
    tempB=sBox(tempB); //
    tempB=XOR(tempB,"0011", 4); //XOR with RCon
    tempC=tempB+tempA; //RotNib
    W4=XOR(W2,tempC,8);
    out+=W4;
    //cout << W4 << endl;
    tempD=XOR(W4,W3,8);
    out+=tempD;
    return out;
}

string mixColumn(string top, string bot){
    string out="00000000";
    out[0]=XORc(top[0],bot[2]); // not doing in order because of the way the table is structured; corner pieces don't rely on each other, inside pieces are a bit XOR with a corner
    out[3]=XORc(top[3],bot[1]);
    out[4]=XORc(top[2],bot[0]);
    out[7]=XORc(top[1],bot[3]);
    out[1]=XORc(out[7],bot[0]);
    out[2]=XORc(out[4],bot[1]);
    out[5]=XORc(top[0],out[3]);
    out[6]=XORc(top[1],out[0]);
    //cout << out << endl;
    return out;
}

string mixColumninv(string top, string bot){
    string out="00000000";
    out[0]=XORc(top[3],bot[1]); // b3b5
    out[1]=XORc(top[0],bot[2]); // b0b6
    out[4]=XORc(top[1],bot[3]); // b1b7
    out[5]=XORc(top[2],bot[0]); // b2b4
    out[2]=XORc(out[4],bot[0]); // b1b4b7
    out[3]=XORc(out[5],top[3]); // b2b3b4
    out[6]=XORc(out[0],top[0]); // b0b3b5
    out[7]=XORc(out[1],bot[3]); // b0b6b7
    return out;
}

string sAESenc(string PT, string k){
    string CT, key, K0, K1, K2, tempA, tempB, tempC, tempD;
    key=keyExpand(k);
    //cout << key << endl;
    K0=key.substr(0,16); // filling out keys
    K1=key.substr(16,16);
    K2=key.substr(32,16);
    CT = XOR(PT,K0,16); // K0
    //cout << CT << endl;
    tempA = sBox(CT.substr(0,4)); // NS
    tempB = sBox(CT.substr(4,4));
    tempC = sBox(CT.substr(8,4));
    tempD = sBox(CT.substr(12,4));
    //cout << tempA + ' ' + tempD + ' ' + tempC + ' ' + tempB << endl;
    // SR - tempB and tempD swap places
    CT = mixColumn(tempA, tempD) + mixColumn(tempC, tempB);
    //cout << CT << endl;
    CT = XOR(CT,K1,16); // K1
    //cout << CT << endl;
    tempA = sBox(CT.substr(0,4)); // NS
    tempB = sBox(CT.substr(4,4));
    tempC = sBox(CT.substr(8,4));
    tempD = sBox(CT.substr(12,4));
    CT = tempA + tempD + tempC + tempB; // SR
    //cout << CT << endl;
    CT = XOR(CT,K2,16); // K2
    //cout << CT << endl;
    return CT;
}

string sAESdec(string CT, string k){
    string PT, key, K0, K1, K2, tempA, tempB, tempC, tempD;
    key=keyExpand(k);
    K0=key.substr(0,16); // filling out keys
    K1=key.substr(16,16);
    K2=key.substr(32,16);
    PT = XOR(CT,K2,16); // AK2
    //cout << "AK2: " << PT << endl;
    tempA = PT.substr(0,4);
    tempB = PT.substr(4,4);
    tempC = PT.substr(8,4);
    tempD = PT.substr(12,4);
    tempA=sBoxinv(tempA); // NS-1
    tempB=sBoxinv(tempB);
    tempC=sBoxinv(tempC);
    tempD=sBoxinv(tempD);
    PT = mixColumninv(tempA, tempD) + mixColumninv(tempC, tempB); // incorporating SR into the mixColumninv function call
    K1 = mixColumninv(K1.substr(0,4),K1.substr(4,4)) + mixColumninv(K1.substr(8,4),K1.substr(12,4));
    PT = XOR(PT, K1, 16); // AMC-1K1
    //cout << "After AMC-1K1: "<< PT << endl;
    tempA=sBoxinv(PT.substr(0,4)); // NS-1
    tempB=sBoxinv(PT.substr(4,4));
    tempC=sBoxinv(PT.substr(8,4));
    tempD=sBoxinv(PT.substr(12,4));
    PT = tempA+tempD+tempC+tempB; // SR-1
    //cout << PT << endl;
    PT = XOR(PT, K0, 16);
    return PT;
}
