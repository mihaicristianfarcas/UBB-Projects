; L3 - Using MAP functions
; 3. Define a function to test the membership of a node in an n-tree represented as (root list_nodes_subtree_1 list_nodes_subtree_2)
; e. g. (a (b (c)) (d) (e (f))), 'b' -> True
; is-node-member(node, (root, L1, L2, ..., Ln)) = NIL, n = 0
;                                               = True, root = node
;                                               = is-node-member(node, L1) or ... or is-node-member(node, Ln), oth.

(defun is-node-member (node tree)
  "Check if NODE is a member of the n-tree TREE using a MAP function."
  (or (equal node (first tree))
      (some #'identity (mapcar #'(lambda (subtree) (is-node-member node subtree)) (rest tree)))
  )
)

(format t "~a~%" (is-node-member 'b '(a (b (c)) (d) (e (f)))))  ; Returns T
(format t "~a~%" (is-node-member 'z '(a (b (c)) (d) (e (f)))))  ; Returns NIL
