// File: widget.h
// Author: Li Hongwei
// Date: 2008-4-23
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
//

// TODO: Widget_Bitmap
//       Widget_RangeHSlider
//       Widget_Arcball

#ifndef GGL_WIDGET_H_
#define GGL_WIDGET_H_

#include "common.h"

#include <vector>

#include "arcball.h"


namespace gGL {

#define GGL_WIDGET_ALIGN_LEFT    0
#define GGL_WIDGET_ALIGN_RIGHT   1
#define GGL_WIDGET_ALIGN_CENTER  2

#define GGL_WIDGET_ALIGN_TOP     0
#define GGL_WIDGET_ALIGN_BOTTOM  1
#define GGL_WIDGET_ALIGN_MIDDLE  2


#define GGL_WIDGET_DOCKING_LT     0
#define GGL_WIDGET_DOCKING_LB     1
#define GGL_WIDGET_DOCKING_RB     2
#define GGL_WIDGET_DOCKING_RT     3

#define GGL_WIDGET_LEFT     0
#define GGL_WIDGET_RIGHT    1
#define GGL_WIDGET_TOP      2
#define GGL_WIDGET_BOTTOM   3

#define GGL_WIDGET_HORIZONTAL 0
#define GGL_WIDGET_VERTICAL   1



class Widget {
    friend class Widget_Master;

public:
    /* ---------------------------------------------------
     * Widget
     *
     *  Constructor
     * ---------------------------------------------------*/
    Widget(Widget* parent);
    
    /* ---------------------------------------------------
     * Widget
     *
     *  Constructor
     * ---------------------------------------------------*/
    virtual ~Widget();

    /* ---------------------------------------------------
     * parent
     *  return the parent of this widget. Every widget should
     *  have only one parent. When widget's parent is NULL, it
     *  belongs the top level.
     * ---------------------------------------------------*/
    Widget* parent() { return m_parent; };
    void set_parent(Widget* parent);

    /* ---------------------------------------------------
     *  Desired size of the widget (minimum requirements)
     * ---------------------------------------------------*/
    virtual int h_hint() const;
    virtual int w_hint() const;

protected:
    /* ---------------------------------------------------
     *  Actual size of the widget (bounding box's size)
     * ---------------------------------------------------*/
    int h() const { return m_h; };
    int w() const { return m_w; };
    
public:
     /* ---------------------------------------------------
     * set_size 
     *
     *   Adjust the geometry of the widget for accommodating
     *   its children.
     * ---------------------------------------------------*/
    virtual void set_size(int x, int y, int w, int h);

    /* ---------------------------------------------------
     *  alignment adjust
     * ---------------------------------------------------*/
    void set_alignment(int align);
    int alignment() const { return m_alignment; };
    void set_valignment(int valign);
    int valignment() const { return m_valignment; };

    /* ---------------------------------------------------
     * visible/enabled/set_visible/set_enabled
     *  status query / set
     * ---------------------------------------------------*/
    void set_visible(bool visible);
    void set_enabled(bool enabled);
    bool visible() const { return m_visible; };
    bool enabled() const { return m_enabled; };
    
    /* ---------------------------------------------------
     * name
     *  return the member m_name
     * ---------------------------------------------------*/
    const char* label() const { return m_label; };
    
    /* ---------------------------------------------------
     * set_fg_color
     *  set the foreground color
     *
     *  @r, @g, @b: the color ranging from 0 to 255
     * ---------------------------------------------------*/
    virtual void set_fg_color(int r, int g, int b);
    
    /* ---------------------------------------------------
     * set_bg_color
     *  set the background color
     *
     *  @r, @g, @b: the color ranging from 0 to 255
     * ---------------------------------------------------*/
    virtual void set_bg_color(int r, int g, int b);

    /* ---------------------------------------------------
     * draw
     *  draw the widget
     * ---------------------------------------------------*/
    virtual void draw() = 0;

    /* ---------------------------------------------------
     * Mouse events:
     *  the callbacks of mouse events
     *
     *  @x, @y: the position of mouse pointer
     *  @button: the clicked/released button
     *
     *  @return: return true if the widget is manipulated.
     * ---------------------------------------------------*/
    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_motion(int x, int y);

    /* ---------------------------------------------------
     * align
     *  With the specified alignment, it computes the the 
     *  starting position of the widget. 
     *
     *  @bx, @by: the left bottom corner of the bounding box
     *  @x, @y: return the the starting point of the widget
     * ---------------------------------------------------*/
    void align(int bx, int by, int* x, int* y) const;

