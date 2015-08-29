use <center_block.scad>
use <corner_block.scad>
use <center_sticker.scad>
use <corner_sticker.scad>

color([0.5, 0.5, 0.5]) {
    center_block();
    corner_block();
}

color([0.9, 0.9, 0.9]) {
    center_sticker();
    corner_sticker();
}