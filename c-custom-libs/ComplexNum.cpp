#include <iostream>

using namespace std;

class complex {
private:
	double re, im;

public:

	complex(double Re = 0, double Im = 0);

	double getRe();
	void setRe(double);
	double getIm();
	void setIm(double);

	bool operator == (complex&);
	bool operator != (complex&);

	double mod();

	complex operator + (const complex&);
	complex operator - (const complex&);
	complex operator * (const complex&);
	complex operator / (const complex&);

	void Output();
	void Input();
};

//конструктор по умолчанию
complex::complex(double Re,double Im) {
	re = Re;
	im = Im;
}

//функция для взятия значения действительной части числа
double complex::getRe() {
	return re;
}

//функция для изменения значения действительной части числа
void complex::setRe(double Re) {
	re = Re;
}

//функция для взятия значения мнимой части числа
double complex::getIm() {
	return im;
}

//функция для изменения значения мнимой части числа
void complex::setIm(double Im) {
	im = Im;
}

bool complex::operator !=(complex& x) {
	if (re == x.re && im == x.im)	//условие равенства
		return false;	//возвращение false
	else
		return true;	//возвращение true
}

bool complex::operator ==(complex& x) {
	if (re == x.re && im == x.im) 	//условие равенства
		return true;	//возвращение true
	else
		return false;	//возвращение false
}

complex complex::operator +(const complex& x) {
	complex res;	//инициализация комплексного числа для записи результата
	res.re = re + x.re;	//сложение дествительной части с действительной частью
	res.im = im + x.im;	//сложение мнимой части с мнимой
	return res;	//возвращение результата
}

complex complex::operator -(const complex& x) {
	complex res;
	res.re = re - x.re;	//вычитание действительной части второго числа из первого 
	res.im = im - x.im; //вычитание мнимой части второго числа из первого
	return res;	//возвращение результата
}

complex complex::operator *(const complex& x) {
	complex res;
	res.re = re * x.re - im * x.im;
	res.im = re * x.im + im * x.re;
	return res;	//возвращение результата
}

complex complex::operator /(const complex& x) {
	complex res;
	res.re = (re * x.re + im * x.im) / (x.re * x.re + x.im * x.im);
	res.im = (x.re * im - re * x.im) / (x.re * x.re + x.im * x.im);
	return res;	//возвращение результата
}

double complex::mod() {	//функция модуля комплексного числа
	return sqrt((re * re + im * im));
}

void complex::Output() {	//функция вывода результата
	//доп. условие на =0
	if (re == 0 && im == 0) cout << "0";
	else
		if (re == 0 && im != 0) cout << im << "i";
		else
			if (re != 0 && im == 0) cout << re;
			else
				if (im >= 0)
					cout << re << "+" << im << "i";
				else
					cout << re << im << "i";
}

void complex::Input() {	//функция ввода значений комплексного числа
	cout << "re: ";
	cin >> re;
	cout << "im: ";
	cin >> im;
}

int main() {
	complex a, b, c;
	cout << "Enter a\n";
	a.Input();
	cout << "b:";//set
	b.setRe(2);
	b.setIm(2);
	cout << b.getRe() << "+" << b.getIm() << "i\n";//get
	cout << "Results:\n";
	c = a.operator +(b);
	cout << "Sum:";
	c.Output();
	c = a.operator -(b);
	cout << "\nSub:";
	c.Output();
	c = a.operator *(b);
	cout << "\nMult:";
	c.Output();
	c = a.operator /(b);
	cout << "\nDiv:";
	c.Output();
	double mod = a.mod();
	cout << "\nMod:" << mod << "\n";
	
	
	cout << "a:";
	cout << a.getRe() << "+" << a.getIm() << "i\n";
	a.setRe(3);
	cout << "Change:\n";
	cout << a.getRe() << "+" << a.getIm() << "i\n";
	c = a.operator +(b);
	cout << "Sum:";
	c.Output();
	

	if (a.operator ==(b)) {
		cout << "\na and b are equal";
	}
	if (a.operator !=(b)) {
		cout << "\na and b are not equal";
	}
}

