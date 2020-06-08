/* [Globals] */
$fn = 32;
$itsy = 10;
$bitsy=1;

/****************************
    Parameters
*****************************/

/* [Box] */
boxHeight = 40;
boxBevel = 3;
boxOvershoot = 3;
boxOverhang = 6;
boxOverhangHeight = 6;
boxColor = "Red";
boxThickness = 4;
boxPosNucleo = [-82, -30];

/* [Backplate] */
bpWidth = 100;
bpHeight = 40;
bpColor = "Crimson";
bpCornerRadius = 10;
bpDock = [2.5, 0];
bpDockThickness = 2.1;
bpDockScrewPadding = 7.5;
bpDockScrewDiameter = 4;
bpNotchPosition = -12;
bpNotchSize = [33.5, 8];
bpNotchScrewDistance = 9.5;
bpNotchScrewDiameter = 4;
bpInset = [2.1, 2.1];
bpInsetThickness = 1;

/* [Front panel] */
fpWidth = 180;
fpLength = 140;
fpColor = "DarkOrange";
fpCornerRadius = 20;
fpThickness = 4;
fpGutterGhostColor = "#4040409F";
fpLcdZoneColor = "#303030";
fpLcdGutterDistance = 50;
fpLcdGutterSize = 2;
fpLcdGutterDepth = 1;
fpPosLcd = [20.1, 30];
fpPosSpeaker = -20.1;
fpPosSelector = 10;

/* [Front panel - Screwing studs] */
fpssPosition1 = [77, 50];
fpssPosition2 = [-74, 56];
fpssPosition3 = [-47, -28];
fpssPosition4 = [77, -32];
fpssAxisDiameter = 4;
fpssAxisHeight = 2;
fpssBaseThickness = 5;
fpssBevelHeight = 5;
fpssCapHeight = 6;
fpssDockHeight = 1.5;
fpssDockThickness = 1.5;
fpssStudDiameter = 6.75;
fpssStudHeight = 11;
fpssStudInsertHeight = 3;
fpssStudInsertDiameter = 8.5;
fpssWallThickness = 1.5;

/* [Front panel - Groups] */
fpgOutsideMargin = 16;
fpgInsideMargin = [16, 16];
fpgLeftRelSize = 3;
fpgRightRelSize = 4;
fpgTop = 0;
fpgLength = 60;
fpgShowAlign = false;

/* [Front panel - Text] */
fptFont = "DejaVu Sans Mono:style=Bold";
fptColorNormal = "White";
fptColorAlt = "DeepSkyBlue";
fptHeight = 0.11;
fptOffset = 0.15;
fptLineSpacing = 1.51;
fptSpacing = 0.25;
fptSize = 3.1;

/* [Components - Cherry MX] */
cmxDrawGhosts = true;
cmxKeycapType = "dcs";  // [dcs, dsa]
cmxSize = 14.0;
cmxHoleCornerRadius = 1.5;
cmxHoleSize = [20.5, 20.5, 5.75];
cmxHoleWallThickness = 2.5;
cmxReducedSize = 20;
cmxThickness = 1.45;

/* [Components - LCD 20x4] */
lcdDrawGhosts = true;
lcdSize = [89.5, 42.5];

/* [Components - Rotary encoders] */
rotDrawGhosts = true;
rotDiameter = 7.3;

/* [Components - Toggle] */
tglDrawGhosts = true;
tglGhostColor = "Grey";
tglDiameter = 6.5;

/* [Components - Light toggle] */
ltglDrawGhosts = true;
ltglBottomMargin = 0.5;
ltglGhostColor = "#303030";
ltglSize = [17.5, 17.5];
ltglStudHeight = 7.2;
ltglStudThickness = 1;

/* [Components - Speaker] */
spkDrawGhosts = true;
spkDiameter = 40;
spkHeight = 6.5;
spkLatticeDiameter = 38;
spkLatticeSize = 2.2;
spkSpikeCount = 6;
spkSpikeRotation = 0;
spkSpikeOffset = 1.5;
spkSpikeHeight = 3;
spkSpikeRadius = 0.75;
spkStudHeightSlop = 0.5;
spkStudOffset = 3;
spkStudRotation = 90;

/* [Components - Nucleo] */
nucleoDrawGhosts = true;
nucleoHideBottom = true;

/* [Connectors - MIDI DIN] */
midiCenterDiameter = 17;
midiScrewDiameter = 3.5;
midiDrawGhosts = true;

/* [Connectors - Power] */
pwrJackDiameter = 13;
pwrSwitchHoleSize = [19.1, 13.5];
pwrJackDrawGhosts = true;
pwrSwitchDrawGhosts = true;

/* [Connectors - USB] */
usbDiameter = 24.9;
usbScrewDiameter = 4.5;
usbDrawGhosts = true;

/* [Connectors - Jacks] */
jack3mmDiameter = 4.5;
jack6mmDiameter = 10.5;
jack3mmDrawGhosts = true;
jack6mmDrawGhosts = true;

/* [Visualisation] */
explode = 0; // [0: 30]

/* [Subrendering] */
renderBoxAround = true;
renderBoxOverhang = true;
renderBoxNucleo = true;
renderBoxScrewStuds = true;
renderPanelPane = true;
renderPanelGutters = true;
renderPanelParts = true;
renderPanelScrewStuds = true;
renderPanelKeyboard = true;
renderPanelKeyboardText = true;
renderBackplateParts = true;

/* [Rendering] */
renderSeparatePanel = false;
renderSeparatePanelGutter = false;
renderPanel = true;
renderBackplate = true;
renderBox = true;


/****************************
        Globals
****************************/

/* [Hidden] */
helperHeight = 3;
fullWidth = fpWidth - 2*fpgOutsideMargin;
sideHeight = (fpgLength - fpgInsideMargin.y) / 2;
fpgLeftRightRatio = fpgLeftRelSize / (fpgLeftRelSize + fpgRightRelSize);
leftSideWidth = fpgLeftRightRatio * (fullWidth - fpgInsideMargin.x);
rightSideWidth = (fullWidth - fpgInsideMargin.x) - leftSideWidth;
leftCaseSize = leftSideWidth / 3;
rightCaseSize = rightSideWidth / 4;  

/****************************
    Basic shapes
*****************************/

/* A round box */
module RoundBox(x,y,z, radius) {
  translate([radius, radius])
  translate([-x/2, -y/2, -z/2])
  minkowski()
  {
    cube([x - 2 * radius, y - 2 * radius, z/2]);
    cylinder(r=radius, h=z/2, $fn=100);
  }
}

/* A round box, bevelled */
module RoundBevelBox(x,y,z, radius, bevel) {
  intersection() {
    minkowski() {
      translate([0, 0, bevel])
      RoundBox(x-2*bevel, y-2*bevel, z, radius-bevel);
      sphere(bevel);
    }
    RoundBox(x, y, z, radius);
  }
}

