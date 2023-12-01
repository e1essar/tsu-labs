#include <iostream>
#include <stdlib.h>
#include <ctime>

using namespace std;

class Array
{
	int* a, n;// указатель на массив, размер массива
public:
	// конструктор 1
	// len – число элементов в массиве
	// t = 1 – неупорядоченный массив
	// t = 2 – массив, упорядоченный по неубыванию
	// t = 3 – массив, упорядоченный по невозрастанию
	// d – диапазон псевдослучайных чисел для неупорядоченного массива (при t = 1)
	
	Array(int len = 1, int t = 1, int d = 10); // конструктор 1
	Array(int*, int); // конструктор 2: по массиву
	Array(Array&); // конструктор копирования

	~Array() { // деструктор
		if (a) delete[]a; a = NULL;
	}

	Array& operator = (Array&); // функция присвоения
	int& operator [](int); // функция возвращения элемента по индексу

	bool Test(); // проверка на упорядоченность по неубыванию
	bool operator == (Array); // равенство элементов массивов (но не порядка)

	void Shell_sort(); // сортировка Шелла
	void Heapsort(); // пирамидальная сортировка
	void Hoar_sort(int, int); // сортировка Хоара
	friend istream& operator >> (istream&, Array&); // ввод
	friend ostream& operator << (ostream&, Array&); // вывод

	void sift(int, int); // функция просеивания
};

// конструктор 1 
Array::Array(int len, int t, int d) {
	if (len > 0 && 1 <= t <= 3 && d > 1) {//проверяем допустимый диапазон значений переменных
		a = new int[len];//выделяем динамическую память для массива
		n = len;//заносим в n размер указанного массива
		if (t == 1) {//неупорядоченный массив
			for (int i = 0; i < len; i++) {
				a[i] = rand() % d;//неважно какое число заносится в массив
			}
		}

		if (t == 2) {//неубывание
			a[0] = rand() % d;//задаем первый элемент
			for (int i = 1; i < len; i++) {
				a[i] = a[i - 1] + rand() % d;//след число больше другого
			}
		}

		if (t == 3) {//невозрастание
			a[len - 1] = rand() % d;//задаем последний элемент
			for (int i = len - 2; i >= 0; i--) {
				a[i] = a[i + 1] + rand() % d;//с конца заполняем по возрастанию чтобы получить итоговый по невозрастанию
			}
		}
	}
}

// конструктор 2
Array::Array(int* Arr, int l) {
	if (Arr && l > 0) {//проверка допустимости
		a = new int[l];//выделяем память
		n = l;//заносим в n длину
		for (int i = 0; i < l; i++) {
			a[i] = Arr[i];//формируем новый массив по уже заданному
		}
	}
}

// конструктор копирования
Array::Array(Array& Arr) {
	if (Arr.a && Arr.n != 0) {//если длина массива и сам массив не равны нулю
		n = Arr.n;//в n присваиваем размер массива для копирования
		a = new int[n];//выделяем память для массива
		for (int i = 0; i < n; i++) {
			a[i] = Arr.a[i];//копируем поэлементно массивы
		}
	}
}

// функция присвоения
Array& Array::operator=(Array& Arr) {
	if (this != &Arr) {
		delete[] a;//освобождаем память *this
		n = Arr.n;//в n заносим размер массива для присвоения
		a = new int[n];//выделяем память для массива a
		for (int i = 0; i < n; i++) {
			a[i] = Arr.a[i];//копируем элементы из Arr в a
		}
		return *this;//a=b
	}
}

// функция возвращения элемента по индексу
int& Array::operator[](int index) {
	if (index >= n || index < 0) {//проверка на допустимость
		cout << "Index is out";//сообщение если индекс вышел за границы
		exit(0);
	}
	else {
		return a[index];//возвращение элемента по индексу
	}
}

// проверка на упорядоченность по неубыванию
bool Array::Test() {
	for (int i = 0; i < n - 1; i++) {//в цикле проверяем
		if (a[i] > a[i + 1]) {//найдется ли следующий элемент меньше предыдущего
			return false;//если нашелся то возвращаем false
		}
	}
	return true;//если все прошло успешно
}

// равенство элементов массивов (но не порядка)
bool Array::operator == (Array Arr) {
	if (n != Arr.n) return false;//проверка на размер массивов 

	int* flag = new int[n];
	for (int i = 0; i < n; i++) flag[i] = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < Arr.n; j++) {
			if ((Arr.a[j] == a[i]) && (flag[j] == 0)) {
				flag[j] = 1;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		if (flag[i] == 0) return false;
	}
	return true;
}

// ввод
istream& operator >> (istream& r, Array& Arr) {
	cout << "Count of values: ";
	cin >> Arr.n;

	int* buf = new int[Arr.n];

	for (int i = 0; i < Arr.n; i++) {
		cin >> buf[i];
	}

	if (Arr.a) delete[] Arr.a;
	Arr.a = buf;

	return r;
}

// вывод
ostream& operator<<(ostream& out, Array& A) {
	for (int i = 0; i < A.n; i++) {
		out << A.a[i] << " ";
	}
	cout << "\n";

	return out;
}