    /* ---------------------------------------------------
     * connect
     *  Register a callback to this widget. The old callback
     *  will be replaced with the new one even if it is a NULL
     *  pointer.
     *
     *  @callback: the callback function pointer.
     * ---------------------------------------------------*/
    void connect(void* value, void (*callback)(void* value) = 0);

    /* ---------------------------------------------------
     * update
     *  Redraw the widget according to its internal states. The
     *  drawing only happens in the local region of widget by
     *  using scissor mask.
     *  
     *  @x, @y, @w, @h: Invalidate the rectangle specified
     *  by these parameters.
     * ---------------------------------------------------*/
    void update(int x, int y, int w, int h);

    /* ---------------------------------------------------
     * set_hover
     *  set the m_hover's value
     *  
     *  @status: the status
     * ---------------------------------------------------*/
    virtual void set_hover(bool status);

    /* ---------------------------------------------------
     * set_clicked
     *  set the m_clicked's value
     *  
     *  @status: the status
     * ---------------------------------------------------*/
    virtual void set_clicked(bool status);

protected:
    int   m_h;    // The widget height
    int   m_w;    // The widget width
    int   m_x;    // The widget left bottom corner x;
    int   m_y;    // The widget left bottom corner y;

    int m_h_hint; // The desired height (minimum requirements)
    int m_w_hint; // The desired width

    int m_alignment;        // The widget's horizontal alignment
    int m_valignment;        // The widget's vertical alignment

    char m_label[1024];      // The label of widget

    Widget*     m_parent;    // The parent

    bool        m_visible;   // Visible or hidden
    bool        m_enabled;   // functional or disabled

    int         m_fg_color[3];
    int         m_bg_color[3];

    typedef void (*callback_func_t)(void* val);

    callback_func_t m_callback;  // The widget's callback

    void* m_live_value; // The callback value

    bool       m_clicked; // The widget has been currently clicked
    bool       m_hover; // The widget is ccurently touched by mouse
};

class Widget_Layout : public Widget {
public:
    Widget_Layout(Widget* parent = 0);
    virtual ~Widget_Layout();

    virtual void add_widget(Widget* widget);
    virtual void set_fg_color(int r, int g, int b);
    virtual void set_bg_color(int r, int g, int b);

    virtual void set_hover(bool status);
    virtual void set_clicked(bool status);

    virtual void draw();
    
    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_motion(int x, int y);

    void set_vspacing(int spacing);
    void set_hspacing(int spacing);

    int get_vspacing() const { return m_vspacing; };
    int get_hspacing() const { return m_hspacing; };

    size_t num_of_widgets() const { return m_widgets.size(); };
    Widget* widget(size_t index) { return m_widgets[index]; };

    void set_visible_border(bool val) { m_visible_border = val; };

protected:
    std::vector<Widget*> m_widgets;

    bool m_visible_border;
    int m_vspacing; // the space betwwen vertical nighboring widgets 
    int m_hspacing; // the space between horizontal neighboring widgets 
};

class Widget_HLayout : public Widget_Layout {
public:
    Widget_HLayout(Widget* parent = 0);
    virtual ~Widget_HLayout();

    virtual int w_hint() const;
    virtual int h_hint() const;

    virtual void set_size(int x, int y, int w, int h);
};

class Widget_VLayout : public Widget_Layout {
public:
    Widget_VLayout(Widget* parent = 0);
    virtual ~Widget_VLayout();

    virtual int w_hint() const;
    virtual int h_hint() const;

    virtual void set_size(int x, int y, int w, int h);
};

class Widget_Statictext : public Widget {
    friend class Widget_Master;

public:
    Widget_Statictext(const char* string, Widget* parent = 0);
    ~Widget_Statictext();

    virtual void draw();
};

class Widget_Checkbox : public Widget {
public:
    Widget_Checkbox(const char* string, Widget* parent = 0);
    ~Widget_Checkbox();

    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual void on_mouse_over(int x, int y);

    virtual void draw();

    void check(int value) { m_value = value; }; // No callback will be triggered
    int checked() const { return m_value; };

private:
    int m_value; // 1 for checked, 0 for unchecked

