#include <iostream>
using namespace std;

// Класс Маршрут
class route {
	int* r, n; // r – массив, представляющий маршрут; n – количество городов
public:
	route(int num = 0); // создает начальную перестановку по возрастанию
	route(const route&); // конструктор копирования
	
	route& operator = (const route&); // операция присваивания
	
	~route() { if (r) delete[]r; r = NULL; } // деструктор
	
	int route_price(int**); // вычисляет стоимость маршрута по матрице стоимости
	bool next_route(); // вычисляет следующий маршрут, используя алгоритм Дейкстры
	
	friend ostream& operator << (ostream& out, const route& a) { // вывод маршрута
		for (int i = 0; i < a.n; i++) out << a.r[i] << " ";
		return out;
	}
};

route::route(int num) { //конструктор, создает начальную перестановку 1,2,3,4....
	n = num;
	r = new int[num]; //выделение памяти для перестановки
	for (int i = 0; i < num; i++) {
		r[i] = i + 1;//начиная с 1 начинает называть города
	}
}

route::route(const route& s) {//конструктор копирования
	n = s.n;
	r = new int[n];
	for (int i = 0; i < n; i++) {
		r[i] = s.r[i];
	}
}

route& route::operator =(const route& s) {//оператор присваивания
	if (n != s.n) {//если не равны длины перестановок
		n = s.n;
		delete[] r;
		r = new int[n];//выделение памяти ддля перестановки
	}

	for (int i = 0; i < n; i++) {//присваивание
		r[i] = s.r[i];
	}

	return *this;
}

int route::route_price(int** m) {//вычисление стоимости маршрута текущей перестановки
	int w = 0;//вес изначально равен 0

	for (int i = 0; i < n - 1; i++)//по матрице стоимости получаем стоимость
		w += m[r[i] - 1][r[i + 1] - 1];

	w = w + m[r[n - 1]-1][r[0] - 1];
	return w;
}

bool route::next_route() {//поиск следующей перестановки
	int i = n - 1;//счетчик начиная с конца перестановки
	int temp = 0;//переменная для обмена
	int j = n;//счетчик j для поиска

	while (r[i] >= r[i + 1] && i > 0) {//начиная с конца перестановки проверяем элементы, если противоречит условию поиска то идем дальше
		i--;
	}
	
	if (i == 0) 
		return false;//если дошли до начала перестановки то возвращаем false

	while (r[i] >= r[j] && (i < j)) {//аналогично проверяем j элемент
		j--;
	}
	
	if (j != i) {//обмен i и j элементов
		temp = r[i];
		r[i] = r[j];
		r[j] = temp;
	}

	for (i = i + 1, j = n - 1; i < j; i++, j--) {//инверсия хвоста
		temp = r[i];
		r[i] = r[j];
		r[j] = temp;
	}

	return true;//после всего возвращаем true;
}

int main() {
	bool flag = true;
	int n = 0;//переменная количества городов
	int w = 0;//вес
	int min = INT_MAX;//минимальный весь равен текущему весу
	int num = 1;//нумерация городов

	cout << "Count of cities: ";
	cin >> n;

	int** matrix = nullptr;//создание матрицы стоимости
	matrix = new int* [n];

	srand(time(NULL));//для рандома

	for (int i = 0; i < n; i++) {//заполняем матрицу стоимости
		matrix[i] = new int[n];

		for (int j = 0; j < n; j++) {
			if (i != j) {
				matrix[i][j] = 1 + rand() % 10;//заполняем случайно стоимости маршрутов
			}
			else {
				matrix[i][j] = 0;//на места маршрутов в один и тот же город ставим 0
			}
		}
	}

	for (int i = 0; i < n; i++) {//вывод матрицы
		for (int j = 0; j < n; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << "\n";
	}

	cout << "\n";

	route a(n), b(n);


	while (flag == true) {
		w = a.route_price(matrix);//считаем стоимость перестановки

		cout << num << ")" << a << ": " << w << "\n";
		if (w < min) {
			min = w;//если нашлась стоимость ниже то переопределяем min
			b = a;
		}

		num++;
		flag = a.next_route();//след. перестановка

	}

	cout << "\n" << b << ":" << "min cost: " << min << "\n";
	
}
