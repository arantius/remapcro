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

depth_to_pcb = 12.6;
module top(l, w, h, t, r) {
  color("silver") difference() {
    union() {
      // Outer walls.
      difference() {
        // Larger outer shape.
        wall_shape(l + 2*t, w + 2*t, h, t, r);
        // Minus inner shape.
        translate([t, t, -3]) wall_shape(l, w, h+6, t, r);
      }

      // Spacer behind 1st row.
      translate([r, 0, 0]) cube([5.5 - r, w + 2*t, depth_to_pcb]);
      // Spacer between 1st and 2nd rows.
      translate([25.9, t/2, 0]) cube([7.5, w + t, depth_to_pcb]);

      // Front support, for bottom to screw into.
      translate([l - 3, t + w/2 - 2.5, depth_to_pcb - 8]) difference() {
        cube([8, 5, 8]);
        translate([-8, -1, -4.5]) rotate([0, 30, 0]) cube([12, 7, 12]);
        translate([2.5, 2.5, 0]) cylinder(r=1.8, h=10);
      }
    }

    // Inset for back to fit into.
    translate([-1, t, t]) cube([1.1+t, w, h - t*2/3 + 5]);
    // Plus a small cutout for space where PCB pins stick out.
    translate([3.6, t + w/2 - 0.5, depth_to_pcb - 0.9]) cube([2, 2, 1]);
    // And room for the bottom plate.
    translate([0, t, h - t]) rotate([0, 5, 0]) cube([20, w, 5]);

    // Minus the angled bottom.
    translate([-5, -5, h]) rotate([0, 5, 0]) cube([l+20, w+20, h]);
  }
}
