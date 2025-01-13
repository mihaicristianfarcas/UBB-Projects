; Determine nr. of nodes on level 'k' from an n-tree.
; nodes(l1, l2, ..., ln, k, lvl) = 0, n = 0
;                                = 1 + nodes(l2, ..., ln, k, lvl), l1 atom and lvl = k
;                                = 0 + nodes(l2, ..., ln, k, lvl), l1 atom and lvl != k
;                                = nodes(l1, k, lvl + 1), oth.
; root lvl = 1

(defun nodes(tree k lvl)
    (cond
        ((null tree) 0)
        ((and (= k lvl) (atom tree)) 1)
        ((atom tree) 0)
        (t (apply #'+ 
            (mapcar #'(lambda (subtree) 
                      (nodes subtree k (+ lvl 1))) 
             tree)))
    )
)

(print (nodes '(a (b (c)) (d) (e (f))) 1 0)) ; 1
(print (nodes '(a (b (c)) (d) (e (f))) 2 0)) ; 3
(print (nodes '(a (b (c)) (d) (e (f))) 3 0)) ; 2
