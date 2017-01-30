include <wall_shape.scad>;

hole_rad = 1.8;  // Holes in PCB, ample room for M3.
depth_to_pcb = 12.6;
module bottom(l, w, h, t, r, hole_height) {
  //color("silver")
  difference() {
    union() {
      // Bottom plate.
      translate([t - 1 + .01, t - 1 + 0.1, h - t]) rotate([0, 5, 0])
        translate([-5, 0, 0]) wall_shape(l + 11 - 0.2, w + 2 - 0.2, 10, t, r);

      // Back plate.
      translate([-1, t - 1, t]) cube([1.0+t, w + 2, h - t*2/3 + 5]);

      // Screw guide for front support.
      translate([l - 6, t + w/2 - 5, depth_to_pcb]) difference() {
        cube([10, 10, 10]);
      }

      // Lattice support beneath the key PCB.
      translate([60, t + w/2 - t/2, 10]) cube([65, 2.5, 15]);
      translate([95, t*2, 10]) cube([2.5, w - t*2, 15]);
      // PCB support at the far back.
      translate([t, t + 4, depth_to_pcb + 1.7]) {
        difference() {
          cube([2, w - 8, 2]);
          translate([2, -1, 0]) rotate([0, -45, 0]) cube([4, w - 6, 4]);
          translate([0.5, (w-8)/2 - t/2, -0.1]) cube([t, t, t]);
        }
      }
    };

    // Minus the angled bottom.
    translate([-5, -5, h - 0.2]) rotate([0, 5, 0]) cube([l+20, w+20, h]);
    // Minus bottom plate overhang for squareness.
    translate([-9.8, -2, -2]) cube([10, w+10, h+10]);
    // Minus any overlap down into the key PCB.
    translate([5, -1, -1]) cube([l + 2*t, w + 2*t, depth_to_pcb + 2.8]);

    // Holes for front screw guide, threads and head.
    translate([l - 0.9, t + w/2, -1]) cylinder(r=hole_rad + 0.2, h=25);
    translate([l - 0.9, t + w/2, 16.5]) cylinder(r=3, h=5);

    // Hole for mounting screw.
    rotate([0, 90, 0]) translate([-hole_height, t + w/2, -1]) {
      cylinder(r=hole_rad, h=9);  // Hole.
    }

    // Hole for USB-A socket.
    translate([-1, 25.7, 23.4]) cube([15.5, 14.8, 7.7]);
    // Hole for Micro USB socket.
    translate([t+1, 8.3, 22.9])
    rotate([0, 0, 90])
    difference() {
      cube([8.8, 4, 3.1]);
      for (x=[0, 8.8]) {
        translate([x, -1, 1.5])
        rotate([0, 135, 0])
        cube([4, 6, 4]);
      }
    }


    // Workaround, cutout for logic PCB misalignment.
    translate([t - 1.9, t + 2, depth_to_pcb + 3.7]) {
      rotate([0, 0, -3]) cube([5, w, 6.3]);
    }
  }
}
