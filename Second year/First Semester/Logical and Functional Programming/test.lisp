; 20. A linear list is given. Insert in the list a given element, E, after every N steps, N given.
; insert-n(l1, l2, ..., ln, N, E, pos) = [], n = 0
;                                      = l1 U E U insert-n(l2, l3, ..., ln, N, E, pos + 1), n > 0 and pos % N = 0
;                                      = l1 U insert-n(l2, l3, ..., ln, N, E, pos + 1), otherwise

; adaug(l1, l2, ..., ln, N, E) = insert-n(l1, l2, ..., ln, N, E, 1).

(defun insert-n(l N E pos)
    (cond
        ((null l) nil)
        ((= (mod pos N) 0) (cons (car l) (cons E (insert-n (cdr l) N E (+ pos 1)))))
        (t (cons (car l) (insert-n (cdr l) N E (+ pos 1))))
    )
)

(defun adaug(l N E)
    (insert-n l N E 1)
)

(print (adaug '(1 2 3 4 5 6 7 8 9 10) 3 7))