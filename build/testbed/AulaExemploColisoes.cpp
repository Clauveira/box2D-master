#include "test.h"
#include <vector>

using namespace std;

enum tiposDeObjetos { PORCO, PASSARO, RESTO };

struct userData {
	int ID;
	float hp;
	bool alive;
};

vector <b2Body*> matadouro; //meu vetor auxiliar para destruir os corpos (porcos) mortos

class MyContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact)
	{ /* handle begin event */


		//Soh pra debug
		b2Fixture* fa, * fb;
		fa = contact->GetFixtureA();
		fb = contact->GetFixtureB();

		b2Body* bodyA = NULL, * bodyB = NULL;
		if (fa) bodyA = fa->GetBody();
		if (fb) bodyB = fb->GetBody();

		if (bodyA && bodyB) //checar se o corpo ainda existe ????
		{
			//Checa se os objetos possuem userData
			void* bodyUserDataA = bodyA->GetUserData();
			void* bodyUserDataB = bodyB->GetUserData();

			if (bodyUserDataA && bodyUserDataB) //soh continua se os 2 tiverem userData -- PORCOS E PASSAROS
			{
				userData* udA = static_cast<userData*>(bodyUserDataA);
				userData* udB = static_cast<userData*>(bodyUserDataB);

				//Provoca dano no porco
				if (udA->ID == PORCO && udB->ID == PASSARO || udA->ID == PASSARO && udB->ID == PORCO)
				{
					if (udA->ID == PORCO)
					{
						udA->hp -= 1.0;
						if (udA->hp <= 0.0)
						{
							udA->alive = false;
							matadouro.push_back(bodyA);
						}
					}
					else
					{
						udB->hp -= 1.0;
						if (udB->hp <= 0.0)
						{
							udB->alive = false;
							matadouro.push_back(bodyB);
						}
					}

				}
			}
		}
	}

	void EndContact(b2Contact* contact)
	{ /* handle end event */
	}

};

class ExemploColisoes : public Test //voce cria a sua classe derivada da classe base Test
{
public:
	ExemploColisoes() {
		// Aqui no construtor voc inicializa a cena
		createBox(b2Vec2(0.0, 0.0), b2Vec2(50.0, 2.0), 1.0, 0.5, 0.5, b2_staticBody);
		createBox(b2Vec2(-45.0, 25.0), b2Vec2(2.0, 30.0), 1.0, 0.5, 0.5, b2_staticBody);
		createBox(b2Vec2(45.0, 25.0), b2Vec2(2.0, 30.0), 1.0, 0.5, 0.5, b2_staticBody);
		createBox(b2Vec2(0.0, 42.0), b2Vec2(50.0, 2.0), 1.0, 0.5, 0.5, b2_staticBody);


		float yi = 0.0;
		for (int i = 0; i < 10; i++)
		{
			b2Body* novoPorco = createCircle(b2Vec2(0.0, yi), 1.5, 2.0, 0.2, 0.2);
			userData* ud = new userData();
			ud->ID = PORCO;
			ud->hp = 1.0;
			ud->alive = true;
			novoPorco->SetUserData(ud);
			yi += 3.0; //2*raio do crculo, para posicion-los corretamente
		}



		//m_world->SetGravity(b2Vec2(0, 10));
		applyingForce = false;
		force.SetZero();
		globalPoint.SetZero();

		m_world->SetContactListener(&contactListener);

	}

	void Step(Settings& settings) override
	{
		//Chama o passo da simulao e o algoritmo de rendering
		Test::Step(settings);

		for (int i = 0; i < matadouro.size(); i++)
		{
			if (matadouro[i])
				m_world->DestroyBody(matadouro[i]);
		}
		matadouro.clear();

		processInputs();

		//show some text in the main screen
		g_debugDraw.DrawString(5, m_textLine, "Este eh um template para os exercicios!! :)");
		m_textLine += 15;
	}

	static Test* Create()  //a classe Test que instancia um objeto da sua nova classe
						   //o autor da Box2D usa um padro de projeto chamado Factory
						   //para sua arquitetura de classes
	{
		return new ExemploColisoes;
	}

