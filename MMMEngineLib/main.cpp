#include <iostream>

#include "App.h"

using namespace MMMEngine;

void Render() {}

int main()
{
	App app;

	Action<> act;
	act += []() { Render(); };

	app.onRender += act;
}