    int m_boxsize; // The box size
};

class Texture2D;
class Texture1D;

class Widget_Bitmap : public Widget {
public:
    Widget_Bitmap(int width, int height, int depth, 
            float* data, int border, Widget* parent = 0);
    Widget_Bitmap(int width, int height, int depth, 
            unsigned char* data, int border, Widget* parent = 0);
    Widget_Bitmap(const Texture2D& texture, int border, Widget* parent = 0);
    Widget_Bitmap(const Texture1D& texture, int height, int border, Widget* parent = 0);

    virtual void draw();
};

class Widget_Listbox : public Widget {
public:
    Widget_Listbox(const char* string, Widget* parent = 0);
    ~Widget_Listbox();

    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual void on_mouse_over(int x, int y);

    virtual void draw();

    void set_current_index(int index);
    int get_current_index() const { return m_current_index; };
    const char* get_item(int index) { return m_item_array[m_current_index].c_str(); };
    void add_item(const char* item); 

private:
    int m_current_index; // The default is zero
    int m_hover_index; // The item currently is touched by mouse
    bool m_collapsed; // Whether the menu is pull down
    std::vector<std::string> m_item_array; // The list box' items

    static const int init_combo_length = 48;
    static const int init_combo_height = 15;

    int m_combo_length; // The length of the combo
    int m_combo_height; // The height of the combo

    int m_box_width;  // The indicator box
    int m_box_height;
};

class Widget_HSlider : public Widget {
public:
    Widget_HSlider(const char* label, int width = 40, Widget* parent = 0);

    virtual void draw();

    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_motion(int x, int y) = 0;

protected:
    int m_slider_width; // Self-explained

    int m_last_x; // The last mouse position
    int m_last_y; 

    int m_indicator_x; // The position of the indicator

    int m_label_width; // The label width

    int m_indicator_height; // The height of the indicator
    static const int indicator_width  = 8;
};

class Widget_Float_HSlider : public Widget_HSlider {
public:
    Widget_Float_HSlider(const char* label, int width = 100, Widget* parent = 0);
    Widget_Float_HSlider(const char* label, float limits[], int width = 100, Widget* parent = 0);

    void set_limits(float min, float max);
    void set_value(float value);
    float get_value() const { return m_value; };

    virtual bool on_mouse_motion(int x, int y);

    virtual void draw();

    virtual void set_size(int x, int y, int w, int h);

private:
    float m_limits[2];  // default: 0.0f, 1.0f
    float m_value; // default 0.0
};

class Widget_Int_HSlider : public Widget_HSlider {
public:
    Widget_Int_HSlider(const char* label, int width = 100, Widget* parent = 0);
    Widget_Int_HSlider(const char* label, int limits[], int width = 100, Widget* parent = 0);

    void set_limits(int min, int max);
    void set_value(int value);
    int get_value() const { return m_value; };

    virtual bool on_mouse_motion(int x, int y);

    virtual void draw();

    virtual void set_size(int x, int y, int w, int h);
private:
    int m_limits[2];  // default: 0, 10
    int m_value; // default 0
};

class Widget_Separator : public Widget {
public:
    /* ---------------------------------------------------
     * Widget_Separator
     *  Constructor. You can also use separator to add space 
     *  into two widgets by set separator invisible.
     *
     * @height: the height of separator. 
     *
     * ---------------------------------------------------*/
    Widget_Separator(int height = 1, Widget* parent = 0);

    virtual void draw();
};

class Widget_RadioGroup;

class Widget_Radio : public Widget {
public:
    /* ---------------------------------------------------
     * Widget_Radio
     *  Constructor. Note that the parent of a radio must be
     *  Widget_RadioGroup
     *
     * ---------------------------------------------------*/
    Widget_Radio(const char* label, int index, Widget_RadioGroup* parent); 
    
    virtual void draw();

    virtual bool on_mouse_button_up(int x, int y, int button);

    void toggle(bool toggled);
    bool toggled() const { return m_toggled; };

private:
    int m_disksize;

    bool m_toggled;

    int m_index; // The order index in the belonging group
};

class Widget_Group : public Widget {
public:
    Widget_Group(const char* label, Widget* parent = 0);
    ~Widget_Group();

    void draw();

    void set_layout(Widget_Layout* layout);

    virtual int w_hint() const;
    virtual int h_hint() const;

    virtual void set_size(int x, int y, int w, int h);

