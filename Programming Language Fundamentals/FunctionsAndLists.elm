module FunctionsAndLists exposing(..)

dbl = \x ->22*x

isEven : Int-> Bool
isEven x = case x of
    0-> True
    1-> False
    n-> isEven (n-2)

fac: Int -> Int
fac x = case x of
    1->1
    n->n*fac(n-1)

isEmpty : List a -> Bool
isEmpty l = case l of 
    [] -> True
    x::xs -> False

head : List a -> Maybe a 
head l = case l of
    x::xs-> Just x
    [] -> Nothing

length: List a -> Int
length l  = case l of 
    [] -> 0
    x::xs -> 1 + length xs

member: Int -> List Int -> Bool
member x l = case l of 
    [] -> False
    y::ys -> if x==y then True else member x ys

delete: Int -> List Int -> List Int
delete x l = case l of 
    [] -> []
    y::ys -> if x==y then delete x ys else x::delete x ys

snd : List a -> Maybe a 
snd l - case l of 
    x::y::xs -> Just y
            -> Nothing

firsts : List (a,b) -> List a 
firsts l = case l of 
    [] -> []
    -- x::xs -> Tuple.firsts x::firsts xs
    (x,_)::xs -> x::firsts xs

replFst : a-> List a -> List a 
replFst x l = case l of
