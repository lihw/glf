// File: widget.h
// Author: Li Hongwei
// Date: 2008-3-29
//
//
// ((( -------------------------------------------------------------- )))
// This file belongs to the gGL framework, which you can distribute and modify
// at your own will. Pls feel free to contact gobinlee@yahoo.com.cn if you find
// any problem or want to talk with me :))))))
// ((( -------------------------------------------------------------- )))
//
//
// Basic control widgets appearing on the rendering window

#include <gGL/widget.h>

#include <gGL/font.h>
#include <gGL/texture.h>

namespace gGL {

// -------------------------------------------------------------- 
// Local variable
// -------------------------------------------------------------- 
static Font widget_font;

static float hover_color[] = {0.60f, 0.60f, 0.60f, 0.50f};
static float on_color[] = {0.99f, 0.25f, 0.17f, 1.00f};
static float off_color[] = {0.30f, 0.30f, 0.30f, 0.80f};
static float static_color[] = {0.129f, 0.576f, 0.796f, 1.000f};



// -------------------------------------------------------------- 
// Missing string.h functions
// -------------------------------------------------------------- 

// -------------------------------------------------------------- 
// Helper functions
// -------------------------------------------------------------- 


// Draw a rounded wireframe rectangle
void draw_line_rect(int x, int y, int w, int h)
{
    glBegin(GL_LINE_LOOP);   
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

// Draw a filled rectangle
void draw_filled_rect(int x, int y, int w, int h)
{
    glBegin(GL_QUADS);   
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

// Draw a filled disk
static void draw_filled_disk(int x, int y, int diameter)
{
    static GLUquadric* quad = gluNewQuadric();

    double inner = 0;
    double outer = (double)diameter * 0.5f;

    float offset_x = (float)x + (float)diameter * 0.5f;
    float offset_y = (float)y + (float)diameter * 0.5f;
    glTranslatef(offset_x, offset_y, 0.0f);
    gluDisk(quad, inner, outer, 20, 1);
    glLoadIdentity();
}

// -------------------------------------------------------------- 
// Class Widget
// -------------------------------------------------------------- 
Widget::Widget(Widget* parent)
{
    m_parent = parent;

    m_visible = true;
    m_enabled = true;

    m_alignment = GGL_WIDGET_ALIGN_LEFT;
    m_valignment = GGL_WIDGET_ALIGN_TOP;

    m_bg_color[0] = m_bg_color[1] = m_bg_color[2] = 0;
    m_fg_color[0] = m_fg_color[1] = m_fg_color[2] = 128;

    m_callback = 0;
    m_live_value = 0;

    m_hover   = false;
    m_clicked = false;
}

Widget::~Widget()
{
}

void Widget::set_parent(Widget* parent)
{
    m_parent = parent;
}

void Widget::set_size(int x, int y, int w, int h)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;

    m_w_hint = w_hint();
    m_h_hint = h_hint();

    if (w < m_w_hint) {
        report_error(__FUNCTION__, "Too small width");
        
        m_w = m_w_hint;
    }

    if (h < m_h_hint) {
        report_error(__FUNCTION__, "Too small height");
        
        m_h = m_h_hint;
    }
}

int Widget::h_hint() const
{
    return m_h_hint;
}

int Widget::w_hint() const
{
    return m_w_hint;
}

void Widget::set_alignment(int align)
{
    m_alignment = align;
}

void Widget::set_valignment(int valign)
{
    m_valignment = valign;
}

void Widget::set_visible(bool visible)
{
    m_visible = visible;
}

void Widget::set_enabled(bool enabled)
{
    m_enabled = enabled;
}

void Widget::on_mouse_over(int x, int y)
{
}

bool Widget::on_mouse_button_down(int x, int y, int button)
{
    return false;
}

bool Widget::on_mouse_button_up(int x, int y, int button)
{
    return false;
}

bool Widget::on_mouse_motion(int x, int y)
{
    return false;
}

void Widget::align(int bx, int by, int* x, int* y) const
{
    // Compute the new position of left bottom corner
    // of the bounding box of the widgets
    if (m_alignment == GGL_WIDGET_ALIGN_LEFT) 
        *x = bx;
    else if (m_alignment == GGL_WIDGET_ALIGN_CENTER) 
        *x = bx + (m_w - w_hint()) / 2;
    else if (m_alignment == GGL_WIDGET_ALIGN_RIGHT) 
        *x = bx + m_w - w_hint();

    if (m_valignment == GGL_WIDGET_ALIGN_TOP) 
        *y = by + m_h - h_hint();
    else if (m_valignment == GGL_WIDGET_ALIGN_MIDDLE) 
        *y = by + (m_h - h_hint()) / 2;
    else if (m_valignment == GGL_WIDGET_ALIGN_BOTTOM) 
        *y = by;
}

void Widget::set_bg_color(int r, int g, int b)
{
    m_bg_color[0] = r;
    m_bg_color[1] = g;
    m_bg_color[2] = b;
}

void Widget::set_fg_color(int r, int g, int b)
{
    m_fg_color[0] = r;
    m_fg_color[1] = g;
    m_fg_color[2] = b;
}

void Widget::connect(void* value, void (*callback)(void* value))
{
    m_callback = callback;

    m_live_value = value;
}

void Widget::update(int x, int y, int w, int h)
{
    /*
    int win_w = glutGet(GLUT_WINDOW_WIDTH);
    int win_h = glutGet(GLUT_WINDOW_HEIGHT);

    // Setup the viewport and matrices
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, win_w, win_h);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, win_w, 0, win_h, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, w, h);

    // Draw the widget 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->draw();
    
    glFinish();
    glutSwapBuffers();

    // Restore the GL context
    glDisable(GL_SCISSOR_TEST);

    glPopAttrib();
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    */

    glutPostRedisplay();
}

void Widget::draw()
{
    /*
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    */
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2i(m_x, m_y);    
        glVertex2i(m_x + m_w, m_y);    
        glVertex2i(m_x + m_w, m_y + m_h);    
        glVertex2i(m_x, m_y + m_h);    
    glEnd();
}

void Widget::set_hover(bool status)
{
    m_hover = status;
}

void Widget::set_clicked(bool status)
{
    m_clicked = status;
}

// -------------------------------------------------------------- 
// Class Widget_Statictext
// -------------------------------------------------------------- 
Widget_Statictext::Widget_Statictext(const char* string, 
                                     Widget* parent)
    : Widget(parent)
{
    strcpy(m_label, string);

    m_w_hint = widget_font.get_string_width(string);
    m_h_hint = widget_font.get_height();

    m_w = m_w_hint;
    m_h = m_h_hint;
}

Widget_Statictext::~Widget_Statictext()
{
}

void Widget_Statictext::draw()
{
    //Widget::draw();

    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    int x, y;

    align(m_x, m_y, &x, &y);
    glRasterPos2i(x, y + widget_font.get_descent());
    widget_font.print("%s", m_label);

    REPORT_GLERROR();
}


// -------------------------------------------------------------- 
// Class Widget_Checkbox
// -------------------------------------------------------------- 
Widget_Checkbox::Widget_Checkbox(const char* string, 
        Widget* parent)
    : Widget(parent)
{
    strcpy(m_label, string);

    m_boxsize = widget_font.get_height() - 
        widget_font.get_descent() - widget_font.get_internal_leading();

    m_w_hint = widget_font.get_string_width(m_label) + 
        m_boxsize + widget_font.get_width();
    m_h_hint = widget_font.get_height();

    m_w = m_w_hint;
    m_h = m_h_hint;

    m_value = 0;
}

Widget_Checkbox::~Widget_Checkbox()
{
}

void Widget_Checkbox::draw()
{
    glPushMatrix();

    int x, y;
    align(m_x, m_y, &x, &y);

    y += widget_font.get_descent();
    
    // Draw the checking box first
    if (m_value)
        glColor4fv(on_color);
    else 
        glColor4fv(off_color);

    draw_filled_rect(x, y, m_boxsize, m_boxsize);
    
    glColor4f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f,
              1.0f);
    glTranslatef(0.0f, 0.0f, 0.1f);
    draw_line_rect(x, y, m_boxsize, m_boxsize);

    if (m_hover) {
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor4fv(hover_color);
        draw_filled_rect(x, y, m_boxsize, m_boxsize);
    }
    
    // Draw the label text later
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);

