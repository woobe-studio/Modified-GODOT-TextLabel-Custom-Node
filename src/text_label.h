/**************************************************************************/
/*  text_label.h                                                          */
/**************************************************************************/

#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include "scene/2d/node_2d.h"
#include "scene/resources/font.h"

class TextLabel : public Node2D {
GDCLASS(TextLabel, Node2D);

public:
    enum VAlign {
        V_ALIGN_TOP,
        V_ALIGN_CENTER
    };

    enum HAlign {
        H_ALIGN_LEFT,
        H_ALIGN_CENTER,
        H_ALIGN_RIGHT
    };

    void set_text(const String& p_text);
    String get_text() const;

    void set_font(const Ref<Font>& p_font);
    Ref<Font> get_font() const;

    void set_font_color(const Color& p_color);
    Color get_font_color() const;

    void set_font_size(int p_font_size);
    int get_font_size() const;

    void set_font_spacing(int p_font_spacing);
    int get_font_spacing() const;

    void set_V_align(VAlign p_align_v);
    VAlign get_V_align() const;

    void set_H_align(HAlign p_align_h);
    HAlign get_H_align() const;

    void set_max_lines_visible(int p_max_lines);
    int get_max_lines_visible() const;

    void set_max_chars_per_line(int p_max_chars);
    int get_max_chars_per_line() const;

    void set_percent_visible(float p_percent);
    float get_percent_visible() const;

    void set_uppercase_enabled(bool p_enabled);
    bool is_uppercase_enabled() const;

    TextLabel();
    ~TextLabel();

private:
    VAlign align_v;
    HAlign align_h;
    String text;
    Ref<Font> font;
    int font_size;
    int font_spacing;
    Color font_color;
    int max_lines_visible;
    float percent_visible;
    int max_chars_per_line;
    Vector<String> processed_lines;
    bool uppercase_enabled;

protected:
    void _notification(int p_what);
    static void _bind_methods();
    void draw();

private:
    void update_text();
};
VARIANT_ENUM_CAST(TextLabel::VAlign);
VARIANT_ENUM_CAST(TextLabel::HAlign);

#endif // TEXT_LABEL_H
