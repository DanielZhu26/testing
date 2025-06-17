// testing.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

using namespace std;

int main()
{
	int** ps = new int* [5];

	for (int i = 1; i < 5; i++)
		ps[i] = new int(i);

	//delete ps[0]; // <- !!!!!! quitar a donde apunta ps[0] para que no se quede perdido suelto

	ps[0] = ps[4]; // haces que la flecha desde ps[0] apunte a 
	// la misma info que la flecha desde ps[4]

	*ps[0] = 0; // cambias la información de ps[0] a 0 

	for (int i = 0; i < 5; i++)
		cout << *ps[i] << " ";

	for (int i = 1; i < 5; i++) // <- falta borrar cada elemento, 
		// EMPEZAR EN 1 porque no hay flecha de ps[0] a 0 
		delete ps[i];

	delete[] ps;

	cout << endl;

	//-------------------------------------------------------------------------------

	//No hay errores de compilacion

	int n = 1, m = 7;
	int* p = &n, * q = &m;
	*q = (*p)++; //POST-INCREMENTO m= 1 y n = 2
	++(*q); //m = 2
	cout << n << " " << m; // 2 2

	cout << endl;

	//----------------------------------------------------------------------------------

	char c = '%';
	char* p = &c;
	char* q = new char;
	//*q = p; //intentas asignar un puntero char* a un char
	delete p; //mal, no es memoria dinamica
	delete q; //bien, si es memoria dinamica

	//----------------------------------------------------------------------------------

	//MAL
	//int** ints = new int* [100];
	//for (int i = 0; i < 100; ++i)
	//	ints[i] = new int(3);

	//delete ints;

	//Correccion: no hay error de compilacion pero si fugas de memoria
	int** ints = new int* [100];
	for (int i = 0; i < 100; ++i)
		ints[i] = new int(3);

	// Liberar primero cada elemento individual
	for (int i = 0; i < 100; ++i)
		delete ints[i];

	// Luego liberar el array de punteros
	delete[] ints;

	//----------------------------------------------------------------------------------

	//#ifndef GAME_H
	//#define GAME_H

	//	class Game {
	//	public:
	//		Ship* player;
	//		void interactWithShip();
	//	};

	//#endif // GAME_H

	//#ifndef SHIP_H
	//#define SHIP_H

	//	class Ship {
	//	public:
	//		Game* game;
	//		void interactWithGame();
	//	};

	//#endif // SHIP_H

	//Problema de inclusion circular, usar forward declarations
	// y si eso poner los includes en los cpp de las clases
	//CORRECCION!!
#ifndef GAME_H
#define GAME_H

	class Ship;  // Declaración anticipada

	class Game {
	public:
		Ship* player;
		void interactWithShip();
	};

#endif // GAME_H


#ifndef SHIP_H
#define SHIP_H

	class Game;  // Declaración anticipada

	class Ship {
	public:
		Game* game;
		void interactWithGame();
	};

#endif // SHIP_H


	//----------------------------------------------------------------------------------

	//class ShooterAlien : Alien { /* ... */ }

	//void move(Alien* alien) { /* ... */ }

	//void shoot(ShooterAlien* shooter) { /* ... */ }

	//Alien* shooter = new ShooterAlien(game, { 200, 400 });
	//move(shooter);
	//shoot(shooter); //ERROR(downcasting) el metodo shoot espera una clase ShooterAlien
					  // el compilador no sabe que shooter apunta a un ShooterAlien.

	//Podrias hacer 3 cosas
	//ShooterAlien* shooter = new ShooterAlien(game, { 200, 400 });
	//move(shooter);     // ok: ShooterAlien* → Alien*
	//shoot(shooter);    // ok

	//Alien* baseShooter = new ShooterAlien(game, { 200, 400 });
	//move(baseShooter);
	//shoot(static_cast<ShooterAlien*>(baseShooter));  // ok solo si seguro que es ShooterAlien

	//Alien* baseShooter = new ShooterAlien(game, { 200, 400 });
	//move(baseShooter);

	//usar dynamic_cast (solo con clases con métodos virtuales)
	//ShooterAlien* actualShooter = dynamic_cast<ShooterAlien*>(baseShooter);
	//if (actualShooter)
	//	shoot(actualShooter);
	//else
	//	std::cerr << "Error: Alien no es un ShooterAlien\n";


	//----------------------------------------------------------------------------------


	class Contador
	{
		int cuenta = 0;
	public: void suma() { cuenta++; }
	};

	Contador* contador = new Contador;// button tiene método connect(function<void(void)>)
	//button.connect(&Contador::suma); <-----------MAL

	//CORRECCION
	//button.connect([contador]() { contador->suma(); }); //usar lambda expression para capturar al contador

	/*#include <functional>
	button.connect(std::bind(&Contador::suma, contador));*/ //otra opcion es usar bind

	//----------------------------------------------------------------------------------

	void sh(const char* s, int arg)
	{
		cout << s << "(" << arg << ")\n";
	}

	void sh(const char* s)
	{ 
		cout << s << "\n"; 
	}

	class A
	{
	protected:
		int a;
	public:
		A(int x) : a(x) { sh("A", a); }
		virtual ~A() { sh("~A", a); }

		void f() { sh("A::f"); }
		virtual void g() { sh("A::g"); }

		virtual void h() { sh("A::h"); }

		virtual void m() = 0;
	};

	class B : public A 
	{ 
		int b;
	public:
		B(int a, int b) : A(a), b(b) { sh("B", a + b); }

		  ~B() { sh("~B", a + b); }
		  void f() { sh("B::f"); }

		  void g() { sh("B::g"); }

		  void m() { sh("B::m"); }
	};

	//APARTADO 1
	A a(4);
	B b = a; // ERROR Crear directamente un objeto b B(4, 5) o cambiar directamente B por A 
	//(A b = a;)
	b.f();  

	//Salida 
	//A(4)
	//A::f
	//~A(4)
	//~A(4)

	//APARTADO 2

	A* a = new B(1, 7);
	a->f();
	a->g();
	a->h();
	a->m();
	delete a;  //TODO CORRECTO

	//SALIDA
	//A(1)
	//B(8)
	//A::f f() no es virtual → se llama A::f()
	//B::g
	//A::h h() es virtual, pero B no la sobrescribe → se llama A::h()
	//B::m
	//~B(8)
	//~A(1)


	//APARTADO 3

	B* b = new B(2, 3);
	b->f();
	b->g();
	b->h(); //como b no tiene h() se llama al de A
	b->m();
	delete b;//TODO CORRECTO


	//SALIDA
	//A(2)
	//B(5)
	//B::f
	//B::g
	//A::h
	//B::m
	//~B(5)
	//~A(2)

	//APARTADO 4
	
	A* a = new B(0, 0);
	B* b1 = static_cast<B*>(a); //se permite si a realmente apunta a un objeto B (y lo hace).
	B* b2 = dynamic_cast<B*>(a); //requiere que A tenga al menos un método virtual 
								 //(lo tiene: m() y el destructor son virtuales).
	(*b1).m();
	b2->m();  //COMPILA SIN PROBLEMAS

	//SALIDA 
	//A(0)
	//B(0)
	//B::m
	//B::m

	//LIMPIEZA 
	delete a;

	//SALIDA
	//~B(0)
	//~A(0)

	//APARTADO 5

	A** as = new A* [3];
	B b2(2, 0);
	as[0] = new B(1, 0);
	as[1] = &b2;
	as[2] = as[0];

	for (int i = 0; i <= 3; ++i) //ERROR hay quitar el =
		as[i]->g();

	//SALIDA
	//A(2)
	//B(2)
	//A(1)
	//B(1)
	//B::g
	//B::g
	//B::g

	//LIMPIEZA
	delete as[0]; // as[2] apunta a lo mismo, NO lo borres de nuevo
	delete[] as;

	//~B(1)
	//~A(1)
	//~B(2)
	//~A(2)








}

