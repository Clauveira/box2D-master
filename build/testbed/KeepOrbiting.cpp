//#include <iostream>
#include <string> 
#include <vector>
#include "imgui/imgui.h"
#include "test.h"
#include <time.h>

std::vector <b2Body*> PonteiroPlanetoides;
std::vector <b2Vec2*> PontosGravitacionais;
b2Body* Projetil = nullptr;

bool FlagColidiu = false;

enum tiposDeObjetos { PROJETIL, PLANETOIDE, PAREDE };

struct userData {
	int ID;
};



class ContactListener : public b2ContactListener
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

			if (bodyUserDataA && bodyUserDataB)
			{
				userData* udA = static_cast<userData*>(bodyUserDataA);
				userData* udB = static_cast<userData*>(bodyUserDataB);

				
				if (udA->ID == PROJETIL && (udB->ID == PLANETOIDE || udB->ID == PAREDE) ||
					udB->ID == PROJETIL && (udA->ID == PLANETOIDE || udA->ID == PAREDE))
				{
					//std::cout << "\nCOLIDIU!";
					FlagColidiu = true;

				}
			}
		}
	}

	void EndContact(b2Contact* contact)
	{ /* handle end event */
		//std::cout << "\nSaiu!";
	}
	
};

class KeepOrbiting : public Test
{
private:
	float
			ConstanteGravitacional = 3200;

	const int		
			Altura = 100,
			Largura = 200,
			DeslocamentoVertical = -Altura;

	int		NivelAtual = 0;

	bool	EhPodeDisparar = true,
			EhArrastandoMouse = false,
			DebugMode = true,
			FlagExcluirProjetil = false,
			FlagAvancaNivel = false,
			EhDistanciaLinear = false;//False = Distancia Quadrática

	b2Vec2	MousePosition,
			MouseDownPosition,
			MouseUpPosition,
			VetorImpulso = b2Vec2(0, 1);

	float	MultiplicadorImpulso = 200.f,
			ForcaAplicada, //Disparo; Apenas para exibir e restringir min/max
			ForcaMinima = 1000.f,
			ForcaMaxima = 20000.f,
			*Massa = new float;
	time_t	timerLancamento = NULL;

	ContactListener contactListener;
	

public:
	KeepOrbiting();
	~KeepOrbiting();

	void Step(Settings& settings) override;

	static Test* Create();

	void Keyboard(int key) override;

	void MouseDown(const b2Vec2& pos) override;
	void MouseMove(const b2Vec2& pos) override;
	void MouseUp(const b2Vec2& pos) override;

	float NivelarForcaAplicada(float forcaAplicada);
	void DesenhaLinhaArrastandoMouse();

	void AplicarGravidade(b2Vec2 pos, float massa);
	float FuncaoDistancia(b2Vec2 posA, b2Vec2 posB);
	
	b2Body* createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);
	b2Body* createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type = b2_dynamicBody);

	void criaParedes();
	void CriarPlanetoides(int n = 1);
	void criarProjetil(b2Vec2 pos);

	void avancaNivel();
	void destruirPlanetoides();
	void destruirProjetil();
	void reiniciaNiveis();

	void UpdateUI() override
	{
		ImGui::SetNextWindowPos(ImVec2(15.0f, 100.0f));
		ImGui::SetNextWindowSize(ImVec2(200.0f, 110.0f));

		if (ImGui::Checkbox("Eh Gravidade Linear", &EhDistanciaLinear)) {}
		if (ImGui::Checkbox("Eh Desenha Vetor de Gravidade", &DebugMode)) {}

		if (ImGui::SliderFloat("Constante Gravitacional", &ConstanteGravitacional, 100.0f, 500000.0f, "%.0f")) {}
	}
};
static int testIndex = RegisterTest("_Grau A", "Keep Orbiting", KeepOrbiting::Create);

KeepOrbiting::KeepOrbiting()
{
	m_world->SetGravity(b2Vec2(0.0, 0.0));
	m_world->SetContactListener(&contactListener);
	criaParedes();
	reiniciaNiveis();
}

KeepOrbiting::~KeepOrbiting()
{
	destruirPlanetoides();
	destruirProjetil();
}

