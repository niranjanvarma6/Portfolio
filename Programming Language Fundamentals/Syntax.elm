module Syntax exposing (..)

import List exposing (map,concat)
import Tuple exposing (first,second)


-- Exercise 1
--

-- a
type Cmd = Pen Mode
         | MoveTo Pos Pos
         | Def String (List String) Cmd
         | Call String (List Int)
         | Sequ Cmd Cmd

type Mode = Up | Down

type Pos = I Int | Var String


-- b
vector : Cmd
vector = Def "vector" ["x1", "y1", "x2", "y2"]
         -- (Sequ (Pen Up)
         --       (Sequ (MoveTo (Var "x1") (Var "y1"))
         --             (Sequ (Pen Down)
         --                   (MoveTo (Var "x2") (Var "y2")) )))
         (Sequ (Pen Up)
         (Sequ (MoveTo (Var "x1") (Var "y1"))
         (Sequ (Pen Down)
               (MoveTo (Var "x2") (Var "y2")) )))


-- c
steps : Int -> Cmd
steps x = case x of
        1 -> (Sequ (Pen Up)
             (Sequ (MoveTo (I 0) (I 0))
             (Sequ (Pen Down)
             (Sequ (MoveTo (I 0) (I 1))
                   (MoveTo (I 1) (I 1)) ))))
        n -> (Sequ (steps (n-1))
             (Sequ (MoveTo (I (n-1)) (I n))
                   (MoveTo (I n)     (I n)) ))


-- Exercise 2
--


-- (a)
type alias NonTerm = String
type alias Term    = String

type alias Grammar = List Prod
type alias Prod    = (NonTerm,List RHS)
type alias RHS     = List Symbol
type       Symbol  = N NonTerm | T Term


-- (b)
cond : Prod
cond = ("cond",[[T "T"],
                [T "not",N "cond"],
                [T "(",N "cond",T ")"]])

stmt : Prod
stmt = ("stmt",[[T "skip"],
                [T "while",N "cond",T "do", T "{", N "stmt", T "}"],
                [N "stmt",T ";",N "stmt"]])

imp : Grammar
imp = [cond,stmt]


-- (c)
nonterminals : Grammar -> List NonTerm
nonterminals = map first


-- auxiliary functions
--
concatMap : (a -> List b) -> List a -> List b
concatMap f = concat << map f

symbolTerminals : Symbol -> List Term
symbolTerminals s = case s of
    T t -> [t]
    N _ -> []

rhsTerminals : RHS -> List Term
rhsTerminals = concatMap symbolTerminals

prodTerminals : Prod -> List Term
prodTerminals (_,rs) = concatMap rhsTerminals rs

terminals : Grammar -> List Term
terminals = concatMap prodTerminals
