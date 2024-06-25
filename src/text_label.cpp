/**************************************************************************/
/*  text_label.cpp                                                          */
/**************************************************************************/

#include "text_label.h"

void TextLabel::set_text(const String& p_text) {
    if (p_text == text)
        return;
    text = p_text;
    update_text();
    update();
}

String TextLabel::get_text() const {
    return text;
}

void TextLabel::set_font(const Ref<Font>& p_font) {
    if (p_font == font)
        return;
    font = p_font;
    update();
}

Ref<Font> TextLabel::get_font() const {
    return font;
}

void TextLabel::set_font_color(const Color& p_color) {
    font_color = p_color;
    update();
}

Color TextLabel::get_font_color() const {
    return font_color;
}

void TextLabel::set_font_size(int p_font_size) {
    if (p_font_size < 0) {
        p_font_size = 0;
    }
    if (font_size != p_font_size) {
        font_size = p_font_size;
        update();
    }
}

int TextLabel::get_font_size() const {
    return font_size;
}

void TextLabel::set_font_spacing(int p_font_spacing) {
    if (p_font_spacing < 0) {
        p_font_spacing = 0;
    }
    if (font_spacing != p_font_spacing) {
        font_spacing = p_font_spacing;
        update();
    }
}

int TextLabel::get_font_spacing() const {
    return font_spacing;
}

void TextLabel::set_V_align(VAlign p_align_v) {
    if (align_v == p_align_v)
        return;
    align_v = p_align_v;
    update();
}

TextLabel::VAlign TextLabel::get_V_align() const {
    return align_v;
}

void TextLabel::set_H_align(HAlign p_align_h) {
    if (align_h == p_align_h)
        return;
    align_h = p_align_h;
    update();
}

TextLabel::HAlign TextLabel::get_H_align() const {
    return align_h;
}

void TextLabel::set_max_lines_visible(int p_max_lines) {
    if (p_max_lines < 0) {
        p_max_lines = 0;
    }
    if (max_lines_visible != p_max_lines) {
        max_lines_visible = p_max_lines;
        update_text();
        update();
    }
}

int TextLabel::get_max_lines_visible() const {
    return max_lines_visible;
}

void TextLabel::set_max_chars_per_line(int p_max_chars) {
    if (p_max_chars < 0) {
        p_max_chars = 0;
    }
    if (max_chars_per_line != p_max_chars) {
        max_chars_per_line = p_max_chars;
        update_text();
        update();
    }
}

int TextLabel::get_max_chars_per_line() const {
    return max_chars_per_line;
}

void TextLabel::set_percent_visible(const float p_percent) {
    percent_visible = CLAMP(p_percent, 0.0f, 1.0f);
    update_text();
    update();
}

float TextLabel::get_percent_visible() const {
    return percent_visible;
}

void TextLabel::set_uppercase_enabled(bool p_enabled) {
    if (uppercase_enabled != p_enabled) {
        uppercase_enabled = p_enabled;
        update();
    }
}

bool TextLabel::is_uppercase_enabled() const {
    return uppercase_enabled;
}

void TextLabel::update_text() {
    processed_lines.clear();

    String remaining_line = text;

    if (percent_visible == 0.0f || max_lines_visible == 0 || max_chars_per_line == 0)
        return;

    Vector<int> processed_spaces;

    int current_space_index = 0;
    int current_line = 1;

    String operative_text;
    int operative_index = 0;
    int total_chars = remaining_line.length();
    int chars_to_display = int(total_chars * percent_visible);
    for(int i = 0; i< total_chars; i++) {
        if (remaining_line[i] == ' ') {
            processed_spaces.push_back(i);
        }
    }
    for(int char_pos = 0; char_pos < chars_to_display; char_pos++) {
        char operative_char = remaining_line[char_pos];
        bool can_add_text = false;
        bool can_switch_line = false;
        if (operative_char != ' ') {
            if (operative_index != max_chars_per_line && operative_char != '\n') {
                can_add_text = true;
            } else {
                can_switch_line = true;
            }
        } else {
            bool can_skip = false;
            if (current_space_index >= processed_spaces.size() -1) {
                can_skip = true;
                if (operative_index == max_chars_per_line - 1) {
                    can_switch_line = true;
                } else if (operative_text.empty() == false) {
                    if (operative_index + (chars_to_display - char_pos -1)  > max_chars_per_line)
                        can_switch_line = true;
                    else
                        can_add_text = true;
                }
            } else {
                current_space_index++;
            }
            if (can_skip == false) {
                if (operative_index + (processed_spaces[current_space_index] - char_pos) >
                    max_chars_per_line) {
                    can_switch_line = true;
                } else {
                    if (operative_text.empty() == false) {
                        can_add_text = true;
                    }
                }
            }
        }
        if (can_add_text) {
            operative_text += operative_char;
            operative_index++;
        }
        if (can_switch_line) {
            processed_lines.push_back(operative_text);
            operative_text = "";
            if (operative_char != ' ') {
                operative_text += operative_char;
                operative_index = 1;
            } else {
                operative_index = 0;
            }
            current_line++;
            if (current_line > max_lines_visible)
                break;
        }
        if (char_pos + 1 == chars_to_display) {
            if (operative_text.empty() == false) {
                processed_lines.push_back(operative_text);
            }
            break;
        }
    }

}

