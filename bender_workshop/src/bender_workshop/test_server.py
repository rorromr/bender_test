#!/usr/bin/env python

import rospy
from std_srvs.srv import Trigger, TriggerResponse, TriggerRequest

def main():
  rospy.init_node('test_server')
  rospy.loginfo('test_server')
  # Server
  server = rospy.Service('trigger', Trigger, server_callback)
  rospy.spin()

def server_callback(req):
  rospy.loginfo('Server callback!')
  resp = TriggerResponse()
  resp.success = True
  resp.message = 'OK'
  return resp


if __name__ == '__main__':
  main()
  