	//Para interagir com teclado
	void Keyboard(int key) override  //esse cabeçalho não mudam! :)
	{
		switch (key)
		{
		case GLFW_KEY_B:
			createBox(b2Vec2(rand() % 61 - 30, rand() % 51), b2Vec2(rand() % 5 + 1, rand() % 5 + 1), 1.0, 0.5, 0.5);
			break;

		case GLFW_KEY_C:
			createCircle(b2Vec2(rand() % 61 - 30, rand() % 51), rand() % 5 + 1, 1.0, 0.5, 0.5);
			break;

		case GLFW_KEY_F:
			applyingForce = true;
			break;

		case GLFW_KEY_N:
			do
			{
				if (selected->GetNext() != NULL)
				{
					selected = selected->GetNext();
				}
				else selected = m_world->GetBodyList();
			} while (selected->GetType() != b2_dynamicBody);
			break;

		case GLFW_KEY_ENTER:
		{
			userData* ud = new userData;
			ud->ID = PASSARO;
			ud->hp = 1.0;
			ud->alive = true;

			//Criando um passarinho...
			b2Body* novoPassaro = createCircle(b2Vec2(-35, 2.0), 1.0, 1.0, 0.5, 0.5);
			novoPassaro->SetUserData(ud);
			//Aplicando uma fora inicial nele...
			b2Vec2 vetorForca;
			vetorForca = getVectorComponents(5000, 45);
			novoPassaro->ApplyForceToCenter(vetorForca, true);
			break;
		}

		}
	}

	b2Body* createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	void processInputs();

	//Algumas funes matemticas
	b2Vec2 getVectorComponents(float length, float angle);
	float degreesToRadians(float angle);
	float radiansToDegrees(float angle);


protected:
	b2Body* selected;
	bool applyingForce;
	b2Vec2 force;
	b2Vec2 globalPoint;
	MyContactListener contactListener;
};

//Aqui fazemos o registro do novo teste 
static int testIndex = RegisterTest("_Aula", "ExemploColisoes", ExemploColisoes::Create);

b2Body* ExemploColisoes::createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newBox;

	//Definio dos atributos gerais do corpo rgido
	b2BodyDef boxDef;
	boxDef.position = pos;
	boxDef.type = type;

	//Definio da forma do corpo*
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dim.x, dim.y);

	//Definio da fixture
	b2FixtureDef boxFix;
	boxFix.shape = &boxShape;
	boxFix.density = density;
	boxFix.friction = friction;
	boxFix.restitution = restitution;

	//Criação do corpo rígido pelo mundo e da fixture pelo corpo rígido
	newBox = m_world->CreateBody(&boxDef);
	newBox->CreateFixture(&boxFix);

	return newBox;
}

b2Body* ExemploColisoes::createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newCircle;

	//Definiçao dos atributos gerais do corpo rigido
	b2BodyDef circleDef;
	circleDef.position = pos;
	circleDef.type = type;

	//Definio da forma do corpo*
	b2CircleShape circleShape;
	circleShape.m_radius = radius;

	//Definio da fixture
	b2FixtureDef circleFix;
	circleFix.shape = &circleShape;
	circleFix.density = density;
	circleFix.friction = friction;
	circleFix.restitution = restitution;

	//Cria��o do corpo rgido pelo mundo e da fixture pelo corpo rgido
	newCircle = m_world->CreateBody(&circleDef);
	newCircle->CreateFixture(&circleFix);

	return newCircle;
}

void ExemploColisoes::processInputs()
{
	if (applyingForce)
	{
		force = getVectorComponents(1000, 45);
		selected->ApplyForceToCenter(force, true);

		//Para aplicar força a um ponto 
		//selected->ApplyForce(force, globalPoint, true);

		applyingForce = false;
	}
}

b2Vec2 ExemploColisoes::getVectorComponents(float length, float angle)
{
	b2Vec2 v;
	v.x = length * cos(degreesToRadians(angle));
	v.y = length * sin(degreesToRadians(angle));
	return v;
}

float ExemploColisoes::degreesToRadians(float angle)
{
	return  angle * b2_pi / 180.0;
}

float ExemploColisoes::radiansToDegrees(float angle)
{
	return angle * 180 / b2_pi;
}