use <top.scad>;

$fs = 0.75;

// Length and width of the main PCB.
pl = 127.0;
pw = 37.8;
// Difference in case to PCB position from the origin (back of model).
pcb_origin_diff = 2.7;

// Length and width of the the case('s inside, plus t for thickness on 4 sides).
// The length takes into account the ten degree pitch.  Length is at the
// bottom surface in the model, which is the top (by the keys).
cl = 129.5 - pcb_origin_diff;
cw = 39.0;
// TODO: An extra 0.2mm thickness to make the connector(s) flush, slightly more
// if I want to try to hide their edges.

// Tallest height, outer wall thickness, corner rounding radius of the case.
ch = 31.9;
ct = 2.5;
cr = 2;


top(cl, cw, ch, ct, cr);

// The key PCB.
pr = 2.54 / 2;
color([0, 0.4, 0, 0.5])
translate([pr + pcb_origin_diff, pr+ct+(cw-pw)/2, 12.7]) hull() {
  for(x = [0, pl-2*pr]) {
    for(y = [0, pw-2*pr]) {
      translate([x, y, 0]) cylinder(r=pr, h=1.6);
    }
  }
};
