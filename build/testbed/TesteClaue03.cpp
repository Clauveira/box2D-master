#include "test.h"
#include <string> 



class TesteClaue03 : public Test
{
private:
	enum DIRECAO
	{
		DIREITA,
		ESQUERDA,
		BAIXO,
		CABECA
	};
	b2Body* Player = nullptr;
	b2Body* obj = nullptr;
	float VelocidadeHorizontal = 400.0f;
	float ForcaPulo = 8000.0f;
	bool EhMovendoDireita = false, EhMovendoEsquerda = false;
	b2Vec2 p;
	////////////////////////

	b2Body* Projetil = nullptr;
	float
		ConstanteGravitacional = 3200;

	bool 
		EhArrastandoMouse = false;

	b2Vec2	MousePosition,
		MouseDownPosition,
		MouseUpPosition,
		VetorImpulso = b2Vec2(0, 1);

	float	MultiplicadorImpulso = 500.f,
		ForcaAplicada,
		ForcaMinima = 5000.f,
		ForcaMaxima = 500000.f;

public:
	TesteClaue03();
	void Step(Settings& settings) override;
	static Test* Create();


	void Keyboard(int key) override;
	void KeyboardUp(int key) override;

	float NivelarForcaAplicada(float forcaAplicada);

	void criaParedes();
	void criaObjetos();
	void criarProjetil(b2Vec2 pos);
	void criarPonte(int unidadesLargura);
	void criaRobo(b2Vec2 pos = b2Vec2(-20.0f, 16.0f))
	{
		int e_count = 4;

		
		b2Body* chest = createBox(pos, b2Vec2(0.5f, 1.7f), 10.0f, 1, 0);

		criaBraco(chest, CABECA, 1, b2Vec2(pos.x, pos.y+2.2f));

		criaBraco(chest, DIREITA, e_count, b2Vec2(pos.x+1, pos.y + 1.0f));
		criaBraco(chest, ESQUERDA, e_count, b2Vec2(pos.x-1, pos.y + 1.0f));

		criaBraco(chest, BAIXO, e_count, b2Vec2(pos.x+ 0.5f, pos.y-6.0f));
		criaBraco(chest, BAIXO, e_count, b2Vec2(pos.x- 0.5f, pos.y-6.0f));

		/*
		{
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			b2WeldJointDef jd;

			b2Body* prevBody = chest;
			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-14.5f + 1.0f * i, 5.0f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(-15.0f + 1.0f * i, 5.0f);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
		}
		*/
		/*
		{
			b2PolygonShape shape;
			shape.SetAsBox(1.0f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			b2WeldJointDef jd;
			float frequencyHz = 5.0f;
			float dampingRatio = 0.7f;

			b2Body* prevBody = chest;
			for (int32 i = 0; i < 3; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-14.0f + 2.0f * i, 15.0f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(-15.0f + 2.0f * i, 15.0f);
				jd.Initialize(prevBody, body, anchor);
				b2AngularStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
		}*/
		/*
		{
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			b2WeldJointDef jd;

			b2Body* prevBody = chest;
			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(-4.5f + 1.0f * i, 5.0f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				if (i > 0)
				{
					b2Vec2 anchor(-5.0f + 1.0f * i, 5.0f);
					jd.Initialize(prevBody, body, anchor);
					m_world->CreateJoint(&jd);
				}

				prevBody = body;
			}
		}*/
		/*
		{//Solto e molenga
		 
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.125f);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 20.0f;

			b2WeldJointDef jd;
			float frequencyHz = 8.0f;
			float dampingRatio = 0.7f;

			b2Body* prevBody = chest;
			for (int32 i = 0; i < e_count; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(5.5f + 1.0f * i, 10.0f);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				if (i > 0)
				{
					b2Vec2 anchor(5.0f + 1.0f * i, 10.0f);
					jd.Initialize(prevBody, body, anchor);

					b2AngularStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, prevBody, body);

					m_world->CreateJoint(&jd);
				}

				prevBody = body;
			}
		}*/


	};
	void criaBraco(b2Body* ground, int dir, int nPartes = 8, b2Vec2 pos = b2Vec2(0,0))
	{

		b2PolygonShape shape;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 20.0f;

		b2WeldJointDef jd;

		b2Body* prevBody = ground;
		switch (dir)
		{
		case 0:
			shape.SetAsBox(0.5f, 0.125f);

			for (int32 i = 0; i < nPartes; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(pos.x + 1.0f * i, pos.y);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(pos.x + 1.0f * i, pos.y);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
			break;
		case 1:
			shape.SetAsBox(0.5f, 0.125f);

			for (int32 i = 0; i < nPartes; ++i)//for (int32 i = nPartes; i > 0; --i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(pos.x - 1.0f * i, pos.y);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(pos.x - 1.0f * i, pos.y);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
			break;
		case 2:
			shape.SetAsBox(0.125f, 0.5f);

			for (int32 i = nPartes; i > 0; --i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(pos.x, pos.y + 1.0f * i);
				b2Body* body = m_world->CreateBody(&bd);
				body->CreateFixture(&fd);

				b2Vec2 anchor(pos.x, pos.y + 1.0f * i);
				jd.Initialize(prevBody, body, anchor);
				m_world->CreateJoint(&jd);

				prevBody = body;
			}
			break;
		case 3:
			shape.SetAsBox(0.8f, 0.5f);

			
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(pos.x, pos.y);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);

			b2Vec2 anchor(pos.x, pos.y);
			jd.Initialize(prevBody, body, anchor);
			m_world->CreateJoint(&jd);

			prevBody = body;
			
			break;
		}
		//b2Body* head = createBox(b2Vec2(pos.x, pos.y + 2), b2Vec2(0.8f, 0.5f), 10.0f, 1, 0);
		
		//return new b2Body();
	};

