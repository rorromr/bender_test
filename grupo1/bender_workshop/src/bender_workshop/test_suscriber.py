#!/usr/bin/env python

import rospy
from sensor_msgs.msg import JointState

def main():
  rospy.init_node('test_suscriber')
  rospy.loginfo('test_suscriber')
  # Sub datos joints
  sub = rospy.Subscriber('/bender/joint_states', JointState, process_callback)
  rospy.spin()

def process_callback(msg):
  rospy.loginfo(msg.name[1:3])


if __name__ == '__main__':
  main()