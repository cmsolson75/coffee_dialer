#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <ostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include "db.h"
#include "model.h"

// State for form window
struct CoffeeForm {
    char beanName[128] = "";
    float doseGrams = 18.0;
    int grindSetting = 12;
    float yieldGrams = 36.0f;
    float brewTimeSeconds = 30.0f;
    char notes[128] = "";

    void reset() {
        *this = CoffeeForm{};
    }
};

void FormWindow(Database* db, std::vector<CoffeeEntry>& coffeeEntries, CoffeeForm& form, int& editIndex) {
    const ImGuiIO& io = ImGui::GetIO();

    ImVec2 pos = ImVec2(0.0f, 0.0f);
    ImVec2 size = ImVec2(io.DisplaySize.x * 0.4, io.DisplaySize.y);

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Form", nullptr, flags);

    ImGui::PushItemWidth(260.0f);

    ImGui::InputText("Bean", form.beanName, IM_ARRAYSIZE(form.beanName));
    ImGui::DragFloat("Dose (g)", &form.doseGrams, 0.1f, 0.0f, 30.0f, "%.1f");
    ImGui::InputInt("Grind Setting", &form.grindSetting);
    ImGui::DragFloat("Yield (g)", &form.yieldGrams, 0.1f, 0.0f, 80.0f, "%.1f");
    ImGui::DragFloat("Brew Time (s)", &form.brewTimeSeconds, 0.1f, 0.0f, 120.0f, "%.1f");
    ImGui::PopItemWidth();
    ImGui::Spacing();
    ImGui::Text("Notes");

    ImGui::InputTextMultiline(
        "##Notes",
        form.notes,
        IM_ARRAYSIZE(form.notes),
        ImVec2(-FLT_MIN, 120.0f)
    );


    if (ImGui::Button(editIndex == -1 ? "Submit" : "Save")) {
        CoffeeEntry entry;
        entry.beanName = form.beanName;
        entry.doseGrams = form.doseGrams;
        entry.grindSetting = form.grindSetting;
        entry.yieldGrams = form.yieldGrams;
        entry.brewTimeSeconds = form.brewTimeSeconds;
        entry.notes = form.notes;

        if (editIndex == -1) {
            if (insertRow(db, entry)) {
                coffeeEntries = loadCoffeeEntries(db);
            }
        } else {
            int rowId = coffeeEntries[editIndex].id;
            if (updateRow(db, entry, rowId)) {
                coffeeEntries = loadCoffeeEntries(db);
                editIndex = -1;
            }
        }
        form.reset();

    }


    ImGui::End();
}

void DisplayWindow(Database* db, std::vector<CoffeeEntry>& coffeeEntries, CoffeeForm& form, int& selectedIndex, int& editIndex) {
    const ImGuiIO& io = ImGui::GetIO();

    ImVec2 pos  = ImVec2(io.DisplaySize.x * 0.4f, 0.0f);
    ImVec2 size = ImVec2(io.DisplaySize.x * 0.6f, io.DisplaySize.y);

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Display", nullptr, flags);

    ImGuiTableFlags tableFlags =
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_ScrollX |
            ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("Past Coffees", 7, tableFlags, ImVec2(0, -40.0f))) {
        // Setup headers
        ImGui::TableSetupColumn("Date");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Dose(g)");
        ImGui::TableSetupColumn("Grind Setting");
        ImGui::TableSetupColumn("Yield (g)");
        ImGui::TableSetupColumn("Brew Time (s)");
        ImGui::TableSetupColumn("Notes");
        ImGui::TableHeadersRow();

        for (int n = 0; n < coffeeEntries.size(); n++) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", coffeeEntries[n].createdAt.c_str());
            // Column 1: ID
            ImGui::TableSetColumnIndex(1);
            ImGui::PushID(n);
            bool isSelected = selectedIndex == n;

            if (ImGui::Selectable(coffeeEntries[n].beanName.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
                // Note(cam): this is okay, but state is tangled
                // Note(cam): better would be to have the save state tied to something else.
                selectedIndex = n;
            }
            ImGui::PopID();
            // ImGui::Text("%s", coffeeEntries[n].beanName.c_str());

            // Column 1: Name
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f", coffeeEntries[n].doseGrams);

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%d", coffeeEntries[n].grindSetting);

            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.1f", coffeeEntries[n].yieldGrams);

            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%.1f", coffeeEntries[n].brewTimeSeconds);

            ImGui::TableSetColumnIndex(6);
            ImGui::Text("%s", coffeeEntries[n].notes.c_str());


        }
        ImGui::EndTable();
    }

    if (selectedIndex >= 0 && selectedIndex < coffeeEntries.size()) {
        if (ImGui::Button("Edit Selected")) {
            editIndex = selectedIndex;
            const CoffeeEntry& selected = coffeeEntries[editIndex];

            std::strncpy(form.beanName, selected.beanName.c_str(), sizeof(form.beanName) - 1);
            form.beanName[sizeof(form.beanName) - 1] = '\0';
            form.doseGrams = selected.doseGrams;
            form.grindSetting = selected.grindSetting;
            form.yieldGrams = selected.yieldGrams;
            form.brewTimeSeconds = selected.brewTimeSeconds;
            std::strncpy(form.notes, selected.notes.c_str(), sizeof(form.notes) - 1);
            form.notes[sizeof(form.notes) - 1] = '\0';
            // Note(cam): this is where you would add in an edit flag flip
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Selected")) {
            int rowId = coffeeEntries[selectedIndex].id;
            if (deleteRow(db, rowId)) {
                // Note(cam): reload in-memory database with changes
                coffeeEntries = loadCoffeeEntries(db);
                selectedIndex = -1;
            }
        }
    }
    ImGui::End();
}

int main() {
    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Coffee Dialer",
        nullptr,
        nullptr
    );

    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");


    Database db;
    const std::string dbPath = getDatabasePath();
    std::cout << "Opening database at: " << dbPath << std::endl;

    if (!openDatabase(&db, dbPath)) {
        std::cerr << "Failed to open database" << std::endl;
        glfwTerminate();
        return 1;
    }
    createTable(&db);

    static CoffeeForm form;
    static std::vector<CoffeeEntry> coffeeEntries = loadCoffeeEntries(&db);
    static int selectedIndex = -1;
    static int editIndex = -1;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(420, 420), ImGuiCond_FirstUseEver);

        FormWindow(&db,coffeeEntries, form, editIndex);
        DisplayWindow(&db, coffeeEntries, form, selectedIndex, editIndex);

        ImGui::Render();

        int display_w = 0;
        int display_h = 0;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);
        glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    closeDatabase(&db);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}