	void criarMatriz(int largura, int altura, b2Vec2 pos);


	b2Body* createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createEdge(b2Vec2 pos1, b2Vec2 pos2, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createTriangle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createTriangle2(b2Vec2 pos);
	b2Body* createObj(b2Vec2 pos);
};
static int testIndex = RegisterTest("_ClaueSS", "Claue Lista 03", TesteClaue03::Create);

TesteClaue03::TesteClaue03()
{
	criaParedes();
	criaObjetos();
}

void TesteClaue03::Step(Settings& settings)
{
	Test::Step(settings);

	g_debugDraw.DrawString(5, m_textLine, "Claue Lista 3");
	m_textLine += 15;

	p = Player->GetWorldPoint(b2Vec2(0.0, 0.0));
	if (EhMovendoDireita)
	{
		Player->ApplyForce(b2Vec2(VelocidadeHorizontal, 0.0), p, true);
	}
	if (EhMovendoEsquerda)
	{
		Player->ApplyForce(b2Vec2(-VelocidadeHorizontal, 0.0), p, true);
	}
}

Test* TesteClaue03::Create()
{
	return new TesteClaue03;
}

void TesteClaue03::Keyboard(int key)
{
	b2Vec2 p = Player->GetWorldPoint(b2Vec2(0.0, 0.0));
	switch (key)
	{
	case GLFW_KEY_B:
		createBox(b2Vec2(rand() % 100 - 50, rand() % 34 + 4), b2Vec2(RandomFloat(0.1, 3), RandomFloat(0.1, 3)), RandomFloat(0.001, 10), RandomFloat(0, 1), RandomFloat(0, 1));
		break;
	case GLFW_KEY_C:
		createCircle(b2Vec2(rand() % 100 - 50, rand() % 34 + 4), RandomFloat(0.1, 3), RandomFloat(0.001, 10), RandomFloat(0, 1), RandomFloat(0, 1));
		break;
	case GLFW_KEY_L:
		createEdge(b2Vec2(rand() % 100 - 50, rand() % 34 + 4), b2Vec2(rand() % 100 - 50, rand() % 100 - 50), 1.0, 0.5, 0.5);
		break;


	case GLFW_KEY_W:
		Player->ApplyForce(b2Vec2(0.0, ForcaPulo), p, true);
		break;
	case GLFW_KEY_S:
		Player->ApplyForce(b2Vec2(0.0, -ForcaPulo), p, true);
		break;
	case GLFW_KEY_A:
		EhMovendoEsquerda = true;
		break;
	case GLFW_KEY_D:
		EhMovendoDireita = true;
		break;


	case GLFW_KEY_8:
		createTriangle2(MousePosition);
		break;

	case GLFW_KEY_6:
		criarMatriz(10, 1, MousePosition);
	}
}

void TesteClaue03::KeyboardUp(int key)
{
	switch (key)
	{
	case GLFW_KEY_A:
		EhMovendoEsquerda = false;
		break;
	case GLFW_KEY_D:
		EhMovendoDireita = false;
		break;
	}
}

float TesteClaue03::NivelarForcaAplicada(float forcaAplicada)
{
	forcaAplicada *= MultiplicadorImpulso;

	if (forcaAplicada > ForcaMaxima) {
		forcaAplicada = ForcaMaxima;
	}
	else if (forcaAplicada < ForcaMinima) {
		forcaAplicada = ForcaMinima;
	}

	return forcaAplicada;
}

void TesteClaue03::criaParedes()
{
	createBox(b2Vec2(0.0, 0.0), b2Vec2(52.0, 2.0), 1.0, 0.5, 0, b2_staticBody);
	createBox(b2Vec2(0.0, 40.0), b2Vec2(52.0, 2.0), 1.0, 0.5, 0, b2_staticBody);
	createBox(b2Vec2(-50.0, 20.0), b2Vec2(2.0, 22.0), 1.0, 0.5, 0, b2_staticBody);
	createBox(b2Vec2(50.0, 20.0), b2Vec2(2.0, 22.0), 1.0, 0.5, 0, b2_staticBody);

	createBox(b2Vec2(-30.0, 2.0), b2Vec2(4.0, 1.0), 1.0, 0.5, 1, b2_staticBody);


	createBox(b2Vec2(0.0, 50.0), b2Vec2(52.0, 1.0), 1.0, 0.5, 0, b2_staticBody);
}

void TesteClaue03::criaObjetos()
{
	criaRobo();
	criarPonte(rand() % 22 + 8);

	criarMatriz(29, 1, b2Vec2(0.0, 51.8));
	Player = createBox(b2Vec2(0.0, 10.0), b2Vec2(1.0, 2.0), 2.0, 1.0, 0);
	Player->SetFixedRotation(true);
}

void TesteClaue03::criarProjetil(b2Vec2 pos)
{
	Projetil = createCircle(pos, 1, 1.0, 1, 0.1);
}

void TesteClaue03::criarPonte(int unidadesLargura)
{

	b2Body* m_middle;
	b2Body* ground = createBox(b2Vec2(-15.3f, 3.7f), b2Vec2(0.5f, 1.7f), 10.0f, 0, 0, b2_staticBody);
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.5f, 0.125f);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 20.0f;
		fd.friction = 0.2f;

