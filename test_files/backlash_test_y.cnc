( Y aYis backlash test )

G20           ( inches )    
G90           ( absolute positioning )

M98 P100 L5   ( call subroutine 5 times)
M2            ( stop program )

O100          ( beginning of subroutine )
F100          ( feed rate 100 in/min )
G4 P2         ( wait 2 seconds to read the dial )
G01 Y0.5      ( move to Y = 0.5" )
G01 Y0        ( move to Y = 0.0" )
G4 P2         ( wait 2 seconds so we can read the dial )
G01 Y-0.5     ( move to Y = -0.5" )
G01 Y0        ( move to Y = 0.0" )
M99           ( end of subroutine )
