#include "Framework.h"
#include "Widget.h"

Widget::Widget()
{
	title = "##";

	windowFlags = ImGuiWindowFlags_NoCollapse;
	bVisible = true;
}

Widget::~Widget()
{

}

void Widget::Begin()
{
	ImGui::Begin(title.c_str(), &bVisible, windowFlags);
}

void Widget::End()
{
	ImGui::End();
}
