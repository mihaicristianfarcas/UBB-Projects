; A linear list is given, eliminate each element on position n, 2n ...
; pixar-lamp(l1, l2, ..., ln, N, cnt) = [], n = 0
;                                     = l1 U pixar-lamp(l2, l3, ..., ln, N, cnt + 1), n > 0 and cnt % N != 0
;                                     = pixar-lamp(l2, l3, ..., ln), n > 0 and cnt mod N = 0
(defun pixar-lamp (l n cnt)
  (cond
    ((null l) nil)
    ((= (mod cnt n) 0) (pixar-lamp (cdr l) n (+ cnt 1)))
    (t (cons (car l) (pixar-lamp (cdr l) n (+ cnt 1))))
    )
)

; (print (pixar-lamp '(1 2 3 4 5 6) 2 1))

; Test if a list has a valley aspect
; are-u-a-valley(l1, l2, ..., ln, f) = true, n = 0 and f = 1
;                                    = valley(l2, l3, ..., ln, 0), l1 > l2 and f = 0
;                                    = valley(l2, l3, ..., ln, 1), l1 < l2 and f = 0
;                                    = valley(l2, l3, ..., ln, 1), l1 < l2 and f = 1
;                                    = false, oth.

(defun are-u-a-valley (l f)
  (cond
    ((null l) (= f 1))
    ((null (cdr l)) (= f 1)) 
    ((and (> (car l) (cadr l)) (= f 0))
     (are-u-a-valley (cdr l) 0))
    ((and (< (car l) (cadr l)) (= f 0))
     (are-u-a-valley (cdr l) 1))
    ((and (< (car l) (cadr l)) (= f 1))
     (are-u-a-valley (cdr l) 1))
    (t nil)
    )
)

; (print (are-u-a-valley '(10 9 8 9 10) 0))  ; Should print T
; (print (are-u-a-valley '(10 8 9 8) 0))     ; Should print NIL

; Build a function that returns the minimum numeric atom from a list at any level.
; min-search(l1, l2, ..., ln) = l1, n = 1 and l1 is an atom
;                             = min-search(l1), l1 is a l
;                             = l1, n > 1 and l1 > min-search(l2, l3, ..., ln)
;                             = min-search(l2, l3, ..., ln), oth.

(defun min-search(l)
    (cond
        ((and (= (length l) 1) (atom (car l))) (car l))
        ((listp (car l)) (min-search (car l)))
        ((and (atom (car l)) (< (car l) (min-search (cdr l)))) (car l))
        (t (min-search (cdr l)))
    )
)

; (print (min-search '(1 (-1 3) 2 3 (a) 5 2)))

; Write a function that deletes all occurences of the maximum element of that list.
(defun delete-max (lst)
  (let ((max-elem (apply #'max lst)))
    (remove max-elem lst)))

(print (delete-max '(1 2 3 2 4 2 4 1)))
(print (delete-max '(1 1 1)))
(print (delete-max '(5 4 5 3 5 2 1)))

; Variants:

(defun delete-max-explicit (lst)
  (let* ((max-elem (reduce #'max lst))     ; Find maximum using reduce
         (filtered (remove-if              ; Remove elements that
                    #'(lambda (x)          ; satisfy the predicate
                        (= x max-elem))    ; "equals maximum"
                    lst)))
    filtered))


(defun find-max (lst)
  (if (null (cdr lst))
      (car lst)
      (max (car lst) (find-max (cdr lst)))))

(defun delete-max-recursive (lst)
  (if (null lst)
      nil
      (let ((max-elem (find-max lst)))
        (cond
          ((null lst) nil)
          ((= (car lst) max-elem)
           (delete-max-recursive (cdr lst)))
          (t
           (cons (car lst) 
                 (delete-max-recursive (cdr lst))))))))

(print (delete-max-recursive '(1 2 3 2 4 2 4 1))) ; Should print (1 2 3 2 2 1)