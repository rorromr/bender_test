#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Wrench
from sensor_msgs.msg import Joy

class WrenchJoyInterface(object):
  def __init__(self, joy_topic = '/joy', wrench_topic = '/quad/force'):
    self.sub = rospy.Subscriber(joy_topic, Joy, self.process_callback)
    self.pub = rospy.Publisher(wrench_topic, Wrench, queue_size=1)
    self.msg = Wrench()

  def process_callback(self, msg):
    throttle = msg.axes[1]+1
    turn = msg.axes[3]
    ahead = msg.axes[4]
    self.msg.force.z = throttle*9.8*0.8*0.65
    self.msg.torque.z = turn*0.05
    self.msg.force.x = ahead*0.5
    self.pub.publish(self.msg)

if __name__ == '__main__':
  rospy.init_node('wrench_joy')
  wji = WrenchJoyInterface()
  rospy.spin()