/* A round box ring */
module RoundBoxRing(x,y,z, thickness, radius) {
  difference() {
    RoundBox(x, y, z, radius);
    RoundBox(x - 2*thickness, y - 2*thickness, z + $itsy, radius);
  }
}

module Hexagon(size, height) {
  boxWidth = size/1.75;
  for (r = [-60, 0, 60])
    rotate([0,0,r])
      cube([boxWidth, size, height], true);
}

module CircularBevelProfile(size) { 
  difference() {  
    square(size);
    translate(size)
    intersection() {        
      /* Make circle */
      resize(newsize=2*size)
      circle(10);
      /* Select quarter */
      translate(-size)
      square(size);
    }
  }
}

module HexBoltStud(height=10, thickness=2, nutHeight=3, nutDiameter = 6, nutThickness=3.5, axisDiameter = 4, $fn=12) {
  difference() {
    /* Make the stud */
    cylinder(height, nutDiameter/2 + thickness, nutDiameter/2 + thickness);
    /* Make the hex bolt insert */
    translate([0, 0, nutHeight])
    hull() {
      Hexagon(nutDiameter, nutThickness);
      translate([nutDiameter + thickness, 0, 0])
      Hexagon(nutDiameter, nutThickness);
    }
    /* Remove the axis */
    translate([0, 0, $itsy/2])
    cylinder(height+$itsy, axisDiameter/2, axisDiameter/2, center=true);
  }
  
}

module RoundPlateWithHandles(radius=10, axisDistance=3, thickness=1.5, handleThickness=2, handleAxisDiameter=4, $fn = 16) {
  linear_extrude(height=thickness) {
    union() {
      /* Round plate */
      circle(radius);
      /* Add handle */      
      rotate([0, 0, 180])
      difference() {
        hull() {
          translate([radius + axisDistance, 0, 0])
          circle(handleThickness + handleAxisDiameter);
          circle(handleThickness + handleAxisDiameter);
        }
        translate([radius + axisDistance, 0, 0])
        circle(handleAxisDiameter/2);
      }
      /* Add handle */      
      rotate([0, 0, 0])
      difference() {
        hull() {
          translate([radius + axisDistance, 0, 0])
          circle(handleThickness + handleAxisDiameter);
          circle(handleThickness + handleAxisDiameter);
        }
        translate([radius + axisDistance, 0, 0])
        circle(handleAxisDiameter/2);
      }
    }
  }
}

module PanelGutterSeparator() {
  let (carveHeight = fpThickness / 2)
  let (spkCarveRadius = 1.5+spkLatticeDiameter/2)
  translate([0, 0, boxHeight]) 
  translate([0, fpPosLcd.y]) {
    union() {
      /* The main gutter carve */
      translate([0, 0, -carveHeight/2])
      cube([fpWidth+$itsy, fpLcdGutterDistance - fpLcdGutterSize, carveHeight], center=true); 
      /* The speaker carve */
      translate([fpPosSpeaker, 0, -($itsy + fpThickness)/2])
      cylinder($itsy + fpThickness, spkCarveRadius, spkCarveRadius);
    }
  }      
}

/****************************
    Components
*****************************/

module Component_MIDI(holeColor=bpColor) {  
  /* Draw ghosts */
  if (midiDrawGhosts) {
    %color("#3030307F")
    translate([-14.65, -3.4, -9.6])
    import("models/connector_midi.stl", 3);
  }
  /* The holes */
  color(holeColor)
  union() {
    /* Main hole */
    rotate([90, 0, 0])
    cylinder($itsy + bpDockThickness + bpInsetThickness, midiCenterDiameter/2, midiCenterDiameter/2, center=true);
    
    /* Screw 1 */
    rotate([90, 0, 0])
    translate([11, 0, 0])
    cylinder($itsy + bpDockThickness + bpInsetThickness, midiScrewDiameter/2, midiScrewDiameter/2, center=true);
    
    /* Screw 2 */
    rotate([90, 0, 0])
    translate([-11, 0, 0])
    cylinder($itsy + bpDockThickness + bpInsetThickness, midiScrewDiameter/2, midiScrewDiameter/2, center=true);
  }
}

module Component_PowerSwitch(holeColor=bpColor) {  
  /* Draw ghosts */
  if (pwrSwitchDrawGhosts) {
    %color("#3030307F")
    translate([0, 0, -14.2])
    import("models/connector_switch.stl", 3);
  }
  /* The holes */
  color(holeColor)
  union() {
    cube([pwrSwitchHoleSize.x, pwrSwitchHoleSize.y, $itsy + bpDockThickness + bpInsetThickness], center=true);
  }
}

module Component_PowerJack(holeColor=bpColor) {  
  /* Draw ghosts */
  if (pwrJackDrawGhosts) {
    %color("#3030307F")
    rotate([0, 180, 0])
    translate([-7, -7, -2])
    import("models/connector_dcjack.stl", 3);
  }
  /* The holes */
  color(holeColor)
  union() {
    cylinder($itsy + bpDockThickness + bpInsetThickness, pwrJackDiameter/2, pwrJackDiameter/2, center=true);
  }
}

module Component_USB(holeColor=bpColor) {  
  /* Draw ghosts */
  if (usbDrawGhosts) {
    %color("#3030307F")
    translate([-13, -15.5, -35])
    import("models/connector_usb.stl", 3);
  }
  /* The holes */
  color(holeColor)
  union() {
    /* Main hole */
    cylinder($itsy + bpDockThickness + bpInsetThickness, usbDiameter/2, usbDiameter/2, center=true);
    /* L screw */ 
    translate([9.5, -12, 0])
    cylinder($itsy + bpDockThickness + bpInsetThickness, usbScrewDiameter/2, usbScrewDiameter/2, center=true);
    translate([-9.5, 12, 0])
    cylinder($itsy + bpDockThickness + bpInsetThickness, usbScrewDiameter/2, usbScrewDiameter/2, center=true);
  }
}


module Component_Jack6mm(holeColor=bpColor) {  
  /* Draw ghosts */
  if (jack6mmDrawGhosts) {
    %color("#3030307F")
    translate([-15.4, -15.6, -24.6])
    import("models/connector_6mmjack.stl", 3);
  }
  /* The holes */
  color(holeColor)
  union() {
    /* Main hole */
    cylinder($itsy + bpDockThickness + bpInsetThickness, jack6mmDiameter/2, jack6mmDiameter/2, center=true);
  }
}


module Component_Jack3mm(holeColor=bpColor) {  
  /* Draw ghosts */
  if (jack3mmDrawGhosts) {
    %color("#3030307F")
    translate([-4.2, -4.55, -18])
    import("models/connector_3mmjack.stl", 3);
  }
  /* The holes */
  color(holeColor)
  union() {
    /* Main hole */
    cylinder($itsy + bpDockThickness + bpInsetThickness, jack3mmDiameter/2, jack3mmDiameter/2, center=true);
  }
}

