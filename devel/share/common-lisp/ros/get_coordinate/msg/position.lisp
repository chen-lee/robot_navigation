; Auto-generated. Do not edit!


(cl:in-package get_coordinate-msg)


;//! \htmlinclude position.msg.html

(cl:defclass <position> (roslisp-msg-protocol:ros-message)
  ((positionX
    :reader positionX
    :initarg :positionX
    :type cl:integer
    :initform 0)
   (positionY
    :reader positionY
    :initarg :positionY
    :type cl:integer
    :initform 0))
)

(cl:defclass position (<position>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <position>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'position)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name get_coordinate-msg:<position> is deprecated: use get_coordinate-msg:position instead.")))

(cl:ensure-generic-function 'positionX-val :lambda-list '(m))
(cl:defmethod positionX-val ((m <position>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader get_coordinate-msg:positionX-val is deprecated.  Use get_coordinate-msg:positionX instead.")
  (positionX m))

(cl:ensure-generic-function 'positionY-val :lambda-list '(m))
(cl:defmethod positionY-val ((m <position>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader get_coordinate-msg:positionY-val is deprecated.  Use get_coordinate-msg:positionY instead.")
  (positionY m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <position>) ostream)
  "Serializes a message object of type '<position>"
  (cl:let* ((signed (cl:slot-value msg 'positionX)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'positionY)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <position>) istream)
  "Deserializes a message object of type '<position>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'positionX) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'positionY) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<position>)))
  "Returns string type for a message object of type '<position>"
  "get_coordinate/position")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'position)))
  "Returns string type for a message object of type 'position"
  "get_coordinate/position")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<position>)))
  "Returns md5sum for a message object of type '<position>"
  "1e6dec27a67212d263c165fa84b8b2ea")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'position)))
  "Returns md5sum for a message object of type 'position"
  "1e6dec27a67212d263c165fa84b8b2ea")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<position>)))
  "Returns full string definition for message of type '<position>"
  (cl:format cl:nil "int32 positionX~%int32 positionY~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'position)))
  "Returns full string definition for message of type 'position"
  (cl:format cl:nil "int32 positionX~%int32 positionY~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <position>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <position>))
  "Converts a ROS message object to a list"
  (cl:list 'position
    (cl:cons ':positionX (positionX msg))
    (cl:cons ':positionY (positionY msg))
))