		b2RevoluteJointDef jd;

		b2Body* prevBody = ground;
		for (int32 i = 0; i < unidadesLargura; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-14.5f + 1.0f * i, 5.0f);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);

			b2Vec2 anchor(-15.0f + 1.0f * i, 5.0f);
			jd.Initialize(prevBody, body, anchor);
			m_world->CreateJoint(&jd);

			if (i == (unidadesLargura >> 1))
			{
				m_middle = body;
			}
			prevBody = body;
		}
		b2Body* ground2 = createBox(b2Vec2(-14.7f + 1.0f * unidadesLargura, 3.7f), b2Vec2(0.5f, 1.7f), 10.0f, 0, 0, b2_staticBody);
		b2Vec2 anchor(-14.7f + 1.0f * unidadesLargura, 5.0f);
		jd.Initialize(prevBody, ground2, anchor);
		m_world->CreateJoint(&jd);
	}


}



void TesteClaue03::criarMatriz(int largura, int altura, b2Vec2 pos)
{
	float distanciaHorizontal = 1.8;
	//b2Body* obj = nullptr;
	for (size_t i = 0; i < largura; i++)
	{
		for (size_t j = 0; j < altura; j++)
		{
			if (i == 0) {
				obj = createBox(b2Vec2((pos.x - largura * distanciaHorizontal) + (i * 2 * distanciaHorizontal), (pos.y + 1) + j * 2), b2Vec2(0.4, 2.0), 1.0, 1, 0);
			}
			else
			{
				createBox(b2Vec2((pos.x - largura * distanciaHorizontal) + (i * 2 * distanciaHorizontal), (pos.y + 1) + j * 2), b2Vec2(0.4, 2.0), 1.0, 1, 0);
			}
		}
	}
	b2Vec2 p = obj->GetWorldPoint(b2Vec2(0.0f, 1.0f));
	obj->ApplyForce(b2Vec2(500, 0), p, true);
}

b2Body* TesteClaue03::createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newBox;

	b2BodyDef boxDef;
	boxDef.position = pos;
	boxDef.type = type;

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dim.x, dim.y);

	b2FixtureDef boxFix;
	boxFix.shape = &boxShape;
	boxFix.density = density;
	boxFix.friction = friction;
	boxFix.restitution = restitution;

	newBox = m_world->CreateBody(&boxDef);
	newBox->CreateFixture(&boxFix);
	return newBox;
}
b2Body* TesteClaue03::createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newBox;

	b2BodyDef circleDef;
	circleDef.position = pos;
	circleDef.type = type;

	b2CircleShape circleShape;
	circleShape.m_radius = radius;

	b2FixtureDef circleFix;
	circleFix.shape = &circleShape;
	circleFix.density = density;
	circleFix.friction = friction;
	circleFix.restitution = restitution;

	newBox = m_world->CreateBody(&circleDef);
	newBox->CreateFixture(&circleFix);
	return newBox;
}

