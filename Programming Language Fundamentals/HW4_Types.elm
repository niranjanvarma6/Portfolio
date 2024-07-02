module HW4_Types exposing (..)

-- Exercise 1
--
type Op = LD Int | ADD | MULT | DUP | DEC | SWAP | POP Int
type alias Prog = List Op

type alias Rank = Int
type alias OpRank = (Int, Int)


-- a
rankOp : Op -> OpRank
rankOp c = case c of
    LD _  -> (0,1)
    ADD   -> (2,1)
    MULT  -> (2,1)
    DUP   -> (1,2)
    DEC   -> (1,1)
    SWAP  -> (2,2)
    POP k -> (k,0)

rankP : Prog -> Maybe Rank
rankP cs = rank cs 0

rank : Prog -> Rank -> Maybe Rank
rank p s = case p of
    []    -> Just s
    c::cs -> let (n,m) = rankOp c in
                 if s<n then Nothing
                        else rank cs (s-n+m)

-- b
type alias Stack = List Int

semOp : Op -> Stack -> Stack
-- semOp _ _ = Debug.todo "Runtime Error"
semOp _ _ = []
-- semOp c s = case (c,s) of
--     (LD i,_)          -> i::s
--     (DUP, v::vs)      -> v::v::vs
--     (ADD, v1::v2::vs) -> v1+v2::vs
--     (MULT,v1::v2::vs) -> v1*v2::vs
--     (DEC, v::vs)      -> v-1::vs
--     (SWAP,v1::v2::vs) -> v2::v1::vs
--     _                 -> Debug.todo "Runtime Error"

semProg : Prog -> Stack -> Stack
semProg p s = case p of
    []    -> s
    c::cs -> semProg cs (semOp c s)

semTC : Prog -> Maybe Stack
semTC p = case rankP p of
        Nothing -> Nothing
        Just r  -> Just (semProg p [])

------------------------------------------------------------------------
-- 2. ------------------------------------------------------------------
------------------------------------------------------------------------

type Shape = X
           | TD Shape Shape
           | LR Shape Shape

type alias BBox = (Int,Int)

-- a
bbox : Shape -> BBox
bbox s = case s of
    X        -> (1,1)
    TD s1 s2 -> let (x1,y1) = bbox s1
                    (x2,y2) = bbox s2
                 in (max x1 x2,y1+y2)
    LR s1 s2 -> let (x1,y1) = bbox s1
                    (x2,y2) = bbox s2
                 in (x1+x2,max y1 y2)

-- b
fromBool : Bool -> a -> Maybe a
fromBool b x = if b then Just x else Nothing

rect : Shape -> Maybe BBox
rect s = case s of
    X        -> Just (1,1)
    TD s1 s2 -> case (rect s1,rect s2) of
                    (Just (x1,y1),Just (x2,y2)) ->
                       fromBool (x1==x2) (x1,y1+y2)
                    _ -> Nothing
    LR s1 s2 -> case (rect s1,rect s2) of
                    (Just (x1,y1),Just (x2,y2)) ->
                        fromBool (y1==y2) (x1+x2,y1)
                    _ -> Nothing
