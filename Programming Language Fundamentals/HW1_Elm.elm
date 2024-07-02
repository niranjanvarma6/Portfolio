--This assignment is done by Niranjan Varma and Logan Helstad

module HW1_Elm exposing (..)
import HW1_Def exposing (..)
import List exposing (length)


ins : a -> Bag a -> Bag a
ins a b = case b of 
    [] -> [(a, 1)]
    x::xs -> if Tuple.first x == a then [(a, Tuple.second x + 1)] ++ xs else x :: ins a xs--increment the second tuple value and return the list else call the function 

del : a -> Bag a -> Bag a
del a b = case b of 
    [] -> []
    x::xs -> if Tuple.first x == a then delete_tup a b else x :: del a xs

delete_tup : a -> Bag a -> Bag a
delete_tup a b = case b of
    [] -> []
    x::xs -> if Tuple.second x - 1  == 0 then [] ++ xs else [(a, Tuple.second x - 1)] ++ xs

bag : List a -> Bag a
bag a = case a of 
    [] -> []
    x::xs -> ins x (bag xs)

subbag : Bag a -> Bag a -> Bool
subbag a b = case a of 
    [] -> True
    x::xs -> case b of
        []-> False
        y::ys -> if x /= y then False  else subbag xs ys

isSet : Bag a -> Bool
isSet a = case a of 
    [] -> True
    x::xs -> if Tuple.second x /= 1 then False else isSet xs

size : Bag a -> Int
size a = case a of 
    [] -> 0
    x::xs -> Tuple.second x + size xs


nodes : Graph -> List Node
nodes a = asSet (nodes1 a)

nodes1 : Graph -> List Node
nodes1 a = case a of 
    [] -> []
    x::xs -> Tuple.first x:: Tuple.second x::nodes xs

suc : Node -> Graph -> List Node
suc a b = case b of 
    [] -> []
    x::xs -> if Tuple.first x == a then (Tuple.second x::(suc a xs)) else suc a xs

detach : Node -> Graph -> Graph
detach a b = case b of 
    [] -> []
    x::xs -> if Tuple.first x == a || Tuple.second x == a then detach a xs else x::detach a xs


width : Shape -> Length
width a = case a of
    Pt point -> 0
    Circle point w -> w *2
    Rect point w h -> w

bbox : Shape -> BBox
bbox a = case a of
    Pt (p1, p2) -> ((p1, p2), (p1, p2))
    Circle (p1, p2) w -> ((p1-w, p2-w), (p1+w, p2+w))
    Rect (p1, p2) w h -> ((p1, p2), (p1+w, p2+h))
    
minX: Shape -> Number
minX a = case a of
    Pt (p1, p2) -> p1
    Circle (p1, p2) w -> p1-w
    Rect (p1, p2) w h -> p1

move: Point -> Shape -> Shape
move (p1, p2) s = case s of
    Pt (p3, p4) -> Pt (p1 + p3, p2 + p4)
    Circle (p3, p4) w -> Circle (p1 + p3, p2 + p4) w
    Rect (p3, p4) w h -> Rect (p1 + p3, p2 + p4) w h





