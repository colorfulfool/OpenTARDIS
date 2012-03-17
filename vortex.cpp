#include "Vortex.h"

Vortex::Vortex(void)
{
	mvp = MVPprovider::Instance();
}

Vortex::~Vortex(void)
{
}

void Vortex::createParticle(particle* init)
{

}

void Vortex::evolveParticle(particle* evolve)
{
	evolve->position += evolve->speed; //движение
}

void Vortex::initialize()
{

}

void Vortex::process()
{

}

void Vortex::render()
{

}