    virtual void set_fg_color(int r, int g, int b);
    virtual void set_bg_color(int r, int g, int b);

    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_motion(int x, int y);

protected:
    Widget_Layout* m_layout;

private:
    int m_label_width;
    int m_label_left_margin;  // The left margin between group label and widget boundary
    int m_label_right_margin;  // The right margin between group label and widget boundary
};

class Widget_RadioGroup : public Widget_Group {
public:
    /* ---------------------------------------------------
     * Widget_RadioGroup
     *  Constructor
     *
     *  @layout: the kind of the bounded layout. Horizontal or
     *  Vertical?
     * ---------------------------------------------------*/
    Widget_RadioGroup(const char* label, int layout, Widget* parent = 0);

    void add_radio(const char* label);

    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual void on_mouse_over(int x, int y);

    void toggle(int index);

    int value() const { return m_value; };

private:
    int m_value;
};

class Widget_RangeHSlider : public Widget {
};

class Widget_Button : public Widget {
public:
    Widget_Button(const char* label, Widget* parent = 0);

    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_button_down(int x, int y, int button);

    virtual void draw();
};

class Widget_Arcball : public Widget {
public:
    Widget_Arcball(const char* label, Widget* parent = 0);
    
    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual bool on_mouse_motion(int x, int y);

    void set_rotation_matrix(float mat[]);

    virtual void draw();

    float* rotation();

private:
    gGL::Arcball m_arcball;

    int m_spacing;
    int m_ball_size;
};


class Widget_Master : public Widget {
public:
    /* ---------------------------------------------------
     * Widget_Master
     *  Constructor
     *
     *  @w, @h: the hint size of the widget window
     * ---------------------------------------------------*/
    Widget_Master(int w, int h);

    /* ---------------------------------------------------
     * ~Widget_Master
     *  Destructor
     * ---------------------------------------------------*/
    ~Widget_Master();

    /* ---------------------------------------------------
     * init
     *  Initialization
     * ---------------------------------------------------*/
    bool init();

    /* ---------------------------------------------------
     * set_layout
     *  Set the layout for the children widgets
     * ---------------------------------------------------*/
    void set_layout(Widget_Layout* layout);

    
    /* ---------------------------------------------------
     * set_alignment 
     *  Set horizontal alignment of the widget window. Override
     *  the base class version
     * ---------------------------------------------------*/
    void set_alignment(int align);
    
    /* ---------------------------------------------------
     * set_valignment 
     *  Set vertical alignment of the widget window. Override
     *  the base class version
     * ---------------------------------------------------*/
    void set_valignment(int align);

    /* ---------------------------------------------------
     * set_size_hint
     *  Set the size hint of the widget window. Note that 
     *  size hint may not be guaranteed because widget window
     *  needs to ensure all widget visible.
     *
     *  @w, @h: the size hint, when the value is smaller than
     *  the necessary value and window will shrink to the minimum
     *  size.
     * ---------------------------------------------------*/
    void set_size_hint(int w, int h);

    /* ---------------------------------------------------
     * draw
     *  draw the widgets on the corner of window
     *  window size
     * ---------------------------------------------------*/
    virtual void draw();

    /* ---------------------------------------------------
     * reshape
     *  Adjust widget window with new render window size
     *
     *  @w, @h: the size of the new render window
     * ---------------------------------------------------*/
    void reshape(int w, int h);
    
    /* ---------------------------------------------------
     * set_fgcolor
     *  set the foreground color of the widgets
     *  
     *  @r, @g, @b: the foreground color
     * ---------------------------------------------------*/
    void set_fg_color(int r, int g, int b);

    /* ---------------------------------------------------
     * set_fgcolor
     *  set the foreground color of the widgets
     *  
     *  @r, @g, @b: the background color
     * ---------------------------------------------------*/
    void set_bg_color(int r, int g, int b);
    

    /* ---------------------------------------------------
     * Event callbacks
     * ---------------------------------------------------*/
    virtual void on_mouse_over(int x, int y);
    virtual bool on_mouse_button_down(int x, int y, int button);
    virtual bool on_mouse_button_up(int x, int y, int button);
    virtual bool on_mouse_motion(int x, int y);
    
private:
    /////////////////////////////////////////// 
    // Helper functions
    /////////////////////////////////////////// 
    
    /////////////////////////////////////////// 
    // Data members
    /////////////////////////////////////////// 
    Widget_Layout* m_layout;
    
    int m_vmargin; // margin to vertical window border
    int m_hmargin; // margin to horizontal window border

    int m_buttons; // The current pressed mouse buttons

    int m_win_w, m_win_h; // The window size

    bool m_inited; // If the panel has been initialized
};


}; // namespace gGL

#endif // GGL_WIDGET
