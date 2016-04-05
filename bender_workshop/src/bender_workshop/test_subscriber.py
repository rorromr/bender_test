#!/usr/bin/env python

import rospy
from sensor_msgs.msg import JointState

JOINT_NAMES = ['l_elbow_pitch_joint', 'l_elbow_yaw_joint', 'l_shoulder_pitch_joint', 'l_shoulder_roll_joint', 'l_shoulder_yaw_joint', 'l_wrist_pitch_joint']

def main():
  rospy.loginfo('test_subscriber')
  # Subscriber for joint strates
  sub = rospy.Subscriber('/bender/joint_states', JointState, process_callback)
  rospy.spin()

def process_callback(msg):
  # Filter postition
  position = [msg.position[i] for i,name in enumerate(msg.name) if name in JOINT_NAMES]
  rospy.loginfo(position)

if __name__ == '__main__':
  rospy.init_node('test_subscriber')
  main()