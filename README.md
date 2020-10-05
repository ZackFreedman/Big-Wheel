# Big Wheel
 3D-printed video control surface

Slapped together in a weekend by Zack Freedman of Voidstar Lab for [a YouTube video! Check it out!](https://www.youtube.com/watch?v=72a85tWOJVY)

Materials:
- 1x each of:
    - Big Ass-Wheel.stl
    - Body.stl
    - Wheel Insert.stl
- 1x of either of the following:
    - One print of Waffle Baseplate Combo.stl
    - One print of Support Waffle.stl and one laser-cut Big Wheel Baseplate.ai
- 7x 10mm M2.5 screws
- 6x M2.5 heat-set inserts, nominal 5mm length
- 1x M2.5 nut
- 14x Cherry MX-compatible keyswitches
- 14x keycaps for keyswitches
- 14x rectifier diodes, such as the 1N4004
- 4x PEC12-compatible rotary encoders with pushbutton, 15mm flatted shaft preferred
- 3x encoder knobs, max. diameter 22mm
- 6x rubber feet, thicker than the head of the M2.5 screws
- 1x Teensy LC, Teensy 3.x, or Teensy 4.0
- Zap-a-Gap or other adhesive that can bond your 3D-printing material
- Lots of wire and heatshrink

Instructions:
1. [Watch video](https://www.youtube.com/watch?v=72a85tWOJVY) to see construction overview
2. Fabricate 3D-printed and laser-cut parts
3. If using laser-cut Big Wheel Baseplate, glue Support Waffle onto the etched side of the Baseplate.
4. Snap keyswitches and encoders into Body
5. Using wiring diagram, build switch matrix. Take care to route wires away from surface that contacts Support Waffle.
6. Connect switch matrix and encoders to Teensy. Take care to leave extra wire!
7. Slide Teensy into its rails. It should fit snugly - if it's loose, add a bit of electrical tape to tighten things up.
8. Connect to computer, load firmware, and test
9. Using a soldering iron, apply heat-set inserts to the bosses on the Body.
10. Mount Waffle and Baseplate combo to the Body using the screws.
11. Apply rubber feet to Baseplate.
12. Slip nut into slot on Big Ass-Wheel and insert screw through side of wheel.
13. Mount Big Ass-Wheel on center encoder and tighten screw. It should turn smoothly but not wobble.
14. Glue Wheel Insert onto Big Ass-Wheel.
15. Mount remaining encoder knobs and keycaps.
16. If desired, modify firmware to bind your preferred commands to the controls.
17. Let Zack know you built the project!