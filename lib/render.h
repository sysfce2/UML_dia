/* Dia -- an diagram creation/manipulation program
 * Copyright (C) 1998 Alexander Larsson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef RENDER_H
#define RENDER_H

#error "The old Renderer is dead."
typedef struct _RenderOps RenderOps;
typedef struct _InteractiveRenderOps InteractiveRenderOps;
typedef struct _Renderer Renderer;

#include "dia-enums.h"
#include "geometry.h"
typedef struct _DiaFont DiaFont;
typedef struct _Color Color;
typedef struct _DiaImage* DiaImage;
#include "arrows.h"
#include "object.h"

/******************************************************
 **  Functions defined for every Renderer
 **
 ******************************************************/

/* Called before rendering begins.
   Can be used to do various pre-rendering setup. */
typedef void (*BeginRenderFunc) (Renderer *renderer);

/* Called after all rendering is done.
   Used to do various clean-ups.*/
typedef void (*EndRenderFunc) (Renderer *renderer);

/* Set the current line width
   if linewidth==0, the line will be an 'hairline' */
typedef void (*SetLineWidthFunc) (Renderer *renderer, real linewidth);

/* Set the current linecap (the way lines are ended) */
typedef void (*SetLineCapsFunc) (Renderer *renderer, LineCaps mode);

/* Set the current linejoin (the way two lines are joined together) */
typedef void (*SetLineJoinFunc) (Renderer *renderer, LineJoin mode);

/* Set the current line style */
typedef void (*SetLineStyleFunc) (Renderer *renderer, LineStyle mode);

/* Set the dash length, when the style is not SOLID
   A dot will be 10% of length */
typedef void (*SetDashLengthFunc) (Renderer *renderer, real length);

/* Set the fill style */
typedef void (*SetFillStyleFunc) (Renderer *renderer, FillStyle mode);

/* Set the current font */
typedef void (*SetFontFunc) (Renderer *renderer, DiaFont *font, real height);

/* Draw a line from start to end, using color and the current line style */
typedef void (*DrawLineFunc) (Renderer *renderer,
			      Point *start, Point *end,
			      Color *color);

/* Draw a line joining multiple points, using color and the current
   line style */
typedef void (*DrawPolyLineFunc) (Renderer *renderer,
				  Point *points, int num_points,
				  Color *color);

/* Draw a polygone, using the current line style
   The polygon is closed even if the first point is not the same as the
   last point */
typedef void (*DrawPolygonFunc) (Renderer *renderer,
				 Point *points, int num_points,
				 Color *color);

/* Same a DrawPolygonFunc, expect the polygon is filled using the current
   fill type */
typedef void (*FillPolygonFunc) (Renderer *renderer,
				 Point *points, int num_points,
				 Color *color);

/* Draw a rectangle, given its upper-left and lower-right corners */
typedef void (*DrawRectangleFunc) (Renderer *renderer,
				   Point *ul_corner, Point *lr_corner,
				   Color *color);

/* Same a DrawRectangleFunc, except the rectangle is filled using the
   current fill style */
typedef void (*FillRectangleFunc) (Renderer *renderer,
				   Point *ul_corner, Point *lr_corner,
				   Color *color);

/* Draw a rounded rectangle, given its upper-left and lower-right corners */
typedef void (*DrawRoundedRectangleFunc) (Renderer *renderer,
					  Point *ul_corner, Point *lr_corner,
					  Color *color, real rounding);

/* Same a DrawRoundedRectangleFunc, except the rectangle is filled using the
   current fill style */
typedef void (*FillRoundedRectangleFunc) (Renderer *renderer,
					  Point *ul_corner, Point *lr_corner,
					  Color *color, real rounding);

/* Draw an arc, given its center, the bounding box (widget, height),
   the start angle and the end angle */
typedef void (*DrawArcFunc) (Renderer *renderer,
			     Point *center,
			     real width, real height,
			     real angle1, real angle2,
			     Color *color);

/* Same a DrawArcFunc except the arc is filled (a pie-chart) */
typedef void (*FillArcFunc) (Renderer *renderer,
			     Point *center,
			     real width, real height,
			     real angle1, real angle2,
			     Color *color);

/* Draw an ellipse, given its center and the bounding box */
typedef void (*DrawEllipseFunc) (Renderer *renderer,
				 Point *center,
				 real width, real height,
				 Color *color);

/* Same a DrawEllipse, except the ellips is filled */
typedef void (*FillEllipseFunc) (Renderer *renderer,
				 Point *center,
				 real width, real height,
				 Color *color);

/* Draw a bezier curve, given it's control points.
 * The first BezPoint must be of type MOVE_TO, and no other ones may be
 * MOVE_TO's.
 */
typedef void (*DrawBezierFunc) (Renderer *renderer,
				BezPoint *points,
				int numpoints,
				Color *color);

/* Same as DrawBezierFunc, except the last point must be the same as the
   first point, and the resulting shape is filled */
typedef void (*FillBezierFunc) (Renderer *renderer,
				BezPoint *points,
				int numpoints,
				Color *color);

/* Print a string at pos, using the current font */
typedef void (*DrawStringFunc) (Renderer *renderer,
				const gchar *text,
				Point *pos,
				Alignment alignment,
				Color *color);

/* Draw an image, given its bounding box */
typedef void (*DrawImageFunc) (Renderer *renderer,
			       Point *point,
			       real width, real height,
			       DiaImage image);

