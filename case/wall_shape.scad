module wall_shape(l, w, h, t, r) {
  module fb_wall() {
    union() {
      translate([r, r, 0]) {
        cylinder(h=h + 2*r, r=r);
        translate([0, w - 2*r, 0])
          cylinder(h=h + 2*r, r=r);
      }
    }
  }

  hull() {
    fb_wall();
    translate([l - 2*r, 0, 0])
      rotate([0, 10, 0])
        fb_wall();
  }
}
