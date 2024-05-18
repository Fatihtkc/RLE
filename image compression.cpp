#include <iostream>
#include <fstream>
#include <new>
#include <string>
using namespace std;

class image{
public:
    string type;
    char** data;
    int width;
    int height;
    int size;
    ~image();    
};

void ignorecomments(ifstream &);
bool openpgm(image &,string);
void printdetails(const image &);
string compressimage(const image &);
image decompressimage(const string &);
int strtoint(string);


int main(){
    image pgm;
    string file = "pgmfile1.pgm";

    if(openpgm(pgm,file))
        printdetails(pgm);
    decompressimage(compressimage(pgm));
}

bool openpgm(image &pgm,string file){
    ifstream pgmfile(file,ios::binary);
    if(pgmfile.is_open()){
        
        ignorecomments(pgmfile);

        pgmfile >> pgm.type;
        ignorecomments(pgmfile);
        pgmfile >> pgm.width >> pgm.height;
        ignorecomments(pgmfile);
        pgmfile >> pgm.size;
        ignorecomments(pgmfile);

        if(pgm.type[1]=='5'){
            pgm.data = new (nothrow) char*[pgm.height];
            if(pgm.data!=nullptr){
                for(int i=0; i<pgm.height; i++){
                    pgm.data[i]= new (nothrow) char[pgm.width];
                    if(pgm.data[i]!=nullptr){
                        pgmfile.read(pgm.data[i],pgm.width*sizeof(char));
                    }
                    else
                        cerr << "matrix couldnt be created";
                }
            }
            else
                cerr << "array couldnt be created";
        }
        else if(pgm.type[1]=='2'){
            pgm.data = new (nothrow) char*[pgm.height];
            if(pgm.data!=nullptr){
                for(int i=0; i<pgm.height; i++){
                    pgm.data[i]= new (nothrow) char[pgm.width];
                    if(pgm.data[i]!=nullptr){
                        for(int j=0; j<pgm.width; j++){
                            pgmfile >> pgm.data[i][j];
                        }
                    }
                    else
                        cerr << "matrix couldnt be created";

                }
            }
            else
                cerr << "array couldnt be created";

        }
        else
            cerr << "wrong file type";

        pgmfile.close();
    }
    else
        cerr << "file couldnt be opened";
    return true;

};

void ignorecomments(ifstream &file){
    char ch;
    string line;

    while(((ch=file.get()) != EOF ) && (ch==(' ' || '\n' || '\t')));

    if(ch=='#'){
        getline(file,line);
        ignorecomments(file);
    }
    else
        file.seekg(-1,ios::cur);

};

void printdetails(const image &pgm){

    cout << "pgm type: " << pgm.type << endl;
    cout << "pgm width: " << pgm.width << endl;
    cout << "pgm height: " << pgm.height << endl;
    cout << "pgm maximum pixel value: " << pgm.size << endl;

    for(int i=0; i<640; i++)
    cout << pgm.data[200][i];
    cout << endl;

};

string compressimage(const image &pgm){

    string compressed;
    int count = 1;

    compressed+=to_string(pgm.type.length());
    compressed+=pgm.type;

    string width=to_string(pgm.width);
    compressed+=to_string(width.length());
    compressed+=to_string(pgm.width);

    string height=to_string(pgm.height);
    compressed+=to_string(height.length());
    compressed+=to_string(pgm.height);

    string size=to_string(pgm.size);
    compressed+=to_string(size.length());
    compressed+=to_string(pgm.size);

        cout << endl << endl;

    for(int i=0; i<pgm.height ; i++){
        for(int j=1; j<=pgm.width; j++){
            if((pgm.data[i][j]!=pgm.data[i][j-1]) || j==pgm.width){
                compressed+=to_string(count);
                count=1;
                compressed+="+";
                compressed+=pgm.data[i][j-1];
            }
            else
                count++;
        }
    };

    ofstream comp("C:\\Users\\Fatih\\Desktop\\comp1.txt");
    if(comp.is_open()){
        comp << compressed;
    };

    return compressed;
}

image decompressimage(const string &compressed){

    image oldimage;

    int a=0;
    int temp;
    temp=compressed[a++]-'0';
    oldimage.type=compressed.substr(a,temp);
    a+=temp;
    temp=0;
    temp=compressed[a++]-'0';
    string tempr;
        tempr=compressed.substr(a,temp);
    oldimage.width=strtoint(tempr);
    a+=temp;
    temp=0;
    tempr="";
    temp=compressed[a++]-'0';
        tempr=compressed.substr(a,temp);
    oldimage.height=strtoint(tempr);
    a+=temp;
    temp=0;
    tempr="";
    temp=compressed[a++]-'0';
        tempr=compressed.substr(a,temp);
    oldimage.size=strtoint(tempr);
    a+=temp;

    oldimage.data = new (nothrow) char*[oldimage.height];
        if(oldimage.data!=nullptr){
            for(int i=0; i<oldimage.height; i++){
                oldimage.data[i]= new (nothrow) char[oldimage.width];
                if(oldimage.data[i]==nullptr){
                    cerr << "decompressed matrix couldnt be created";
                }
            }
        }
        else
            cerr << "decompressed array couldnt be created";

    int j=0;
    int d=0;
    int p=(int)compressed.length();
    while (a < p) {
        string tm="+";
        size_t loc2= compressed.find(tm, a);
        int count;
        count = strtoint(compressed.substr(a, loc2-a));
        a=loc2+1;
        for (int k = 0; k < count; k++){
            if (d < oldimage.height && j < oldimage.width) {
                oldimage.data[d][j++] = compressed[a];
            } else
                break;
            if (j == oldimage.width) {
                j = 0;
                d++;
            }
        }

        a++;             

    }

    printdetails(oldimage);

    return oldimage;
}

int strtoint(string str){
    int result=0;
    for (int i=0; i < (int)str.length(); i++) {
        result = result * 10 + (str[i] - '0');
    }
    return result;
}

image::~image(){                
    delete[] this->data;
    this->data=nullptr;
}