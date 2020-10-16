#pragma once
#include "test.h"
#include <string> 

class ClaueBase : public Test
{
	float var_restituicao = 0, var_atrito = 0;
public:
	ClaueBase() {
		criaParedes();

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
		return new ClaueBase;

	}
	

	void criaParedes()
	{
		createBox(b2Vec2(0.0, 0.0), b2Vec2(52.0, 2.0), 1.0, 0.5, 0, b2_staticBody);
		createBox(b2Vec2(0.0, 40.0), b2Vec2(52.0, 2.0), 1.0, 0.5, 0, b2_staticBody);
		createBox(b2Vec2(-50.0, 20.0), b2Vec2(2.0, 22.0), 1.0, 0.5, 0, b2_staticBody);
		createBox(b2Vec2(50.0, 20.0), b2Vec2(2.0, 22.0), 1.0, 0.5, 0, b2_staticBody);

		createBox(b2Vec2(-30.0, 2.0), b2Vec2(4.0, 1.0), 1.0, 0.5, 1, b2_staticBody);
	}


	b2Body* createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createEdge(b2Vec2 pos1, b2Vec2 pos2, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createTriangle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createTriangle2(b2Vec2 pos);
	b2Body* createObj(b2Vec2 pos);
};


static int testIndex = RegisterTest("_ClaueSS", "Claue Lista 01", ClaueBase::Create);

b2Body* ClaueBase::createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type)
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
b2Body* ClaueBase::createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type)
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

b2Body* ClaueBase::createEdge(b2Vec2 pos1, b2Vec2 pos2, float density, float friction, float restitution, b2BodyType type)
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


b2Body* ClaueBase::createTriangle(b2Vec2 pos, float scale, float density, float friction, float restitution, b2BodyType type)
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



b2Body* ClaueBase::createTriangle2(b2Vec2 pos){
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
b2Body* ClaueBase::createObj(b2Vec2 pos) {
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
