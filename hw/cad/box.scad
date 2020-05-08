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

/* [Components - Cherry MX] */
cmxDrawGhosts = true;
cmxKeycapType = "dcs";  // [dcs, dsa]
cmxSize = 14.0;
cmxHoleSize = [20.5, 20.5, 5.75];
cmxHoleWallThickness = 2.5;
cmxReducedSize = 20;
cmxThickness = 1.5;

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

/* [Rendering] */
renderPanel = true;
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

/****************************
    Components
*****************************/

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
        translate([0, 0, +(mulHoleSize.z)/2 + $itsy/2]) cube(mulHoleSize + [0, 0, $itsy], center = true);
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
      let(studSize = [40, 9]) {
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
      HexBoltStud(studHeight, 1, studHeight/2);
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

module Component_LightToggle_Studs_v2(height=$itsy, hollow=true, additional_radius=0) {
  difference() {
    cylinder(height, 5.5+additional_radius, 5.5+additional_radius, center=true);
    if(hollow) {
      cylinder(height+$bitsy, 2, 2, center=true);
    }
  }
}

module Component_LightToggle_Studs(top=true, bottom=true, circuit_thickness=2) {
  /* Breadboard */
  %translate([0, 0, -8-circuit_thickness/2])
  cube([12.3, 45, circuit_thickness], center=true);
  /* Studs */
  if (top) {
    translate([0, -15, (-8-fpThickness/2)/2])
    {
      difference() {
        /* The stud's body */
        union() {
          Hexagon(9, 8-fpThickness/2);
        }      
        /* The nut holder */
        hull() {
          Hexagon(6.5, 3.5);
          translate([0, -10, 0])
          Hexagon(6.5, 3.5);
        }
        /* The axis */
        cylinder(10, 2, 2, center=true);
      }
    }
  }
  if(bottom) {
    translate([0, 15, (-8-fpThickness/2)/2])
    {
      difference() {
        /* The stud's body */
        union() {
          Hexagon(9, 8-fpThickness/2);
        }      
        /* The nut holder */
        hull() {
          Hexagon(6.5, 3.5);
          translate([0, 10, 0])
          Hexagon(6.5, 3.5);
        }
        /* The axis */
        cylinder(10, 2, 2, center=true);
      }
    }
  }
}

module Component_LightToggle(hcolor="Blue", holder=false, side=true, $itsy=$itsy) {
  /* Check if we're doing holders */
  if (holder) {
    /* Rotate everything since we're upside down */
    color(fpColor)
    rotate([180, 0, 0]) 
    translate([0, 0, fpThickness]) {
      /* Add the studs */
      rotate([0, 0, side ? 0 : 90]) {
        union() {        
          /* If not side, rotate */
          /* The studs */
          translate([ltglSize.x/2 + ltglStudThickness + 6/2, 0, 0])
          rotate([0, 0, 0])
          HexBoltStud(ltglStudHeight, ltglStudThickness, ltglStudHeight/2);
          /* The studs */
          translate([-(ltglSize.x/2 + ltglStudThickness + 6/2), 0, 0])
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
    let (studTopDiameter = 2*fpssWallThickness + fpssStudInsertDiameter)
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

module Part_Box() {
  fullHeight = boxOvershoot + boxHeight;
  /* Box is red */
  union() {
    /* Make box */
    if (renderBoxAround) {
      color(boxColor)
      
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
        Component_LightToggle_Studs_v2(boxOverhangHeight*2, false, 1);
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
if (renderPanel) {
  Part_Panel();
}

if (renderBox) {
  Part_Box();
}

*union() {
  let(studTopHeight = fpssAxisHeight + fpssStudHeight + fpssStudInsertHeight)
  let (studTopDiameter = 2*fpssWallThickness + fpssStudInsertDiameter) {
    /* Top stud */
    translate([0, 0, studTopHeight])
    Component_ScrewStudTop(
      fpssAxisDiameter, fpssAxisHeight,
      fpssBaseThickness, fpThickness/2, fpssBevelHeight, 
      fpssStudDiameter, fpssStudHeight, 
      fpssStudInsertHeight, fpssStudInsertDiameter, 
      fpssWallThickness);     
    /* Bottom stud */
    translate([0, 0, -30]) {
      Component_ScrewStudBottom(
        30 /*height*/, studTopDiameter + fpssDockThickness,
        fpssAxisDiameter, fpssCapHeight, /*capBevelHeight*/3,
        fpssDockHeight, studTopDiameter, 
        fpssBaseThickness, boxThickness/2, fpssBevelHeight,
        fpssWallThickness);
      translate([0, 0, -boxThickness/2])
      cylinder(boxThickness, 
        (studTopDiameter + fpssDockThickness)/2 - fpssWallThickness,
        (studTopDiameter + fpssDockThickness)/2 - fpssWallThickness, center=true);
    }
    
  }
}