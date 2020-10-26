﻿#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Haketon
{


EditorLayer::EditorLayer()
	: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
{
}

void EditorLayer::OnAttach()
{
	HK_PROFILE_FUNCTION();

	m_Texture = Texture2D::Create();
	
	FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Framebuffer::Create(fbSpec);
}

void EditorLayer::OnDetach()
{
	HK_PROFILE_FUNCTION();

}

void EditorLayer::OnUpdate(Timestep ts)
{
	HK_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Renderer2D::ResetStats();

	m_Framebuffer->Bind();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	uint32_t maxQuads = 10;
	for(uint32_t x = 0; x < maxQuads; x++)
	{
		for(uint32_t y = 0; y < maxQuads; y++)
		{
			Renderer2D::DrawRotatedQuad({x, y, -0.1f}, glm::radians(45.0f),{1.0f, 1.0f}, {(float)x / (float)maxQuads, (float)y / (float)maxQuads, 0.5f, 1.0f});
		}
	}

	Renderer2D::EndScene();
	m_Framebuffer->Unbind();
}

void EditorLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}

void EditorLayer::OnImGuiRender()
{
	HK_PROFILE_FUNCTION();

	/*static bool show = true;
	ImGui::ShowDemoWindow(&show);*/

	// Set this to true to enable dockspace
	static bool dockingEnabled = true;
	if(dockingEnabled)
	{
		static bool dockspaceOpen = true;
	    static bool opt_fullscreen = true;
	    static bool opt_padding = false;
	    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	    // because it would be confusing to have two docking targets within each others.
	    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	    if (opt_fullscreen)
	    {
	        ImGuiViewport* viewport = ImGui::GetMainViewport();
	        ImGui::SetNextWindowPos(viewport->GetWorkPos());
	        ImGui::SetNextWindowSize(viewport->GetWorkSize());
	        ImGui::SetNextWindowViewport(viewport->ID);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	    }
	    else
	    {
	        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	    }

	    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	    // and handle the pass-thru hole, so we ask Begin() to not render a background.
	    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	        window_flags |= ImGuiWindowFlags_NoBackground;

	    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	    // all active windows docked into it will lose their parent and become undocked.
	    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	    if (!opt_padding)
	        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	    if (!opt_padding)
	        ImGui::PopStyleVar();

	    if (opt_fullscreen)
	        ImGui::PopStyleVar(2);

	    // DockSpace
	    ImGuiIO& io = ImGui::GetIO();
	    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	    {
	        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	    }

	    if (ImGui::BeginMenuBar())
	    {
	        if (ImGui::BeginMenu("File"))
	        {
	            if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }     
	            ImGui::EndMenu();
	        }
	        ImGui::EndMenuBar();
	    }

		ImGui::Begin("Stats");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if(m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
			
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();
		
	    ImGui::End();
	}
}
}
