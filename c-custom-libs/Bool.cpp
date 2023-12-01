#include <iostream>
#include <cmath>
#include <stdbool.h>
#include <string.h>
#include <array>
#include <fstream>

using namespace std;

// Класс булев вектор
class BoolV
{
	unsigned int* v;
	int nbit, m; // nbit – количество бит вектора, m – количество элементов массива v
public:
	BoolV(int nn = 1); // формируется булев вектор из nn бит; все биты со значениями 0
	BoolV(const char*); // формируется булев вектор по строке из нулей и единиц
	BoolV(const BoolV&);
	~BoolV();

	void Set1(int); // устанавливает указанный бит в 1
	void Set0(int); // устанавливает указанный бит в 0
	int operator[](int);
	BoolV operator = (const BoolV&);
	bool operator == (const BoolV&); // равенство векторов
	BoolV operator | (const BoolV&);
	BoolV operator & (const BoolV&);
	BoolV operator ~ ();

	friend ostream& operator << (ostream&, const BoolV&);
	friend istream& operator >> (istream&, BoolV&);
};

// Класс булева матрица
class BoolM
{
	BoolV* bm;
	int n, m; // m – количество строк, n – количество столбцов
public:
	BoolM(int k = 1, int l = 1); // формируется нулевая булева матрица размера kl
	BoolM(const BoolM&);

	BoolV& operator[] (int);
	BoolM operator = (const BoolM&);
	friend ostream& operator << (ostream&, BoolM&);
};
//Топологическая сортировка на булевой матрице
bool topsort(BoolM& M, int m, int* a);  // a – результат сортировки

BoolV::BoolV(int nn) {
	if (nn > 0) {
		nbit = nn;//заносим кол-во битов в векторе в переменную nbit
		m = ((nbit - 1) / 32) + 1;//память для массива по формуле 

		v = new unsigned int[m] {};//выделяем память

		for (int i = 0; i < m; i++)	//по умолчанию образуем нулевой булев вектор
			v[i] = 0;
	}
	else {
		cout << "\nFailed\n";//иначе выдаем ошибку
		exit(0);
	}
}

BoolV::BoolV(const char* s) {
	for (int i = 0; i < strlen(s); i++) {//проверка на корректность строки белевого вектора
		if (s[i] != '0' && s[i] != '1') {
			cout << "String is not correct";
			exit(0);
		}
	}
	nbit = strlen(s);//количество битов = длина строки
	m = (nbit - 1) / 32 + 1;//высчитываем память
	v = new unsigned int[m] {};//выделяем нужную память

	unsigned int mask = 1;//создаем маску равную 1
	int index = m - 1;//создаем переменную index для

	for (int i = nbit - 1; i >= 0; i--, mask <<= 1) {//в цикле проходим
		//по каждому биту вектора и сдвигаем маску =1
		if (!mask) {//на случай если вышли за пределы вектор первых 4 байтов
			mask = 1;//маску приравниваем 1
			index--;//уменьшаем index
		}
		if (s[i] == '1') {//если в строке встретили 1
			v[index] |= mask;//то выполняем дизъюнкцию векторов и в нужной позиции устанавливается 1
		}
	}
}

BoolV::BoolV(const BoolV& x) {
	nbit = x.nbit;//кол-во битов
	m = x.m;//размер 

	v = new unsigned int[m];//выделяем память для вектора

	for (int i = 0; i < m; i++) {//копируем
		v[i] = x.v[i];
	}
}

BoolV::~BoolV() {//деструктор
	delete[] v;//удаляем вектор
}

void BoolV::Set1(int k) {
	if (k >= 0 && k < nbit) {//проверяем пределы бита в вектора
		int i = m - 1 - k / 32;//определяем в каких 4 байтах находится k
		int bit = k % 32;//задаем бит который меняем
		unsigned int mask = 1 << bit;//сдвигаем единичную маску, чтобы в позиции k была 1
		v[i] |= mask;//дизъюнкция вектора и маски
	}
	else {
		cout << "k is out";//иначе ошибка
		exit(0);
	}
}