module Component_ScrewStudTop(
  axisDiameter = 4,
  axisHeight = 10,
  baseWallThickness = 6,
  baseHeight = fpThickness/2,
  bevelHeight = 5,
  studDiameter = 6.75,
  studHeight = 11,
  studInsertHeight = 4,
  studInsertDiameter = 8,
  wallThickness = 1.5,
  squareBase=false) {
    /* Total diameter */
    totalRadius = wallThickness + (studInsertDiameter/ 2);
    totalHeight = axisHeight + studHeight + studInsertHeight;
    
    /* Merge all */
    rotate([180, 0, 0])
    union() {
      /* Base is a cube ? */
      if (squareBase) {
        translate([0, 0, -baseHeight/2])
        cube([(totalRadius+baseWallThickness)*2, (totalRadius+baseWallThickness)*2, baseHeight], center=true);
      }
      /* Rotational symmetry */
      rotate_extrude(convexity=4) {
        /* Remove center axis */
        difference() {
          /* Body */
          union() {        
            /* Shaft */
            square([totalRadius, totalHeight]);
            /* Base */
            translate([0, -baseHeight]) 
            square([totalRadius + baseWallThickness, baseHeight]);
            /* Bevel */
            translate([totalRadius, 0])
            CircularBevelProfile([baseWallThickness, bevelHeight]);
          }
          /* Axis */
          square([axisDiameter/2, totalHeight]);
          /* Stud */
          translate([0, axisHeight])
          union() {
            /* Holder */
            square([studDiameter/2, studHeight + studInsertHeight]);
            /* Insert */
            translate([0, studHeight])
            hull() {
              /* The insert */
              square([studInsertDiameter/2, studInsertHeight]);
              translate([0, -studInsertHeight])
              square([studDiameter/2, studInsertHeight]);
            }
        }
      }
    }
  }
    
}

module Component_ScrewStudBottom(

  height=30,
  diameter=10,

  axisDiameter = 4,
  
  capHeight = 10,
  capBevelHeight = 5,
  
  dockHeight = 3,
  dockDiameter = 9.5,

  baseWallThickness = 6,
  baseHeight = boxThickness/2,

  bevelHeight = 5,

  wallThickness = 1.5,
  
  squareBase=false) {   
    
  /* Radius */
  radius= diameter/2;
  insideRadius = radius - wallThickness;
    
  /* Rotational symmetry */
  rotate_extrude(convexity=4) {
    /* Remove center axis */
    difference() {
      /* Body */
      union() {        
        /* Shaft */
        square([radius, height]);
        /* Base */
        translate([0, -baseHeight]) 
        square([radius + baseWallThickness, baseHeight]);
        /* Bevel */
        translate([radius, 0])
        CircularBevelProfile([baseWallThickness, bevelHeight]);
        /* Dock */
        translate([dockDiameter/2, height])
        square([radius - (dockDiameter/2), dockHeight]);
      }
      /* Inside */
      translate([0, -baseHeight])
      square([insideRadius, height - capHeight - capBevelHeight + baseHeight]);
      
      /* Bevel on the inside */
      hull() {
        square([insideRadius, height - capHeight - capBevelHeight]);
        translate([0, height - capHeight - capBevelHeight]) square([axisDiameter/2, capBevelHeight]);
      }
      
      /* Axis */
      translate([0, -baseHeight])
      square([axisDiameter/2, height + baseHeight]);
    }
  }
    
}

module Component_CherryMX(large, keycolor="#ff00007f", hcolor="Blue", holder=false) {
  
  /* Is this the key or the holder ? */
  let (multiplier = large ? 2 : 1)
  let (mulHoleSize = [multiplier * cmxHoleSize.x, cmxHoleSize.y, cmxHoleSize.z])
  if (holder) {
    /* Add matter for cmx holes */
    color(fpColor)
    let (wallThickness = [cmxHoleWallThickness*2, cmxHoleWallThickness*2, fpThickness/2])
    let (holeSize = [mulHoleSize.x, mulHoleSize.y, mulHoleSize.z - fpThickness + cmxThickness])
    translate([0, 0, -fpThickness-(holeSize.z)/2 + fpThickness/4])
    cube(holeSize + wallThickness, center = true);
    
  } else {
    /* Make square for hole */
    translate([0, 0, -cmxHoleSize.z])
    color(hcolor)
    union() {
      /* Full hole */
      translate([0, 0, -cmxThickness/2])
      cube([cmxSize, cmxSize, cmxThickness*2], center=true);
      /* Small hole to have the right thickness */
      translate([0, 0, -cmxThickness-($itsy /2)])
      cube([cmxReducedSize, cmxReducedSize, $itsy], center=true);
      /* Top hole */
      if (cmxHoleSize.z > 0) {
        let (holepos = mulHoleSize + [0, 0, $itsy])
        translate([0, 0, +(mulHoleSize.z)/2 + $itsy/2]) 
        RoundBox(holepos.x, holepos.y, holepos.z, cmxHoleCornerRadius);
      }
    }
    
    /* If enabled, draw the ghost models */
    translate([0, 0, -cmxHoleSize.z])
    if (cmxDrawGhosts) {
      /* Draw switch*/
      translate([0, 0, 13.13])
      color("#303030")
      %import("models/mx_switch.stl", 3);
      /* Draw Key */
      translate([0, 0, 6])
      color(keycolor)
      %import(str((large ? "models/mx_key_large_" : "models/mx_key_"), cmxKeycapType, ".stl"), 3);
    }
  }
}

module Component_LCD20x4(holder=false, thickness=$itsy) {
  /* The holder */
  if (holder) {
    /* The stud height */
    studHeight = 4.75;
    /* Add the supports */
    color(fpColor)
    translate([0, 0, -fpThickness-studHeight/2 + fpThickness/4])
    union() {
      let(studSize = [60, 9]) {
        translate([0, -23])
        translate([0, -studSize.y/2])
        cube([studSize.x, studSize.y, studHeight + fpThickness/2], center=true);
      } 
      let(studSize = [35, 9]) {
        translate([22, 23, 0])
        translate([0, +studSize.y/2])
        cube([studSize.x, studSize.y, studHeight + fpThickness/2], center=true);
      } 
      let(studSize = [4, 20]) {
        translate([-50, 0])
        translate([+studSize.x/2, 0])
        cube([studSize.x, studSize.y, studHeight + fpThickness/2], center=true);
      } 
    }
    /* Add the screw studs */
    color(fpColor)
    rotate([180, 0, 0]) 
    translate([0, 0, fpThickness])
    for (i=[-1:2:1]) for(j=[-1:2:1]) {
      translate([i*46.5, j*27.6]) 
      rotate([0, 0, i > 0 ? 0 : 180])
      HexBoltStud(studHeight, 1.5, studHeight/2);
    }    
  }
  /* The holes */
  else {
    /* Make hole */
    color(fpLcdZoneColor)
    union() {
      /* The hole for the LCD */
      translate([0, 0, -thickness/2])
      cube([lcdSize.x, lcdSize.y, thickness*2], center=true);
    }
    
    if (lcdDrawGhosts) {
      %color(fpGutterGhostColor)
      rotate([90, 0, 0])
      translate([-4.65, -7.8, -9.1])
      translate([-88.7/2, -8.75, -41.8/2])
      import("models/lcd_20x4.stl", convexity=1);
    }
  }
}

