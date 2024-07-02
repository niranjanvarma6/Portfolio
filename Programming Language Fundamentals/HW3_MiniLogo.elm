module HW3_MiniLogo exposing (..) 

-- this code written by Logan Helstad and Niranjan Varma

import Html exposing (Html)
import Svg exposing (..)
import Svg.Attributes exposing (..)

-- Elm/SVG auxiliary definitions
--
scale = 30

xPt : Int -> String
xPt i = String.fromInt (scale+i*scale)

yPt : Int -> String
yPt i = String.fromInt (398-i*scale)

svgLine : Line -> Svg a
svgLine ((a,b),(c,d)) = line
    [ x1 (xPt a), y1 (yPt b), x2 (xPt c), y2 (yPt d)
    , stroke "green", strokeWidth "4", strokeLinecap "round"] []

main : Html msg
main = svg [viewBox "0 0 400 400", width "800", height "800"]
           (List.map svgLine logoResult)

----- BEGIN HW3 solution

type alias Point = (Int,Int)

type Cmd = Pen Mode
         | MoveTo Point
         | Seq Cmd Cmd

type Mode = Up | Down

type alias Line = (Point,Point)
type alias Lines = List Line
type alias State = (Mode,Point)

semCmd : Cmd -> State -> (State,Lines)
semCmd a b = case a of 
    MoveTo p  -> case b of 
        (Up, j) -> ((Up, p), [])
        (Down, j) -> ((Down, p), [(j, p)])
    Pen p -> case b of 
        (mode, j) -> ((p, j), ([]))
    Seq e1 e2 -> case (semCmd e1 b) of 
        (d, g) -> case (semCmd e2 d) of
            (e, r) -> (e, (g++r))

lines : Cmd -> Lines
lines a = Tuple.second(semCmd a (Up, (0,0)))

logoResult : Lines
logoResult = lines (Seq (Seq (Seq (Pen Up) (Seq (MoveTo (0,0)) (Seq (Pen Down) (MoveTo (0,1))))) (MoveTo (1,1))) (Seq (MoveTo (1,2)) (MoveTo (2,2))))