void BoolV::Set0(int k) {
	if (k >= 0 && k < nbit) {
		int i = m - 1 - k / 32;//определяем в каких 4 байтах находится k
		int bit = k % 32;//задаем бит который меняем
		unsigned int mask = 1 << bit;//сдвигаем единичную маску, чтобы в позиции k была 1

		if (operator[](k)) {//воспользуемся функцией возвращения значения по индексу
			mask = ~mask;//инвертируем маску
			v[i] &= mask;//выполняем коъюнкцию вектора и маски
		}
	}
	else {
		cout << "k is out";//иначе ошибка
		exit(0);
	}
}

int BoolV::operator[](int k) {
	if (k >= 0 && k < nbit) {////проверяем пределы бита в векторе
		int i = m - 1 - k / 32;//определяем в каких 4 байтах находится k
		int bit = k % 32;//задаем бит который находим
		unsigned int mask = 1 << bit;//сдвигаем единичную маску, чтобы в позиции k была 1

		if (v[i] & mask) {//если конъюнкция вектора и маски не дает 0
			return 1;//возвращаем 1
		}
		return 0;//иначе возвращаем 0
	}
	else {
		return -1;//в случае выхода из пределов возвращаем -1
	}
}

BoolV BoolV::operator=(const BoolV& v1) {
	if (this != &v1) {//проверяем не указывает ли вектор сам на себя
		m = v1.m;//приравниваем размеры
		nbit = v1.nbit;//приравниваем биты
		delete[] v;//удаляем память для v
		v = new unsigned int[m];//выделяем новую память для v
		for (int i = 0; i < m; i++)//присваиваем элементам из v элементы v1
			v[i] = v1.v[i];
	}
	return *this;//возвращаем указатель на вектор
}

bool BoolV::operator==(const BoolV& v1) {
	if (this != &v1) {//проверяем указание на самого себя
		if (nbit != v1.nbit) return false;//если количество битов не равно, то возвращаем false
		else {
			for (int i = 0; i < m; i++) {
				if (v[i] != v1.v[i]) {//в цикле ищем несоответствие
						return false;//если нашли то false
					}
				}
			}
			return true;//если все ок, то true
	}
	return true;//сравнение одного и того же вектора
}

BoolV BoolV::operator | (const BoolV& vec) {
	if (&vec == this) {//если выполняем логическую сумму одного и того же вектора
		cout << "Error";
		exit(0);
	}

	int t, max;//создаем переменные для обозначения большего вектора

	if (m > vec.m) {//если текущий вектор больше второго
		t = nbit;//задаем параметры большего вектора
		max = m - 1;

		BoolV temp(t);//создаем вектор в котором будем хранить рельтат
		int i = vec.m - 1;//фиксируем значение размера меньшего вектора в i
		while (max >= 0 && i >= 0) {//начиная с младших битов вектора начинаем выполнять операцию
			temp.v[max] = v[max] | vec.v[i];//заносим в элемент нового вектора логическую сумму 
			max--;//уменьшаем индексы векторов
			i--;//по окончанию, если меньший вектор "закончится" выйдем из цикла
		}
		while (max >= 0) {//если остались биты в старших битах, то просто их заносим в новый вектор путем 
			temp.v[max] = temp.v[max] | v[max];//логической суммы нулей и большего вектора
			max--;
		}
		return temp;//возвращаем результат
	}
	else {//иначе выполняем те же операции, если большим вектором является второй 
		t = vec.nbit;
		max = vec.m - 1;

		BoolV temp(t);
		int i = m - 1;
		while (max >= 0 && i >= 0) {
			temp.v[max] = vec.v[max] | v[i];
			max--;
			i--;
		}
		while (max >= 0) {
			temp.v[max] = temp.v[max] | vec.v[max];
			max--;
		}
		return temp;
	}
}

