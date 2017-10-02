
(cl:in-package :asdf)

(defsystem "multisensor_information_fusion-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "pos" :depends-on ("_package_pos"))
    (:file "_package_pos" :depends-on ("_package"))
  ))