void KeepOrbiting::Step(Settings& settings)
{
	Test::Step(settings);
	g_debugDraw.DrawString(5, m_textLine, "Keep Orbiting");
	m_textLine += 15;
	g_debugDraw.DrawString(5, m_textLine, "Passar nivel: Tecla 1");
	m_textLine += 15;
	g_debugDraw.DrawString(5, m_textLine, "Destruir projetil: Tecla 2");
	m_textLine += 15;
	g_debugDraw.DrawString(5, m_textLine, "Debug mode: Tecla D");
	m_textLine += 15;
	g_debugDraw.DrawString(5, m_textLine, "Lancar projetil: Clicar, arrastar e soltar.");

	if (EhPodeDisparar && EhArrastandoMouse) {
		DesenhaLinhaArrastandoMouse();
	} 
	else if (!FlagExcluirProjetil) {
		//AplicarGravidade(MousePosition, 400);
		if (Projetil != nullptr) {
			for (int i = 0; i < PontosGravitacionais.size(); i++)
			{
				AplicarGravidade(*PontosGravitacionais[i], Massa[i]);
			}
		}
	}//STEP NÃO SABE A POSIÇÃO DO MOUSE m_mouseWorld!!!!!!!!!
	else {
		destruirProjetil();
	}

	if (FlagColidiu) {
		FlagExcluirProjetil = true;
		destruirProjetil();
		FlagColidiu = false;
	}
	if (timerLancamento != NULL && difftime(time(NULL), timerLancamento) > 10 && !EhPodeDisparar) {
		timerLancamento = NULL;
		FlagAvancaNivel = true;
	}
	if (FlagAvancaNivel) {
		avancaNivel();
	}
}
Test* KeepOrbiting::Create()
{
	return new KeepOrbiting;
}

void KeepOrbiting::Keyboard(int key)
{
	switch (key)
	{
	case GLFW_KEY_1:
		FlagAvancaNivel = true;
		FlagExcluirProjetil = true;
		break;
	
	case GLFW_KEY_2:
		FlagExcluirProjetil = true;
		break;

	case GLFW_KEY_D:
		DebugMode = !DebugMode;
		break;

	case GLFW_KEY_W:
		ConstanteGravitacional *= 2;
		break;
	case GLFW_KEY_S:
		ConstanteGravitacional *= 0.5f;
		break;
		
	case GLFW_KEY_Q:
		EhDistanciaLinear = !EhDistanciaLinear;
		if (!EhDistanciaLinear)
		{
			ConstanteGravitacional *= 8;
		}
		else {
			ConstanteGravitacional *= 0.125;
		}
		break;

	}
}

void KeepOrbiting::MouseDown(const b2Vec2& pos)
{
	if (EhArrastandoMouse == false) {
		MouseDownPosition = pos;
		EhArrastandoMouse = true;
		if (EhPodeDisparar) {
			criarProjetil(pos);
		}
	}
}

void KeepOrbiting::MouseMove(const b2Vec2& pos)
{
	MouseUpPosition = pos;
	MousePosition = pos;
}

void KeepOrbiting::MouseUp(const b2Vec2& pos)
{
	MouseUpPosition = pos;

	VetorImpulso = (MouseDownPosition - MouseUpPosition);
	if (VetorImpulso != b2Vec2(0, 0)) {
		if (EhPodeDisparar && EhArrastandoMouse) {
			time(&timerLancamento);

			ForcaAplicada = NivelarForcaAplicada(b2Distance(MouseDownPosition, pos));

			VetorImpulso.Normalize();
			VetorImpulso *= ForcaAplicada;

			b2Vec2 p = Projetil->GetWorldPoint(b2Vec2(0.0f, 0.0f));
			b2Vec2 f = Projetil->GetWorldPoint(VetorImpulso);

			g_debugDraw.DrawSegment(MouseDownPosition, pos, b2Color(1.0f, 0.0f, 0.0f));
			g_debugDraw.DrawSegment(MouseDownPosition, f, b2Color(0.7f, 0.7f, 1.0f));

			Projetil->ApplyForce(f, p, true);
			EhPodeDisparar = false;
		}
		EhArrastandoMouse = false;
	}
}

float KeepOrbiting::NivelarForcaAplicada(float forcaAplicada)
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

void KeepOrbiting::DesenhaLinhaArrastandoMouse()
{
	g_debugDraw.DrawSegment(MouseDownPosition, MouseUpPosition, b2Color(0.7f, 1.0f, 0.7f));
}

float KeepOrbiting::FuncaoDistancia(b2Vec2 posA, b2Vec2 posB) {
	float resultado;
	resultado = b2Distance(posA, posB);
	if (!EhDistanciaLinear) {
		resultado *= resultado;
	}

	return resultado;
}

