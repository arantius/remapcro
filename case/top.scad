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

hole_rad = 1.8;
depth_to_pcb = 12.6;
module top(l, w, h, t, r, hole_height) {
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
      translate([l - 3.4, t + w/2 - 2.5, depth_to_pcb - 8]) difference() {
        cube([9, 5, 8]);
        translate([-8, -1, -4.5]) rotate([0, 30, 0]) cube([12, 7, 12]);
        translate([2.5, 2.5, 0]) cylinder(r=hole_rad, h=10);
      };

      // Rear peg slotting into that hole from the first spacer.
      translate([25.8 + 7.5/2, t + w/2, depth_to_pcb]) {
        cylinder(r=hole_rad, h=2);
        translate([0, 0, 2]) cylinder(r1=hole_rad, r2=0, h=1);
      }
    };

    // Inset for back to fit into.
    translate([-1, t, t]) cube([1.3+t, w, h - t*2/3 + 5]);
    // Plus a small cutout for space where PCB pins stick out.
    translate([3.6, t + w/2 - 0.5, depth_to_pcb - 1.0]) cube([2, 2, 2]);
    // And room for the bottom plate.
    translate([0, t, h - t - 0.5]) rotate([0, 5, 0]) cube([20, w, 5]);

    // Minus the angled bottom.
    translate([-5, -5, h]) rotate([0, 5, 0]) cube([l+20, w+20, h]);

    // Hole for mounting screw.
    rotate([0, 90, 0]) translate([-hole_height, t + w/2, -1]) {
      cylinder(r=hole_rad, h=9);  // Hole.
      translate([0, 0, 4.5]) cylinder(d=6.4, h=3, $fn=6);  // Nut cutout.
    };

    // Work around for first logic PCB misalignment, cut out space where
    // it unintentionally sticks out.
    translate([t, t, depth_to_pcb + 2]) {
      rotate([0, 0, -2.5]) cube([35, 4, 10]);
      translate([34.9, -1.525, 0]) rotate([0, 0, 2.5]) cube([15, 4, 5]);
    }
  }
}
