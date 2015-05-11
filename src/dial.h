#include <pebble.h>

#ifndef PBL_COLOR
#error "The Chromesthesia watchface depends on color support and is not available for the aplite platform. Sorry."
#endif

/// Call at program startup before creating the layer
void dial_init(void);

/// Layer update function for the color dial
void dial_update_layer(Layer *layer, GContext *ctx);

/// user-defined colors
#define CHROMESTHESIA_SCHEME_CUSTOM     0

/// Implementation of a color scheme devised by Alexander Scriabin
/// (according to Sabaneev [ref!])
#define CHROMESTHESIA_SCHEME_SCRIABIN   1


/// Load a predefined color scheme
void dial_load_color_scheme(unsigned int scheme_index);