    glRasterPos2i(x + m_boxsize + widget_font.get_width(), y);
    widget_font.print("%s", m_label);

    REPORT_GLERROR();

    glPopMatrix();
}

bool Widget_Checkbox::on_mouse_button_up(int x, int y, int button)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (m_hover) {
            m_value = 1 - m_value;

            // Invoke callbacks
            if (m_callback)
                m_callback(&m_value);
            if (m_live_value)
                *((int*)m_live_value) = m_value;

            // Redraw myself
            update(0, 0, 0, 0);

            return true;
        }
    }

    return false;
}

void Widget_Checkbox::on_mouse_over(int x, int y)
{
    // Is mouse pointer inside the checkbox?
    int xx, yy;
    align(m_x, m_y, &xx, &yy);

    if (x >= xx - 1 && x <= xx + m_boxsize + 1 &&
        y >= yy - 1 && y <= yy + m_boxsize + 1) {

        m_hover = true;
    } else {
        m_hover = false;
    }

    // Redraw myself
    update(xx, yy, m_w, m_h);
}

// -------------------------------------------------------------- 
// Class Widget_Listbox
// -------------------------------------------------------------- 
Widget_Listbox::Widget_Listbox(const char* string, Widget* parent)
    : Widget(parent)
{
    m_current_index = 0;
    m_hover_index = 1;

    m_collapsed = false;

    strcpy(m_label, string);
    
    m_combo_length = init_combo_length;
    m_combo_height = widget_font.get_height();

    m_box_width  = m_combo_height - 6;
    m_box_height = m_box_width;

    m_w_hint = widget_font.get_string_width(m_label) + 
        widget_font.get_width() + m_combo_length;
    m_h_hint = m_combo_height;

    m_w = m_w_hint;
    m_h = m_h_hint;

    m_current_index = 0;

    m_hover_index = -1;
}

Widget_Listbox::~Widget_Listbox()
{
}



void Widget_Listbox::draw()
{
    glPushMatrix();

    // Draw the label text 
    int x, y;
    align(m_x, m_y, &x, &y);

    y += widget_font.get_descent();
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);

    glRasterPos2i(x, y);
    widget_font.print("%s", m_label);

    // Draw the list box
    y -= widget_font.get_descent();
    x += w_hint() - m_combo_length;

    glColor4fv(off_color);
    draw_filled_rect(x, y, m_combo_length, m_combo_height);

    glTranslatef(0.0f, 0.0f, 0.1f);
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    draw_line_rect(x, y, m_combo_length, m_combo_height);

    // Draw the indication box
    glTranslatef(0.0f, 0.0f, 0.1f);
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    int pos_x = x + m_combo_length - 4 - m_box_width;
    int pos_y = y + (m_combo_height - m_box_height) / 2;
    draw_filled_rect(pos_x, pos_y, m_box_width, m_box_height);

    if (m_hover && !m_collapsed) {
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor4fv(hover_color);
        draw_filled_rect(x, y, m_combo_length, m_combo_height);
    }

    // Draw the text in the list box
    if (!m_item_array.empty()) {
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor3f((float)m_fg_color[0] / 255.0f, 
                  (float)m_fg_color[1] / 255.0f, 
                  (float)m_fg_color[2] / 255.0f);
        glRasterPos2i(x, y + widget_font.get_descent());

        widget_font.print(m_item_array[m_current_index].c_str());
    }

    if (m_collapsed) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.1f);

        // Draw the popup item menu
        size_t n = m_item_array.size();

        int height = (int)n * m_combo_height;

        // Draw the background of the popup menu
        glColor4fv(off_color);
        draw_filled_rect(x, y - height, m_combo_length, height);
        
        // Hightlight the current active item
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor4fv(on_color);
        draw_filled_rect(x, y - m_combo_height * (m_current_index + 1), 
                m_combo_length, m_combo_height);
        
        // Draw the outline of the popup menu
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor3f((float)m_fg_color[0] / 255.0f, 
                  (float)m_fg_color[1] / 255.0f, 
                  (float)m_fg_color[2] / 255.0f);
        draw_line_rect(x, y - height, m_combo_length, height);

        // Highlight the hovered item
        if (m_hover_index >= 0 && m_hover_index != m_current_index) {
            glTranslatef(0.0f, 0.0f, -0.05f);
            glColor4fv(hover_color);
            draw_filled_rect(x, y - m_combo_height * (m_hover_index + 1), 
                m_combo_length, m_combo_height);
        }

        // Render the items in the list
        glColor3f((float)m_fg_color[0] / 255.0f, 
                  (float)m_fg_color[1] / 255.0f, 
                  (float)m_fg_color[2] / 255.0f);
        for (size_t i = 0; i < n; i++) {
            glTranslatef(0.0f, 0.0f, 0.1f);
            y -= m_combo_height;
            glRasterPos2i(x, y + widget_font.get_descent());
            widget_font.print("%s", m_item_array[i].c_str());
        }
        

        glPopMatrix();
    }

    REPORT_GLERROR();

    glPopMatrix();
}

void Widget_Listbox::set_current_index(int index)
{
    m_current_index = index;
}

void Widget_Listbox::add_item(const char* item)
{
    m_item_array.push_back(std::string(" ") + std::string(item) + std::string(" "));

    int str_len = widget_font.get_string_width(m_item_array.back().c_str()) + m_box_width;
    m_combo_length = std::max(m_combo_length, str_len);
    m_w_hint = widget_font.get_string_width(m_label) + widget_font.get_width() + 
        m_combo_length;
}

