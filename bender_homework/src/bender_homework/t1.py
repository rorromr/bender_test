#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import division

__author__ = 'Rodrigo Muñoz'
__email__ = 'rorro.mr@gmail.com'

import rospy
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan
import numpy as np

class LookAtMe(object):
  
  def __init__(self):
    rospy.loginfo('Init LookAtMe')
    # Sub datos laser
    self.laser_sub = rospy.Subscriber('/bender/sensors/laser_front/scan', LaserScan, self.process_laser)
    # Pub comando base
    self.base_pub = rospy.Publisher('/bender/nav/base/cmd_vel', Twist, queue_size=1)
    self.command = Twist()
    
  def process_laser(self, msg):
    cluster = [msg.angle_min+msg.angle_increment*i for i,dist in enumerate(msg.ranges) if dist < msg.range_max*0.9 ]
    if cluster: # Verificar 
      self.command.angular.z = 2*np.mean(cluster) # Controlador P usando angulo
      self.base_pub.publish(self.command) # Enviar comando a la base

if __name__ == '__main__':
  rospy.init_node('t1')
  t1 = LookAtMe()
  rospy.spin()