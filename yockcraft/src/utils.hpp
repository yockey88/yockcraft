#ifndef YOCKCRAFT_UTILS_HPP
#define YOCKCRAFT_UTILS_HPP

#include <string>
#include <random>
#include <chrono>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <spdlog/fmt/fmt.h>

namespace std {

  template <typename Clock>
  using tp = chrono::time_point<Clock>;

  using stdy_clck = chrono::steady_clock;

} // namespace std

namespace yockcraft {
namespace utils {

  std::string ReadFile(const std::string& file);

  template <typename... Args>
  void println(const std::string_view fmt_str , Args&&... args) {
    std::cout << fmt::format(fmt::runtime(fmt_str) , std::forward<Args>(args)...) << std::endl;
  }
  
  template <typename... Args>
  void printerr(const std::string_view fmt_str , Args&&... args) {
    std::cerr << fmt::format(fmt::runtime(fmt_str) , std::forward<Args>(args)...) << std::endl;
  }

  struct PrintfProfiler {
    std::string name = "";
    std::tp<std::stdy_clck> before;

    PrintfProfiler(const std::string& name)
        : name(name) {
      before = std::stdy_clck::now();
    }
    ~PrintfProfiler() {
      std::tp<std::stdy_clck> after = std::stdy_clck::now();
      std::chrono::milliseconds dur = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
      println("{} took [{}]ms" , name , dur.count());
    }
  };

} // namespace utils

  constexpr static float eps = 0.0001f;

  bool FloatEQ(float a , float b);
  
  float Clamp(float min , float max , float val);
  float Clamp01(float val);

  float InverseLerp(float min , float max , float val);

  template <typename T>
  class RandomGenerator {
    public:
      RandomGenerator()
        : gen(rand()) {}
      RandomGenerator(T min , T max) 
        : gen(rand()) , dist(min , max) {}
      ~RandomGenerator() {}

      T Next() { return dist(gen); }

    private:
      std::random_device rand;
      std::mt19937 gen;
      std::uniform_int_distribution<T> dist;
  };

  class ScopedID {
    public:
      ScopedID(const ScopedID&) = delete;
      ScopedID& operator=(const ScopedID&) = delete;
  
      template<typename T>
      ScopedID(T id) { 
        ImGui::PushID(id); 
      }

      ~ScopedID() { 
        ImGui::PopID(); 
      }
  };
  
  class ScopedFont {
    bool pushed = false;
  
    public:
      ScopedFont(ImFont* font) {
        if (font != nullptr) {
          ImGui::PushFont(font);
          pushed = true;
        }
      }
      ~ScopedFont(){
        if (pushed) {
          ImGui::PopFont();
        }
      }
  };
  
  class ScopedStyle {        
    bool pushed = false;
  
    public:
      ScopedStyle(ImGuiStyleVar var , float new_val) {
        const ImGuiDataVarInfo* var_info = ImGui::GetStyleVarInfo(var);
        if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1) {
          ImGui::PushStyleVar(var , new_val);
          pushed = true;
        }
      }

      ScopedStyle(ImGuiStyleVar var , const ImVec2& new_col) {
        const ImGuiDataVarInfo* var_info = ImGui::GetStyleVarInfo(var);
        if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2) {
          ImGui::PushStyleVar(var , new_col);
          pushed = true;
        }
      }

      ~ScopedStyle() {
        if (pushed) {
          ImGui::PopStyleVar();
        }
      }
  };
  
  class ScopedColor {
    public:
      ScopedColor(ImGuiCol col , uint32_t new_col) {
        ImGui::PushStyleColor(col , new_col);
      }

      ScopedColor(ImGuiCol col , const ImVec4& new_col) {
        ImGui::PushStyleColor(col , new_col);
      }

      ~ScopedColor() {
        ImGui::PopStyleColor();
      }  
  };
  
  class ScopedColorStack {
    uint32_t count;
  
    template <typename ColorType , typename... Colors>
    void PushColor(ImGuiCol col , ColorType color , Colors&&... colors) {
      if constexpr (sizeof... (colors) == 0) {
        ImGui::PushStyleColor(col , ImColor(color).Value);
      } else {
        ImGui::PushStyleColor(col , ImColor(color).Value);
        PushColor(std::forward<Colors>(colors)...);
      }
    }
  
    public:
      ScopedColorStack(const ScopedColorStack&) = delete;
      ScopedColorStack& operator=(const ScopedColorStack&) = delete;
  
      template <typename ColorType , typename... Colors>
      ScopedColorStack(ImGuiCol first_color , ColorType first , Colors&&... colors) 
          : count((sizeof... (colors) / 2) + 1) {
        static_assert(
          (sizeof... (colors) & 1u) == 0 ,
          "ScopedColorStack requires an even number of arguments"
        );
  
        PushColor(first_color , first , std::forward<Colors>(colors)...);
      }
  
      ~ScopedColorStack() {
        ImGui::PopStyleColor(count);
      }
  };

namespace ui {
namespace theme {

