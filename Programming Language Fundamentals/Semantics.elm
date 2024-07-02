module Semantics exposing (..)
import List exposing (..)

type Op = LD Int | ADD | MULT | DUP
type alias Prog = List Op

type alias Stack = List Int

type alias D = Stack->(Maybe Stack)

semProg : Prog -> D
semProg p s = case p of
    []    -> Just s
    o::op -> case (semOp o s) of
        Nothing  -> semProg op s 
        Just ret -> semProg op ret
 
semOp : Op -> D
semOp o s = case o of
    LD val -> Just (val::s) 
    ADD    -> case s of 
                []    -> Nothing
                w::wx -> case wx of 
                    []    -> Nothing
                    y::yz -> Just ([(y+w)]++yz)
    MULT   -> case s of 
                    []    -> Nothing
                    w::wx -> case wx of 
                        []    -> Nothing
                        y::yz -> Just ([(y+w)]++yz)
    DUP    -> case s of 
                []    -> Nothing 
                w::wx -> Just ([w]++s)
         
semADD : D
semADD s = case s of 
    []    -> Nothing
    w::wx -> case wx of 
        []    -> Nothing
        y::yz -> Just ([(y+w)]++yz)

semMULT : D
semMULT s = case s of 
    []    -> Nothing
    w::wx -> case wx of 
        []    -> Nothing
        y::yz -> Just ([(y*w)]++yz)

semDUP : D
semDUP s = case s of 
    []    -> Nothing 
    w::wx -> Just ([w]++s)
