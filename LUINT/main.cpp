#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "machine.h"
#include "gui.h"
#include "hostdata.h"

// Using OpenGL 4.3
#define GLSL_VERSION "#version 430"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1600, 800, "LUINT", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Enable vsync
	glfwSwapInterval(1);

	if (!gladLoadGL()) {
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	io.IniFilename = NULL; // Don't save windows

	//LUINT::Data::machines.emplace_back(std::make_unique<LUINT::Machines::Machine>(LUINT::Machines::ProcessingUnit(std::string("Default computer"), std::string("aleok studios"))));
	LUINT::Data::SessionData session;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		LUINT::GUI::DrawMainMenuBar(session);
		using Machine = LUINT::Machines::Machine;
		for (std::unique_ptr<Machine>& machine : session.machines)
		{
			machine->Render();
		}

		ImGui::Render();

		{
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}