void Widget_Listbox::on_mouse_over(int x, int y)
{
    int xx, yy;
    align(m_x, m_y, &xx, &yy);

    xx += w_hint() - m_combo_length;

    if (m_collapsed) {
        // See if which item is being touched by the mouse
        size_t n = m_item_array.size();
        int menu_height = m_combo_height * (int)n;

        int combo_x = xx;
        int combo_y = yy - menu_height;

        if (x >= combo_x && x <= combo_x + m_combo_length &&
            y >= combo_y && y <= combo_y + menu_height) {
            m_hover_index = (int)n - 1 - (y - combo_y) / m_combo_height;
        } else {
            m_hover_index = -1;
            m_hover =  false;
        }
    } else {
        // Is mouse pointer inside the combo?
        if (x >= xx && x <= xx + m_combo_length &&
            y >= yy && y <= yy + m_combo_height) {
            m_hover = true;
        } else {
            m_hover = false;
        }
    }
    // Redraw myself
    update(xx, yy, m_w, m_h);
}

bool Widget_Listbox::on_mouse_button_down(int x, int y, int button)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (!m_collapsed && !m_hover)
            return false;
        else if (!m_collapsed && m_hover) {
            m_collapsed = true;
        } else if (m_collapsed && m_hover_index >= 0) {
            m_collapsed = false;
            
            if (m_current_index != m_hover_index) {
                m_current_index = m_hover_index;
                if (m_callback)
                    m_callback(&m_current_index);
                if (m_live_value)
                    *((int*)m_live_value) = m_current_index;
            }
        } else if (m_collapsed && !m_hover)
            m_collapsed = false;

        return true;
    }

    return false;
}

// -------------------------------------------------------------- 
// Class Widget_Separator
// -------------------------------------------------------------- 
Widget_Separator::Widget_Separator(int height, Widget* parent)
    : Widget(parent)
{
    m_w_hint = 1;
    m_h_hint = std::max(height, 1);
}

void Widget_Separator::draw()
{
    int x, y;
    align(m_x, m_y, &x, &y);

    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    
    glBegin(GL_LINES);
        glVertex2i(m_x, m_y + m_h / 2);
        glVertex2i(m_x + m_w, m_y + m_h / 2);
    glEnd();
}

// -------------------------------------------------------------- 
// Class Widget_Group
// -------------------------------------------------------------- 
Widget_Group::Widget_Group(const char* label, Widget* parent)
    : Widget(parent)
{
    strcpy(m_label, label);

    m_layout = 0;

    m_label_width  = widget_font.get_string_width(m_label);
    m_label_left_margin = widget_font.get_string_width("xx");
    m_label_right_margin = widget_font.get_string_width("xxx");
}

Widget_Group::~Widget_Group()
{
    delete m_layout;
}

void Widget_Group::set_layout(Widget_Layout* layout)
{
    m_layout = layout;
}

int Widget_Group::w_hint() const
{
    int w_hint = m_label_width;

    if (m_layout) 
        w_hint = std::max(w_hint, m_layout->w_hint());

    w_hint += m_label_right_margin + m_label_right_margin;

    return w_hint;
}

int Widget_Group::h_hint() const
{
    int h_hint = 0;

    h_hint = widget_font.get_height();
    
    if (m_layout)
        h_hint += m_layout->h_hint() +
            2 * m_layout->get_hspacing();
    else
        h_hint += widget_font.get_height();

    return h_hint;
}

void Widget_Group::set_size(int x, int y, int w, int h)
{
    Widget::set_size(x, y, w, h);

    if (m_layout) {
        x += m_label_left_margin;
        y += m_layout->get_hspacing();
        w -= m_label_left_margin + m_label_right_margin;
        h -= widget_font.get_height() + 2 * m_layout->get_hspacing();

        m_layout->set_size(x, y, w, h);
    }
}

void Widget_Group::draw()
{
    //Widget::draw();

    int x, y;
    align(m_x, m_y, &x, &y);

    // Draw the bounding box
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    int w = m_w;
    int h = m_h;
    int ascent = widget_font.get_ascent(); 
    int lm = m_label_left_margin;
    int rm = m_label_right_margin;
    glBegin(GL_LINE_STRIP);
        glVertex2i(x + lm, y + h - ascent);
        glVertex2i(x, y + h - ascent);
        glVertex2i(x, y);
        glVertex2i(x + w, y);
        glVertex2i(x + w, y + h - ascent);
        glVertex2i(x + m_label_width + lm, y + h - ascent);
    glEnd();

    // Draw label
    glRasterPos2i(x + lm, y + h - ascent);   
    widget_font.print("%s", m_label);
    
    if (m_layout)
        m_layout->draw();
}

void Widget_Group::set_fg_color(int r, int g, int b)
{
    Widget::set_fg_color(r, g, b);
    if (m_layout)
        m_layout->set_fg_color(r, g, b);
}

void Widget_Group::set_bg_color(int r, int g, int b)
{
    Widget::set_bg_color(r, g, b);
    if (m_layout)
        m_layout->set_bg_color(r, g, b);
}

void Widget_Group::on_mouse_over(int x, int y)
{
    if (m_layout)
        m_layout->on_mouse_over(x, y);
}

bool Widget_Group::on_mouse_button_down(int x, int y, int button)
{
    if (m_layout)
        return m_layout->on_mouse_button_down(x, y, button);

    return false;
}

bool Widget_Group::on_mouse_button_up(int x, int y, int button)
{
    if (m_layout)
        return m_layout->on_mouse_button_up(x, y, button);
    
    return false;
}

bool Widget_Group::on_mouse_motion(int x, int y)
{
    if (m_layout)
        return m_layout->on_mouse_motion(x, y);

    return false;
}

// -------------------------------------------------------------- 
// Class Widget_Radio
// -------------------------------------------------------------- 
Widget_Radio::Widget_Radio(const char* label, int index, Widget_RadioGroup* parent)
    : Widget(parent)
{
    strcpy(m_label, label);

    m_toggled = false;

    m_index = index;

    m_disksize = widget_font.get_height() - 
        widget_font.get_descent() - 
        widget_font.get_internal_leading() + 4;

    m_w_hint = widget_font.get_string_width(m_label) + 
        m_disksize + widget_font.get_width();
    m_h_hint = std::max(widget_font.get_height(), m_disksize);

    m_w = m_w_hint;
    m_h = m_h_hint;
}

void Widget_Radio::draw()
{
    glPushMatrix();

    int x, y;
    align(m_x, m_y, &x, &y);
    
    glColor4f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f,
              1.0f);
    draw_filled_disk(x, y, m_disksize);
    
    glTranslatef(0.0f, 0.0f, 0.1f);
    if (m_toggled)
        glColor4fv(on_color);
    else
        glColor4fv(off_color);
    draw_filled_disk(x + 2, y + 2, m_disksize - 4);

    if (m_hover) {
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor4fv(hover_color);
        draw_filled_disk(x, y, m_disksize);
    }

    y += widget_font.get_descent();

    // Draw the label text later
    glColor4f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f,
              1.0f);
    glRasterPos2i(x + m_disksize + widget_font.get_width(), y);
    widget_font.print("%s", m_label);

    REPORT_GLERROR();
    
    glPopMatrix();
}

