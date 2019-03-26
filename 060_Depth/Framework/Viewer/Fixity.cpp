#include "framework.h"
#include "Fixity.h"

Fixity::Fixity()
{
	Position(0, 0, 0);

	Rotation();
	View();
}

Fixity::~Fixity()
{
}

void Fixity::Update()
{
	Rotation();
	View();
}