module Component_Rotary(large=false, hcolor="Blue", thickness=$itsy) {
  /* The model */
  if (rotDrawGhosts) {
    %color(fpGutterGhostColor) {    
      difference() {      
        translate([-6.6, -7.45, -10.1])
        import("models/encoder.stl", convexity=4);
        
        translate([0,0,10+10])
        cube([10, 10, 20], center=true);
      }
      
      if (large) {
        translate([-10, 10, 5])
        rotate([90, 0, 0])
        %import("models/encoder_knob_large.stl", convexity=2);
      } else {
        translate([-8, -8, 6])
        %import("models/encoder_knob.stl", convexity=2);
      }
    }
  }
  
  /* Hole */
  color(hcolor)
  cylinder(thickness, rotDiameter/2, rotDiameter/2, center=true);
}

module Component_Nucleo144(thickness=$itsy) {
  /* The holder */
  translate([132, 0, 0])
  rotate([0, 0, 90])
  color("Grey")
  difference() {
    /* The model */
    translate([0, 0, !nucleoHideBottom ? 0 : -5])
    import("models/nucleo_holder.stl",convexity=4);
    /* Remove the bottom */
    translate([0, 75, -5])
    cube([200, 200, 10], center=true);
  }
  /* Ghost */
  if (nucleoDrawGhosts) {
    %translate([76.25, -24.1, 6.5])
    rotate([0, 0, 90])
    color("#505050df")
    difference() {
      /* Nucleo */
      import("models/nucleo.stl",convexity=7);
      /* Cut feet */
      translate([40-15, 10, -12])
      cube([80, 130, 20], center=true);      
    }
  }
}

module Component_SpeakerGrid(latticeSize, radius, $itsy=$itsy) {
  /* The shape to remove */
  translate([0, 0, -fpThickness/2])
  minkowski(convexity=8) {
    /* The square object */
    intersection() {
      /* The cylinder */
      cylinder(fpThickness+$itsy, radius, radius, center=true);
      /* The square lattice */
      union() {
        n = round((radius/(latticeSize*2)))+1;
        for (i = [-n+1:n+1]) {
          translate([0, i*latticeSize*2, 0])
          cube([radius*2, latticeSize/4, fpThickness+2*$itsy], center=true);
        }
      }
    }
    /* Some bevel */
    sphere((latticeSize* 3/4)/2, $fn=12);
  }
}

module Component_Speaker(radius) {
  /* The speaker */
  if (spkDrawGhosts) {
    color(fpGutterGhostColor)
    translate([0, 0, -spkHeight])
    %cylinder(spkHeight, radius, radius);
  }
  /* The studs */
  color(fpColor)
  rotate([0, 0, spkStudRotation]){
    translate([radius + spkStudOffset, 0, 0]) 
    rotate([0, 180, 180])
    HexBoltStud(spkHeight - spkStudHeightSlop);
    translate([-(radius + spkStudOffset), 0, 0])  
    rotate([0, 180, -60]) 
    HexBoltStud(spkHeight - spkStudHeightSlop);
  }
  /* The plate. It MUST be exported separately */
  rotate([0, 180, spkStudRotation])
  translate([0, 0, spkHeight - spkStudHeightSlop])
  %RoundPlateWithHandles(radius, spkStudOffset, 2);
  
  /* The spikes */
  color(fpColor)
  union() {
    /* Make n of them */
    for (a = [0:360/spkSpikeCount:360]) {
      /* Lower number of faces */
      $fn=12;
      /* Rotate it */
      rotate([0, 0, a + spkSpikeRotation])
      translate([radius + spkSpikeRadius+ spkSpikeOffset, 0, -spkSpikeHeight])
      {
        /* The cylinder */
        cylinder(spkSpikeHeight, spkSpikeRadius, spkSpikeRadius);
        rotate([0, 180, 0])
        /* Add some bevel so it holds better */
        translate([0, 0, -spkSpikeHeight])
        rotate_extrude() {
          union() {
            /* The profile */
            translate([spkSpikeRadius, 0])
            CircularBevelProfile([spkSpikeHeight/4, spkSpikeHeight/2]);
            /* The fill */
            square([spkSpikeRadius, spkSpikeHeight/2]);
            /* The base */
            translate([0, -$bitsy/4])
            square([spkSpikeRadius + spkSpikeHeight/4, $bitsy/4]);
          }
        }
      }
    }
  }
}

module Component_LightToggle(hcolor="Blue", holder=false, side=true, $itsy=$itsy) {
  /* Check if we're doing holders */
  if (holder) {
    /* Rotate everything since we're upside down */
    let (ltglSizeSide = ltglSize.y)
    let (ltglOffset = side ? 0.5 : 2)
    color(fpColor)
    rotate([180, 0, 0]) 
    translate([0, 0, fpThickness]) {
      /* Add the studs */
      rotate([0, 0, side ? 0 : 90]) {
        union() {        
          /* If not side, rotate */
          /* The studs */
          translate([ltglSizeSide/2 + ltglStudThickness + 6/2 + ltglOffset, 0, 0])
          rotate([0, 0, 0])
          HexBoltStud(ltglStudHeight, ltglStudThickness, ltglStudHeight/2);
          /* The studs */
          translate([-(ltglSizeSide/2 + ltglStudThickness + 6/2 + ltglOffset), 0, 0])
          rotate([0, 0, 180])
          HexBoltStud(ltglStudHeight, ltglStudThickness, ltglStudHeight/2);
        }
      }
    }
  }
  else {
    /* Doing holes */
    /* If enabled, draw the ghost models */
    translate([0, 0, -11.3]) 
    if (ltglDrawGhosts) {
      %union() {
        /* Draw top */
        translate([0, 0, 14.3 - 5])
        union() {
          /* Body */
          color(ltglGhostColor) {
            translate([0, 0, 1]) cube([17.1, 17.3, 2], center=true);
            translate([0, -(17.1-11.3)/2, 3.5])
            hull() {
              linear_extrude(height=3, center=true, convexity=2, scale=0.8) {
                square([17.1, 11.3], center=true);
              }
              translate([0, 0, -0.5-2])
              cube([17.1, 11.3, 0.5], center=true);
            }
            translate([-5.8, -7.3, 0])
            hull() {
              rotate([0, 90, 0])
              cylinder(1, 1, 1, center=true);
              translate([0, 0, -2])
              rotate([0, 90, 0])
              cylinder(1, 1, 1, center=true);
            }
            translate([5.8, -7.3, 0])
            hull() {
              rotate([0, 90, 0])
              cylinder(1, 1, 1, center=true);
              translate([0, 0, -2])
              rotate([0, 90, 0])
              cylinder(1, 1, 1, center=true);
            }
          }
          /* Led */
          color("DarkRed") {
            translate([0, 5.5, 2])
            minkowski() {
              cube([0.1, 0.1, 0.75], center=true);
              sphere(1.2);
            }
          }
        }
        /* Draw bottom */
        translate([0, 0, 0])
        color("White") {    
          translate([0, 0, 4])
          cube([12.3, 17.5, 8], center=true);
          translate([0, -7.3, 9])
          cube([10.3, 2, 2], center=true);
        }
      }
    }
    
    /* Holes */
    lpStudHoleHeight = fpThickness*0.75;
    /* Hole for the cap */
    color(hcolor)
    translate([0, ltglBottomMargin/2, 0])
    cube([ltglSize.x, ltglSize.y + ltglBottomMargin, $itsy], center=true);
    /* Hole for studs */
    *color(hcolor) 
    translate([0, 0, -(fpThickness + $bitsy)])
    {
        /* The axis */
        translate([0, -14.5, ((lpStudHoleHeight+$bitsy)/2)]) 
        Hexagon(6.5, lpStudHoleHeight+$bitsy);
    }
  }
}