bool Widget_Radio::on_mouse_button_up(int x, int y, int button)
{
    if (button != GLUT_LEFT_BUTTON) 
        return false;

    int xx, yy;
    align(m_x, m_y, &xx, &yy);

    if (x >= xx && x <= xx + m_disksize &&
        y >= yy && y <= yy + m_disksize) {
        m_toggled = true;

        // Expire other radios within the same 
        // group
        Widget_RadioGroup* parent = 
            dynamic_cast<Widget_RadioGroup*>(m_parent);
        parent->toggle(m_index);

        update(0, 0, -1, -1);

        return true;
    } 

    return false;
}

void Widget_Radio::toggle(bool toggled)
{
    m_toggled = toggled;
}


// -------------------------------------------------------------- 
// Class Widget_RadioGroup
// -------------------------------------------------------------- 
Widget_RadioGroup::Widget_RadioGroup(const char* label, int layout,
        Widget* parent)
    : Widget_Group(label, parent)
{
    if (layout == GGL_WIDGET_HORIZONTAL)
        m_layout = new Widget_HLayout;
    else if (layout == GGL_WIDGET_VERTICAL)
        m_layout = new Widget_VLayout;
    else {
        report_error(__FUNCTION__, "Bad parameters");
        m_layout = 0;
    }

    m_value = -1;
}

void Widget_RadioGroup::add_radio(const char* label)
{
    int index = (int)m_layout->num_of_widgets();
    Widget_Radio* radio = new Widget_Radio(label, index, this);
    m_layout->add_widget(radio);
    // The layout will reset the parent of the radio
    radio->set_parent(this);
}

void Widget_RadioGroup::on_mouse_over(int x, int y)
{
    // If mouse is inside the group region,
    // highlight the radios
    int xx, yy;
    align(m_x, m_y, &xx, &yy);
    
    bool old_hover = m_hover;

    if (x >= xx && x <= xx + w_hint() &&
        y >= yy && y <= yy + h_hint()) {
        m_hover = true;
    } else {
        m_hover = false;
    }

    size_t n = m_layout->num_of_widgets();
    for (size_t i = 0; i < n; i++) {
        m_layout->widget(i)->set_hover(m_hover);
    }

    if (old_hover != m_hover)
        update(0, 0, -1, -1);
}

bool Widget_RadioGroup::on_mouse_button_up(int x, int y, int button)
{
    int xx, yy;
    align(m_x, m_y, &xx, &yy);
    
    if (x >= xx && x <= xx + w_hint() &&
        y >= yy && y <= yy + h_hint()) {
        return m_layout->on_mouse_button_up(x, y, button);
    }

    return false;
}

void Widget_RadioGroup::toggle(int index)
{
    if (index != m_value) {
        Widget_Radio* radio;  
        radio = dynamic_cast<Widget_Radio*>(m_layout->widget(index));
        radio->toggle(true);
        if (m_value >= 0) {
            radio = dynamic_cast<Widget_Radio*>(m_layout->widget(m_value));
            radio->toggle(false);
        }
    
        m_value = index;

        if (m_callback)
            m_callback(&m_value);
        if (m_live_value)
            *((int*)m_live_value) = m_value;
    }
}

// -------------------------------------------------------------- 
// Class Widget_HSlider
// -------------------------------------------------------------- 
Widget_HSlider::Widget_HSlider(const char* label, int width, Widget* parent)
    : Widget(parent)
{

    strcpy(m_label, label);

    m_slider_width = width;
    
    m_label_width =  widget_font.get_string_width(m_label) + 
        widget_font.get_width() * 2;

    m_w_hint = m_label_width + m_slider_width + widget_font.get_string_width("9999");
    m_h_hint = widget_font.get_height();

    m_indicator_height =  widget_font.get_height() - 
        widget_font.get_descent() - widget_font.get_internal_leading();

    m_w = m_w_hint;
    m_h = m_h_hint;

}

void Widget_HSlider::draw()
{
    glPushMatrix();

    int x, y;
    align(m_x, m_y, &x, &y);
    
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    glRasterPos2i(x, y + widget_font.get_descent());
    widget_font.print("%s", m_label);

    y += m_h_hint / 2;

    int x1 = x + m_label_width; 
    int x2 = x + m_label_width + m_slider_width;

    glBegin(GL_LINES);
    glVertex2i(x1, y);
    glVertex2i(x2, y);
    glEnd();

    glTranslatef(0.0f, 0.0f, 0.1f);
    glBegin(GL_LINES);
    glVertex2i(x1, y - 2);
    glVertex2i(x1, y + 2);
    
    glVertex2i(x2, y - 2);
    glVertex2i(x2, y + 2);
    glEnd();


    // Draw the indicator
    glTranslatef(0.0f, 0.0f, 0.1f);
    glColor4fv(static_color);
    draw_filled_rect(m_indicator_x - indicator_width / 2, 
                     y - m_indicator_height / 2,
                     indicator_width, m_indicator_height);

    glTranslatef(0.0f, 0.0f, 0.1f);
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    draw_line_rect(m_indicator_x - indicator_width / 2, 
                   y - m_indicator_height / 2,
                   indicator_width, m_indicator_height);

    if (m_hover) {
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor4fv(hover_color);
        draw_filled_rect(m_indicator_x - indicator_width / 2, 
                         y - m_indicator_height / 2,
                         indicator_width, m_indicator_height);
    }

    glPopMatrix();
}

void Widget_HSlider::on_mouse_over(int x, int y)
{
    int xx, yy;
    align(m_x, m_y, &xx, &yy);

    yy += m_h_hint / 2;

    if (x >= m_indicator_x - indicator_width / 2 && 
        x <= m_indicator_x + indicator_width / 2 &&
        y >= yy - m_indicator_height / 2 && 
        y <= yy + m_indicator_height / 2) {

        m_hover = true;
    } else {
        m_hover = false;
    }
}

bool Widget_HSlider::on_mouse_button_down(int x, int y, int button)
{
    if (button != GLUT_LEFT_BUTTON)
        return false;

    if (m_hover) {

        m_last_x = x;
        m_last_y = y;

        m_clicked = true;

        return true;
    }

    return false;
}

bool Widget_HSlider::on_mouse_button_up(int x, int y, int button)
{
    if (m_clicked) {
        m_clicked = false;
        return true;
    }

    return false;
}

bool Widget_HSlider::on_mouse_motion(int x, int y)
{
    if (!m_clicked) 
        return false;
    
    int dx = x - m_last_x;
    
    m_indicator_x += dx;
    
    int xx, yy;
    align(m_x, m_y, &xx, &yy);
    int x1 = xx + m_label_width; 
    int x2 = xx + m_label_width + m_slider_width;

    if (m_indicator_x < x1)
        m_indicator_x = x1;
    if (m_indicator_x > x2)
        m_indicator_x = x2;

    m_last_x = x;
    m_last_y = y;

    return true;
}