b2Body* TesteClaue03::createEdge(b2Vec2 pos1, b2Vec2 pos2, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newObj;

	b2BodyDef objDef;
	//objDef.position = pos;
	//objDef.type = type;

	b2EdgeShape shape;

	shape.SetTwoSided(pos1, pos2);
	//shape.SetOneSided(b2Vec2(0, -1), b2Vec2(dim, 0), b2Vec2(-dim, 0), b2Vec2(0, 1));

	b2FixtureDef objFix;
	objFix.shape = &shape;
	objFix.density = density;
	objFix.friction = friction;
	objFix.restitution = restitution;

	newObj = m_world->CreateBody(&objDef);
	newObj->CreateFixture(&objFix);
	return newObj;
}


b2Body* TesteClaue03::createTriangle(b2Vec2 pos, float scale, float density, float friction, float restitution, b2BodyType type)
{

	b2Vec2 vertices[3];
	vertices[0].Set(0.0f, 1.0f * scale);
	vertices[1].Set(-1.0f * scale, -1.0f * scale);
	vertices[2].Set(1.0f * scale, -1.0f * scale);

	b2Body* newObj;



	b2BodyDef polygonDef;
	polygonDef.position = pos;
	polygonDef.type = type;


	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 3);


	b2FixtureDef polygonFix;
	polygonFix.shape = &polygonShape;
	polygonFix.density = density;
	polygonFix.friction = friction;
	polygonFix.restitution = restitution;

	newObj = m_world->CreateBody(&polygonDef);
	newObj->CreateFixture(&polygonFix);
	return newObj;
}



b2Body* TesteClaue03::createTriangle2(b2Vec2 pos) {
	b2Body* m_body;
	b2Transform xf1;
	xf1.q.Set(0.3524f * b2_pi);
	xf1.p = xf1.q.GetXAxis();

	b2Vec2 vertices[3];
	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));

	b2PolygonShape poly1;
	poly1.Set(vertices, 3);

	b2FixtureDef sd1;
	sd1.shape = &poly1;
	sd1.density = 2.0f;

	b2Transform xf2;
	xf2.q.Set(-0.3524f * b2_pi);
	xf2.p = -xf2.q.GetXAxis();

	vertices[0] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));

	b2PolygonShape poly2;
	poly2.Set(vertices, 3);

	b2FixtureDef sd2;
	sd2.shape = &poly2;
	sd2.density = 2.0f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	bd.position.Set(pos.x, pos.y);
	bd.angle = b2_pi;
	bd.allowSleep = false;
	m_body = m_world->CreateBody(&bd);
	m_body->CreateFixture(&sd1);
	m_body->CreateFixture(&sd2);
	return m_body;
}

//modelo para replicar
b2Body* TesteClaue03::createObj(b2Vec2 pos) {
	b2Body* m_body;
	b2Transform xf1;
	xf1.q.Set(0.3524f * b2_pi);
	xf1.p = xf1.q.GetXAxis();

	b2Vec2 vertices[3];
	vertices[0] = b2Mul(xf1, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf1, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf1, b2Vec2(0.0f, 0.5f));

	b2PolygonShape poly1;
	poly1.Set(vertices, 3);

	b2FixtureDef sd1;
	sd1.shape = &poly1;
	sd1.density = 2.0f;

	b2Transform xf2;
	xf2.q.Set(-0.3524f * b2_pi);
	xf2.p = -xf2.q.GetXAxis();

	vertices[0] = b2Mul(xf2, b2Vec2(-1.0f, 0.0f));
	vertices[1] = b2Mul(xf2, b2Vec2(1.0f, 0.0f));
	vertices[2] = b2Mul(xf2, b2Vec2(0.0f, 0.5f));

	b2PolygonShape poly2;
	poly2.Set(vertices, 3);

	b2FixtureDef sd2;
	sd2.shape = &poly2;
	sd2.density = 2.0f;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	bd.position.Set(pos.x, pos.y);
	bd.angle = b2_pi;
	bd.allowSleep = false;
	m_body = m_world->CreateBody(&bd);
	m_body->CreateFixture(&sd1);
	m_body->CreateFixture(&sd2);
	return m_body;
}