void TextLabel::draw() {
    if (is_visible_in_tree() == false || text.empty() || font.is_null() || font.is_valid() == false || max_lines_visible == 0 ||
        max_chars_per_line == 0 || font_size <= 0)
        return;


    Vector<String> lines = processed_lines;
    int lines_size = lines.size();
    int y_offset = font_size / 2.0;
    if (max_lines_visible > 1 ) {
        if (align_v == V_ALIGN_CENTER) {
            if (lines_size <= max_lines_visible)
                y_offset -= (lines_size - 1) * font_size;
            else if (lines_size > max_lines_visible)
                y_offset -= (max_lines_visible - 1) * font_size;
        } else if (align_v == V_ALIGN_TOP) {
            y_offset *= -1;
            y_offset += 2 * font_size;
        }
    }
    int text_box_width = font_size * (max_chars_per_line) + (max_chars_per_line -1) * font_spacing;

    for (int i = 0; i < lines_size; i++) {
        String line_part = lines[i];
        if (uppercase_enabled) {
            line_part = line_part.to_upper();
        }
        int line_width = font->get_string_size(line_part).width;
        int line_height = font->get_string_size(line_part).height;
        Point2 line_position;
        if (line_width == text_box_width)
            line_position.x = -1 * ((text_box_width) / 2.0);
        else {
            if (align_h == H_ALIGN_CENTER)
                line_position.x = -1 * ((line_width) / 2.0);
            else if(align_h == H_ALIGN_LEFT)
                line_position.x = -0.5 * text_box_width;
            else if(align_h == H_ALIGN_RIGHT)
                line_position.x = (0.5 * text_box_width - line_width);
        }
        line_position.y = y_offset;
        y_offset += line_height;
        draw_string(font, line_position, line_part, font_color, -1);
        if (i == max_lines_visible -1)
            break;
    }
}

void TextLabel::_notification(int p_what) {
    if (p_what == NOTIFICATION_DRAW) {
        draw();
    }
}

void TextLabel::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_text", "text"), &TextLabel::set_text);
    ClassDB::bind_method(D_METHOD("get_text"), &TextLabel::get_text);

    ClassDB::bind_method(D_METHOD("set_font", "font"), &TextLabel::set_font);
    ClassDB::bind_method(D_METHOD("get_font"), &TextLabel::get_font);

    ClassDB::bind_method(D_METHOD("set_font_color", "color"), &TextLabel::set_font_color);
    ClassDB::bind_method(D_METHOD("get_font_color"), &TextLabel::get_font_color);
    ClassDB::bind_method(D_METHOD("set_V_align", "align_V"), &TextLabel::set_V_align);
    ClassDB::bind_method(D_METHOD("get_V_align"), &TextLabel::get_V_align);
    ClassDB::bind_method(D_METHOD("set_H_align", "align_H"), &TextLabel::set_H_align);
    ClassDB::bind_method(D_METHOD("get_H_align"), &TextLabel::get_H_align);
    ClassDB::bind_method(D_METHOD("set_max_lines_visible", "max_lines"), &TextLabel::set_max_lines_visible);
    ClassDB::bind_method(D_METHOD("get_max_lines_visible"), &TextLabel::get_max_lines_visible);
    ClassDB::bind_method(D_METHOD("set_percent_visible", "percent"), &TextLabel::set_percent_visible);
    ClassDB::bind_method(D_METHOD("get_percent_visible"), &TextLabel::get_percent_visible);
    ClassDB::bind_method(D_METHOD("set_max_chars_per_line", "max_chars"), &TextLabel::set_max_chars_per_line);
    ClassDB::bind_method(D_METHOD("get_max_chars_per_line"), &TextLabel::get_max_chars_per_line);
    ClassDB::bind_method(D_METHOD("set_font_size", "font_size"), &TextLabel::set_font_size);
    ClassDB::bind_method(D_METHOD("get_font_size"), &TextLabel::get_font_size);
    ClassDB::bind_method(D_METHOD("set_font_spacing", "font_spacing"), &TextLabel::set_font_spacing);
    ClassDB::bind_method(D_METHOD("get_font_spacing"), &TextLabel::get_font_spacing);
    ClassDB::bind_method(D_METHOD("set_uppercase_enabled", "enabled"), &TextLabel::set_uppercase_enabled);
    ClassDB::bind_method(D_METHOD("is_uppercase_enabled"), &TextLabel::is_uppercase_enabled);


    ADD_PROPERTY(PropertyInfo(Variant::STRING, "text", PROPERTY_HINT_MULTILINE_TEXT), "set_text", "get_text");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "font", PROPERTY_HINT_RESOURCE_TYPE, "Font"), "set_font", "get_font");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "font_size"), "set_font_size", "get_font_size");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "font_spacing"), "set_font_spacing", "get_font_spacing");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "font_color"), "set_font_color", "get_font_color");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "align_v", PROPERTY_HINT_ENUM, "Top,Center"), "set_V_align", "get_V_align");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "align_h", PROPERTY_HINT_ENUM, "Left,Center,Right"), "set_H_align", "get_H_align");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_lines_visible"), "set_max_lines_visible", "get_max_lines_visible");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_chars_per_line"), "set_max_chars_per_line", "get_max_chars_per_line");
    ADD_PROPERTY(PropertyInfo(Variant::REAL, "percent_visible", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_percent_visible", "get_percent_visible");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "uppercase_enabled"), "set_uppercase_enabled", "is_uppercase_enabled");
}

TextLabel::TextLabel() {
    font_color = Color(1, 1, 1);
    font_size = 24;
    font_spacing = 8;
    align_v = V_ALIGN_CENTER;
    align_h = H_ALIGN_CENTER;
    max_lines_visible = 0;
    max_chars_per_line = 0;
    uppercase_enabled = true;
    percent_visible = 1.0f;
}

TextLabel::~TextLabel() {
    // Destructor
}