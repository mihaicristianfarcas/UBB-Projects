; Define a function that replaces one node with another in an n tree, first representation.
; swap-my-node(l1, l2, ..., ln, node1, node2) = l1, if l1 != node1
;                                             = node2, if l1 = node1
;                                             = swap-my-node(l1, node1, node2), oth.

(defun swap-my-node(tree node1 node2)
    (cond
        ((null tree) nil)
        ((and (atom tree) (eq tree node1)) node2)
        ((and (atom tree) (not (eq tree node1))) tree)
        (t (mapcar #'(lambda(subtree) (swap-my-node subtree node1 node2)) tree))
    )
)

(print (swap-my-node '(a (b (c)) (d) (e (f))) 'b 'g))
; (a (g (c)) (d) (e (f)))