// сортировка Шелла
void Array::Shell_sort() {
	int i, j, step;//i,j - счетчики, step - расстояние между элементами сравнения
	int tmp;//переменная для обмена
	for (step = n / 2; step > 0; step /= 2) {//цикл начиная с step = n/2 и далее n/4, n/8...
		for (i = step; i < n; i++) {//сортировка вставками
			tmp = a[i];//запоминаем i-тый элемент
			for (j = i; j >= step; j -= step) {//начинаем сортировку
				if (tmp < a[j - step]) {//если сравниваемые элементы не отсортированы
					a[j] = a[j - step];//то меняем элементы местами
				}
				else
					break;//иначе ничего не делаем
			}
			a[j] = tmp;//возвращаем на нужное место элемент который запомнили
		}
		//cout << step << '\n';
		//cout << *this << '\n';
	}
}

// пирамидальная сортировка
void Array::Heapsort() {
	int tmp;
	for (int i = n / 2 - 1; i >= 0; i--) {// строим правильную пирамиду(кучу) (начиная с основания(нижних элементов))
		sift(i, n);
		//cout << *this << '\n';
	}
	//cout << *this << '\n';

	for (int i = n - 1; i >= 0; i--) {
		tmp = a[0];
		a[0] = a[i];
		a[i] = tmp;
		sift(0, i); // просеиваем уменьшенную кучу
		
		//cout << i << ") ";
		//cout << *this << '\n';
	}
}

// функция просеивания
void Array::sift(int i, int n) {
	if (i >= 0) {//проверка i
		int j = 2 * i + 1;//левый потомок
		int x = a[i];//запоминаем в x значение родителя
		int f = 1;//флаг 1
		while (j < n && f) {//пока выполняются условия
			if (j + 1 < n && a[j + 1] > a[j]) j++;//сравниваем два потомка
			if (a[j] > x) {//если потомок больше родителя
				a[i] = a[j];//ставим на место родителя потомка
				i = j;//идем дальше по дереву
				j = 2 * i + 1;
			}
			else f = 0;//если закончили просеивание 
		}
		a[i] = x;//после обмена ставим значение на место потомка
	}
}

// сортировка Хоара(быстрая сортировка)
void Array::Hoar_sort(int l, int r) {
	int i = l, j = r; // границы индексов фрагмента массива сортировки
	float x = a[(l + r) / 2]; // эталонное значение x(из элементов фрагмента)
	int tmp;
	if (l >= r) exit(0); // если границы заданы неверно - выход

	while (i <= j) { // выполняем сортировку относительно эталона чтобы слева были значения меньше, справа - больше
		while (a[i] < x) i++; 
		while (a[j] > x) j--; 
		if (i <= j) {//если нашлись элементы для обмена то меняем их местами
			tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
			i++;
			j--;
		}
	}
	//cout << x << " ";
	//cout << j << " " << i << '\n';
	//cout << *this << '\n';
	if(l<j) Hoar_sort(l, j); // вызываем функцию рекурсивно для того, чтобы отсортировать оставшиеся фрагменты массива
	if(i<r) Hoar_sort(i, r);
}


int main(int argc, char const* argv[]) {
    srand(time(0));
	
	int n = 10000;
	Array A(n, 1, n);
	Array B(n, 1, n);
	cout << "Start Sorted:" << B.Test() << '\n';
	if (A == B) cout << "Equal\n";
	else cout << "NotEqual\n";

	Array D = B;
	//clock_t start = clock();//начала отсчета времени
	//A.Shell_sort();
	D.Shell_sort();
	//C.Shell_sort();
	//clock_t end = clock();//конец отсчета
	//double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	//printf("\nShell sorting: %f seconds\n", seconds);//вывод времени отсчета
	cout << "Shell Sorted:" << D.Test() << '\n';
	if (D == B) cout << "Equal\n";
	else cout << "NotEqual\n";

	D = B;
	//clock_t start2 = clock();//начала отсчета времени
	//A.Heapsort();
	D.Heapsort();
	//C.Heapsort();
	//clock_t end2 = clock();//конец отсчета
	//double seconds2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
	//printf("\nHeap sorting: %f seconds\n", seconds2);//вывод времени отсчета
	cout << "Heap Sorted:" << D.Test() << '\n';
	if (D == B) cout << "Equal\n";
	else cout << "NotEqual\n";

	D = B;
	//clock_t start3 = clock();//начала отсчета времени
	//A.Hoar_sort(0, 9);
	D.Hoar_sort(0, n-1);
	//C.Hoar_sort(0, 99999999);
	//clock_t end3 = clock();//конец отсчета
	//double seconds3 = (double)(end3 - start3) / CLOCKS_PER_SEC;
	//printf("\nHoar sorting: %f seconds\n", seconds3);//вывод времени отсчета
	cout << "Hoar Sorted:" << D.Test() << '\n';
	if (D == B) cout << "Equal\n";
	else cout << "NotEqual\n";
}