BoolV BoolV::operator & (const BoolV& vec) {
	if (&vec == this) {//если выполняем логическое умножение одного и того же вектора
		cout << "Error";
		exit(0);
	}

	int t, max;

	if (m > vec.m) {//аналогично выполняем все те эе операции для логического умножения
		t = nbit;
		max = m-1;

		BoolV temp(t);
		int i = vec.m-1;
		while (max >= 0 && i >= 0) {
			temp.v[max] = v[max] & vec.v[i];//операция меняется на умножение 
			max--;
			i--;
		}
		while (max >= 0) {
			temp.v[max] = temp.v[max] | v[max];//чтобы перенести нетронутые старшие биты используем |
			max--;
		}
		return temp;//возвращаем результат
	}
	else {
		t = vec.nbit;
		max = vec.m-1;

		BoolV temp(t);
		int i = m-1;
		while (max >= 0 && i >= 0) {
			temp.v[max] = vec.v[max] & v[i];
			max--;
			i--;
		}
		while (max >= 0) {
			temp.v[max] = temp.v[max] | vec.v[max];
			max--;
		}
		return temp;
	}
}

BoolV BoolV::operator ~ () {
		if (m == 0) {//если размер равен 0 - ошибка
			cout << "Error";
			exit(0);
		}
		BoolV temp(*this);//чтобы не изменять текущий вектор создадим результирующий

	for (int i = 0; i < m; i++) {//проходим по вектору и инвертируем его биты
		temp.v[i] = ~v[i];//операция инверсии
	}

	unsigned int mask = 1;//создаем маску равную 1
	int zeros = m * 32 - nbit;//вычисляем количество незначащих нулей в векторе

	if (zeros) {//пока есть незначащие нули
		for (int i = 31 - zeros; i > 0; i--) {//сдвигаем и наращиваем маску чтобы на месте незначащих нулей
			mask = mask << 1;					//в маске были нули
			mask++;
		}
		temp.v[0] = temp.v[0] & mask;//в старшем байте, где содержатся незначащие нули выполняем лог.умножение на маску
							//получаем на месте незначащих нулей все те же нули
	}

	//return *this;//вовзращаем указатель на вектор
	return temp;//возвращаем результирующий вектор
}

ostream& operator<<(ostream& out, BoolV const& vec)
{
	unsigned int mask;;
	int zeros = vec.m * 32 - vec.nbit;//вводим переменную кол-ва незначащих нулей

	if (zeros) {
		mask = 1 << (31 - zeros);//если он есть, то изначально сдвигаем маску, чтобы начать вывод после незначащих нулей
	}
	else {
		mask = 1 << 31;//иначе маска пройдет по всем битам
	}

	for (int index = 0; index < vec.m; index++) {//начиная со старших битов проходим по вектору
		if (!mask) {//если маска занулилась, то снова сдвигаем налево, но уже работаем со след. 4 байтами
			mask = 1 << 31;
		}
		 while (mask) {//сдвигаем маску пока не дойдем до конца текущих 4 байтов
			if (vec.v[index] & mask) {//если лог умножение вектора и маски дает не 0
				out << '1';//то выводим единицу
			}
			else {
				out << '0';//иначе 0
			}
			mask = mask >> 1;	
		}
	}

	return out;
}

istream& operator >> (istream& in, BoolV& x) {
	char* s = new char[INT_MAX];//выделяем память для строки
	cin >> s;//заполняем строку

	BoolV invec(s);//помещаем строку в функцию образования вектора
	x = invec;//заносим по адресу x наш вектор

	delete[] s;//удаляем память

	return in;//возвращаем in
}

BoolM::BoolM(int k, int l) {
	m = l; //количество строк
	n = k; //кол-во столбцов
	bm = new BoolV[m];//выделяем память для строк
	BoolV v(n);//создаем нулевой вектор

	for (int i = 0; i < m; i++) {//заполняем строки нулевыми векторами
		bm[i] = v;
	}
}

BoolM::BoolM(const BoolM& m1) {
	m = m1.m;//заносим в m и n значения по вектору копирования
	n = m1.n;
	bm = new BoolV[m];//выделяем память для матрицы

	for (int i = 0; i < m; i++) {//копируем строки из первого вектора во второй
		bm[i] = m1.bm[i];
	}
}

BoolV& BoolM::operator[] (int x) {
	if (x < 0 || x > m) {//проверка пределов
		cout << "Index is out";
		exit(0);
	}

	return bm[x];//возвращаем строку(вектор) по индексу х
}

