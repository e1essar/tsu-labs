#include <iostream>

using namespace std;

class String
{
    char* str;
    int len;
public:
    String(int l = 0);
    String(const char*);
    String(const String&);
    ~String() { if (str) delete[]str; str = NULL; }//деструктор, при выходе из поля видимости функции, 
                                                   //он освобождает память перед выходом,но new не удаляет, new удаляется delete в функции
    int Len() { return len; }//возвращение длины строки

    String& operator = (const String&);
    char& operator [](int);

    bool operator == (String&);
    bool operator != (String&);

    String operator + (const String&);
    String& operator += (const String&);

    friend istream& operator >> (istream& r, String& s) {
        char buf[100];
        cout << "\n String:";
        gets_s(buf, 100);//конструктор
        String q(buf);//операция =
        s = q;
        return r;
    }
    friend ostream& operator << (ostream& r, String& s) {
        r << s.str;
        return r;
    }
    int BMH_search(String&);
};

String::String(int l) {//конструктор с аргументом по умолчанию (L=0 не повторяем, ошибка)
    str = new char[l + 1]; len = 0; str[0] = '\0';//выделяем память размером l
}

String::String(const char* s) {
    str = new char[strlen(s) + 1];//память длины строки +1 для нуля
    for (len = 0; s[len] != '\0'; str[len] = s[len], len++);//скопировали символы со cтроки s в строку str
    str[len] = '\0';
}

String::String(const String& s) {//конструктор копирования
    str = new char[s.len + 1];//выделяем память нужной длины
    for (len = 0; len <= s.len; str[len] = s.str[len], len++); len--;//копируем строку s
}

String& String::operator=(const String& s) {//функция присвоения !не при объявлении(конструктор)
    if (this != &s) { //если s=s, ничего не делать
        delete[] str;//освобождаем память *this
        str = new char[(len = s.len) + 1];//выделяем память длины len
        strcpy_s(str, len + 1, s.str);//копируем в str из s
    }
    return *this;//a=b
}

char& String::operator [](int i) {//функция возвращения символа по индексу
    if (i<0 || i>len) {//не допускает отрицательные значения
        printf("\n Index out"); exit(0);
    }
    return str[i];//возвращение символа
}

bool String::operator == (String& s) {//функция сравнения на равенство
    if (strcmp(str, s.str) == 0) return true;
    return false;
}

bool String::operator != (String& s) {//функция сравнения на неравенство
    if (strcmp(str, s.str) != 0) return true;
    return false;
}

String String::operator + (const String& s) {//перегрузка операции +
    String z(len + s.len + 1);//строка суммарной длины
    z.len = len + s.len;
    strcpy_s(z.str, z.len + 1, str);//в z пишем первую строку
    strcat_s(z.str, z.len + 1, s.str);//дописываем строку s
    return z;//работает конструктор копирования и уничтожается деструктором
}

String& String::operator += (const String& s) {//перегрузка операции +=
    char* t = new char[len + 1];//выделяем память
    strcpy_s(t, len + 1, str);//копируем в t старую строку
    delete[]str;//удалили память с str
    len = len + s.len;//новая длина
    str = new char[len + 1];
    strcpy_s(str, len + 1, t);//копируем в t старую строку
    strcat_s(str, len + 1, s.str);//дописываем строку s
    delete[]t;
    return *this; //возвращаем объект
}

int String::BMH_search(String& s) {

    int tab[256];//создаем массив символов ASCII
    int sh = 0;//объявляем переменную для обозначения индекса сдвига
    int j = -1;//переменная для индекса в образе

    for (int i = 0; i < 255; i++)//заполняем весь массив символов значениями длины образа
        tab[i] = s.len;

    for (int i = 0; i < s.len - 1; i++) {//значения элементов для массива tab для БМ поиска
        tab[(int)s.str[i]] = s.len - i - 1;
    }

    while (sh <= (len - s.len)) {

        j = s.len - 1;//индекс образа на конец самого образа

        while (j >= 0 && s.str[j] == str[sh+j])//если символы образа и строки совпадают то понижаем счетчик j
            j--;

        if (j < 0) return sh;//если в итоге j<0 то возвращаем нужный сдвиг
        else
            j = s.len - 1;
            sh += tab[str[sh+j]];//повышаем сдвиг на значения по символу в массиве tab
    }
    
    if (j >= 0) return (-1);//если по выходу из цикла j равно отрицательному значению, то образа в строке нет
    else 
        return sh;//возвращение сдвига
}

int main() {
    /*
    String s1("The Lord of the"); //конструкторы с аргументом строкой
    String s2(6);
    cin >> s2;
    String s3 = s2;//конструктор копирования
    cout << "1:" << s1 << '\n';
    cout << "2:" << s2 << '\n';
    cout << "3:" << s3 << '\n';

    s3[1] = 'S'; //меняем значения символов в s3
    s3[2] = 'o';
    cout << "new 3:" << s3 << '\n';

    String s4;
    s4 = s1 + s2;
    cout << s4 << '\n';

    s3 += s2;//перегрузка +=
    cout << s3;

    if (s2 == s3) cout << "\nEqual";//равенство
    if (s2 != s3) cout << "\nNot Equal";//неравенство

    s3 = s2;//перегрузка ==
    cout << "\n------Changing------";
    if (s2 == s3) cout << "\nEqual\n";//равенство
    if (s2 != s3) cout << "\nNot Equal\n";//неравенство
    */

    String s5("THIS IS A TEST");
    String s6("TEST");
    int shift = s5.BMH_search(s6);
    if (shift >= 0)
        cout << "Pattern found out at:" << shift;
    else
        cout << "No pattern";
}
