use <selector_cw.scad>
use <selector_ccw.scad>
include <selector_0.scad>

color([1, 0, 0]) {
    selector_cw();
    selector_ccw();
}