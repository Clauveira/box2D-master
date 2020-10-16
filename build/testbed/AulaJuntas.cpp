#include "test.h"

class ExemploJuntas : public Test //voc cria a sua classe derivada da classe base Test
{
public:
	ExemploJuntas() {
		// Aqui no construtor voc inicializa a cena
		createBox(b2Vec2(0.0, 0.0), b2Vec2(50.0, 2.0), 1.0, 0.5, 0.5, b2_staticBody);
		createBox(b2Vec2(-45.0, 25.0), b2Vec2(2.0, 30.0), 1.0, 0.5, 0.5, b2_staticBody);
		createBox(b2Vec2(45.0, 25.0), b2Vec2(2.0, 30.0), 1.0, 0.5, 0.5, b2_staticBody);
		createBox(b2Vec2(0.0, 42.0), b2Vec2(50.0, 2.0), 1.0, 0.5, 0.5, b2_staticBody);

		selected = createBox(b2Vec2(0.0, 20.0), b2Vec2(0.5, 1), 1.0, 0.5, 0.5);



		//m_world->SetGravity(b2Vec2(0, 10));
		applyingForce = false;
		force.SetZero();
		globalPoint.SetZero();

	}

	void Step(Settings& settings) override
	{
		//Chama o passo da simulao e o algoritmo de rendering
		Test::Step(settings);

		processInputs();

		force.Normalize();
		force *= 5;

		g_debugDraw.DrawPoint(selected->GetWorldCenter(), 5, b2Color(1.0, 0.0, 0.0));
		g_debugDraw.DrawSegment(selected->GetWorldCenter(), selected->GetWorldCenter() + force, b2Color(1.0, 0.0, 0.0));

		//Para poder desenhar o ponto referente ao local onde est se aplicando uma fora
		b2Vec2 localPoint = b2Vec2(-0.5, 1.0);
		globalPoint = b2Vec2(selected->GetWorldPoint(localPoint));
		g_debugDraw.DrawPoint(globalPoint, 5, b2Color(1.0, 1.0, 0.0));
		//show some text in the main screen
		g_debugDraw.DrawString(5, m_textLine, "Este e' um template para os exercicios!! :)");
		m_textLine += 15;
	}

	static Test* Create()  //a classe Test que instancia um objeto da sua nova classe
						   //o autor da Box2D usa um padro de projeto chamado Factory
						   //para sua arquitetura de classes
	{
		return new ExemploJuntas;
	}

	//Para interagir com teclado
	void Keyboard(int key) override  //esse cabealho no mudam! :)
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

		case GLFW_KEY_1:
		{
			b2Body* body1 = createBox(b2Vec2(0.0, 15.0), b2Vec2(0.5, 2), 1.0, 0.5, 0.5);
			b2Body* body2 = createBox(b2Vec2(0.0, 11.0), b2Vec2(0.5, 2), 1.0, 0.5, 0.5);


			b2Vec2 worldAnchorOnBody1 = body1->GetWorldPoint(b2Vec2(0, -2.0));

			body1->SetType(b2_staticBody); //para ficar esttico

			b2RevoluteJointDef rJointDef;
			rJointDef.Initialize(body1, body2, worldAnchorOnBody1);
			//rJointDef.collideConnected = true;

			//Para limitar o angulo
			//rJointDef.lowerAngle = degreesToRadians(-45); // -45 degrees
			//rJointDef.upperAngle = degreesToRadians(45); // 45 degrees
			//rJointDef.enableLimit = true;

			//Para criar um motor
			rJointDef.maxMotorTorque = 100.0f; //N*m
			rJointDef.motorSpeed = 10.0f; //radianos por segundo 
			rJointDef.enableMotor = true;

			b2RevoluteJoint* rjoint = (b2RevoluteJoint*)m_world->CreateJoint(&rJointDef);

			break;
		}
		case GLFW_KEY_2:
		{
			b2Body* body1 = createBox(b2Vec2(0.0, 15.0), b2Vec2(0.5, 2), 1.0, 0.5, 0.5);
			b2Body* body2 = createBox(b2Vec2(0.0, 11.0), b2Vec2(0.5, 2), 1.0, 0.5, 0.5);
			b2Vec2 worldAnchorOnBody1 = body1->GetWorldPoint(b2Vec2(0, -2.0));
			body1->SetType(b2_staticBody); //para ficar esttico

			b2Vec2 axis(0, 1);
			//Para definir um outro eixo (em termos de um vetor normalizado)
			//axis = getVectorComponents(1, 45);

			b2PrismaticJointDef pJointDef;
			pJointDef.Initialize(body1, body2, worldAnchorOnBody1, axis);
			pJointDef.collideConnected = false;

			//Para limitar a translao
			/*pJointDef.lowerTranslation = -5.0;
			pJointDef.upperTranslation = 5.0;
			pJointDef.enableLimit = true;*/

			//Para criar um motor
			//pJointDef.maxMotorForce = 50.0f; //N
			//pJointDef.motorSpeed = -10.0f; //metros por segundo 
			//pJointDef.enableMotor = true;

			b2PrismaticJoint* pJoint = (b2PrismaticJoint*)m_world->CreateJoint(&pJointDef);
			break;
		}
		case GLFW_KEY_3:
		{
			b2Body* body1 = createBox(b2Vec2(-5.0, 15.0), b2Vec2(0.5, 0.5), 1.0, 0.5, 0.5);
			b2Body* body2 = createBox(b2Vec2(5.0, 15.0), b2Vec2(0.5, 0.5), 1.0, 0.5, 0.5);

			b2Vec2 worldAnchorOnBody1 = body1->GetWorldCenter();
			b2Vec2 worldAnchorOnBody2 = body2->GetWorldCenter();

			b2DistanceJointDef dJointDef;
			dJointDef.Initialize(body1, body2, worldAnchorOnBody1, worldAnchorOnBody2);
			dJointDef.collideConnected = true;

			b2DistanceJoint* dJoint = (b2DistanceJoint*)m_world->CreateJoint(&dJointDef);
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
};

//Aqui fazemos o registro do novo teste 
static int testIndex = RegisterTest("_Aula", "ExemploJuntas", ExemploJuntas::Create);

b2Body* ExemploJuntas::createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type)
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

	//Criao do corpo rgido pelo mundo e da fixture pelo corpo rgido
	newBox = m_world->CreateBody(&boxDef);
	newBox->CreateFixture(&boxFix);

	return newBox;
}

b2Body* ExemploJuntas::createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newCircle;

	//Definio dos atributos gerais do corpo rgido
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

	//Criao do corpo rgido pelo mundo e da fixture pelo corpo rgido
	newCircle = m_world->CreateBody(&circleDef);
	newCircle->CreateFixture(&circleFix);

	return newCircle;
}

void ExemploJuntas::processInputs()
{
	if (applyingForce)
	{
		force = getVectorComponents(1000, 45);
		selected->ApplyForceToCenter(force, true);

		//Para aplicar fora a um ponto 
		//selected->ApplyForce(force, globalPoint, true);

		applyingForce = false;
	}
}

b2Vec2 ExemploJuntas::getVectorComponents(float length, float angle)
{
	b2Vec2 v;
	v.x = length * cos(degreesToRadians(angle));
	v.y = length * sin(degreesToRadians(angle));
	return v;
}

float ExemploJuntas::degreesToRadians(float angle)
{
	return  angle * b2_pi / 180.0;
}

float ExemploJuntas::radiansToDegrees(float angle)
{
	return angle * 180 / b2_pi;
}