module Component_Toggle(hcolor="Blue", thickness=$itsy) {
  /* Ghosts */
  if (tglDrawGhosts) {
    /* Load toggle switch */
    %color(tglGhostColor) {  
      translate([-13.1/2, 12.7/2, -15.75])
      rotate([90, 0, 0])
      import("models/toggle.stl", convexity=4);
    }
  }
  /* Hole */
  color(hcolor)
  cylinder(thickness, tglDiameter/2, tglDiameter/2, center=true);
}

/****************************
    Groups
*****************************/

module Group_Banks(width=leftSideWidth, height=sideHeight, caseSize=leftCaseSize, $bitsy=$bitsy/2) {
  
  /* Make the plate */
  translate([0, 0, fpThickness]) {
    difference() {
      color(fpColor)
      union() {
        /* Full plate no holes */
        translate([-$bitsy/2, -$bitsy/2, -fpThickness])
        cube([width+$bitsy, height+$bitsy, fpThickness]);
        /* Add matter for cmx holes */
        translate([caseSize/2, height/2]) {
          translate([0*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor, holder=true); 
          translate([1*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor, holder=true); 
        }  
      }
      /* Holes */
      translate([caseSize/2, height/2]) {
        translate([0*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor);
        translate([1*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor); 
        translate([2*caseSize, 0, -fpThickness]) Component_Rotary(false, fpColor);  
      }  
    }
  }  
}


module Group_Global(width=leftSideWidth, height=sideHeight, caseSize=leftCaseSize, $bitsy=$bitsy/2) {
  
  /* Make the plate */
  translate([0, 0, fpThickness]) {
    difference() {
      color(fpColor)
      union() {
        /* Full plate no holes */
        translate([-$bitsy/2, -$bitsy/2, -fpThickness])
        cube([width+$bitsy, height+$bitsy, fpThickness]);
        /* Add matter for cmx holes */
        translate([caseSize/2, height/2]) {
          translate([0.5*caseSize, 0, 0]) Component_CherryMX(true, "Red", fpColor, holder=true);
        }  
      }
      /* Holes */
      translate([caseSize/2, height/2]) {
        translate([0.5*caseSize, 0, 0]) Component_CherryMX(true, "Red", fpColor);
        translate([2*caseSize, 0, -fpThickness]) Component_Toggle(fpColor);
      }  
    }
  }  
}


module Group_Voice_Bottom(width=rightSideWidth, height=sideHeight, caseSize=rightCaseSize, $bitsy=$bitsy/2) {
  /* Make the plate */
  translate([0, 0, fpThickness]) {
    difference() {
      color(fpColor)
      union() {
        /* Full plate no holes */
        translate([-$bitsy/2, -$bitsy/2, -fpThickness])
        cube([width+$bitsy, height+$bitsy, fpThickness]);
        /* Add matter for cmx holes */
        translate([caseSize/2, height/2]) {
          translate([0*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor, holder=true);
          translate([1*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor, holder=true);
        }  
      }
      /* Holes */
      translate([caseSize/2, height/2]) {
        translate([0*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor);
        translate([1*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor);
        translate([2*caseSize, 0, 0]) Component_LightToggle(fpColor);
        translate([3*caseSize, 0, -fpThickness]) Component_Rotary(false, fpColor);
      }  
    }
    /* Add light toggle stud */
    translate([caseSize/2, height/2]) 
    translate([2*caseSize, 0, 0]) Component_LightToggle(fpColor, holder=true, side = false);
  }  
}

module Group_Voice_Top(width=rightSideWidth, height=sideHeight, caseSize=rightCaseSize, $bitsy=$bitsy/2) {
  
  /* Make the plate */
  translate([0, 0, fpThickness]) {
    difference() {
      color(fpColor)
      union() {
        /* Full plate no holes */
        translate([-$bitsy/2, -$bitsy/2, -fpThickness])
        cube([width+$bitsy, height+$bitsy, fpThickness]);
        /* Add matter for cmx holes */
        translate([caseSize/2, height/2]) {
          translate([0*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor, holder=true);
        }  
      }
      /* Holes */
      translate([caseSize/2, height/2]) {
        translate([0*caseSize, 0, 0]) Component_CherryMX(false, "Red", fpColor);
        translate([2*caseSize, 0]) Component_LightToggle(fpColor);
        translate([3*caseSize, 0, -fpThickness]) Component_Rotary(false, fpColor);
      }  
    }
    /* Add light toggle stud */
    translate([caseSize/2, height/2]) 
    translate([2*caseSize, 0, 0]) Component_LightToggle(fpColor, holder=true);
  }  
}

module Group_Voice() {
  translate([0, fpgInsideMargin.y + sideHeight]) 
  Group_Voice_Top();
  Group_Voice_Bottom();
}

/****************************
    Parts
*****************************/

module Part_Group_Text(words, alt, height, caseSize=rightCaseSize) {
  /* Check if word not null */
  if (len(words) != 0)
  /* For each word */
  for (word=words) {
    /* Go to this location */
      translate([caseSize/2, 0])
      translate([word[0]*caseSize, 0, 0]) {
        /* Draw text */
        if (word[1] && !alt) {
          color(fptColorNormal)
          //resize(newsize=[0, fptSize, 0], auto=true)  
          translate([0, 0, -$itsy/2])
          linear_extrude(height) {
            offset(fptOffset)
            text(word[1], spacing=1+fptSpacing,font=fptFont, fptSize, halign="center", valign="center");
          }
        }
        /* Draw alt text */
        if (word[2] && alt) {          
          color(fptColorAlt)
          translate([0, is_undef(word[1]) ? 0 : -fptSize - fptLineSpacing])
          //resize(newsize=[0, fptSize, 0], auto=true) 
          translate([0, 0, -$itsy/2])
          linear_extrude(height) {
            offset(fptOffset)
            text(word[2], font=fptFont, spacing=1+fptSpacing, fptSize, halign="center", valign="center");
          }
        }
      }
  }
  
  
}

module Part_Keyboard_Text(alt=false, textHeight=fpThickness+$bitsy*2) {

  /* Center ourselves */
  translate([-fullWidth/2, -fpgLength + fpgTop]) 
  translate([0, fptHeight]){
      
    /* For each part */
    translate([0, 0]) 
    Part_Group_Text([[0, "SAVE"], [1, "LOAD", "SPELL"], [2, "FILE"]], alt, textHeight, caseSize=leftCaseSize);
    translate([0, fpgInsideMargin.y + sideHeight]) 
    Part_Group_Text([[0.5, undef, "ALT"], [2, "EDIT"]], alt, textHeight, caseSize=leftCaseSize);
    translate([leftSideWidth + fpgInsideMargin.x, 0]) 
    Part_Group_Text([[0, "PREV", "START"], [1, "NEXT", "END"], [2, "CMD"], [3, "CODE", "PARAM"]], alt, textHeight, caseSize=rightCaseSize);
    translate([leftSideWidth + fpgInsideMargin.x, fpgInsideMargin.y + sideHeight]) 
    Part_Group_Text([[0, "LISTEN", "EXPLAIN"], [2, "DISCRETE", "STUTTER"], [3, "PITCH", "SPEED"]], alt, textHeight, caseSize=rightCaseSize);
  }
  
}

module Part_Keyboard(holes) {
  /* Check if holes */
  if (holes) {
    /* Make the hole for the keyboard */
    color(fpColor) {
      /* Align to left */
      translate([-fullWidth/2, -fpgLength + fpgTop])
      translate([0, 0, -(fpThickness+$itsy)/2 - (fpThickness/2)])
      cube([fullWidth, fpgLength, fpThickness+$itsy]);
    }
  } else {
    /* Fill in the keyboard */
    translate([-fullWidth/2, -fpgLength + fpgTop]) 
    union() {
      /* Make the holed main plate */
      color(fpColor)
      difference() {
        /* The main plate */
        translate([-$bitsy/2, -$bitsy/2, -fpThickness])
        cube([fullWidth + $bitsy, fpgLength + $bitsy, fpThickness]);
        /* Make a hole for each section */
        translate([0, 0, -fpThickness/2])
        translate([0, 0, -(fpThickness+$itsy)/2])
        union() {
          /* Bottom left */
          translate([0, 0]) cube([leftSideWidth, sideHeight, fpThickness+$itsy]);
          /* Top left */
          translate([0, fpgInsideMargin.y + sideHeight]) 
          cube([leftSideWidth, sideHeight, fpThickness+$itsy]);
          /* Bottom right */
          translate([leftSideWidth + fpgInsideMargin.x, 0, 0]) 
          cube([rightSideWidth, sideHeight, fpThickness+$itsy]);
          /* Top right */
          translate([leftSideWidth + fpgInsideMargin.x, fpgInsideMargin.y + sideHeight, 0])
          cube([rightSideWidth, sideHeight, fpThickness+$itsy]);
        }
      }
      /* Fill it in */
      translate([0, 0, -fpThickness])
      union() {
        translate([0, 0]) Group_Banks();
        translate([0, fpgInsideMargin.y + sideHeight]) Group_Global();
        translate([leftSideWidth + fpgInsideMargin.x, 0]) Group_Voice();
      }
    }
  }
}

module Part_TopScrewStud() {
  Component_ScrewStudTop(
      fpssAxisDiameter, fpssAxisHeight,
      fpssBaseThickness, fpThickness/2, fpssBevelHeight, 
      fpssStudDiameter, fpssStudHeight, 
      fpssStudInsertHeight, fpssStudInsertDiameter, 
      fpssWallThickness);    
}

module Part_BottomScrewStud(holes=false) {
  if (holes) {
   let (studTopDiameter = 2*fpssWallThickness + fpssStudInsertDiameter)
      cylinder(boxThickness+$itsy, 
        (studTopDiameter + fpssDockThickness)/2 - fpssWallThickness,
        (studTopDiameter + fpssDockThickness)/2 - fpssWallThickness, center=true);
  }
  else {
    let (studTopDiameter = 2*fpssWallThickness + fpssStudInsertDiameter + 1.5)
    /* Compute bevel for screw */
    let (insideRadius = (studTopDiameter + fpssDockThickness)/2 - fpssWallThickness)
    let (axisRadius = fpssAxisDiameter/2)
    let (capBevelHeight = (insideRadius - axisRadius) * (2.0/2.5))
    /* Compute height */
    let(studTopHeight = fpssAxisHeight + fpssStudHeight + fpssStudInsertHeight)
    let (height = boxHeight - boxThickness - fpThickness - studTopHeight )
    /* Draw */
    Component_ScrewStudBottom(
      height, studTopDiameter + fpssDockThickness,
      fpssAxisDiameter, fpssCapHeight, capBevelHeight,
      fpssDockHeight, studTopDiameter, 
      fpssBaseThickness, boxThickness/2, fpssBevelHeight,
      fpssWallThickness);
  }
      
}

module Part_Panel() {
  /* Panel is orange */
    /* Make holes for the parts */
  
  translate([0, 0, boxHeight])
  union() {
    difference() {
      /* The panel */
      if (renderPanelPane) {
        translate([0, 0, - fpThickness/2])
        color(fpColor) {
          RoundBox(fpWidth, fpLength, fpThickness, fpCornerRadius);
        }
      }
      /* The holes for parts */
      union() {
        if (renderPanelParts) {
          /* The LCD */
          translate(fpPosLcd)
          Component_LCD20x4();
          /* The speaker grid */
          color(fpLcdZoneColor)
          translate([fpPosSpeaker, fpPosLcd.y])
          Component_SpeakerGrid(spkLatticeSize, spkLatticeDiameter/2);
          /* The selector */
          translate([fpPosSelector, fpPosLcd.y, -fpThickness])
          Component_Rotary(true, fpLcdZoneColor);
        }        
        if(renderPanelPane) {
          /* The keyboard */
          translate([0, 0, 0])
          Part_Keyboard(holes=true);
        }
      }
      /* The panel gutter */
      if (renderPanelGutters) {
        translate([0, fpPosLcd.y]) 
        union() {
          /* The gutter backdrop color */
          color(fpLcdZoneColor)
          translate([0, 0, 0])
          cube([fpWidth+$itsy, fpLcdGutterDistance, 0.05], center=true);
          /* Gutters */
          color(fpColor) {
            translate([0, -fpLcdGutterDistance/2])
            cube([fpWidth+$itsy, fpLcdGutterSize, fpLcdGutterDepth*2], center=true);
            translate([0, fpLcdGutterDistance/2])
            cube([fpWidth+$itsy, fpLcdGutterSize, fpLcdGutterDepth*2], center=true);
          }
        }
      }
    } 
    
    /* Add the keyboard */
    if (renderPanelKeyboard) {
      translate([0, 0, 0])
      Part_Keyboard(holes=false);
    }
    
    if(renderPanelPane) {
      /* Add the speaker holder*/
      translate([fpPosSpeaker, fpPosLcd.y, -fpThickness])
      Component_Speaker(spkDiameter/2);
      
      /* The text */
      if (renderPanelKeyboardText) {        
        %Part_Keyboard_Text(false);
        %Part_Keyboard_Text(true);
      }
      
      /* Add the LCD studs */
      translate(fpPosLcd)
      Component_LCD20x4(holder=true);        
      
      /* Add screw studs */
      if (renderPanelScrewStuds) {
        translate([0, 0, -fpThickness])
        color(fpColor) {
          translate(fpssPosition1) Part_TopScrewStud();
          translate(fpssPosition2) Part_TopScrewStud();
          translate(fpssPosition3) Part_TopScrewStud();
          translate(fpssPosition4) Part_TopScrewStud();
        }
      }
    }
  }
}

module Part_Backplate(holes=false) {
  /* Center is box */
  translate([0, fpLength/2 + boxThickness, boxThickness/2 + (boxHeight-boxOvershoot-boxOverhangHeight)/2])
  /* Check if hole */
  if (holes) {
    /* The main hole */
    translate([0, -boxThickness/2, 0])
    rotate([90, 0, 0])
    RoundBox(bpWidth, bpHeight, boxThickness + $itsy, bpCornerRadius);
    /* The Dock hole */
    translate([0, -(boxThickness + $itsy)/2 - (boxThickness - bpDockThickness), 0])
    rotate([90, 0, 0])
    RoundBox(bpWidth + 2*bpDock.x, bpHeight + 2*bpDock.y, boxThickness + $itsy, bpCornerRadius);
    /* The screw holes */
    translate([(bpWidth + bpDock.x)/2, 0, 0])
    rotate([90, 0, 0])
    union() {
      translate([0, bpHeight/2 - bpDockScrewPadding, 0])
      cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
      translate([0, -(bpHeight/2 - bpDockScrewPadding), 0])
      cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
    }
    /* The screw holes */
    translate([-(bpWidth + bpDock.x)/2, 0, 0])
    rotate([90, 0, 0])
    union() {
      translate([0, bpHeight/2 - bpDockScrewPadding, 0])
      cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
      translate([0, -(bpHeight/2 - bpDockScrewPadding), 0])
      cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
    }
    /* The top notch */
    translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2, bpHeight/2 - bpNotchSize.y/2])
    rotate([90, 0, 0])
    union() {          
      translate([bpNotchScrewDistance, 0, 0])
      cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
      translate([-bpNotchScrewDistance, 0, 0])
      cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
    }
    /* The bottom notch */
    translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2, -bpHeight/2 + bpNotchSize.y/2])
    rotate([90, 0, 0])
    union() {          
      translate([bpNotchScrewDistance, 0, 0])
      cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
      translate([-bpNotchScrewDistance, 0, 0])
      cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
    }
  }
  else {
    difference() {
      /* The main plate */
      color(bpColor)
      difference() {
        /* Make the plate*/
        difference() {
          color(bpColor)
          union() {
            /* The dock part */
            translate([0, -bpDockThickness/2 - (boxThickness - bpDockThickness), 0])
            rotate([90, 0, 0])
            RoundBox(bpWidth + 2*bpDock.x, bpHeight + 2*bpDock.y, bpDockThickness, bpCornerRadius);
            /* The plate part*/
            translate([0, -(boxThickness - bpDockThickness)/2, 0])
            rotate([90, 0, 0])
            RoundBox(bpWidth, bpHeight, (boxThickness - bpDockThickness), bpCornerRadius);
          }
          /* The top notch */
          translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2  + $itsy/2, bpHeight - bpNotchSize.y])
          rotate([90, 0, 0])
          RoundBox(bpNotchSize.x, bpHeight, (boxThickness - bpDockThickness) + $itsy, bpCornerRadius);
          /* The bottom notch */
          translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2  + $itsy/2, -(bpHeight - bpNotchSize.y)])
          rotate([90, 0, 0])
          RoundBox(bpNotchSize.x, bpHeight, (boxThickness - bpDockThickness) + $itsy, bpCornerRadius);
        }
        
        color(bpColor)
        union() {
          /* The top notch */
          translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2, bpHeight/2 - bpNotchSize.y/2])
          rotate([90, 0, 0])
          union() {          
            translate([bpNotchScrewDistance, 0, 0])
            cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
            translate([-bpNotchScrewDistance, 0, 0])
            cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
          }
          /* The bottom notch */
          translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2, -bpHeight/2 + bpNotchSize.y/2])
          rotate([90, 0, 0])
          union() {          
            translate([bpNotchScrewDistance, 0, 0])
            cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
            translate([-bpNotchScrewDistance, 0, 0])
            cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
          }
          
          /* The inset */
          translate([0, -(boxThickness - bpDockThickness)/2 - bpInsetThickness - $itsy/2, 0])
          union() {
            /* Switch */
            translate([65, 0, 0])
            rotate([90, 0, 0])
            RoundBox(16, bpHeight- bpInset.y, (boxThickness - bpDockThickness  + $itsy), bpCornerRadius);
            /* Jack 3mm */
            translate([-22, 0, 0])
            rotate([90, 0, 0])
            RoundBox(11.5, 13, (boxThickness - bpDockThickness  + $itsy), bpCornerRadius);
          }
          /* The screw holes */
          translate([(bpWidth + bpDock.x)/2, 0, 0])
          rotate([90, 0, 0])
          union() {
            translate([0, bpHeight/2 - bpDockScrewPadding, 0])
            cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
            translate([0, -(bpHeight/2 - bpDockScrewPadding), 0])
            cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
          }
          /* The screw holes */
          translate([-(bpWidth + bpDock.x)/2, 0, 0])
          rotate([90, 0, 0])
          union() {
            translate([0, bpHeight/2 - bpDockScrewPadding, 0])
            cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
            translate([0, -(bpHeight/2 - bpDockScrewPadding), 0])
            cylinder($itsy + bpDockThickness, bpDockScrewDiameter/2, bpDockScrewDiameter/2, center=true);
          }
        }
      }
      
      /* The components */
      rotate([0, 0, 180]) {
        /* The switch */
        translate([-65, 0, 0])
        rotate([90, 90, 0])
        Component_PowerSwitch();
        
        /* The DC jack ports */
        translate([-45, 0, 0])
        rotate([90, 90, 0])
        Component_PowerJack();
        
        /* The USB port */
        translate([-20, 0, 0])
        rotate([90, 0, 0])
        Component_USB();
        
        /* The line out jack */
        translate([6, 0, 0])
        rotate([90, 0, 0])
        Component_Jack6mm();
        
        /* The headphones jack */
        translate([22, 0, 0])
        rotate([90, 0, 0])
        Component_Jack3mm();
        
        translate([42, 0, 0])
        union() {
          /* Midi IN */
          rotate([0, 90, 0])
          Component_MIDI();
          /* Midi OUT */
          translate([20.5, 0, 0])
          rotate([0, 90, 0])
          Component_MIDI();
        }
      }
    }
    
  }
}