// -------------------------------------------------------------- 
// Class Widget_Float_HSlider
// -------------------------------------------------------------- 
Widget_Float_HSlider::Widget_Float_HSlider(const char* label, int width,
        Widget* parent)
    : Widget_HSlider(label, width, parent)
{
    m_limits[0] = 0.0f;
    m_limits[1] = 1.0f;

    m_value = 0.0f;
}

Widget_Float_HSlider::Widget_Float_HSlider(const char* label, float limits[],
        int width, Widget* parent)
    : Widget_HSlider(label, width, parent)
{
    m_limits[0] = limits[0];
    m_limits[1] = limits[1];

    m_value = 0.0f;
}

void Widget_Float_HSlider::draw()
{
    // Set the indicator position at the first draw 
    static bool inited = false;
    if (!inited) {
        set_value(m_value);
        inited = true;
    }

    Widget_HSlider::draw();

    int x, y;

    align(m_x, m_y, &x, &y);
    
    x = x + m_label_width + m_slider_width + widget_font.get_width();
    
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    glRasterPos2i(x, y + widget_font.get_descent());

    char buffer[1024];
    sprintf(buffer, "%4.2f", m_value);
    widget_font.print("%s", buffer);
}

void Widget_Float_HSlider::set_limits(float min, float max)
{
    if (fabs(min - max) < 1e-5) {
        report_error(__FUNCTION__, "bad parameters\n");
    } else if (min < max) {
        m_limits[0] = min;
        m_limits[1] = max;
    } else if (max > min) {
        m_limits[1] = min;
        m_limits[0] = max;
    } else {
        report_error(__FUNCTION__, "bad parameters\n");
    }
}

void Widget_Float_HSlider::set_value(float value)
{
    m_value = value;
    
    if (m_value < m_limits[0]) 
        m_value = m_limits[0];
    if (m_value > m_limits[1])
        m_value = m_limits[1];

    int x, y;
    align(m_x, m_y, &x, &y);
    int x1 = x + m_label_width;

    float ratio = (m_value - m_limits[0]) / (m_limits[1] - m_limits[0]);
    m_value = ratio * (m_limits[1] - m_limits[0]) + m_limits[0];

    // Update the indicator position
    m_indicator_x = x1 + (int)(ratio * (float)m_slider_width);
}

void Widget_Float_HSlider::set_size(int x, int y, int w, int h)
{
    Widget::set_size(x, y, w, h);
    set_value(m_value);
}

bool Widget_Float_HSlider::on_mouse_motion(int x, int y)
{
    if (!m_clicked) 
        return false;

    Widget_HSlider::on_mouse_motion(x, y);

    int xx, yy;
    align(m_x, m_y, &xx, &yy);
    int slider_left = xx + m_label_width;

    float ratio = (float)(m_indicator_x - slider_left) / (float)m_slider_width;
    float v = ratio * (m_limits[1] - m_limits[0]) + m_limits[0];

    float diff = fabs(v - m_value);
    m_value = v;
    if (diff > 1e-5 && m_callback)
        m_callback(&m_value);
    if (diff > 1e-5 && m_live_value)
        *((float*)(m_live_value)) = m_value;


    update(0, 0, -1, -1);

    return true;
}

// -------------------------------------------------------------- 
// Class Widget_Int_HSlider
// -------------------------------------------------------------- 
Widget_Int_HSlider::Widget_Int_HSlider(const char* label, int width,
        Widget* parent)
    : Widget_HSlider(label, width, parent)
{
    m_limits[0] = 0;
    m_limits[1] = 10;
    m_value = 0;
}

Widget_Int_HSlider::Widget_Int_HSlider(const char* label, int limits[],
                                       int width, Widget* parent)
    : Widget_HSlider(label, width, parent)
{
    m_limits[0] = limits[0];
    m_limits[1] = limits[1];
    m_value = 0;
}


void Widget_Int_HSlider::draw()
{
    // Set the indicator position at the first draw 
    static bool inited = false;
    if (!inited) {
        set_value(m_value);
        inited = true;
    }

    Widget_HSlider::draw();

    int x, y;
    align(m_x, m_y, &x, &y);
    
    x = x + m_label_width + m_slider_width + widget_font.get_width();
    
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    glRasterPos2i(x, y + widget_font.get_descent());

    char buffer[1024];
    sprintf(buffer, "%4d", m_value);
    widget_font.print("%s", buffer);
}

void Widget_Int_HSlider::set_limits(int min, int max)
{
    if (min < max) {
        m_limits[0] = min;
        m_limits[1] = max;
    } else if (min > max) {
        m_limits[1] = min;
        m_limits[0] = max;
    } else {
        report_error(__FUNCTION__, "bad parameters\n");
    }
}

void Widget_Int_HSlider::set_value(int value)
{
    m_value = value;
    if (m_value < m_limits[0]) 
        m_value = m_limits[0];
    if (m_value > m_limits[1])
        m_value = m_limits[1];

    int x, y;
    align(m_x, m_y, &x, &y);
    int x1 = x + m_label_width;

    float ratio = (float)(m_value - m_limits[0]) / 
        (float)(m_limits[1] - m_limits[0]);
    float v = ratio * (float)(m_limits[1] - m_limits[0]) + 
        (float)m_limits[0];
    m_value = (int)(v);
    
    // Update the indicator position
    m_indicator_x = x1 + (int)(ratio * (float)m_slider_width);
}

bool Widget_Int_HSlider::on_mouse_motion(int x, int y)
{
    if (!m_clicked) 
        return false;

    Widget_HSlider::on_mouse_motion(x, y);

    int xx, yy;
    align(m_x, m_y, &xx, &yy);
    int slider_left = xx + m_label_width;

    float ratio = (float)(m_indicator_x - slider_left) / (float)m_slider_width;
    int v = (int)(ratio * (float)(m_limits[1] - m_limits[0])) + m_limits[0];

    int diff = abs(v - m_value);
    m_value = v;
    if (diff >= 1 && m_callback)
        m_callback(&m_value);
    if (diff >= 1 && m_live_value)
        *((int*)(m_live_value)) = m_value;

    update(0, 0, -1, -1);

    return true;
}

void Widget_Int_HSlider::set_size(int x, int y, int w, int h)
{
    Widget::set_size(x, y, w, h);
    set_value(m_value);
}

// -------------------------------------------------------------- 
// Class Widget_Layout
// -------------------------------------------------------------- 
Widget_Layout::Widget_Layout(Widget* parent)
    : Widget(parent)
{
    //m_visible = true;

    m_visible_border = false;

    m_vspacing = 5;
    m_hspacing = 5;
}

Widget_Layout::~Widget_Layout()
{
    size_t n = m_widgets.size();
    
    for (size_t i = 0; i < n; i++)
        if (m_widgets[i]) 
            delete m_widgets[i];
}

