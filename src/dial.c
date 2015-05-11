#include <pebble.h>
#include "dial.h"
#include "paths.h"

#define PERSIST_KEY_SELECTED_SCHEME  10
#define PERSIST_KEY_CUSTOM_COLORS      11

#define PERSIST_KEY_LABEL_SCHEME 20


static GPath *pad_path[12];
static GRect label_box[12];

static GColor8 pad_colors[12];

static GColor8 CHROMESTHESIA_SCHEMES[][12] =
{
	{
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
		{ .argb = 0b11111111 },
	},
	{
		{ .a=0b11, .r=0b11, .g=0b00, .b=0b00 }, // 12
		{ .a=0b11, .r=0b11, .g=0b10, .b=0b01 }, // 1
		{ .a=0b11, .r=0b11, .g=0b11, .b=0b00 }, // 2
		{ .a=0b11, .r=0b01, .g=0b11, .b=0b00 }, // 3
		{ .a=0b11, .r=0b01, .g=0b11, .b=0b10 }, // 4
		{ .a=0b11, .r=0b00, .g=0b11, .b=0b11 }, // 5
		{ .a=0b11, .r=0b10, .g=0b11, .b=0b11 }, // 6
		{ .a=0b11, .r=0b10, .g=0b01, .b=0b11 }, // 7
		{ .a=0b11, .r=0b10, .g=0b00, .b=0b11 }, // 8
		{ .a=0b11, .r=0b11, .g=0b01, .b=0b11 }, // 9
		{ .a=0b11, .r=0b11, .g=0b10, .b=0b11 }, // 10
		{ .a=0b11, .r=0b10, .g=0b00, .b=0b00 }  // 11
	}
};

static const size_t N_THEMES =
	sizeof(CHROMESTHESIA_SCHEMES) / sizeof(GColor8[12]);

static const char *label[12] =
{
	"C", "G", "D", "A", "E", "H",
	"F#", "Db", "Ab", "Eb", "Bb", "F"
};

void dial_init(void)
{
	for (int h=0; h<12; h++) {
		pad_path[h] = gpath_create(&LAYER1_PATHS[h]);

		GPoint *box = LAYER4_PATHS[h].points;
		label_box[h].origin = box[0];
		label_box[h].origin.y -= 4;
		label_box[h].size = GSize(box[2].x - box[0].x, box[2].y - box[0].y);
	}
	
	// load custom colors
	if (persist_exists(PERSIST_KEY_CUSTOM_COLORS))
		persist_read_data(PERSIST_KEY_CUSTOM_COLORS,
			CHROMESTHESIA_SCHEMES[0], sizeof(GColor8) * 12);

	// load default scheme
	int scheme_idx = CHROMESTHESIA_SCHEME_SCRIABIN;
	if (persist_exists(PERSIST_KEY_SELECTED_SCHEME))
	  scheme_idx = persist_read_int(PERSIST_KEY_SELECTED_SCHEME);

	dial_load_color_scheme(scheme_idx);
}

void dial_update_layer(Layer *layer, GContext *ctx)
{
//    GRect bounds = layer_get_bounds(layer);
    GFont *regular = fonts_get_system_font(FONT_KEY_GOTHIC_24);

	// pads and labels
	graphics_context_set_stroke_color(ctx, GColorBlack);
	graphics_context_set_text_color(ctx, GColorBlack);
	for (int h=0; h<12; h++) {
		graphics_context_set_fill_color(ctx, pad_colors[h]);
		gpath_draw_filled(ctx, pad_path[h]);
		gpath_draw_outline(ctx, pad_path[h]);

		graphics_draw_text(ctx, label[h], regular, label_box[h],
			GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	}
}

void dial_load_color_scheme(unsigned int scheme_idx)
{
	if (scheme_idx < N_THEMES)
		memcpy(pad_colors, CHROMESTHESIA_SCHEMES[scheme_idx], sizeof(pad_colors));
}