module Part_Box() {
  fullHeight = boxOvershoot + boxHeight;
  /* Box is red */
  union() {
    /* Make box */
    if (renderBoxAround) {
      color(boxColor)
      union() {
        difference() {
          /* The filled box */
          translate([0, 0, fullHeight/2])
          RoundBox(fpWidth + 2*boxThickness, fpLength + 2*boxThickness, fullHeight, fpCornerRadius);
          /* Hollow it */
          translate([0, 0, fullHeight/2])
          translate([0, 0, boxThickness + $itsy/2])
          RoundBevelBox(fpWidth, fpLength, fullHeight + $itsy, fpCornerRadius, boxBevel);
         /* Remove screw stud holes */
          if (renderBoxScrewStuds) {
            union()
            /*color(boxColor)*/
            translate([0, 0, boxThickness/2]) {
              translate(fpssPosition1) Part_BottomScrewStud(true);
              translate(fpssPosition2) Part_BottomScrewStud(true);
              translate(fpssPosition3) Part_BottomScrewStud(true);
              translate(fpssPosition4) Part_BottomScrewStud(true);
            }
          }
         /* Remove the backplate */
         Part_Backplate(holes=true); 
        }
        /* Add the notches */
        color(boxColor)
        translate([0, fpLength/2 + boxThickness, boxThickness/2 + (boxHeight-boxOvershoot-boxOverhangHeight)/2])
        difference() {
          /* Make the notches */
          intersection() {
            /* The dock part */
            translate([0, -(boxThickness - bpDockThickness)/2, 0])
            rotate([90, 0, 0])
            RoundBox(bpWidth, bpHeight + 2, (boxThickness - bpDockThickness), bpCornerRadius);
            /* The notches */
            union() {          
              /* The top notch */
              translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2  + $itsy/2, bpHeight - bpNotchSize.y])
              rotate([90, 0, 0])
              RoundBox(bpNotchSize.x, bpHeight, (boxThickness - bpDockThickness) + $itsy, bpCornerRadius);
              /* The bottom notch */
              translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2  + $itsy/2, -(bpHeight - bpNotchSize.y)])
              rotate([90, 0, 0])
              RoundBox(bpNotchSize.x, bpHeight, (boxThickness - bpDockThickness) + $itsy, bpCornerRadius);
            }
          }
          /* Remove the holes */
          union() {
            /* The top notch */
            translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2, bpHeight/2 - bpNotchSize.y/2])
            rotate([90, 0, 0])
            union() {          
              translate([bpNotchScrewDistance, 0, 0])
              cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
              translate([-bpNotchScrewDistance, 0, 0])
              cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
            }
            /* The bottom notch */
            translate([bpNotchPosition, -(boxThickness - bpDockThickness)/2, -bpHeight/2 + bpNotchSize.y/2])
            rotate([90, 0, 0])
            union() {          
              translate([bpNotchScrewDistance, 0, 0])
              cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
              translate([-bpNotchScrewDistance, 0, 0])
              cylinder($itsy + bpDockThickness, bpNotchScrewDiameter/2, bpNotchScrewDiameter/2, center=true);
            }
          }
        }
      }
    }
    /* Make overhang */
    if (renderBoxOverhang) {      
      color(boxColor) 
      translate([0, 0, -boxOverhangHeight/2 + boxHeight - fpThickness])
      difference() {
        intersection() {        
          /* The inside of the box is used as the delimiter  */
          RoundBox(fpWidth + boxThickness/2, fpLength + boxThickness/2, boxOverhangHeight, fpCornerRadius);  
          /* Make the trapezoid */
          minkowski(convexity=4) {
            RoundBoxRing(fpWidth, fpLength, 0.001, 0.001, fpCornerRadius);
            cylinder(h = boxOverhangHeight, r1 = 0, r2 = boxOverhang, center = true, $fn=3);
          }      
        }
        /* Remove the stud for one of the ltgls */
        translate([-fullWidth/2, -fpgLength + fpgTop])
        translate([leftSideWidth + fpgInsideMargin.x, 0, 0])
        translate([rightCaseSize/2, sideHeight/2])
        translate([2*rightCaseSize, 0])
        translate([0, -14.5])
        cylinder(boxOverhangHeight*2, 5.5+1, 5.5+1, center=true);
      }
    }
    /* Add screw studs */
    if (renderBoxScrewStuds) {
      color(boxColor)
      translate([0, 0, boxThickness]) {
        translate(fpssPosition1) Part_BottomScrewStud();
        translate(fpssPosition2) Part_BottomScrewStud();
        translate(fpssPosition3) Part_BottomScrewStud();
        translate(fpssPosition4) Part_BottomScrewStud();
      }
    }
    /* Make nucleo */
    if (renderBoxNucleo) {
      translate([boxPosNucleo.x, boxPosNucleo.y, boxThickness -0.1])
      Component_Nucleo144();
    }
  }
}


