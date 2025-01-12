; Write a function that substitutes an element 'e' with all elements of a list 'l1' at all levels of a given list 'l'.
; subway(e, l1, ..., ln, v1, ..., vm) = nil, n = 0
;                                     = l1 U subway(e, l2, ..., ln, v1, ..., vm), n > 0 and l1 != e
;                                     = v1 U ... U vm U subway(e, l2, ..., ln, v1, ..., vm), n > 0 and l1 = e
;                                     = subway(l1), l1 is a list

(defun subway (e l l1)
  (cond
    ( (null l) nil)
    ( (listp (car l))(append (mapcar #'(lambda (x) (subway e (list x) l1)) (car l)) (subway e (cdr l) l1)))
    ( (equal e (car l)) (append l1 (subway e (cdr l) l1)))
    (t (cons (car l) (subway e (cdr l) l1)))))

(print (subway 1 '(2 1 3 1 4) '(7 8 9)))  

(print (subway 'a '((b c) (a d) e) '(x y)))  
