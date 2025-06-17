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


	//---------------------------------------------------------------------------------------

	int* p1, * p2;
	int n = 7;
	p1 = &n;
	p2 = p1; // NO HAY Q LIBERAR NINGUNA MEMORIA

	//---------------------------------------------------------------------------------------

	auto env = new char* [10];
	for (int i = 0; i < 10; ++i)
		env[i] = new char(i);

	for (int i = 0; i < 10; ++i) //Limpiar memoria 
		delete env[i];

	delete[] env;

	//¿Cuántos bytes de memoria dinámica se han reservado suponiendo que el programa se ejecuta
	//en una arquitectura de 64 bits ?

	//En una arquitectura de 64 bits, cada puntero ocupa 8 bytes.
	//Total : 10 * 8 = 80 bytes
	//Para cada i de 0 a 9, se reserva memoria para 1 char(no un array, sino un solo char) y se inicializa con el valor i.
	//Cada new char(i) reserva 1 byte.
	//80 bytes(punteros) + 10 bytes(char) = 90 bytes

	//---------------------------------------------------------------------------------------

	int* ps[3];
	int** p = ps;
	*(p++) = new int(1);   // ps[0] = new int(1); p apunta a ps[1]
	*(++p) = new int(2);   // p apunta a ps[2], ps[2] = new int(2)
	*p = new int(3);       // ps[2] = new int(3); (se pierde el puntero anterior de ps[2], fuga de memoria)
	++*p;                  // incrementa el puntero ps[2], NO el valor apuntado (ps[2] ahora apunta a una dirección incorrecta)

	delete ps[0]; // Libera la memoria de new int(1)
	// No se puede liberar ps[2] porque su valor ha sido modificado por ++*p
	//ps[2] primero apunta a new int(2), pero luego se sobrescribe con new int(3) (fuga de memoria), 
	//y después se incrementa el puntero (++*p), por lo que ya no apunta a memoria válida.

	//¿Cuáles de los elementos del array ps apuntan a posiciones válidas de memoria al final del bloque y cuáles son
	//los valores en esas posiciones ?
	//ps[0] → Apunta a la memoria reservada con new int(1), valor 1 (válido).
	//ps[1] → No inicializado(apunta a basura, acceso indefinido).
	//ps[2] → Apunta a una dirección incorrecta(no válida), por el incremento de puntero(++*p).

		//---------------------------------------------------------------------------------------

	class A {};
	class B : public A {
		int* p;
	public:
		B(int n) : p(new int[n]) {} //Reserva memoria dinámica para un array de int de tamaño n (en este caso, 3).
		~B() {} //El destructor de B no libera la memoria reservada con new int[n].
				//Debería ser : ~B() { delete[] p; }
	};

	A* a1 = new B(3); //Se crea un objeto de tipo B, pero el puntero es de tipo base A*.
	A* a2 = a1; //a2 apunta al mismo objeto que a1.

	//Fuga de memoria:
	//Cuando hagas delete a1; (o delete a2;), se llamará al destructor de A(que es el tipo estático del puntero),
	//no al de B, porque el destructor de A no es virtual.
	//Por tanto, no se libera la memoria dinámica reservada en B, y hay una fuga de memoria.

	//SOLUCION Haz el destructor de la clase base A virtual para asegurar la destrucción polimórfica:
	class A {
	public:
		virtual ~A() {}
	};
	class B : public A {
		int* p;
	public:
		B(int n) : p(new int[n]) {}
		~B() { delete[] p; }
	};
	A* a1 = new B(3);
	A* a2 = a1;
	delete a1;

	//---------------------------------------------------------------------------------------

	std::string* p = new std::string("hola"); //Se reserva memoria dinámica para un std::string con el valor
											  //"hola" y se almacena el puntero en p.
	std::shared_ptr<std::string> sp(p); //Se crea un std::shared_ptr<std::string> llamado sp que toma
										//la propiedad del puntero p.
	//Si un objeto está gestionado por un shared_ptr, no lo borres manualmente. El shared_ptr se encarga de todo.
	//No hay fugas de memoria si solo usas sp para gestionar ese puntero.

	//---------------------------------------------------------------------------------------

	//Explica la diferencias conceptuales, sintácticas y de funcionamiento entre los métodos de instancia y los métodos
	//de clase(o estáticos).Ilustra con un ejemplo la utilidad de cada tipo de método.

	//|	Característica		  | Método de instancia				   | Método de clase(estático)					   |

	//| Pertenece a...		  | Una instancia concreta de la clase | A la clase en sí, no a una instancia           |
	//| Accede a atributos	  | Sí(usa `this`)					   | No(no tiene `this`)                            |
	//| Se usa para...		  | Operar con datos de un objeto	   | Funcionalidad general que no depende del objeto|
	//| Necesita crear objeto | Sí								   | No											    |

	//Diferencia sintáctica
	
	//Método de instancia :
	class Persona {
	public:
		std::string nombre;

		void saludar() {
			std::cout << "Hola, soy " << nombre << std::endl;
		}
	};

	Persona p;
	p.nombre = "Carlos";
	p.saludar();  // Usa un objeto

	//Método estático :
	class Matematica {
	public:
		static int suma(int a, int b) {
			return a + b;
		}
	};

	int resultado = Matematica::suma(3, 4);  // Sin crear ningún objeto

	//Diferencias de funcionamiento
	//Método de instancia :

	//Tiene acceso a todos los atributos y métodos del objeto.
	//Internamente recibe un puntero oculto this que apunta al objeto desde el que fue llamado.

	//Método estático :

	//No tiene this.
	//No puede acceder directamente a miembros no estáticos.
	//Solo accede a miembros static, variables globales o lo que se le pase por parámetros.

		//---------------------------------------------------------------------------------------

	class A {
	protected:
		int n;
	public:
		A(int n) : n(n) {
			println("A({})", n);
		}
		~A() {
			println("~A({})", n);
		}
		virtual void f() {
			println("A::f");
			++n;
		}
		virtual int g() const {
			return n;
		}
	};

	class B {
	public:
		B() {
			println("B()");
		}
		B(const B&) = delete;
		~B() {
			println("~B");
		}
		void m1(A* a) {
			println("m1({})",
				a->g());
		}
		void m2(const A& a) {
			println("m2({})",
				a.g());
		}
	}

	class C : public A {
		B c;
	public:
		C(int n) : A(2 * n) {
			println("C({})", n);
		}
		~C() {
			println("~C({})", n / 2);
		}
		void f() override { //se pone override xq es virtual en A
			println("C::f"));
		}
		int g() { //no override xq no tiene const
			return 3 * n;
		}
		virtual void h() {
			println("C::h");
		}
	};

	//salidas sin errores
	A a(1); //A(1)
	B b;  //B()
	C c(2); //  B() A(4) C(2)
	b.m1(c); //M1(4)
	b.m2(c); //m2(4)
	c.h();	//C(h)

	//---------------------------------------------------------------------------------------

	array<A*, 2> as; //declara un array de 2 punteros a A, pero no inicializa sus valores. 
					 //Por defecto, contienen basura (punteros colgantes).
	as[0]->f(); //Esto es un acceso inválido
	as[1] = new C(3); //Aquí se asigna a as[1] un puntero dinámico a una instancia de C(3)
	as[0] = as[1]; //Ahora ambos punteros (as[0] y as[1]) apuntan al mismo objeto dinámico.
	for (A* a : as) { //recorre ambos punteros (que apuntan al mismo objeto)
		//se repite 2 veces
		a->f(); //Se llama C::f()
		cout << a->g() << endl; //se llama A::g() (porque en C no tiene const)
		a->h(); //se llama C::h()
	}
	delete[] as; //no ha sido creado con new[], y no debe borrarse con delete[].

	//CORECCION
	delete[1];// Solo una vez
	
	//SALIDA
	//B()
	//A(6)
	//C(3)
	//C::f
	//6
	//C::h
	//C::f
	//6
	//C::h
	//~C(3)
	//~B
	//~A(6)

	//---------------------------------------------------------------------------------------

	C c1(4), c2(5);
	C c3 = c1; //Esto provoca error de compilación, 
			//porque el constructor de copia de C está implícitamente eliminado.
	c2 = c1; //Esto invoca el operador de asignación, que también está eliminado 
			 //porque B no puede copiarse.
	A* ap = &c1; //UPCAST SIN PROBLEMA
	C* cp = ap; //downcast no permitido
	cout << (dynamic_cast<C*>(ap) == nullptr ? "sí" : "no") << endl; //correcto

	//CORRECCION
	C c1(4), c2(5);
	// C c3 = c1;         // ERROR: constructor de copia eliminado
	// c2 = c1;           // ERROR: operador de asignación eliminado

	A* ap = &c1;
	// C* cp = ap;        // ERROR: cast inseguro
	cout << (dynamic_cast<C*>(ap) == nullptr ? "sí" : "no") << endl;
	//Esto sí está bien : hace un dynamic_cast de A * a C*, que :
	//Retorna un puntero válido si ap realmente apunta a un C
	//Retorna nullptr si no
	//Aquí ap = &c1, y c1 sí es un C, por tanto el cast funciona.
	//→ Se imprime : no

	//SALIDA
	B()
	A(8)
	C(4)
	B()
	A(10)
	C(5)
	no

	//---------------------------------------------------------------------------------------

	B b;
	b.m1(nullptr); //Si compila pero esta mal

	//cuando llamas a a->g() equivale a nullptr->g() error en tiempo de ejecucion 
	//se arregla o no pasando nullptr o poniendo:
	void m1(A* a) {
		if (a)
			println("m1({})", a->g());
		else
			println("m1(nullptr)");
	}


}

