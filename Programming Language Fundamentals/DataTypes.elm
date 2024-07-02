module DataTypes exposing(..)

type Tree a = Node a (Tree a)  (Tree a)
            | Leaf

lft = Node 3 (Node 1 Leaf Leaf) (Node 5 Leaf Leaf)
tr = Node 6 lft (Node 9 Leaf(Node 8 Leaf Leaf))

inorder : Tree a -> List a 
inorder t = case t of 
    Leaf        -> []
    Node x l r -> inorder l ++ [x] ++ inorder r

preorder : Tree a -> List a
preorder t = case t of 
    Leaf        -> []
    Node x l r -> preorder l ++ [x] ++ preorder r

find: a -> Tree a -> Bool
find x t = case t of 
    Leaf        -> []
    Node y l r -> x ==y || find x l || find x r