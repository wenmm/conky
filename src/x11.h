/*
 *
 * Conky, a system monitor, based on torsmo
 *
 * Please see COPYING for details
 *
 * Copyright (c) 2005-2018 Brenden Matthews, Philip Kovacs, et. al.
 *	(see AUTHORS)
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef BUILD_X11
#ifndef X11_H_
#define X11_H_

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#ifdef BUILD_XFT
#include <X11/Xft/Xft.h>
#endif

#ifdef BUILD_XDBE
#include <X11/extensions/Xdbe.h>
#endif

#include "colours.h"
#include "setting.hh"

#define ATOM(a) XInternAtom(display, #a, False)

#ifdef OWN_WINDOW
enum window_type {
  TYPE_NORMAL = 0,
  TYPE_DOCK,
  TYPE_PANEL,
  TYPE_DESKTOP,
  TYPE_OVERRIDE
};

enum window_hints {
  HINT_UNDECORATED = 0,
  HINT_BELOW,
  HINT_ABOVE,
  HINT_STICKY,
  HINT_SKIP_TASKBAR,
  HINT_SKIP_PAGER
};

#define SET_HINT(mask, hint) (mask |= (1 << (hint)))
#define TEST_HINT(mask, hint) (mask & (1 << (hint)))
#endif

struct conky_window {
  Window root, window, desktop;
  Drawable drawable;
  Visual *visual;
  Colormap colourmap;
  GC gc;

#ifdef BUILD_XDBE
  XdbeBackBuffer back_buffer;
#else
  Pixmap back_buffer;
#endif
#ifdef BUILD_XFT
  XftDraw *xftdraw;
#endif

  int width;
  int height;
#ifdef OWN_WINDOW
  int x;
  int y;
#endif
};

#if defined(BUILD_ARGB) && defined(OWN_WINDOW)
/* true if use_argb_visual=true and argb visual was found*/
extern bool have_argb_visual;
#endif

extern Display *display;

void destroy_window(void);
void create_gc(void);
void set_transparent_background(Window win);
void get_x11_desktop_info(Display *current_display, Atom atom);
void set_struts(int);

void print_monitor(struct text_object *, char *, unsigned int);
void print_monitor_number(struct text_object *, char *, unsigned int);
void print_desktop(struct text_object *, char *, unsigned int);
void print_desktop_number(struct text_object *, char *, unsigned int);
void print_desktop_name(struct text_object *, char *, unsigned int);

/* Num lock, Scroll lock, Caps Lock */
void print_key_num_lock(struct text_object *, char *, unsigned int);
void print_key_caps_lock(struct text_object *, char *, unsigned int);
void print_key_scroll_lock(struct text_object *, char *, unsigned int);

/* Keyboard layout and mouse speed in percentage */
void print_keyboard_layout(struct text_object *, char *, unsigned int);
void print_mouse_speed(struct text_object *, char *, unsigned int);

#ifdef BUILD_XDBE
void xdbe_swap_buffers(void);
#else
void xpmdb_swap_buffers(void);
#endif /* BUILD_XDBE */

namespace priv {
class own_window_setting : public conky::simple_config_setting<bool> {
  typedef conky::simple_config_setting<bool> Base;

 protected:
  virtual void lua_setter(lua::state &l, bool init);

 public:
  own_window_setting() : Base("own_window", false, false) {}
};

#ifdef BUILD_XDBE
class use_xdbe_setting : public conky::simple_config_setting<bool> {
  typedef conky::simple_config_setting<bool> Base;

  bool set_up(lua::state &l);

 protected:
  virtual void lua_setter(lua::state &l, bool init);

 public:
  use_xdbe_setting() : Base("double_buffer", false, false) {}
};

#else
class use_xpmdb_setting : public conky::simple_config_setting<bool> {
  typedef conky::simple_config_setting<bool> Base;

  bool set_up(lua::state &l);

 protected:
  virtual void lua_setter(lua::state &l, bool init);

 public:
  use_xpmdb_setting() : Base("double_buffer", false, false) {}
};
#endif
}  // namespace priv

#ifdef BUILD_XFT
extern conky::simple_config_setting<bool> use_xft;
#endif

#ifdef OWN_WINDOW
extern conky::simple_config_setting<bool> set_transparent;
extern conky::simple_config_setting<std::string> own_window_class;
extern conky::simple_config_setting<std::string> own_window_title;
extern conky::simple_config_setting<window_type> own_window_type;

struct window_hints_traits {
  static const lua::Type type = lua::TSTRING;
  typedef uint16_t Type;
  static std::pair<Type, bool> convert(lua::state &l, int index,
                                       const std::string &name);
};
extern conky::simple_config_setting<uint16_t, window_hints_traits>
    own_window_hints;

#ifdef BUILD_ARGB
extern conky::simple_config_setting<bool> use_argb_visual;

/* range of 0-255 for alpha */
extern conky::range_config_setting<int> own_window_argb_value;
#endif
#endif /*OWN_WINDOW*/
extern priv::own_window_setting own_window;

#ifdef BUILD_XDBE
extern priv::use_xdbe_setting use_xdbe;
#else
extern priv::use_xpmdb_setting use_xpmdb;
#endif

#endif /*X11_H_*/
#endif /* BUILD_X11 */
