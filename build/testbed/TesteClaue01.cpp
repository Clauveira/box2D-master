#include "test.h"
#include <string> 

class TesteClaue01 : public Test
{
	float var_restituicao = 0, var_atrito = 0;
public:
	TesteClaue01() {
		criaParedes();
		criaObjetos();

		var_restituicao = 0;
		var_atrito = 0;
	}

	void Step(Settings& settings) override
	{
		Test::Step(settings);

		g_debugDraw.DrawString(5, m_textLine, "Claue Lista 01");
		m_textLine += 15;
		g_debugDraw.DrawString(10, m_textLine, "B, C, L, 6, 7, Y, U, H, J, V, 8: Criar Objetos");
		m_textLine += 15;
		g_debugDraw.DrawString(10, m_textLine, "W, A, S, D: Muda o direcao e sentido da gravidade.");
		m_textLine += 15;
		g_debugDraw.DrawString(10, m_textLine, "");
		m_textLine += 15;
	}

	static Test* Create()
	{
		return new TesteClaue01;

	}
	
	void Keyboard(int key) override 
	{
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
			m_world->SetGravity(b2Vec2(0.0, 10.0));
			break;
		case GLFW_KEY_S:
			m_world->SetGravity(b2Vec2(0.0, -10.0));
			break;
		case GLFW_KEY_A:
			m_world->SetGravity(b2Vec2(-10.0, 0.0));
			break;
		case GLFW_KEY_D:
			m_world->SetGravity(b2Vec2(10.0, 0.0));
			break;

		case GLFW_KEY_4:
			createBox(m_mouseWorld, b2Vec2(1,1), 1, 0, var_restituicao);
			if (var_restituicao < 1) { var_restituicao += 0.1; }
			break;
		case GLFW_KEY_5:
			createBox(m_mouseWorld, b2Vec2(1, 1), 1, var_atrito, 0);
			if (var_atrito < 1) { var_atrito += 0.1; }
			break;

		case GLFW_KEY_8:
			createTriangle2(m_mouseWorld);
			break;

		case GLFW_KEY_6:
			criarMatriz(1, 6, false);
			break;
		case GLFW_KEY_Y:
			criarMatriz(1, 6, true);
			break;
		case GLFW_KEY_7:
			criarMatriz(10, 10, false);
			break;
		case GLFW_KEY_U:
			criarMatriz(10, 10, true);
			break;
		case GLFW_KEY_H:
			criarMatriz(50, 50, false);
			break;
		case GLFW_KEY_J:
			criarMatriz(50, 50, true);
			break;
		}
	}

	void criaParedes()
	{
		createBox(b2Vec2(0.0, 0.0), b2Vec2(52.0, 2.0), 1.0, 0.5, 0, b2_staticBody);
		createBox(b2Vec2(0.0, 40.0), b2Vec2(52.0, 2.0), 1.0, 0.5, 0, b2_staticBody);
		createBox(b2Vec2(-50.0, 20.0), b2Vec2(2.0, 22.0), 1.0, 0.5, 0, b2_staticBody);
		createBox(b2Vec2(50.0, 20.0), b2Vec2(2.0, 22.0), 1.0, 0.5, 0, b2_staticBody);

		createBox(b2Vec2(-30.0, 2.0), b2Vec2(4.0, 1.0), 1.0, 0.5, 1, b2_staticBody);
	}
	void criaObjetos() {
		createBox(b2Vec2(0.0, 10.0), b2Vec2(1.0, 1.0), 1.0, 1, 0);
		createCircle(b2Vec2(0.0, 12.0), 1.0, 1.0, 1, 1);
		createEdge(b2Vec2(10.0, 10.0), b2Vec2(30.0, 18.0), 1.0, 0.5, 0.5);
		createTriangle(b2Vec2(2.0, 8.0), 1.0, 1.0, 1, 0);
	}

	void criarMatriz(int largura = 1, int altura = 6, bool ehCirculo = false) 
	{
		//int i = 0, j = 0;

		for (size_t i = 0; i < largura; i++)
		{
			for (size_t j = 0; j < altura; j++)
			{
				if (ehCirculo) {
					createCircle(b2Vec2((m_mouseWorld.x -largura +1) + (i*2),( m_mouseWorld.y +1) + j*2), 1.0, 1.0, 1, 0);
				}
				else
				{
					createBox(b2Vec2((m_mouseWorld.x -largura +1) + (i*2),( m_mouseWorld.y +1) + j*2), b2Vec2(1.0, 1.0), 1.0, 1, 0);
				}
			}
		}
	}

	b2Body* createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createEdge(b2Vec2 pos1, b2Vec2 pos2, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createTriangle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createTriangle2(b2Vec2 pos);
	b2Body* createObj(b2Vec2 pos);
};


static int testIndex = RegisterTest("_ClaueSS", "Claue Lista 01", TesteClaue01::Create);

b2Body* TesteClaue01::createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type)
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
b2Body* TesteClaue01::createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type)
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

b2Body* TesteClaue01::createEdge(b2Vec2 pos1, b2Vec2 pos2, float density, float friction, float restitution, b2BodyType type)
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


b2Body* TesteClaue01::createTriangle(b2Vec2 pos, float scale, float density, float friction, float restitution, b2BodyType type)
{
	
	b2Vec2 vertices[3];
	vertices[0].Set(0.0f , 1.0f * scale);
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



b2Body* TesteClaue01::createTriangle2(b2Vec2 pos){
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
b2Body* TesteClaue01::createObj(b2Vec2 pos) {
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