  constexpr auto accent = IM_COL32(236 , 158 , 36 , 255);
  constexpr auto highlight = IM_COL32(39 , 185 , 242 , 255);
  /* ?? */ constexpr auto nice_blue = IM_COL32(83 , 232 , 254 , 255);
  constexpr auto compliment = IM_COL32(78 , 151 , 166 , 255);
  constexpr auto background = IM_COL32(36 , 36 , 36 , 255);
  constexpr auto background_dark = IM_COL32(26 , 26 , 26 , 255);
  constexpr auto title_bar = IM_COL32(21 , 21 , 21 , 255);
  constexpr auto title_bar_orange = IM_COL32(186 , 66 , 30 , 255);
  constexpr auto title_bar_green = IM_COL32(18 , 88 , 30 , 255);
  constexpr auto title_bar_red = IM_COL32(185 , 30 , 30 , 255);
  constexpr auto property_field = IM_COL32(15 , 15 , 15 , 255);
  constexpr auto text = IM_COL32(192 , 192 , 192 , 255);
  constexpr auto text_bright = IM_COL32(210 , 210 , 210 , 255);
  constexpr auto text_dark = IM_COL32(128 , 128 , 128 , 255);
  constexpr auto text_error = IM_COL32(230, 51, 51, 255);
  constexpr auto muted = IM_COL32(77, 77, 77, 255);
  constexpr auto group_header = IM_COL32(47, 47, 47, 255);
  constexpr auto selection = IM_COL32(237, 192, 119, 255);
  constexpr auto selection_muted  = IM_COL32(237, 201, 142, 23);
  constexpr auto background_popup = IM_COL32(50, 50, 50, 255);
  constexpr auto valid_prefab = IM_COL32(82, 179, 222, 255);
  constexpr auto invalid_prefab = IM_COL32(222, 43, 43, 255);
  constexpr auto missing_mesh = IM_COL32(230, 102, 76, 255);
  constexpr auto mesh_not_set = IM_COL32(250, 101, 23, 255);

  static inline ImU32 ColorWithMultiplier(const ImColor& color , float multiplier) {
    const ImVec4& color_val = color.Value;
    float hue , sat , val;
    ImGui::ColorConvertRGBtoHSV(color_val.x , color_val.y , color_val.z , hue , sat , val);
#undef min
    return ImColor::HSV(hue , sat , std::min(val * multiplier , 1.f));
  }

} // namespace theme
} // namespace ui

  static inline void ShiftCursor(float x , float y) {
    const ImVec2 cursor_pos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor_pos.x + x , cursor_pos.y + y));
  }
  
  static inline void ShiftCursorX(float x) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x);
  }
  
  static inline void ShiftCursorY(float y) {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + y);
  }
  
  static inline void Underline(bool full_width , float offx , float offy) {
    if (full_width) {
      if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)  {
        ImGui::PushColumnsBackground();
      } else if (ImGui::GetCurrentTable() != nullptr) {
        ImGui::TablePushBackgroundChannel();
      }
    }
  
    const float width = full_width ? 
      ImGui::GetWindowWidth() : 
      ImGui::GetContentRegionAvail().x;
    
    const ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddLine(
      ImVec2(cursor.x  + offx , cursor.y + offy),
      ImVec2(cursor.x + width , cursor.y + offy),
      IM_COL32(26, 26, 26, 255) /* dark background */, 1.0f
    );
  
    if (full_width) {
      if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr) {
        ImGui::PopColumnsBackground();
      } else if (ImGui::GetCurrentTable() != nullptr) {
        ImGui::TablePopBackgroundChannel();
      }
    }
  }

  template <typename T>
  void Table(
    const std::string& name , const std::vector<std::string>& headers , 
    const ImVec2& size , uint32_t columns , T callback
  ) {
    ScopedStyle style(ImGuiStyleVar_CellPadding , ImVec2(4.f , 0.f));
    ImColor bg_color = ImColor(ui::theme::background);
    const ImColor col_row_alt = ui::theme::ColorWithMultiplier(bg_color , 1.2f);
    ScopedColor row_color(ImGuiCol_TableRowBg , bg_color.Value);
    ScopedColor row_alt_color(ImGuiCol_TableRowBgAlt , col_row_alt.Value);
    ScopedColor table_color(ImGuiCol_ChildBg , bg_color.Value);

    ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX | 
      ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | 
      ImGuiTableFlags_ScrollY   | ImGuiTableFlags_RowBg;
    if (!ImGui::BeginTable("Project-Table" , columns , flags , size)) {
      return;
    }

    float edge_offset = 4.f;
    const float cursor_x = ImGui::GetCursorScreenPos().x;

    for (uint32_t i = 0; i < columns; ++i) {
      ImGui::TableSetupColumn(headers[i].c_str());
    }

    {
      const ImColor active_color = ui::theme::ColorWithMultiplier(bg_color , 1.2f);
      ScopedColorStack color_stack(ImGuiCol_TableHeaderBg , active_color);

      ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount() , 1);
      ImGui::TableNextRow(ImGuiTableRowFlags_Headers , 22.f);
      
      for (uint32_t i = 0; i < columns; ++i) {
        const char* header = ImGui::TableGetColumnName(i);
        ImGui::PushID(header);
        ShiftCursor(edge_offset * 3.f , edge_offset * 2.f);
        ImGui::TableHeader(header);
        ShiftCursor(-edge_offset * 3.f , -edge_offset * 2.f);
        ImGui::PopID();
      }
      ImGui::SetCursorScreenPos(ImVec2(cursor_x , ImGui::GetCursorScreenPos().y));
      Underline(false , 0.f , 5.f);
    }

    callback();

    ImGui::EndTable();
  }

} // namespace yockcraft

#endif // !YOCKCRAFT_UTILS_HPP
