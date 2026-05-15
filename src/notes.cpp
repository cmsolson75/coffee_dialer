// #define GL_SILENCE_DEPRECATION
//
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
//
// #include <OpenGL/gl3.h>
// #include <GLFW/glfw3.h>
//
// struct AppState {
//     int counter = 0;
//     float value = 0.5f;
//     bool enabled = true;
//     int mode = 0;
//     char text[128] = "hello";
// };
//
// void MainWindow(AppState& state) {
//
//     ImGui::Begin("Starter App");
//
//     ImGui::Text("Hello world");
//     ImGui::Separator();
//
//     if (ImGui::Button("Increment")) {
//         state.counter++;
//     }
//
//     ImGui::SameLine();
//
//     if (ImGui::Button("Reset")) {
//         state.counter = 0;
//     }
//
//     ImGui::Text("Counter: %d", state.counter);
//
//     ImGui::Spacing();
//     ImGui::Separator();
//
//     ImGui::Checkbox("Enabled", &state.enabled);
//     ImGui::SliderFloat("Value", &state.value, 0.0f, 1.0f);
//     ImGui::InputText("Text", state.text, sizeof(state.text));
//
//     ImGui::Spacing();
//
//     ImGui::RadioButton("Mode A", &state.mode, 0);
//     ImGui::SameLine();
//     ImGui::RadioButton("Mode B", &state.mode, 1);
//     ImGui::SameLine();
//     ImGui::RadioButton("Mode C", &state.mode, 2);
//
//     ImGui::Spacing();
//
//     const char* items[] = { "Apple", "Banana", "Orange" };
//     static int selected_item = 0;
//
//     ImGui::Combo(
//         "Dropdown",
//         &selected_item,
//         items,
//         IM_ARRAYSIZE(items)
//     );
//
//     ImGui::Spacing();
//     ImGui::Separator();
//
//     if (ImGui::Button("Open popup")) {
//         ImGui::OpenPopup("My Popup");
//     }
//
//     if (ImGui::BeginPopup("My Popup")) {
//         ImGui::Text("This is a popup.");
//         ImGui::Text("Counter is %d", state.counter);
//
//         if (ImGui::Button("Close")) {
//             ImGui::CloseCurrentPopup();
//         }
//
//         ImGui::EndPopup();
//     }
//
//     ImGui::Spacing();
//     ImGui::Separator();
//
//     ImGui::Text("Preview");
//     ImGui::BulletText("enabled = %s", state.enabled ? "true" : "false");
//     ImGui::BulletText("value = %.2f", state.value);
//     ImGui::BulletText("text = %s", state.text);
//     ImGui::BulletText("mode = %d", state.mode);
//     ImGui::BulletText("dropdown = %s", items[selected_item]);
//
//     ImGui::End();
// }
//
// void SecondWindow() {
//     ImGui::Begin("Second Window");
//     ImGui::Text("HELLO FROM WINDOW TWO");
//     if (ImGui::Button("Click for fun")) {
//         ImGui::OpenPopup("Pop 2");
//     }
//
//     if (ImGui::BeginPopup("Pop 2")) {
//         ImGui::Text("Second Popup");
//
//         if (ImGui::Button("Close")) {
//             ImGui::CloseCurrentPopup();
//         }
//
//         ImGui::EndPopup();
//     }
//
//     ImGui::End();
// }
//
// int main() {
//     if (!glfwInit()) {
//         return 1;
//     }
//
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif
//
//     GLFWwindow* window = glfwCreateWindow(
//         1280,
//         720,
//         "ImGui Starter",
//         nullptr,
//         nullptr
//     );
//
//     if (!window) {
//         glfwTerminate();
//         return 1;
//     }
//
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1);
//
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGui::StyleColorsDark();
//
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init("#version 150");
//
//     // int counter = 0;
//     // float value = 0.5f;
//     // bool enabled = true;
//     // int mode = 0;
//     // char text[128] = "hello";
//     AppState state;
//
//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();
//
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
//
//         ImGui::SetNextWindowSize(ImVec2(420, 420), ImGuiCond_FirstUseEver);
//
//         MainWindow(state);
//         SecondWindow();
//
//         ImGui::Render();
//
//         int display_w = 0;
//         int display_h = 0;
//         glfwGetFramebufferSize(window, &display_w, &display_h);
//
//         glViewport(0, 0, display_w, display_h);
//         glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);
//
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//         glfwSwapBuffers(window);
//     }
//
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//
//     glfwDestroyWindow(window);
//     glfwTerminate();
//
//     return 0;
// }//
// // Created by Cameron Olson on 5/8/26.
// //