void KeepOrbiting::AplicarGravidade(b2Vec2 pos, float massa)
{
	b2Vec2 p = Projetil->GetWorldPoint(b2Vec2(0.0f, 0.0f));
	b2Vec2 v = (pos - p);
	v.Normalize();
	float dist = FuncaoDistancia(pos, p);
	v *= (massa * ConstanteGravitacional) / (dist);// * dist );

	b2Vec2 f = Projetil->GetWorldPoint(v);

	//b2Distance(pos, p);

	if (DebugMode){
		if (!EhDistanciaLinear) {
			g_debugDraw.DrawSegment(pos, p, b2Color(0.2f, 0.2f, 0.35f));
			g_debugDraw.DrawSegment(f, p, b2Color(0.1f, 0.0f, 0.9f));
		}
		else
		{
			g_debugDraw.DrawSegment(pos, p, b2Color(0.32f, 0.15f, 0.32f));
			g_debugDraw.DrawSegment(f, p, b2Color(0.4f, 0.0f, 0.7f));
		}
	}

	Projetil->ApplyForce(v, p, true);
	
}

void KeepOrbiting::criaParedes()
{
	createBox(b2Vec2(0.0, DeslocamentoVertical), b2Vec2(Largura + 2, 2.0), 1.0, 0.5, 1, b2_staticBody);
	createBox(b2Vec2(0.0, (Altura * 2) + DeslocamentoVertical), b2Vec2(Largura + 2, 2.0), 1.0, 0.5, 1, b2_staticBody);
	createBox(b2Vec2(-Largura, Altura + DeslocamentoVertical), b2Vec2(2.0, Altura + 2), 1.0, 0.5, 1, b2_staticBody);
	createBox(b2Vec2(Largura, Altura + DeslocamentoVertical), b2Vec2(2.0, Altura + 2), 1.0, 0.5, 1, b2_staticBody);

}

b2Body* KeepOrbiting::createBox(b2Vec2 pos, b2Vec2 dim, float density, float friction, float restitution, b2BodyType type)
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

	userData* ud = new userData;
	ud->ID = PAREDE;
	newBox->SetUserData(ud);

	return newBox;
}
b2Body* KeepOrbiting::createCircle(b2Vec2 pos, float radius, float density, float friction, float restitution, b2BodyType type)
{
	b2Body* newObj;

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

	newObj = m_world->CreateBody(&circleDef);
	newObj->CreateFixture(&circleFix);
	return newObj;
}


void KeepOrbiting::CriarPlanetoides(int n)
{
	float l, a, m;
	for (int i = 0; i < n; i++)
	{
		if (i < 3) 
		{
			l = (rand() % (Largura - 8)) - (Largura / 2) + 4;
			a = ((rand() % (Altura - 8)) + Altura / 2) + DeslocamentoVertical;
			m = RandomFloat(4 - i, 7 - i);
			PonteiroPlanetoides.push_back(
				createCircle(b2Vec2(l, a),m, 1, 1, 0, b2_staticBody));
		}
		else
		{
			l = (rand() % (Largura * 2 - 8)) - Largura + 4;
			a = ((rand() % (Altura * 2 - 8)) + 4) + DeslocamentoVertical;
			m = RandomFloat(0.5, 3);
			PonteiroPlanetoides.push_back(
				createCircle(b2Vec2(l, a), m, 1, 1, 0, b2_staticBody));
		}
		PontosGravitacionais.push_back(new b2Vec2(l, a));
		Massa[i] = m;// *2;

		userData* ud = new userData;
		ud->ID = PLANETOIDE;
		PonteiroPlanetoides[i]->SetUserData(ud);

		
	}
}

void KeepOrbiting::criarProjetil(b2Vec2 pos)
{
	Projetil = createCircle(pos, 1, 0.2, 0, 0);
	userData* ud = new userData;
	ud->ID = PROJETIL;
	Projetil->SetUserData(ud);
}

void KeepOrbiting::avancaNivel()
{
	destruirPlanetoides();
	FlagExcluirProjetil = true;
	destruirProjetil();
	
	NivelAtual++;
	PontosGravitacionais.clear();
	Massa = new float[NivelAtual];
	CriarPlanetoides(NivelAtual);
	FlagAvancaNivel = false;
}

void KeepOrbiting::destruirPlanetoides()
{
	for (int i = 0; i < PonteiroPlanetoides.size(); i++)
	{
		if (PonteiroPlanetoides[i]) {
			m_world->DestroyBody(PonteiroPlanetoides[i]);
		}
	}
	PonteiroPlanetoides.clear();
}

void KeepOrbiting::destruirProjetil()
{
	if (FlagExcluirProjetil && Projetil != nullptr) {
		m_world->DestroyBody(Projetil);
		EhPodeDisparar = true;
		EhArrastandoMouse = false;
	}
	Projetil = nullptr;
	FlagExcluirProjetil = false;
}

void KeepOrbiting::reiniciaNiveis()
{
	NivelAtual = 0;
	EhPodeDisparar = true;
	EhArrastandoMouse = false;
	FlagExcluirProjetil = true;
	FlagAvancaNivel = true;
}