//
// Created by lbert on 4/15/2023.
//

#ifndef ZYGISK_MENU_TEMPLATE_MENU_H
#define ZYGISK_MENU_TEMPLATE_MENU_H
#include "functions.h"

using namespace ImGui;

void DrawMenu()
{
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    {
        Begin(OBFUSCATE("EYEFREE | TORAM ONLINE"));
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_FittingPolicyResizeDown;
        if (BeginTabBar("Menu", tab_bar_flags)) {
            if (BeginTabItem(OBFUSCATE("Player Menu"))) {
                Checkbox(OBFUSCATE("God Mode"), &cGod);
                EndTabItem();
            }
            EndTabBar();
        }
//        Patches();
        End();
    }
}

void SetupImgui() {
    IMGUI_CHECKVERSION();
    CreateContext();
    ImGuiIO &io = GetIO();
    io.DisplaySize = ImVec2((float) glWidth, (float) glHeight);
    ImGui_ImplOpenGL3_Init("#version 100");
    StyleColorsDark();
    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowRounding = 5.3f;
    style->FrameRounding = 2.3f;
    style->ScrollbarRounding = 0;
    style->WindowTitleAlign = ImVec2(0.5, 0.5);
    style->WindowMinSize = ImVec2(200, 80);
    style->FramePadding = ImVec2(8, 6);
    style->ScaleAllSizes(2.0f);
    style->ScrollbarSize /= 1;
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, 30, 30.0f);
}


EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!setupimg)
    {
        SetupImgui();
        setupimg = true;
    }

    ImGuiIO &io = GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    NewFrame();

    DrawMenu();

    EndFrame();
    Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

#endif //ZYGISK_MENU_TEMPLATE_MENU_H
