// Length and width of the PCB.
pl = 127.0;
pw = 37.8;
// Difference in case to PCB position from the origin (back of model).
pcb_origin_diff = 2.7;
// Radius of PCB's rounded corners.
pr = 2.54;

module key_pcb() {
  color([0, 0.4, 0, 0.5])
  translate([pr + pcb_origin_diff, pr+ct+(cw-pw)/2, 12.7]) {
    difference() {
      hull() {
        for(x = [0, pl-2*pr]) {
          for(y = [0, pw-2*pr]) {
            translate([x, y, 0]) cylinder(r=pr, h=1.6);
          }
        }
      };

      // Hole between the 1st and second rows.
      translate([-pr + 26.7, -pr + pw/2, -1]) cylinder(d=3.7, h=4);
      // Hole near the last row.
      translate([-pr + 123.2, -pr + pw/2, -1]) cylinder(d=3.7, h=4);
    }
  };
};
