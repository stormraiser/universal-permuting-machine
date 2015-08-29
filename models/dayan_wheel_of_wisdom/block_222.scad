include <def.scad>

module block_222() {
    minkowski() {
        difference() {
            intersection() {
                _cube();
                intersection_for (vec = [[0, 0, 0], [-90, 0, 0], [0, 90, 0]])
                    rotate(vec)
                        _int_cut1();
            }
        }
        sphere(r = round_radius, $fn = 24);
    }
}

block_222();