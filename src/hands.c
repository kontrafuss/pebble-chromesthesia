#include "hands.h"
#include "paths.h"

static GPath *minute_hand, *hour_hand;

static const struct GPathInfo HANDS_PATH[] = {
  { // minute
    .num_points = 11,
    .points = (GPoint []) {
		{0, -75},
		{-2, -73},
		{-3, -30},
		{-8, -25},
		{-3, -20},
		{-4,   0},
		{ 4,   0},
		{ 3, -20},
		{ 8, -25},
		{3, -30},
		{2, -73}
    }
  },
  { // hour
	  .num_points = 5,
	  .points = (GPoint[]) {
		  {-1, -6},
		  {-2, -10},
		  {0, -32},
		  {2, -10},
		  {1, -6},
	  }
  }
};


void hands_init(GRect bounds)
{
	minute_hand = gpath_create(&HANDS_PATH[0]);
	hour_hand = gpath_create(&HANDS_PATH[1]);

	GPoint center = grect_center_point(&bounds);
	gpath_move_to(minute_hand, center);
	gpath_move_to(hour_hand, center);
}

void hands_update_layer(Layer *layer, GContext *ctx)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

	graphics_context_set_fill_color(ctx, GColorWhite);
	gpath_rotate_to(minute_hand, TRIG_MAX_ANGLE * t->tm_min / 60);
	gpath_rotate_to(hour_hand, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));

	gpath_draw_filled(ctx, minute_hand);

	graphics_fill_circle(ctx, GPoint(71,85), 7);
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_circle(ctx, GPoint(71,85), 5);

	// hour
#ifdef PBL_COLOR
	graphics_context_set_stroke_color(ctx, GColorLightGray);
	graphics_context_set_fill_color(ctx, GColorLightGray);
#else
	graphics_context_set_stroke_color(ctx, GColorWhite);
#endif
	gpath_draw_filled(ctx, hour_hand);
	gpath_draw_outline(ctx, hour_hand);
}
