//2. ����������� ������ ������ ��� ������ �� ������, 
//������������� � ������������ �������

#include <iostream>
#include <ctime>
#include "Stack.h"
#include "Visitor.h"
#include <algorithm>

using namespace std;

enum Color
{
	Red,
	Green,
	Blue
};


class Railway_carriage
{
public:

	Railway_carriage() {}
	Railway_carriage(Color color, int type = 1, int weight = 20, int dimensions = 4) : color(color), type(type), weight(weight), dimensions(dimensions) {}
	Color getColor() { return color; }

private:

	int type;
	Color color;
	int weight;
	int dimensions;

};


int main()
{
	srand(time(NULL));

	int intArr[]{ 1,2,3,4,5,6,7,8,9,10,11,12 };
	Stack<int> S1, S2(intArr, 12), S3(S2);

	S1 = S2;
	S1.pop();
	S3.push(13);

	cout << "int:"
		<< "\n1st: ";
	S1.print();
	cout << "\n2nd: ";
	S2.print();
	cout << "\n3rd: ";
	S3.print();
	char ab[] = "ab", bc[] = "bc", cd[] = "cd", de[] = "de";
	char* charArr[]{ ab,bc,cd };
	Stack<char*> S4, S5(charArr, 3), S6(S5);

	S4 = S5;
	S4.pop();
	S6.push(de);

	cout << "\n\nchar*:"
		<< "\n4th: ";
	S4.print();
	cout << "\n5th: ";
	S5.print();
	cout << "\n6th: ";
	S6.print();



	{// ������ � ��������
		const int size = 50;
		Railway_carriage *initial = new Railway_carriage[size];

		for (int i = 0; i < size; ++i)
			initial[i] = Railway_carriage(Color(rand() % 2));

		Stack<Railway_carriage> Station;

		for (int i = 0; i < size; ++i)
			Station.push(initial[i]);

		Railway_carriage *first = new Railway_carriage[size], *second = new Railway_carriage[size];
		int count1 = 0, count2 = 0;

		for (int i = 0; i < size; ++i)
		{
			if (Station.top().getColor() == Red)
				first[count1++] = Station.top();
			else
				second[count2++] = Station.top();
			Station.pop();
		}

		cout << "\n\nCarriages:\n"
			<< "Number of red railway carriages: " << count1 << endl
			<< "Number of green railway carriages: " << count2 << endl;
	}

	{//Visitor

		Stack<int> S;
		for (int i = 0; i < 10; ++i)
			S.push(rand() % 20);
		cout << "\nRandom generated Stack<int> with ten elements (range 0 - 19):\n";
		S.print();

		min_Visitor<int> min;
		max_Visitor<int> max;
		imin_Visitor<int> imin;
		imax_Visitor<int> imax;
		S.accept(min);
		S.accept(max);
		S.accept(imin);
		S.accept(imax);
		cout << "\nMin: " << min.getMin()
			<< "\nMax: " << max.getMax() 
			<<"\nMin using iterator: "<< *imin.get_imin()
			<< "\nMax using iterator: " << *imax.get_imax()
			<< endl;

	}




	system("pause");
	return 0;
}