void Widget_Layout::add_widget(Widget* widget)
{
    if (widget == 0)
        return ;

    m_widgets.push_back(widget);

    widget->set_parent(this);

    widget->set_fg_color(m_fg_color[0], m_fg_color[1], m_fg_color[2]);
    widget->set_bg_color(m_bg_color[0], m_bg_color[1], m_bg_color[2]);
}

void Widget_Layout::draw()
{
    //
    //Widget::draw();

    // Draw the widgets
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] == 0) 
            continue;
        if (m_widgets[i]->visible())
            m_widgets[i]->draw();
    }
    
    // Draw the layout border
    if (m_visible_border) {
        glColor3f((float)m_fg_color[0] / 255.0f, 
                  (float)m_fg_color[1] / 255.0f, 
                  (float)m_fg_color[2] / 255.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(m_x, m_y);
        glVertex2i(m_x + m_w, m_y);
        glVertex2i(m_x + m_w, m_y + m_h);
        glVertex2i(m_x, m_y + m_h);
        glEnd();
    }
}

void Widget_Layout::set_fg_color(int r, int g, int b)
{
    // Set my foreground color
    Widget::set_fg_color(r, g, b);

    // Set the foreground color of belonging widgets
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] == 0) 
            continue;
        m_widgets[i]->set_fg_color(r, g, b);
    }
}

void Widget_Layout::set_bg_color(int r, int g, int b)
{
    // Set my background color
    Widget::set_bg_color(r, g, b);

    // Set the background color of belonging widgets
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] == 0) 
            continue;
        m_widgets[i]->set_bg_color(r, g, b);
    }
}
    
void Widget_Layout::on_mouse_over(int x, int y)
{
    // Inside or outside
    //if (x < m_x || x > m_x + m_w && y < m_y && y > m_y + m_h) 
    //    return ;

    // Distribute the event to all belong widgets
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] &&
            m_widgets[i]->enabled() && 
            m_widgets[i]->visible()) 
            m_widgets[i]->on_mouse_over(x, y);
    }
}

bool Widget_Layout::on_mouse_button_down(int x, int y, int button)
{
    // Inside or outside
    //if (x < m_x || x > m_x + m_w && y < m_y && y > m_y + m_h) 
    //    return ;

    // Distribute the event to all belong widgets
    size_t n = m_widgets.size();
    int ret = 0;
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] &&
            m_widgets[i]->enabled() && 
            m_widgets[i]->visible()) 
            ret += (int)m_widgets[i]->on_mouse_button_down(x, y, button);
    }
    
    return ret > 0;
}

bool Widget_Layout::on_mouse_button_up(int x, int y, int button)
{
    size_t n = m_widgets.size();
    // Inside or outside
    /*
    if (x < m_x || x > m_x + m_w && y < m_y && y > m_y + m_h) {
        for (size_t i = 0; i < n; i++) {
            if (m_widgets[i] &&
                m_widgets[i]->enabled() && 
                m_widgets[i]->visible()) 
            m_widgets[i]->set_hover(false);
        }
        return ;
    }
    */

    // Distribute the event to all belong widgets
    int ret = 0;
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] &&
            m_widgets[i]->enabled() && 
            m_widgets[i]->visible()) 
            ret += (int)m_widgets[i]->on_mouse_button_up(x, y, button);
    }
    
    return ret > 0;
}

bool Widget_Layout::on_mouse_motion(int x, int y)
{
    // Inside or outside
    //if (x < m_x || x > m_x + m_w && y < m_y && y > m_y + m_h) 
    //    return ;

    // Distribute the event to all belong widgets
    int ret = 0;
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] &&
            m_widgets[i]->enabled() && 
            m_widgets[i]->visible()) 
            ret += (int)m_widgets[i]->on_mouse_motion(x, y);
    }

    return ret > 0;
}

void Widget_Layout::set_hover(bool status)
{
    Widget::set_hover(status);

    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] &&
            m_widgets[i]->enabled() && 
            m_widgets[i]->visible()) 
            m_widgets[i]->set_hover(status);
    }
}

void Widget_Layout::set_clicked(bool status)
{
    Widget::set_clicked(status);

    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) {
        if (m_widgets[i] &&
            m_widgets[i]->enabled() && 
            m_widgets[i]->visible()) 
            m_widgets[i]->set_clicked(status);
    }
}

void Widget_Layout::set_vspacing(int spacing)
{
    m_vspacing = spacing;
}

void Widget_Layout::set_hspacing(int spacing)
{
    m_hspacing = spacing;
}

// -------------------------------------------------------------- 
// Class Widget_HLayout
// -------------------------------------------------------------- 
Widget_HLayout::Widget_HLayout(Widget* parent)
    : Widget_Layout(parent)
{
}

Widget_HLayout::~Widget_HLayout()
{
}

int Widget_HLayout::w_hint() const
{
    int w_hint = 0;
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) 
        w_hint += m_widgets[i]->w_hint();
    
    int nn = (int)n;
    return w_hint + m_vspacing * (nn - 1 >= 0? nn - 1 : 0);
}

int Widget_HLayout::h_hint() const
{
    int h_hint = 0;
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) 
        h_hint = std::max(m_widgets[i]->h_hint(), h_hint);

    return h_hint;
}

void Widget_HLayout::set_size(int x, int y, int w, int h)
{
    Widget::set_size(x, y, w, h);

    size_t n = m_widgets.size();
    if (n <= 0)
        return ;

    // Adjust the position of each widget
    int space = m_w - m_w_hint;   
    space /= (int)n;

    int widget_x = m_x;
    int widget_y = m_y;
    for (size_t i = 0; i < n; i++) {
        int widget_h = m_h;
        int widget_w = m_widgets[i]->w_hint() + space;

        m_widgets[i]->set_size(
                widget_x, widget_y, 
                widget_w, widget_h);
        
        widget_x += widget_w + m_vspacing;
    }
}

// -------------------------------------------------------------- 
// Class Widget_VLayout
// -------------------------------------------------------------- 
Widget_VLayout::Widget_VLayout(Widget* parent)
    : Widget_Layout(parent)
{
}

Widget_VLayout::~Widget_VLayout()
{
}

int Widget_VLayout::h_hint() const
{
    int h_hint = 0;
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) 
        h_hint += m_widgets[i]->h_hint();

    int nn = (int)n;
    return h_hint + m_hspacing * (nn - 1 >= 0? nn - 1 : 0);
}

int Widget_VLayout::w_hint() const
{
    int w_hint = 0;
    size_t n = m_widgets.size();
    for (size_t i = 0; i < n; i++) 
        w_hint = std::max(m_widgets[i]->w_hint(), w_hint);

    return w_hint;
}

void Widget_VLayout::set_size(int x, int y, int w, int h)
{
    Widget::set_size(x, y, w, h);

    size_t n = m_widgets.size();
    if (n == 0)
        return ;

    // Update the position of each widget
    int space = m_h - m_h_hint;   
    space /= (int)n;

    int widget_x = m_x;
    int widget_y = m_y;
    for (int i = (int)n - 1; i >= 0; i--) {
        int widget_h = m_widgets[i]->h_hint() + space;
        int widget_w = m_w;

        m_widgets[i]->set_size(
                widget_x, widget_y, 
                widget_w, widget_h);
        
        widget_y += widget_h + m_hspacing;
    }
}

