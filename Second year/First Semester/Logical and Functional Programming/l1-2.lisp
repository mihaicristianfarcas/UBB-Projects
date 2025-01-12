; Write a function to return the product of two vectors.
; dot-prod(l1, l2, ..., ln, v1, v2, ..., vm) = 0, if length of l or length of v = 0
;                                            = nil, if length of l != length of v
;                                            = l1 * v1 + dot-prod(l2, ..., ln, v2, ..., vm), n and m > 0

(defun dot-prod(l v)
    (cond
        ( (or (= (length l) 0) (= (length v) 0)) 0)
        ( (/= (length l) (length v)) nil)
        ( t (+ (* (car l) (car v)) (dot-prod (cdr l) (cdr v))))
    )
)

; (print (dot-prod '(1 2) '(2 3)))

; Write a function to return the depth of a list. e.g. the linear list has depth 1.
; list-depth(l1, l2, ..., ln) = 0, n = 0
;                             = 0, if l is not a list
;                             = 1 + max(list-depth(l1), ..., list-depth(ln)), oth.

(defun list-depth (lst)
    (cond 
        ((null lst) 0)
        ((not (listp lst)) 0)
        (t (+ 1 (apply #'max (mapcar #'list-depth lst))))
    )
)

; (print (list-depth '(1 2 3)))
; (print (list-depth '((1 2) 3)))
; (print (list-depth '(((1 ((((2)))) 3)))))

; Write a function to sort a linear list without keeping duplicates.

(defun insert-ordered (elem lst)
  (cond 
    ((null lst) (list elem))
    ((<= elem (car lst)) (cons elem lst))
    (t (cons (car lst) (insert-ordered elem (cdr lst))))))

(defun sort-my-list (l)
  (cond
    ((null l) nil)
    (t (let ((first (car l))
             (sorted-rest (sort-my-list (cdr l))))
         (if (already-exists first sorted-rest)
             sorted-rest
             (insert-ordered first sorted-rest))))))

(defun already-exists (elem lst)
  (cond
    ((null lst) nil)
    ((= elem (car lst)) t)
    (t (already-exists elem (cdr lst)))))

; (print (sort-my-list '(3 1 4 1 5 9 2 6 5 3 5)))
; (print (sort-my-list '(5 5 5 3 3 2 2)))

; Write a function to return the intersection of two sets.
; merge-sets(l1, l2, ..., ln, v1, v2, ..., vm) = nil, n = 0 and m = 0
;                                              = v, n = 0
;                                              = l, m = 0
;                                              = merge-sets(l2, ..., ln, v1, v2, ..., vm), l1 is a member in v
;                                              = l1 U merge-sets(l2, ..., ln, v1, v2, ..., vm), l1 is not a member in v

(defun merge-sets(l v)
    (cond 
        ((and (null l) (null v)) nil)
        ((null l) v)
        ((null v) l)
        ((member (car l) v) (merge-sets (cdr l) v))
        (t (cons (car l) (merge-sets (cdr l) v)))
    )
)

(print (merge-sets '(1 2 3 4) '(1 2 5 6)))