BoolM BoolM::operator = (const BoolM& m1) {
	if (this != &m1) {//если указывает сам на себя
		m = m1.m;//присваиваем параметры матриц друг другу
		n = m1.n;
		delete[] bm;//удаляем память первой матрицы
		bm = new BoolV[m];//выделяем новую память для матрицы

		for (int i = 0; i < m; i++)//копируем строки из второй матрицы в первую
			bm[i] = m1.bm[i];
	}
	return *this;//возвращаем указатель на текущую матрицу
}

ostream& operator << (ostream& out, BoolM& m1) {
	for (int i = 0; i < m1.m; i++) {//в цикле выводим строки(вектора) в 1 столбец
		out << m1.bm[i] << "\n";
	}

	return out;//возвращаем out
}

bool topsort(BoolM& M, int m, int* a) {
	if (m <= 0 || !a) {//обработка исключений
		return false;
	}

	BoolV v0(m), v1(m), v2(m);//создаем вспомогающие вектора
	//v0 - хранитель(хранит в себе все обработанные вершины)
	//v1 - дизъюнкция строк(определяет какой столбец нулевой)
	//v2 - вектор новых вершин(определяет какие новые вершины войдут в решение)
	BoolV z(m);//вспомогательный нулевой вектор, чтобы занулять строки в матрице
	bool flag = true;//flag для 
	int j = 0;//переменная для индексов результирующего массива

	while (flag) {
		v1 = z;//зануляем v1
		for (int i = 0; i < m; i++) {	//формируем v1 - дизъюнкцию строк
			v1 = v1 | M[i];				//для того чтобы узнать какие столбцы нулевые
		}								//получим вектор с 0-ми в нулевых столбцах

		//cout << v0 << '\n'; //0000000  0001011  0101111 111111
		//cout << v1 << '\n'; //1110100  1010000  0000000 000000
		
		v2 = ~v1 & ~v0;	//выполняем операцию лог. умножения инверсионных v1 и v0 для определения новых вершин			
		v0 = v0 | v2;  //заносим в v0 путем лог. сложения новые вершины, чтобы обозначить обработанные
		flag = false;   //устанавливаем flag = false для потенциального завершения цикла
		//cout << v2 << '\n'; //0001011  0100100 10100000 000000
		//cout << '\n';

		for (int i = 0; i < m; i++) {//проверка на наличие новых вхождений
			if (v2[i] != 0) {//смотрим какие позиции являются новыми вхождениями
				flag = true; //пока имеются новые вершины, продолжаем выполнять цикл
				a[j] = m - i;//если новое вхождение, то заносим в результирующий массив
				j++;//повышаем счетчик
				M[(m-1)-i] = z;//текущую строку(вектор) соответствующий нулевому столбцу зануляем
			}
		}
	}

	if (j != m) {							//если на выходе количество элементов в массиве не сходится с размером массива
		cout << "Cycle\n";				//значит возник цикл в графе
		return false;				//возвращаем false
	}
	else {
		return true;			//иначе возвращаем true
	}
}


int main() {
	//BoolV a("00011111111111110011010101010011111010101010101");
	//BoolV b("1010111");
	//cout << a << '\n';
	//cout << b << '\n';
	//BoolV c = a | b;
	//cout << ~a << '\n';
	
	int size, i, k;//размер матрицы, "исходящая" вершина, "входящая" вершина
	ifstream in;//создаем объект класса для чтения ifstream 
	in.open("C:\\Users\\igor_\\OneDrive\\Desktop\\graph.txt");//открываем нужный файл
	
	in >> size;//считываем в size первое число в файле(т.е. количество вершин)
	BoolM matrix(size, size);//создаем матрицу размером size X size
	int* a = new int[size];//создаем массив на size элементов
	
	while (!in.eof()) {//цикл для чтения
		in >> i;//считываем "исходящую" вершину в i(строка матрицы)
		in >> k;//считываем "входящую" вершину в k(
		matrix[i - 1].Set1(size - k);//устанавливаем 1 в позиции "входящей" вершины в матрице
	}
	in.close();//закрываем файл
	
	cout << matrix << '\n';//выводим матрицу
	if (topsort(matrix, size, a)) {//выполняем сортировку
		for (int i = 0; i < size; i++) {//выводим значения результирующего массива
			cout << a[i] << " ";
		}
	}
}