; Compute the depth of a tree repr. as (node subtree1 subtree2 ...) using a MAP function.
; depth(l) = 0, l is an atom
;          = 1 + max(depth(l1), depth(l2), ..., depth(ln)), l is a list

(defun depth(tree)
    (cond
        ((atom tree) 0)
        (t (+ 1(apply #'max (mapcar #'depth tree))))
    )
)

; (print (depth '(1 2 (3) 4 (5 (6) 7)))) ; 3
; (print (depth '(1 2 (3) 4 (5 7)))) ; 2

; Write a function to remove all elements multiple of k from a nonlinear list using a MAP function.
; removeK(l, k) = [], l is a number and l % k = 0
;               = [l], (l is a number and l % k != 0) or l non-numerical atom
;               = [removeK(l1, k) U ... U removeK(ln k)], l = [l1, l2, ..., ln]

; removeKMain(l, k) = first element of removeK(l, k)

(defun removeK(l k)
    (cond
        ((and (numberp l) (= (mod l k) 0)) nil)
        ((atom l) (list l))
        (t (list (apply #'append (mapcar #'(lambda (x) (removeK x k)) l))))
        ;    \  or use mapcon #'...   /
    )
)

(defun removeKMain(l k)
    (car (removeK l k))
)

; Explanation: () is nil, aka the empty list -> when we append, it disappears; otherwise we would have had NIL instead of removing the element completely
; (1 2 (3 a 4) b) -> append (3) (a) () -> (1) () ((3 a)) (b) -> ((1 (3 a) b))


; (print (removeKMain '(1 2 (3 a 4) 5 b) 2)) ; (1 (3 a) 5 b)

; Count the leaves of a tree using MAP functions
; => we count the sublists with one element
; (a (b (d (e) (f))) (c)) -> 3

; leaf-count(l) = 0, l is an atom
;               = 1, l is a list and length of l is 1
;               = leaf-count(l1) + ... + leaf-count(ln)

(defun leaf-count(l)
    (cond
        ((atom l) 0)
        ((and (listp l) (= (length l) 1)) 1)
        (t (apply #'+ (mapcar #'leaf-count l)))
    )
)

; (print (leaf-count '(a (b (d (e) (f))) (c)))) ; 3