/****************************
    Entry point
*****************************/
translate([0, 0, explode]) {
  /* Basic panel */
  if (renderPanel) {
    Part_Panel();
  } else {
    /* Separate them ! */
    if (renderSeparatePanel) {
      difference() {
        Part_Panel(); 
        color(fpColor)
        PanelGutterSeparator();
      }
    }
    if (renderSeparatePanelGutter) {
      intersection() {
        Part_Panel(); 
        color(fpLcdZoneColor)
        PanelGutterSeparator();
      }
    }  
  }
}

/* Paint helper */
*translate([0, 0, boxHeight]) {
  let (paintZoneThickness = 0.5) 
  let (alt = false)
  intersection(){
  union() {
    /* The surface plate of the holder */
    difference() {
      /* Make the basic shape of the holder */
      difference() {      
        /* The base */
        translate([0, -fpgLength/2 - sideHeight/2 + 2, +paintZoneThickness/2])
        cube([fpWidth+4, fpgLength+2, paintZoneThickness], center=true);
        /* Cut what's too much */
        translate([0, +fpgLength/2 - fpgLength - fpLength/2 - 2, 0])
        cube([fpWidth + $itsy, fpgLength, $itsy], center=true);
      }
      /* Remove the text */
      Part_Keyboard_Text(alt, $itsy*6 + paintZoneThickness);
    }
    /* The bottom */
    translate([0, -fpLength/2 - fpThickness/2 + 1, -5 + paintZoneThickness])
    cube([20, 2, 10], center=true);
    /* The side */
    translate([-fpWidth/2 - 1, -sideHeight/2 - fpgLength/2, -5 + paintZoneThickness])
    cube([2, 20, 10], center=true);
  }
  //translate([-10, -35, -10])
  //cube([30, 30, 200]);
  }
}

if (renderBox) {
  Part_Box();
}

if (renderBackplate) {
  translate([0, explode, 0])
  Part_Backplate();
}