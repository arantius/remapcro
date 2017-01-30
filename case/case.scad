include <pcbs.scad>;
include <top.scad>;
include <bottom.scad>;

$fs = 0.5;

// Length and width of the the case('s inside, plus t for thickness on 4 sides).
// The length takes into account the ten degree pitch.  Length is at the
// bottom surface in the model, which is the top (by the keys).
cl = 129.5 - pcb_origin_diff;
cw = 39.0;
// TODO: An extra 0.2mm thickness to make the connector(s) flush, slightly more
// if I want to try to hide their edges.

// Tallest height, outer wall thickness, corner rounding radius of the case.
ch = 34.25;
ct = 2.5;
cr = 2;

// Height from origin for mounting hole in the back.
hole_height = 6;


%key_pcb();
%logic_pcb();


top(cl, cw, ch, ct, cr, hole_height);
bottom(cl, cw, ch, ct, cr, hole_height);
