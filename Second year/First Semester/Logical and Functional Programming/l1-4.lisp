; L1
; 4.
; a. Write a function to return the sum of two vectors.
; total-sum-vectors(l1, l2, ..., ln, c1, c2, ..., cm) = [], if l and c are not lists
;                                                     = l1 + l2 + ... + ln + c1 + c2 + ... + cm, otherwise

(defun total-sum-vectors (v1 v2)
  "Returns the total sum of two vectors."
  (if (and (listp v1) (listp v2))
      (+ (reduce #'+ v1) (reduce #'+ v2)) ; reduce - to apply a function to a list
      NIL))

(print (total-sum-vectors '(1 2 3) '(4 5 6))) ; 21
(print (total-sum-vectors '(10 20) '(30 40))) ; 100


; b. Write a function to get from a given list the list of all atoms, on any level, but on the same order. 
; Example: (((A B) C) (D E)) ==> (A B C D E)
; flatten-list(l1, l2, ..., ln) = [], if l is empty
;                               = l1 U flatten-list(l2, l3, ..., ln), if l1 is not a list
;                               = flatten-list(l1) U flatten-list(l2, l3, ..., ln), if l1 is a list

(defun flatten-list (lista)
  "Returns a list of all atoms from a nested list, in the same order."
  (if (null lista)
      NIL
      (if (atom (car lista))
          (cons (car lista) (flatten-list (cdr lista)))
          (append (flatten-list (car lista)) (flatten-list (cdr lista))))))

(print (flatten-list '(((A B) C) (D E)))) ; (A B C D E)
(print (flatten-list '(1 (2 (3 4)) (5 6)))) ; (1 2 3 4 5 6)


; c. Write a function that, with a list given as parameter, inverts only continuous sequences of atoms. 
; Example: (a b c (d (e f) g h i)) ==> (c b a (d (f e) i h g))
; collect-atoms(l1, l2, ..., ln) = [], if n = 0 or l1 is not an atom
;                                = l1 U collect-atoms(l2, l3, ..., ln), otherwise
;
; reverse-atoms(l1, l2, ..., ln) = [], n = 0
;                                = reverse(collect-atoms(l1)) U reverse-atoms(l_length(reverse(collect-atoms(l1)), ..., ln), l1 is not a list
;                                = reverse-atoms(l1) U reverse-atoms(l2, l3, ..., ln), l1 is a list


(defun reverse-atoms (lista)
  "Reverses only the continuous sequences of atoms in a list."
  (if (null lista)
      NIL
      (if (atom (car lista))
          (let ((atoms (collect-atoms lista)))
            (append (reverse atoms)
                    (reverse-atoms (nthcdr (length atoms) lista))))
          (cons (reverse-atoms (car lista)) (reverse-atoms (cdr lista))))))

(defun collect-atoms (lista)
  "Collects consecutive atoms from a list."
  (if (and lista (atom (car lista)))
      (cons (car lista) (collect-atoms (cdr lista)))
      NIL))

(print (reverse-atoms '(a b c (d (e f) g h i)))) ; (c b a (d (f e) i h g))
(print (reverse-atoms '(1 2 3 (4 (5 6) 7 8))))   ; (3 2 1 (4 (6 5) 8 7))


; d. Write a function to return the maximum value of the numerical atoms from a list, at superficial level.
; max-numerical(l1, l2, ..., ln) = [], if l does not contain numbers
;                                = max(remove-if-not numberp [l1, l2, ..., ln]), otherwise

(defun max-numerical (lista)
  "Finds the maximum numerical atom in a superficial list."
  (let ((nums (remove-if-not #'numberp lista))) ; keep only numbers in the list
    (if nums
        (reduce #'max nums)
        NIL)))

(print (max-numerical '(3 a 7 2 b))) ; 7
(print (max-numerical '(a b c)))     ; NIL




