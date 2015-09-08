include <def.scad>

module selector_ccw() {
    difference() {
        intersection() {
            _selector_base(inner_radius);
            rotate([0, -theta, 0])
                translate([-inf, -inf, cut1_dis])
                    cube([inf * 2, inf * 2, inf]);
        }
        rotate([0, theta, 0])
            rotate([90, 0, -36])
                translate([-inf, -inf, 0])
                    cube([inf * 2, inf * 2, inf]);
        rotate([0, -theta, 0])
            rotate([-90, 0, 36])
                translate([-inf, -inf, 0])
                    cube([inf * 2, inf * 2, inf]);
    }
}

for (ang = [0 : 72 : 288])
    rotate([0, -theta, 0])
        rotate([0, theta, ang])
            selector_ccw();