/******************************************************
 **  Functions defined for every Interactive Renderer
 **  Interactive renderers are renderers that render
 **  to pixels on the screen.
 ******************************************************/

/* Returns the EXACT width of text in cm, using the current font.
   There has been some confusion as to the definition of this.
   It used to say the width was in pixels, but actual width returned
   was cm.  You shouldn't know about pixels anyway.
 */
typedef real (*GetTextWidthFunc) (Renderer *renderer,
				  const gchar *text, int length);

/* Clear the current clipping region.
   This function needs only be defined for interactive
   renderers.
*/
typedef void (*ClipRegionClearFunc) (Renderer *renderer);

/* Add a rectangle to the current clipping region.
   This function needs only be defined for interactive
   renderers.
*/
typedef void (*ClipRegionAddRectangleFunc) (Renderer *renderer,
					    Rectangle *rect);

/* Draw a line from start to end, using color and the current line style */
typedef void (*DrawPixelLineFunc) (Renderer *renderer,
				   int x1, int y1,
				   int x2, int y2,
				   Color *color);

/* Draw a rectangle, given its upper-left and lower-right corners
   in pixels.
*/
typedef void (*DrawPixelRectangleFunc) (Renderer *renderer,
					int x, int y,
					int width, int height,
					Color *color);

/* Same a DrawPixelRectangleFunc, except the rectangle is filled using the
   current fill style */
typedef void (*FillPixelRectangleFunc) (Renderer *renderer,
					int x, int y,
					int width, int height,
					Color *color);

/* Functions to draw lines with various arrows */
/* All Draw*WithArrowsFuncs assume that any arrow transformation is included
   in the points.  If a renderer wants to get the original points, it must
   untransform the points.
 */
typedef void (*DrawLineWithArrowsFunc) (Renderer *renderer,
					Point *from, Point *to,
					real line_width,
					Color *color,
					Arrow *start_arrow,
					Arrow *end_arrow);

typedef void (*DrawPolyLineWithArrowsFunc) (Renderer *renderer,
					    Point *points, int num_points,
					    real line_width,
					    Color *color,
					    Arrow *start_arrow,
					    Arrow *end_arrow);

/* Note that this is different from the normal DrawArc function */
typedef void (*DrawArcWithArrowsFunc) (Renderer *renderer,
				       Point *start, Point *end,
				       Point *mid,
				       real line_width,
				       Color *color,
				       Arrow *start_arrow,
				       Arrow *end_arrow);

typedef void (*DrawBezierWithArrowsFunc) (Renderer *renderer, 
					  BezPoint *points,
					  int num_points,
					  real line_width,
					  Color *color,
					  Arrow *start_arrow,
					  Arrow *end_arrow);

typedef void (*DrawObjectFunc) (Renderer *renderer,
				DiaObject *object);

struct _RenderOps {
  /* Control ops: */
  BeginRenderFunc   begin_render;
  EndRenderFunc     end_render;
  
  /* Line attributes: */
  SetLineWidthFunc  set_linewidth;
  SetLineCapsFunc   set_linecaps;
  SetLineJoinFunc   set_linejoin;
  SetLineStyleFunc  set_linestyle;
  SetDashLengthFunc set_dashlength;
  /* Fill attributes: */
  SetFillStyleFunc  set_fillstyle;
  /* DiaFont stuff: */
  SetFontFunc       set_font;
  
  /* Lines: */
  DrawLineFunc      draw_line;
  DrawPolyLineFunc  draw_polyline;

  /* Polygons: */
  DrawPolygonFunc   draw_polygon;
  FillPolygonFunc   fill_polygon;

  /* Rectangles: */
  DrawRectangleFunc draw_rect;
  FillRectangleFunc fill_rect;
  
  /* Arcs: */
  DrawArcFunc       draw_arc;
  FillArcFunc       fill_arc;
  
  /* Ellipses: */
  DrawEllipseFunc   draw_ellipse;
  FillEllipseFunc   fill_ellipse;

  /* Bezier curves: */
  DrawBezierFunc    draw_bezier;
  FillBezierFunc    fill_bezier;
  
  /* Text: */
  DrawStringFunc    draw_string;

  /* Images: */
  DrawImageFunc     draw_image;

  DrawRoundedRectangleFunc draw_rounded_rect;
  FillRoundedRectangleFunc fill_rounded_rect;

  DrawLineWithArrowsFunc draw_line_with_arrows;
  DrawPolyLineWithArrowsFunc draw_polyline_with_arrows;
  DrawArcWithArrowsFunc draw_arc_with_arrows;
  DrawBezierWithArrowsFunc draw_bezier_with_arrows;

  DrawObjectFunc draw_object;
};

struct _InteractiveRenderOps {
  GetTextWidthFunc get_text_width;

  ClipRegionClearFunc clip_region_clear;
  ClipRegionAddRectangleFunc clip_region_add_rect;

  DrawPixelLineFunc draw_pixel_line;
  DrawPixelRectangleFunc draw_pixel_rect;
  FillPixelRectangleFunc fill_pixel_rect;
};

struct _Renderer {
  RenderOps *ops;

  int is_interactive;
  InteractiveRenderOps *interactive_ops;
  int pixel_width; /* Only needed for interactive renderers.*/
  int pixel_height; /* Only needed for interactive renderers.*/
};

/* Use this function to initialize all empty ops slots */
void inherit_renderer(Renderer *child_ops);
RenderOps *create_renderops_table(void);

#endif /* RENDER_H */