// -------------------------------------------------------------- 
// Class Widget_Button
// -------------------------------------------------------------- 
Widget_Button::Widget_Button(const char* label, Widget* parent)
    : Widget(parent)
{
    strcpy(m_label, label);
    
    m_w_hint = widget_font.get_string_width(label) + 
        widget_font.get_width() * 2;
    m_h_hint = widget_font.get_height();

    m_w = m_w_hint;
    m_h = m_h_hint;

    m_hover = false;
    m_clicked = false;
}

void Widget_Button::draw()
{
#define DOWN_COLOR   0.1f, 0.40f, 0.60f, 1.0f

    glPushMatrix();

    int x, y;
    align(m_x, m_y, &x, &y);
    
    // Draw the button background
    if (m_clicked)
        glColor4f(DOWN_COLOR);
    else 
        glColor4fv(static_color);
    draw_filled_rect(x, y, m_w_hint, m_h_hint);

    if (m_hover && !m_clicked) {
        glTranslatef(0.0f, 0.0f, 0.1f);
        glColor4fv(hover_color);
        draw_filled_rect(x, y, m_w_hint, m_h_hint);
    }

    glTranslatef(0.0f, 0.0f, 0.1f);
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    draw_line_rect(x, y, m_w_hint, m_h_hint);
    
    // Draw the label
    glTranslatef(0.0f, 0.0f, 0.1f);
    x += widget_font.get_width();
    y += widget_font.get_descent();
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);

    glRasterPos2i(x, y);
    widget_font.print("%s", m_label);

    glPopMatrix();

#undef DOWN_COLOR
}

void Widget_Button::on_mouse_over(int x, int y)
{
    int xx, yy;
    align(m_x, m_y, &xx, &yy);

    if (x >= xx && x <= xx + m_w_hint &&
        y >= yy && y <= yy + m_h_hint) {
        m_hover = true;
    } else {
        m_hover = false;
    }

    // Redraw myself
    update(0, 0, -1, -1);
}

bool Widget_Button::on_mouse_button_down(int x, int y, int button)
{
    if (button == GLUT_LEFT_BUTTON && m_hover) {
        m_clicked = true;
        // Redraw myself
        update(0, 0, -1, -1);

        return true;
    }

    return false;
}

bool Widget_Button::on_mouse_button_up(int x, int y, int button)
{
    m_clicked = false;

    if (button == GLUT_LEFT_BUTTON) {
        int xx, yy;
        align(m_x, m_y, &xx, &yy);

        xx += widget_font.get_width();

        if (x >= xx && x <= xx + m_w_hint &&
            y >= yy && y <= yy + m_h_hint) {
            if (m_callback)
                m_callback(0);

            return true;
        } 
    }

    return false;
}

// -------------------------------------------------------------- 
// Class Widget_Arcball
// -------------------------------------------------------------- 
Widget_Arcball::Widget_Arcball(const char* label, Widget* parent)
    : Widget(parent)
{
    strcpy(m_label, label);

    m_ball_size = 56;
    m_spacing = 4;
    
    int label_w_hint = widget_font.get_string_width(label) + 
        widget_font.get_width() * 2;
    int label_h_hint = widget_font.get_height() + m_spacing;

    m_w_hint = std::max(label_w_hint, m_ball_size);
    m_h_hint = label_h_hint + m_ball_size + m_spacing;

    m_w = m_w_hint;
    m_h = m_h_hint;

    m_hover = false;
    m_clicked = false;
}

void Widget_Arcball::draw()
{
    static GLUquadric* qobj = 0;
    static bool inited = false;
    static gGL::Texture2D tex;
    
    if (!inited) {
        qobj = gluNewQuadric();

        int tex_size = 256;
        int blk_size = 32;
        unsigned char* texdata = new unsigned char [tex_size * tex_size * 3];
        memset(texdata, 255, tex_size * tex_size * 3);

        int index = 0; 
        for (int i = 0; i < tex_size / blk_size; i++) {
            for (int j = 0; j < tex_size / blk_size; j++) {
                if (index % 2 == 0)
                    for (int y = 0; y < blk_size; y++)
                    for (int x = 0; x < blk_size; x++) {
                        int yy = (i * blk_size + y);
                        int xx = (j * blk_size + x);

                        int idx = yy * tex_size + xx; 

                        //texdata[idx * 3 + 0] = 33;
                        //texdata[idx * 3 + 1] = 147;
                        //texdata[idx * 3 + 2] = 203;
                        texdata[idx * 3 + 0] = 120;
                        texdata[idx * 3 + 1] = 120;
                        texdata[idx * 3 + 2] = 120;
                    }

                index++;
             }
            index++;
        }

        tex.bind();
        tex.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        tex.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        tex.parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
        tex.parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
        tex.create(0, GL_RGB, tex_size, tex_size, 0, GL_RGB, texdata);
        tex.unbind();

        delete [] texdata;

        inited = true;
    }
    
    gluQuadricTexture(qobj,GL_TRUE);
    
    glPushMatrix();
    
    // Draw the label
    int x, y;
    align(m_x, m_y, &x, &y);
    
    glColor3f((float)m_fg_color[0] / 255.0f, 
              (float)m_fg_color[1] / 255.0f, 
              (float)m_fg_color[2] / 255.0f);
    glRasterPos2i(x + widget_font.get_width(), y + widget_font.get_descent());
    widget_font.print("%s", m_label);

    // Draw the ball
    int c_x = x + m_w_hint / 2;
    int c_y = y + widget_font.get_height() + m_spacing + m_ball_size / 2;

    glTranslatef((float)c_x, (float)c_y, 0.0f);
    
    // Draw the silhouette
    glPushAttrib(GL_ENABLE_BIT | GL_HINT_BIT);
    glEnable(GL_BLEND);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glColor3f(0, 0, 0);
    gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
    gluDisk(qobj, 0, m_ball_size / 2 + 2, 32, 2);
    gluQuadricDrawStyle(qobj, GLU_FILL);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
    glPopAttrib();

    // Draw the ball
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    tex.bind();
    glMultMatrixf(m_arcball.rotation4x4());
    gluSphere(qobj, m_ball_size / 2, 50, 50);
    tex.unbind();
    glDisable(GL_TEXTURE_2D); 


    glPopMatrix();    
}
    
void Widget_Arcball::on_mouse_over(int x, int y)
{
}

bool Widget_Arcball::on_mouse_button_up(int x, int y, int button)
{
    if (m_clicked) {
        m_clicked = false;
        return true;
    }
    return false;
}

bool Widget_Arcball::on_mouse_button_down(int x, int y, int button)
{
    if (button == GLUT_LEFT_BUTTON) {
        int xx, yy;
        align(m_x, m_y, &xx, &yy);

        int font_height = widget_font.get_height();
        int viewport[] = {
            xx + m_w_hint / 2 - m_ball_size / 2,
            yy + font_height + m_spacing, 
            m_ball_size,
            m_ball_size 
        };
        
        if (x >= viewport[0] && x <= viewport[0] + viewport[2] &&
            y >= viewport[1] && y <= viewport[1] + viewport[3]) {

            m_clicked = true;

            m_arcball.set_viewport(viewport);
            m_arcball.begin(x, y);


            return true;
        }
    }

    return false;
}

bool Widget_Arcball::on_mouse_motion(int x, int y)
{
    if (m_clicked) {
        int viewport[4];
        m_arcball.get_viewport(viewport);

        //if (x >= viewport[0] && x <= viewport[0] + viewport[2] &&
        //    y >= viewport[1] && y <= viewport[1] + viewport[3]) {
            m_arcball.move(x, y);
            update(0, 0, -1, -1);

            if (m_callback)
                m_callback(m_arcball.rotation4x4());
            if (m_live_value)
                memcpy(m_live_value, m_arcball.rotation4x4(), sizeof(float) * 16);
        //}
        return true;
    }
    return false;
}

void Widget_Arcball::set_rotation_matrix(float mat[])
{
    m_arcball.set_rotation4x4(&mat[0]);
}

// -------------------------------------------------------------- 
// Class Widget_Master
// -------------------------------------------------------------- 
Widget_Master::Widget_Master(int w, int h)
    : Widget(0)
{
    m_vmargin = 10;
    m_hmargin = 10;

    m_alignment  = GGL_WIDGET_ALIGN_LEFT;
    m_valignment = GGL_WIDGET_ALIGN_TOP;

    m_w_hint = w;
    m_h_hint = h;

    m_w = m_w_hint;
    m_h = m_h_hint;
    m_x = m_hmargin; 
    m_y = m_vmargin;

    m_buttons = 0;

    m_layout = 0;

    m_inited = false;
}

bool Widget_Master::init()
{
    // Init font
    if (widget_font.create("Arial", 
                            true, 
                            -12, 
                            true, 
                            false, 
                            false, 
                            false) < 0) {
        report_error(__FUNCTION__, "Can't init font");
        return false;
    }

    m_inited = true;

    return true;
}

Widget_Master::~Widget_Master()
{
    if (m_layout)
        delete m_layout;
}

void Widget_Master::set_layout(Widget_Layout* layout)
{
    m_layout = layout;

    // Update the foreground and background color
    // of widget
    if (m_layout) {
        m_layout->set_bg_color(m_bg_color[0], m_bg_color[1], m_bg_color[2]);
        m_layout->set_fg_color(m_fg_color[0], m_fg_color[1], m_fg_color[2]);
    }
}

void Widget_Master::set_alignment(int align)
{
    m_alignment = align;
        
    Widget::align(m_hmargin, m_vmargin, &m_x, &m_y);

    if (m_layout) 
        m_layout->set_size(m_x, m_y, m_w_hint, m_h_hint);
}

void Widget_Master::set_valignment(int align)
{
    m_valignment = align;
        
    Widget::align(m_hmargin, m_vmargin, &m_x, &m_y);

    if (m_layout) 
        m_layout->set_size(m_x, m_y, m_w_hint, m_h_hint);
}

void Widget_Master::set_size_hint(int w, int h)
{
    m_w_hint = w;
    m_h_hint = h;

    if (m_layout) {
        int w_hint = m_layout->w_hint();
        int h_hint = m_layout->h_hint();
    
        m_w_hint = std::max(w, w_hint);
        m_h_hint = std::max(h, h_hint);

        Widget::align(m_hmargin, m_vmargin, &m_x, &m_y);
        m_layout->set_size(m_x, m_y, m_w_hint, m_h_hint);
    } 
}

void Widget_Master::draw()
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return ;
    }

    if (!m_visible || !m_enabled)
        return ;

    int win_w = glutGet(GLUT_WINDOW_WIDTH);
    int win_h = glutGet(GLUT_WINDOW_HEIGHT);

    // Setup the viewport and matrices
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, win_w, win_h);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, win_w, 0, win_h, -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glLineWidth(2.0f);

    // Draw the widgets in the layout
    if (m_layout)
        m_layout->draw();

    // Restore the GL context
    glLineWidth(1.0f);

    glPopAttrib();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Widget_Master::reshape(int w, int h) 
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return ;
    }

    m_w = w - m_hmargin * 2;
    m_h = h - m_vmargin * 2;
        
    if (m_layout) {
        m_w_hint = std::max(m_layout->w_hint(), m_w_hint);
        m_h_hint = std::max(m_layout->h_hint(), m_h_hint);

        align(m_hmargin, m_vmargin, &m_x, &m_y);

        m_layout->set_size(m_x, m_y, m_w_hint, m_h_hint);
    }

    m_win_w = w;
    m_win_h = h;
}

void Widget_Master::set_bg_color(int r, int g, int b)
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return ;
    }

    m_fg_color[0] = r;
    m_fg_color[1] = g;
    m_fg_color[2] = b;

    // Update the foreground color of the widgets
    if (m_layout)
        m_layout->set_bg_color(r, g, b);
}

void Widget_Master::set_fg_color(int r, int g, int b)
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return ;
    }

    m_bg_color[0] = r;
    m_bg_color[1] = g;
    m_bg_color[2] = b;

    // Update the background color of widgets
    if (m_layout)
        m_layout->set_fg_color(r, g, b);
}

void Widget_Master::on_mouse_over(int x, int y)
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return ;
    }

    if (!visible() || !enabled())
        return ;

    y = m_win_h - y - 1;

    if (m_layout)
        m_layout->on_mouse_over(x, y);
}

bool Widget_Master::on_mouse_button_down(int x, int y, int button)
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return false;
    }

    if (!visible() || !enabled())
        return false;

    y = m_win_h - y - 1;

    m_buttons |= (1 << button);

    int ret = 0;

    if (m_layout)
        ret += (int)m_layout->on_mouse_button_down(x, y, button);

    return ret > 0;
}

bool Widget_Master::on_mouse_button_up(int x, int y, int button)
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return false;
    }

    if (!visible() || !enabled())
        return false;

    y = m_win_h - y - 1;

    m_buttons &= ~(1 << button);

    int ret = 0;

    if (m_layout)
        ret += (int)m_layout->on_mouse_button_up(x, y, button);

    return ret > 0;
}

bool Widget_Master::on_mouse_motion(int x, int y)
{
    if (!m_inited) {
        report_error(__FUNCTION__, "not initialized. Call init() first\n");
        return false;
    }

    if (!visible() || !enabled())
        return false;

    y = m_win_h - y - 1;

    int ret = 0;

    if (m_layout)
        ret += (int)m_layout->on_mouse_motion(x, y);

    return ret > 0;
}

// -------------------------------------------------------------- 
// Local functions
// -------------------------------------------------------------- 


}; // namespace gGL
